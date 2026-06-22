#include <iostream>
#include "linked_list.h"
using namespace std;

// =====================================================================
// Node
// =====================================================================

Node::Node(int value)
{
    prev = nullptr;
    next = nullptr;
    val = value;
}

// =====================================================================
// List
// =====================================================================

List::List()
{
    root = nullptr;
    tail = nullptr;
    len = 0;
}

List::~List()
{
    Node* cur = root;
    while (cur != nullptr)
    {
        Node* nextNode = cur->next;
        delete cur;
        cur = nextNode;
    }
}

int List::attach(int val)
{
    Node* node = new Node(val);

    if (root == nullptr)
    {
        root = node;
        tail = node;
        len = 1;
    }
    else
    {
        node->prev = tail;
        tail->next = node;
        tail = node;
        len++;
    }
    return len;
}

int List::print()
{
    if (root == nullptr)
    {
        cout << "List is empty.\n";
        return 0;
    }

    int i = 0;
    Node* cur = root;
    while (cur != nullptr)
    {
        cout << i << " : " << cur->val << "\n";
        cur = cur->next;
        i++;
    }
    return i;
}

int List::nth_from_start(int n)
{
    if (n < 0 || n >= len)
        return -1;

    Node* cur = root;
    for (int i = 0; i < n; i++)
        cur = cur->next;
    return cur->val;
}

int List::nth_from_end(int n)
{
    if (n < 0 || n >= len)
        return -1;

    Node* cur = tail;
    for (int i = 0; i < n; i++)
        cur = cur->prev;
    return cur->val;
}

int List::insert_at(int val, int n)
{
    if (n < 0 || n > len)
        return -1;

    // Insert at head
    if (n == 0)
    {
        Node* node = new Node(val);
        node->next = root;
        if (root != nullptr)
            root->prev = node;
        root = node;
        if (tail == nullptr)   // list was empty
            tail = node;
        len++;
        return len;
    }

    // Insert at tail
    if (n == len)
    {
        return attach(val);
    }

    // Insert in middle: walk to node currently at index n
    Node* cur = root;
    for (int i = 0; i < n; i++)
        cur = cur->next;

    Node* node = new Node(val);
    node->prev = cur->prev;
    node->next = cur;
    cur->prev->next = node;
    cur->prev = node;
    len++;
    return len;
}

int List::delete_at(int n)
{
    if (n < 0 || n >= len)
        return -1;

    // Find the node to delete
    Node* cur = root;
    for (int i = 0; i < n; i++)
        cur = cur->next;

    // Delete head
    if (cur == root)
    {
        root = cur->next;
        if (root != nullptr)
            root->prev = nullptr;
        else
            tail = nullptr;   // list became empty
    }
    // Delete tail
    else if (cur == tail)
    {
        tail = cur->prev;
        tail->next = nullptr;
    }
    // Delete from middle
    else
    {
        cur->prev->next = cur->next;
        cur->next->prev = cur->prev;
    }

    delete cur;
    len--;
    return len;
}

void List::reverse()
{
    if (root == nullptr || root == tail)
        return;   // empty or single-element: nothing to do

    Node* cur = root;
    while (cur != nullptr)
    {
        // Swap prev and next pointers
        Node* tmp = cur->prev;
        cur->prev = cur->next;
        cur->next = tmp;

        // Move to next node (which is now prev, since we swapped)
        cur = cur->prev;
    }

    // Swap root and tail
    Node* tmp = root;
    root = tail;
    tail = tmp;
}

int List::swap(int i, int j)
{
    if (i < 0 || i >= len || j < 0 || j >= len)
        return -1;

    if (i == j)
        return 0;

    // Walk to node_i
    Node* node_i = root;
    for (int k = 0; k < i; k++)
        node_i = node_i->next;

    // Walk to node_j
    Node* node_j = root;
    for (int k = 0; k < j; k++)
        node_j = node_j->next;

    // Swap values
    int tmp = node_i->val;
    node_i->val = node_j->val;
    node_j->val = tmp;

    return 0;
}

// main() lives in sorting.cpp — compile with:
//   clang++ -g -O0 -std=c++11 -o build/sorting sorting.cpp linked_list.cpp
