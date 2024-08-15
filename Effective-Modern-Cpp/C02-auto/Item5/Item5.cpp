#include <iostream>
#include <vector>

/*
 *  Item5: Prefer auto to explicit type declarations
 */

int main()
{

    // auto variable have their type deduced from their initializer, so they must be initialized.
    int x1;      // potentially uninitizlied
   // auto x2;     // error! initializer required
    auto x3 = 0; // fine, x's value is well-defined

    return 0;
}
