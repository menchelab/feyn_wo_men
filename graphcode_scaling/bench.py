#!/usr/bin/env python3
"""Scaling benchmark: graphcodes of the Delaunay density bifiltration on 2D point clouds.

Pipeline per run
  1. sample n points in the plane, attach a function value (k-NN codensity, or random)
  2. function_delaunay  -> sublevel Delaunay-Cech bifiltration as scc2020 chain complex
  3. graphcode          -> graphcode of H1 (--level 2) or H0 (--level 3)

Usage: bench.py <function_delaunay/main> <graphcode binary> <workdir> <out.csv> [max_n]
"""
import csv
import os
import re
import subprocess
import sys
import time

import numpy as np
from scipy.spatial import cKDTree

FD, GC, WORK, OUT = sys.argv[1:5]
MAX_N = int(sys.argv[5]) if len(sys.argv) > 5 else 1_024_000
K_NN = 15
TIMEOUT = 1800


def sample(kind, n, rng):
    if kind == "uniform":
        return rng.random((n, 2))
    if kind == "annuli":  # 3 noisy circles + 10% uniform background noise
        n_bg = n // 10
        centers = np.array([[0.3, 0.3], [0.7, 0.35], [0.5, 0.72]])
        radii = np.array([0.18, 0.14, 0.2])
        which = rng.integers(0, 3, n - n_bg)
        theta = rng.uniform(0, 2 * np.pi, n - n_bg)
        r = radii[which] + rng.normal(0, 0.012, n - n_bg)
        pts = centers[which] + np.c_[r * np.cos(theta), r * np.sin(theta)]
        return np.vstack([pts, rng.random((n_bg, 2))])
    raise ValueError(kind)


def function_values(func, pts, rng):
    if func == "knn":  # codensity: distance to K-th nearest neighbour (small = dense)
        d, _ = cKDTree(pts).query(pts, k=K_NN + 1)
        return d[:, -1]
    if func == "random":
        return rng.random(len(pts))
    raise ValueError(func)


def run(cmd):
    """Run cmd; return (stdout, wall seconds, peak RSS in MB) or None on timeout."""
    t0 = time.perf_counter()
    p = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
    try:
        out, _ = p.communicate(timeout=TIMEOUT)
    except subprocess.TimeoutExpired:
        p.kill()
        p.communicate()
        return None
    wall = time.perf_counter() - t0
    if p.returncode != 0:
        raise RuntimeError(f"{cmd} failed:\n{out[-2000:]}")
    return out, wall


def run_rss(cmd):
    """Like run, but also measures the child's peak RSS via wait4."""
    t0 = time.perf_counter()
    with open(os.path.join(WORK, "log.txt"), "w") as log:
        p = subprocess.Popen(cmd, stdout=log, stderr=subprocess.STDOUT)
        deadline = t0 + TIMEOUT
        while True:
            pid, status, ru = os.wait4(p.pid, os.WNOHANG)
            if pid:
                break
            if time.perf_counter() > deadline:
                p.kill()
                os.wait4(p.pid, 0)
                return None
            time.sleep(0.002)
    wall = time.perf_counter() - t0
    p.returncode = os.waitstatus_to_exitcode(status)
    out = open(os.path.join(WORK, "log.txt")).read()
    if p.returncode != 0:
        raise RuntimeError(f"{cmd} failed ({p.returncode}):\n{out[-2000:]}")
    return out, wall, ru.ru_maxrss / 1024


def grab(pattern, text, cast=float):
    m = re.search(pattern, text)
    return cast(m.group(1)) if m else None


# variant name -> (extra function_delaunay args, extra graphcode args)
VARIANTS = {
    "default": ([], []),
    "uncompressed": ([], ["--uncompressed"]),
    "slices20": ([], ["--slices", "20"]),
    # NB: the code's default primary parameter is 1 (= radius; the help text says 0).
    # primary0 slices at fixed function value (density) instead.
    "primary0": ([], ["--primary-parameter", "0"]),
}
# the uncompressed graphcode grows quadratically (memory!), so cap its input size
MAX_N_VARIANT = {"uncompressed": 8000}
LEVEL = {"H1": "2", "H0": "3"}

# (dataset, function, variants, homology dims)
CONFIGS = [
    ("uniform", "knn", ["default", "uncompressed", "slices20", "primary0"], ["H1", "H0"]),
    ("annuli", "knn", ["default", "primary0"], ["H1", "H0"]),
    ("uniform", "random", ["default", "primary0"], ["H1", "H0"]),
]
SIZES = [1000 * 2**i for i in range(0, 11) if 1000 * 2**i <= MAX_N]
SEEDS = [0, 1, 2]

FIELDS = ["dataset", "func", "n", "seed", "hom", "variant", "fd_wall", "fd_rss_mb",
          "cells3", "cells2", "cells1", "cells0", "scc_mb", "gc_wall", "gc_io", "gc_build",
          "gc_rss_mb", "vertices", "edges"]

os.makedirs(WORK, exist_ok=True)
pts_file = os.path.join(WORK, "pts.txt")
scc_file = os.path.join(WORK, "cc.scc")
dead = set()  # (dataset, func, hom, variant) combos that timed out: skip larger n

with open(OUT, "w", newline="") as fh:
    w = csv.DictWriter(fh, FIELDS)
    w.writeheader()
    for n in SIZES:
        for dataset, func, variants, homs in CONFIGS:
            for seed in SEEDS if n <= 256_000 else SEEDS[:1]:
                rng = np.random.default_rng(seed)
                pts = sample(dataset, n, rng)
                f = function_values(func, pts, rng)
                np.savetxt(pts_file, np.c_[pts, f], fmt="%.17g")
                res = run_rss([FD, "--no-delaunay-compare", pts_file, scc_file])
                if res is None:
                    continue
                _, fd_wall, fd_rss = res
                with open(scc_file) as s:
                    s.readline(); s.readline()
                    sizes = s.readline().split()
                base = dict(dataset=dataset, func=func, n=n, seed=seed, fd_wall=round(fd_wall, 4),
                            fd_rss_mb=round(fd_rss, 1), cells3=sizes[0], cells2=sizes[1],
                            cells1=sizes[2], cells0=sizes[3],
                            scc_mb=round(os.path.getsize(scc_file) / 2**20, 2))
                for hom in homs:
                    for variant in variants:
                        key = (dataset, func, hom, variant)
                        if key in dead or n > MAX_N_VARIANT.get(variant, MAX_N):
                            continue
                        res = run_rss([GC, scc_file, "--level", LEVEL[hom], "--no-output",
                                       *VARIANTS[variant][1]])
                        if res is None:
                            dead.add(key)
                            print("TIMEOUT", key, n, flush=True)
                            continue
                        out, wall, rss = res
                        row = dict(base, hom=hom, variant=variant, gc_wall=round(wall, 4),
                                   gc_io=grab(r"IO timer:\s+([\d.e+-]+)", out),
                                   gc_build=grab(r"Graphcode timer\s+([\d.e+-]+)", out),
                                   gc_rss_mb=round(rss, 1),
                                   vertices=grab(r"In Boost graph: (\d+) vertices", out, int),
                                   edges=grab(r"vertices and (\d+) edges", out, int))
                        w.writerow(row)
                        fh.flush()
                        print(row, flush=True)
