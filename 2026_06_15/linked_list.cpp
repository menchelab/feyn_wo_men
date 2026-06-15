// #include pulls in code from another file so we can use it here.
// <iostream> gives us cout (console output) and other I/O tools.
#include <iostream>
using namespace std;  // So we can write "cout" instead of "std::cout"

// =====================================================================
// CLASS: Node — one element in a doubly linked list.
//
// Class syntax:
//   class Name { ... };
//       ^--- semicolon required at the end (unlike functions)
//
// public: / private:
//   - public:  anyone outside the class can access these members
//   - private: only the class's own methods can access them
//   - If you don't write public/private, everything defaults to private
//
// This Node class keeps everything public for simplicity.
// =====================================================================
class Node
{
public:              // Everything below is accessible from outside
    int val;         // The integer value stored in this node
    Node* prev;      // Pointer to the previous node (left neighbor)
    Node* next;      // Pointer to the next node (right neighbor)

    // ---------------------------------------------------------------
    // CONSTRUCTOR: Node(int value)
    //
    // A constructor runs automatically when a new object is created.
    // It has NO return type (not even void) and is named after the class.
    //
    // Here it:
    //   1. Sets both pointers to nullptr (points to nothing)
    //   2. Stores the passed value in val
    //
    // nullptr = special value meaning "points to nothing".
    //           Safer than NULL or 0 — the compiler catches misuse.
    // ---------------------------------------------------------------
    Node(int value)
    {
        prev = nullptr;   // New node has no neighbors yet
        next = nullptr;
        val = value;      // Store the data we were given
    }
};  // <--- semicolon: required after class closing brace


// =====================================================================
// CLASS: List — a doubly linked list that owns and manages Nodes.
//
// Tracks the first node (root), last node (tail), and total length.
// Its destructor cleans up all nodes — no memory leaks.
// =====================================================================
class List
{
public:
    // --- Member variables ---
    Node* root;   // Pointer to first node (head of list), nullptr if empty
    Node* tail;   // Pointer to last node, nullptr if empty
    int len;      // How many nodes are in the list

    // ---------------------------------------------------------------
    // CONSTRUCTOR: List()
    //
    // Called when a List object is born. Sets everything to zero/null
    // because an empty list has no nodes.
    //
    // Default constructor = a constructor that takes no arguments.
    // ---------------------------------------------------------------
    List()
    {
        root = nullptr;   // Empty list: no first node
        tail = nullptr;   // Empty list: no last node
        len = 0;          // Empty list: zero items
    }

    // ---------------------------------------------------------------
    // DESTRUCTOR: ~List()
    //
    // Called automatically when a List object dies (goes out of scope
    // or is deleted).
    //
    // Named: tilde (~) + class name. No return type, no arguments.
    //
    // WHY needed: The List allocated Nodes with "new" (on the heap).
    // Those Nodes do NOT die automatically when the List dies —
    // only the pointers (root, tail) die. Without this destructor,
    // the Nodes would stay in memory forever = memory leak.
    //
    // This destructor walks the chain and deletes every Node.
    // ---------------------------------------------------------------
    ~List()
    {
        Node* cur = root;            // Start at the head
        while (cur != nullptr)       // While there's still a node to delete
        {
            // CRITICAL: Save next pointer BEFORE deleting cur.
            // After "delete cur", cur->next is invalid memory —
            // reading it would be undefined behavior (crash/corruption).
            Node* nextNode = cur->next;
            delete cur;              // Free this node's memory
            cur = nextNode;          // Move to the next node
        }
    }

    // ---------------------------------------------------------------
    // attach(val): Add a new node with value 'val' to the END of list.
    // Returns the new length of the list.
    //
    // Uses "new" to allocate the Node on the heap.
    // "new" = ask OS for memory, construct object there, return pointer.
    // Heap memory lives until you "delete" it — it survives function returns.
    // ---------------------------------------------------------------
    int attach(int val)
    {
        // "new Node(val)" does two things:
        //   1. Allocates memory on the heap (enough for one Node)
        //   2. Calls Node's constructor with val
        // Returns a pointer to the newly created Node.
        Node* node = new Node(val);

        if (root == nullptr)         // List is currently empty
        {
            root = node;             // This node becomes the first
            tail = node;             // ...and also the last
            len = 1;                 // List now has exactly one node
        }
        else                         // List already has nodes
        {
            node->prev = tail;       // New node's prev points to old tail
                                     // (node->prev is same as *(node).prev)
            tail->next = node;       // Old tail's next points to new node
            tail = node;             // Update tail to be the new node
            len++;                   // One more node in the list
        }
        return len;
    }

    // ---------------------------------------------------------------
    // print(): Walk the list from root to end, printing each value
    // with its index. Returns how many items were printed.
    //
    // Pointer traversal pattern:
    //   cur = root       — start at first node
    //   cur = cur->next  — advance to next node
    //   cur != nullptr   — stop when we've passed the last node
    // ---------------------------------------------------------------
    int print()
    {
        if (root == nullptr)
        {
            cout << "List is empty.\n";
            return 0;
        }

        int i = 0;
        Node* cur = root;
        while (cur != nullptr)       // nullptr = past the last node
        {
            cout << i << " : " << cur->val << "\n";  // Print "index : value"
            cur = cur->next;         // Advance to next node
            i++;
        }
        return i;
    }
    // ===========================================================
    // HOMEWORK: Implement these functions.
    //
    // For each: draw the pointer diagram on paper first.
    // Then translate the arrows into code.
    //
    // Edge cases to handle in every function:
    //   - empty list (root == nullptr, len == 0)
    //   - index out of bounds (n < 0 or n >= len)
    //   - single-element list
    //   - first/last element (prev or next is nullptr)
    // ===========================================================

    // ---------------------------------------------------------------
    // nth_from_start(n): Return value of node at index n from the
    // START of the list (0-indexed).
    //   - Walk root->next n times to reach the nth node.
    //   - If n is out of bounds, return -1 (or throw).
    //
    // Example: list = [5, 3, 8], nth_from_start(1) returns 3.
    // ---------------------------------------------------------------
    int nth_from_start(int n)
    {
        // TODO: Your implementation here.
        return -1;
    }

    // ---------------------------------------------------------------
    // nth_from_end(n): Return value of node at index n from the
    // END of the list (0-indexed).
    //   - nth_from_end(0) = last element (tail).
    //   - Walk tail->prev n times to reach the nth-from-end node.
    //   - If n is out of bounds, return -1.
    //
    // Example: list = [5, 3, 8], nth_from_end(0) returns 8,
    //          nth_from_end(1) returns 3.
    // ---------------------------------------------------------------
    int nth_from_end(int n)
    {
        // TODO: Your implementation here.
        return -1;
    }

    // ---------------------------------------------------------------
    // insert_at(val, n): Insert a new node with value 'val' at
    // index n (0-indexed). Returns the new length of the list.
    //
    // Cases:
    //   n == 0:     insert at head (update root)
    //   n == len:   insert at tail (same as attach)
    //   0 < n < len: insert in the middle (rewire prev/next of neighbors)
    //
    // Steps for middle insert:
    //   1. Walk to the node currently at index n (call it 'cur')
    //   2. Create new node
    //   3. new->prev = cur->prev
    //   4. new->next = cur
    //   5. cur->prev->next = new
    //   6. cur->prev = new
    //   7. len++
    //
    // If n is out of bounds (n < 0 or n > len), return -1.
    //
    // Example: list = [5, 3, 8], insert_at(7, 1) -> [5, 7, 3, 8]
    // ---------------------------------------------------------------
    int insert_at(int val, int n)
    {
        // TODO: Your implementation here.
        return -1;
    }

    // ---------------------------------------------------------------
    // delete_at(n): Remove the node at index n (0-indexed).
    // Returns the new length of the list.
    //
    // Cases:
    //   n == 0:       delete head (update root)
    //   n == len-1:   delete tail (update tail)
    //   0 < n < len-1: delete from middle (rewire neighbors)
    //
    // Steps for middle delete:
    //   1. Walk to the node at index n (call it 'cur')
    //   2. cur->prev->next = cur->next  (bypass cur from left)
    //   3. cur->next->prev = cur->prev  (bypass cur from right)
    //   4. delete cur                   (free memory)
    //   5. len--
    //
    // IMPORTANT: Draw the before/after diagram so you don't lose
    // references before rewiring.
    //
    // If n is out of bounds (n < 0 or n >= len), return -1.
    //
    // Example: list = [5, 3, 8], delete_at(1) -> [5, 8]
    // ---------------------------------------------------------------
    int delete_at(int n)
    {
        // TODO: Your implementation here.
        return -1;
    }

    // ---------------------------------------------------------------
    // reverse(): Reverse the list in-place.
    //   - Every node's prev and next pointers are swapped.
    //   - root becomes tail, tail becomes root.
    //
    // Approach:
    //   1. Walk from root to nullptr, swapping prev<->next at each node.
    //   2. After the loop, swap root and tail.
    //
    // Edge: empty list (do nothing), single-element list (do nothing).
    //
    // Example: [1, 2, 3] -> [3, 2, 1]
    // ---------------------------------------------------------------
    void reverse()
    {
        // TODO: Your implementation here.
    }

    // ---------------------------------------------------------------
    // swap(i, j): Swap the two nodes at indices i and j.
    // Returns 0 on success, -1 if either index is out of bounds.
    //
    // If i == j, nothing to do (return 0).
    //
    // Approach:
    //   1. Walk to node at index i (call it node_i)
    //   2. Walk to node at index j (call it node_j)
    //   3. Swap their val fields (easy) — or fully rewire pointers (hard).
    //      Start with swapping values, then try pointer rewiring for
    //      extra challenge.
    //
    // For pointer rewiring: each node has 4 connections to update
    // (its own prev/next and its neighbors' pointers back to it).
    // Handle the case where i and j are adjacent separately.
    //
    // Example: list = [a, b, c, d], swap(0, 2) -> [c, b, a, d]
    // ---------------------------------------------------------------
    int swap(int i, int j)
    {
        // TODO: Your implementation here.
        return -1;
    }
};  // <--- semicolon: required after class closing brace


// =====================================================================
// main(): Demonstrate two ways to create and use a List.
//
// Shows the difference between heap allocation (new/delete) and
// stack allocation (automatic lifetime).
// =====================================================================
int main()
{
    // --- METHOD 1: Heap allocation with "new" ---
    // "new List" allocates the List on the heap and calls the constructor.
    // Returns a POINTER (List*), so we use "->" to access members.
    //
    // "->" is shorthand for: dereference pointer, then access member.
    //   L->attach(i)  =  (*(L)).attach(i)
    //
    // Heap objects live until you explicitly "delete" them.
    // If you forget to delete, memory leaks — the memory stays claimed
    // even after the program moves on.
    List* L = new List;              // L is a pointer to a heap-allocated List
    for (int i = 0; i < 100; i++)
    {
        L->attach(i);                // -> used because L is a pointer
    }
    cout << "L: \n";
    L->print();

    delete(L);                       // MANUAL cleanup: free the List AND
                                     // trigger its destructor, which deletes
                                     // all 100 Nodes. Without this line:
                                     // memory leak of 1 List + 100 Nodes.

    // --- METHOD 2: Stack allocation (automatic) ---
    // "List K = List()" creates the List on the stack.
    // Stack objects die automatically when they go out of scope
    // (at the closing } of the block they were created in).
    //
    // Access members with "." (dot) because K is an object, not a pointer.
    //
    // When K goes out of scope (at "return 0" or the closing } of main),
    // the destructor ~List() runs automatically. No manual delete needed.
    List K = List();                 // K is a stack-allocated List object
    for (int i = 100; i < 200; i++)
    {
        K.attach(i);                 // . used because K is an object
    }
    cout << "K: \n";
    K.print();

    // K's destructor runs automatically here when K goes out of scope.
    // All 100 Nodes inside K are cleaned up — no leak.

    // Key syntax recap:
    //   new Type        — allocate on heap, return pointer (you must delete)
    //   Type var        — allocate on stack (auto-cleaned)
    //   ptr->member     — access member through pointer (same as (*(ptr)).member)
    //   obj.member      — access member of object directly
    //   delete ptr      — free heap memory, triggers destructor
    //   nullptr         — special value: "points to nothing"

    return 0;
}