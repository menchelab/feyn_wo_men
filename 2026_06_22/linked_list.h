#ifndef LINKED_LIST_H
#define LINKED_LIST_H

// =====================================================================
// Doubly linked list — Node and List class declarations.
// =====================================================================

class Node
{
public:
    int val;
    Node* prev;
    Node* next;

    Node(int value);
};

class List
{
public:
    Node* root;
    Node* tail;
    int len;

    List();
    ~List();

    int attach(int val);
    int print();

    // Return value at index n from start (0-indexed).  -1 if out of bounds.
    int nth_from_start(int n);

    // Return value at index n from end (0-indexed).  -1 if out of bounds.
    int nth_from_end(int n);

    // Insert value at index n.  Returns new length, or -1 if n out of bounds.
    int insert_at(int val, int n);

    // Remove node at index n.  Returns new length, or -1 if n out of bounds.
    int delete_at(int n);

    // Reverse list in-place.
    void reverse();

    // Swap values at indices i and j.  0 on success, -1 on out of bounds.
    int swap(int i, int j);
};

#endif  // LINKED_LIST_H
