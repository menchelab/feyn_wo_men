#!/usr/bin/env python3
"""Plot sort benchmark CSV(s) against n^2, n log n, n on log-log axes.

Usage: python plot_bench.py [csv_file ...]

If no files given, looks for *.csv in the current directory.
"""

import sys
import glob
import csv
import numpy as np
import matplotlib.pyplot as plt

def load_csv(path):
    """Return (n, avg_ns) lists from a benchmark CSV."""
    ns, times = [], []
    with open(path) as f:
        reader = csv.DictReader(f)
        for row in reader:
            ns.append(int(row["n"]))
            times.append(float(row["avg_ns"]))
    return np.array(ns), np.array(times)


def main():
    # --- find CSV files ---
    if len(sys.argv) > 1:
        paths = sys.argv[1:]
    else:
        paths = sorted(glob.glob("*.csv"))

    if not paths:
        print("no CSV files found", file=sys.stderr)
        sys.exit(1)

    plt.figure(figsize=(10, 7))

    # --- reference curves ---
    # pick x-range spanning all data
    all_ns = []
    for p in paths:
        ns, _ = load_csv(p)
        all_ns.extend(ns)
    x_min = max(1, min(all_ns))
    x_max = max(all_ns) * 1.2
    x = np.logspace(np.log10(x_min), np.log10(x_max), 200)

    # scale references to pass through the first data point of the first CSV
    _, ref_t = load_csv(paths[0])
    ref_n = x_max  # will be overwritten below

    # n^2  — pick constant so curve is visible near data
    plt.loglog(x, x**2,   "--", color="gray",  label=r"$n^2$")
    plt.loglog(x, x * np.log(x), "-.", color="gray",  label=r"$n \log n$")
    plt.loglog(x, x,            ":",  color="gray",  label=r"$n$")

    # --- CSV data ---
    for p in paths:
        ns, times = load_csv(p)
        label = p.rsplit("/", 1)[-1].replace(".csv", "")  # basename sans .csv
        plt.loglog(ns, times, "o-", label=label)

    plt.xlabel("n (list length)")
    plt.ylabel("avg time (ns)")
    plt.title("Sorting algorithm benchmarks")
    plt.legend()
    plt.grid(True, which="both", alpha=0.3)
    plt.tight_layout()
    plt.savefig("benchmark.png", dpi=120)
    plt.show()


if __name__ == "__main__":
    main()
