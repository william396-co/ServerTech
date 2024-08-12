#include <iostream>

template<typename F>
class ComplexFunctor : public F
{
public:
    explicit ComplexFunctor( F f )
        : F( f ) {}
};

template<typename F>
ComplexFunctor<F> MakeComplexFunctor( F && f )
{
    return ComplexFunctor<F>( f );
};

int main()
{

    auto const func = MakeComplexFunctor( []() {
        std::cout << "Hello Functor!\n";
    } );

    func();
    return 0;
}
