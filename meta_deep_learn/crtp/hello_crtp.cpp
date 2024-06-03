#include <iostream>

template<typename D>
struct Base
{
    template<typename TI>
    void Fun( TI const & input )
    {
        D * ptr = static_cast<D *>( this );
        ptr->Imp( input );
    }
    static void Fun2()
    {
        D::Imp2();
    }
};

struct Derive : public Base<Derive>
{
    template<typename TI>
    void Imp( TI const & input )
    {
        std::cout << input << std::endl;
    }

    static void Imp2()
    {
        std::cout << "Static Implemenation from derive class\n";
    }
};

int main()
{
    Derive d;
    d.Fun( "Implemantation from derive class" );

    Derive::Fun2();
    return 0;
}

