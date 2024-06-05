#include <iostream>

#include <type_traits>

template<bool is_ref> struct add_const_ref_impl;
template<typename T>
struct add_const_ref
{
    bool const is_ref = std::is_reference_v<T>;
    using type = add_const_ref_impl<is_ref>;
};

template<>
struct add_const_ref_impl<true>
{
    template<typename T>
    static void do_it( T t )
    {
        return T const &;
    }
};

template<false, typename T>
struct add_const_ref_imp<false, T>
{
    typedef T const & type;
};

void exe1()
{
    std::cout << __FUNCTION__ << "\n";

    std::cout << std::is_same_v<add_const_ref<int>, const int &> << "\n";
}

int main()
{

    exe1();

    return 0;
}
