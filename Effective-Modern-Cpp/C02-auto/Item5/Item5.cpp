#include <iostream>
#include <vector>
#include <vector>

/*
 *  Item5: Prefer auto to explicit type declarations
 */

class Widget
{};

int main()
{

    // auto variable have their type deduced from their initializer, so they must be initialized.
    int x1;      // potentially uninitizlied
                 // auto x2;     // error! initializer required
    auto x3 = 0; // fine, x's value is well-defined

    // use auto to replace std::function<> 
    // std::function<> uses more memory than auto-declared object
    // std::function<> approach
    auto derefUpLess = []( std::unique_ptr<Widget> const & p1, std::unique_ptr<Widget> const & p2 ) { return *p1 < *p2; };
    // std::function<bool(const std::unique_ptr<Widget>&,const std::unique_ptr<Widget>&)> = []( std::unique_ptr<Widget> const & p1, std::unique_ptr<Widget> const & p2 ) { return *p1 < *p2; };

    return 0;
}
