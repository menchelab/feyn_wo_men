#include "linked_list.h"
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <iostream>

using namespace std;
using namespace std::chrono;

int bubblesort(List *L) {
  int swap_count = 0;
  bool swapped;

  do {
    swapped = false;
    Node *cur = L->root;
    while (cur != nullptr && cur->next != nullptr) {
      if (cur->val < cur->next->val) {
        int tmp = cur->val;
        cur->val = cur->next->val;
        cur->next->val = tmp;
        swapped = true;
        swap_count++;
      }
      cur = cur->next;
    }
  } while (swapped);

  return swap_count;
}

int selectionsort(List *L) {
  int swap_count = 0;
  Node *cur = L->root;

  while (cur != nullptr && cur->next != nullptr) {
    // find node with max value in unsorted portion starting at cur
    Node *max_node = cur;
    Node *scan = cur->next;
    while (scan != nullptr) {
      if (scan->val > max_node->val)
        max_node = scan;
      scan = scan->next;
    }
    if (max_node != cur) {
      int tmp = cur->val;
      cur->val = max_node->val;
      max_node->val = tmp;
      swap_count++;
    }
    cur = cur->next;
  }

  return swap_count;
}

void benchmark_sort(int (*sort_fn)(List *), FILE *csv) {
  const int runs = 10;
  const int sizes[] = {10, 100, 1000, 10000, 100000};
  const int n_sizes = 5;

  fprintf(csv, "n,avg_ns\n");

  for (int s = 0; s < n_sizes; s++) {
    int size = sizes[s];

    // build list with random values
    List L = List();
    for (int i = 0; i < size; i++)
      L.attach(rand() % 100000);

    // snapshot initial values once
    int *snap = new int[size];
    for (int i = 0; i < size; i++)
      snap[i] = L.nth_from_start(i);

    long long total_ns = 0;
    for (int r = 0; r < runs; r++) {
      // restore list to unsorted state from snapshot
      while (L.len > 0) L.delete_at(0);
      for (int i = 0; i < size; i++)
        L.attach(snap[i]);

      auto t0 = high_resolution_clock::now();
      sort_fn(&L);
      auto t1 = high_resolution_clock::now();
      total_ns += duration_cast<nanoseconds>(t1 - t0).count();
    }

    delete[] snap;
    long long avg_ns = total_ns / runs;
    cout << "n=" << size << "  avg over " << runs << " runs: "
         << avg_ns << " ns\n";
    fprintf(csv, "%d,%lld\n", size, avg_ns);
  }
}

int main(void) {
  // seed random number generator
  srand(42);

  // --- bubble sort demo ---
  cout << "=== Bubble Sort ===\n";
  List L1 = List();
  for (int i = 0; i < 20; i++)
    L1.attach(rand() % 100);

  cout << "Unsorted:\n";
  L1.print();

  int b_swaps = bubblesort(&L1);

  cout << "Sorted (" << b_swaps << " swaps):\n";
  L1.print();

  // // --- selection sort demo ---
  // cout << "\n=== Selection Sort ===\n";
  // List L2 = List();
  // for (int i = 0; i < 20; i++)
  //   L2.attach(rand() % 100);

  // cout << "Unsorted:\n";
  // L2.print();

  // int s_swaps = selectionsort(&L2);

  // cout << "Sorted (" << s_swaps << " swaps):\n";
  // L2.print();

  // // --- benchmark both ---
  // cout << "\n--- Benchmarking ---\n";

  // FILE *csv = fopen("bubblesort.csv", "w");
  // if (csv == nullptr) {
  //   cout << "failed to open bubblesort.csv\n";
  //   return 1;
  // }
  // benchmark_sort(bubblesort, csv);
  // fclose(csv);

  // csv = fopen("selectionsort.csv", "w");
  // if (csv == nullptr) {
  //   cout << "failed to open selectionsort.csv\n";
  //   return 1;
  // }
  // benchmark_sort(selectionsort, csv);
  // fclose(csv);

  return 0;
}