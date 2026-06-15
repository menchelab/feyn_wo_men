// #include pulls in code from another file so we can use it here.
// <iostream> gives us cout (console output) and other input/output tools.
#include <iostream>
using namespace std;  // So we can write "cout" instead of "std::cout"

// -------------------------------------------------------
// PASS-BY-VALUE: The function gets its own COPY of a and b.
// Changing the copy does NOT change the original variable.
// -------------------------------------------------------
int sum(int a, int b)
{
    a = a + b;   // Modifies local copy of 'a', original untouched
    return a;    // Returns the sum (caller must capture it)
}

// -------------------------------------------------------
// PASS-BY-POINTER: The function receives the MEMORY ADDRESS
// of the original variables. By dereferencing (*), we can
// read and modify the originals directly.
//
// int* a  means: "a is a pointer — it holds a memory address,
//                 and at that address lives an int"
// *(a)    means: "go to the address stored in a and grab the value there"
// -------------------------------------------------------
int sum_ptr(int* a, int* b)
{
    *(a) = *(a) + *(b);  // Read values at both addresses, add them,
                         // write result back to address a points to
    return *(a);         // Return the value at address a
}


int main()
{
    // --- Stack variables ---
    // These live on "the stack" — fast, automatic lifetime.
    // They die when the function ends (here: when main() returns).
    int a = 10;
    int b = 1;

    // Pass-by-value: sum() gets copies of a and b.
    // a stays 10, b stays 1. We capture the return value in c.
    int c = sum(a, b);
    // c = 11, a = 10, b = 1 (a and b unchanged!)

    // Pass-by-pointer: sum_ptr() gets the ADDRESSES of a and b.
    // &a means "the memory address where a lives".
    // sum_ptr uses *(a) to reach into main()'s variables and change them.
    int d = sum_ptr(&a, &b);
    // d = 11, a = 11, b = 1  (a was changed through its pointer!)

    // Key syntax recap:
    //   int* p   — declare a pointer-to-int named p
    //   &x       — get the memory address of variable x
    //   *(p)     — dereference: go to address p and access the value there
    //   *p       — same as *(p) (operator precedence rarely matters here)

    return 0;
}