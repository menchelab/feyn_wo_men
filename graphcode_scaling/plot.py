#!/usr/bin/env python3
"""Plot results.csv from bench.py and print fitted scaling exponents."""
import csv
import sys
from collections import defaultdict

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np

src = sys.argv[1] if len(sys.argv) > 1 else "results.csv"
rows = list(csv.DictReader(open(src)))

C = {"blue": "#2a78d6", "orange": "#eb6834", "aqua": "#1baf7a", "yellow": "#eda100"}
INK, MUTED, GRID = "#0b0b0b", "#52514e", "#e4e3df"


def series(metric, **sel):
    """median over seeds of `metric` per n, for rows matching sel."""
    acc = defaultdict(list)
    for r in rows:
        if all(r[k] == v for k, v in sel.items()) and r[metric] not in ("", None):
            acc[int(r["n"])].append(float(r[metric]))
    ns = np.array(sorted(acc))
    return ns, np.array([np.median(acc[n]) for n in ns])


def slope(ns, ys, nmin=16000):
    m = (ns >= nmin) & (ys > 0)
    if m.sum() < 3:
        m = ys > 0
    return np.polyfit(np.log(ns[m]), np.log(ys[m]), 1)[0]


def draw(ax, metric, specs, title, ylabel):
    for label, color, sel in specs:
        ns, ys = series(metric, **sel)
        if len(ns) == 0:
            continue
        k = slope(ns, ys)
        ax.loglog(ns, ys, "-o", color=color, lw=2, ms=4, label=f"{label}  (~n^{k:.2f})")
        print(f"{title:42s} {label:38s} exponent {k:.2f}  last: n={ns[-1]} -> {ys[-1]:.4g}")
    ax.set_title(title, loc="left", fontsize=11, color=INK)
    ax.set_xlabel("number of points n", color=MUTED)
    ax.set_ylabel(ylabel, color=MUTED)
    ax.grid(True, which="major", color=GRID, lw=0.8)
    ax.set_axisbelow(True)
    for s in ("top", "right"):
        ax.spines[s].set_visible(False)
    for s in ("left", "bottom"):
        ax.spines[s].set_color(GRID)
    ax.tick_params(colors=MUTED)
    ax.legend(frameon=False, fontsize=8.5, loc="lower right")


DATA = [("uniform, k-NN codensity", C["blue"], dict(dataset="uniform", func="knn")),
        ("3 noisy circles, k-NN codensity", C["orange"], dict(dataset="annuli", func="knn")),
        ("uniform, random function", C["aqua"], dict(dataset="uniform", func="random"))]
with_ = lambda specs, **kw: [(l, c, dict(s, **kw)) for l, c, s in specs]
U = dict(dataset="uniform", func="knn", hom="H1")
VAR = [("default (compressed, all radius slices)", C["blue"], dict(U, variant="default")),
       ("--primary-parameter 0 (density slices)", C["orange"], dict(U, variant="primary0")),
       ("--slices 20", C["aqua"], dict(U, variant="slices20")),
       ("--uncompressed", C["yellow"], dict(U, variant="uncompressed"))]
STAGE = [("function_delaunay (build bifiltration)", C["blue"], dict(U, variant="default")),
         ]

fig, axes = plt.subplots(2, 3, figsize=(16, 9), facecolor="#fcfcfb")
for ax in axes.flat:
    ax.set_facecolor("#fcfcfb")
draw(axes[0, 0], "gc_wall", with_(DATA, hom="H1", variant="default"),
     "graphcode wall time, H1 (default options)", "seconds")
draw(axes[0, 1], "gc_wall", with_(DATA, hom="H0", variant="default"),
     "graphcode wall time, H0 (default options)", "seconds")
draw(axes[0, 2], "gc_wall", VAR, "graphcode wall time by option (uniform, H1)", "seconds")
draw(axes[1, 0], "vertices", with_(DATA, hom="H1", variant="default"),
     "graphcode size, H1 (default options)", "graph vertices")
draw(axes[1, 1], "vertices", VAR, "graphcode size by option (uniform, H1)", "graph vertices")
draw(axes[1, 2], "fd_wall", [("function_delaunay: build bifiltration", C["blue"], dict(U, variant="default"))]
     , "pipeline stages (uniform, k-NN codensity, H1)", "seconds")
draw(axes[1, 2], "gc_io", [("graphcode: parse scc2020 file", C["orange"], dict(U, variant="default"))],
     "pipeline stages (uniform, k-NN codensity, H1)", "seconds")
draw(axes[1, 2], "gc_build", [("graphcode: build graphcode", C["aqua"], dict(U, variant="default"))],
     "pipeline stages (uniform, k-NN codensity, H1)", "seconds")
fig.suptitle("Graphcode scaling on the 2D Delaunay density bifiltration (log-log; exponents fitted for n >= 16k)",
             x=0.01, ha="left", fontsize=13, color=INK)
fig.tight_layout(rect=(0, 0, 1, 0.96))
fig.savefig("scaling.png", dpi=150)

for label, _, sel in DATA:
    ns, m = series("gc_rss_mb", **dict(sel, hom="H1", variant="default"))
    _, c = series("cells2", **dict(sel, hom="H1", variant="default"))
    print(f"{label}: peak RSS at n={ns[-1]}: {m[-1]:.0f} MB; total 2-cells/n = {c[-1]/ns[-1]:.2f}")
