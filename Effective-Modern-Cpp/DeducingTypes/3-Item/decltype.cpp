#include <iostream>

template<typename Container, typename Index>
auto authAndAccess( Container & c, Index i ) -> decltype( c[i] )
{
    authenticateUser();
    return c[i];
}

int main()
{

    return 0;
}
