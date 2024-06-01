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
};

struct Derive : public Base<Derive>
{
    template<typename TI>
    void Imp( TI const & input )
    {
        std::cout << input << std::endl;
    }
};

int main()
{
    Derive d;
    d.Fun( "Implemantation from derive class" );

    return 0;
}

