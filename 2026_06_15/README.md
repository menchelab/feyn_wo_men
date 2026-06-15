# Week 1 (June 15, 2026) — Linked Lists, Pointers & Classes in C++

## Topics Discussed

### Pointer syntax (Simon)

- `int*` — pointer type declaration
- `&` (address-of) — get the memory address of a variable
- `*` (dereference) — access the value at a memory address
- `->` vs `.` — member access through pointer vs object
- Pass-by-value vs pass-by-pointer
- Stack vs heap allocation with `new` and `delete`

### Class syntax (Simon)

- `class` declaration syntax (semicolon after closing brace!)
- `public:` vs `private:` — access control
- Constructors — special methods that run on object creation
- Destructors (`~ClassName`) — special methods that run on object destruction
- Why destructors matter: manual memory management, preventing leaks

### Doubly linked list (Simon)

- `Node` class with `val`, `prev*`, `next*`
- `List` class with `root*`, `tail*`, `len`
- `attach()` — add node to end of list
- `print()` — traverse and print all values

## Code

- [`linked_list.cpp`](./linked_list.cpp) — fully commented doubly linked list implementation
- [`pointer.cpp`](./pointer.cpp) — pass-by-value vs pass-by-pointer demo

## Homework

Implement these functions in `linked_list.cpp` (stubs provided with detailed comments):

1. **`nth_from_start(n)`** — return value of nth node from start (0-indexed)
2. **`nth_from_end(n)`** — return value of nth node from end (0-indexed)
3. **`insert_at(val, n)`** — insert node with value `val` at index `n`
4. **`delete_at(n)`** — remove node at index `n`
5. **`reverse()`** — reverse the list in-place
6. **`swap(i, j)`** — swap nodes at indices `i` and `j`

### Tips

- Draw pointer diagrams on paper before writing code
- Handle edge cases: empty list, single element, first/last element, out-of-bounds indices
- For `delete_at`: save `next` before deleting, or you lose the reference
- For `reverse`: swap `prev` and `next` at every node, then swap `root` and `tail`
