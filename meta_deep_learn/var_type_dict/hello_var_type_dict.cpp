#include <iostream>

#include "vartypedict.h"

struct FParams : public VarTypeDirct<A, B, Weight>
{};

template<typename TIn>
float fun( TIn const & in )
{
    auto a = in.template Get<A>();
    auto b = in.template Get<B>();
    auto weight = in.template Get<Weight>();

    return a * weight + b * ( 1 - weight );
}

int main()
{

    std::cerr < fun( FParams::Create()
                         .Set<A>( 1.3f )
                         .Set<B>( 2.4f )
                         .Set<Weight>( 0.1f ) );

    return 0;
}
