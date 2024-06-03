#pragma once

#include <memory>

namespace NSVarTypeDict {
struct NullParameter;

template<size_t N, template<typename...> class TCont, typename... T>
struct Create_
{
    using type = typename Create_<N - 1, TCont, NullParameter, T...>::type;
};
template<template<typename...> class TCont, typename... T>
struct Create_<0, TCont, T...>
{
    using type = TCont<T...>;
};

template<typename TVal, size_t N, size_t M, typename TProcessTypes, typename... TRemainTypes>
struct NewTupleType_;

template<typename TVal, size_t N, size_t M, template<typename...> class TCont,
    typename... TModifiedTypes,
    typename TCurType,
    typename... TRemainTypes>
struct NewTupleType_<Tval, N, M, TCont<TModifiedTypes...>, TCurType, TRemainTypes...>
{
    using type = typename NewTupleType_<TVal, N, M + 1, TCont<TModifiedTypes..., TCurType>, TRemainTypes...>::type;
};

template<typename TVal, size_t N, template<typename...> class TCont,
    typename... TModifiedTypes,
    typename TCurType,
    typename... TRemainTypes>
struct NewTupleType_<Tval, N, N, TCont<TModifiedTypes...>, TCurType, TRemainTypes...>
{
    using type = TCont<TModifiedTypes..., TVal, TRemainTypes...>;
};

template<typename TVal, size_t TagPos, typename TCont, typename... TRemainTypes>
using NewTupleType = typename NewTupleType_<TVal, TagPos, 0, TCont, TRemainTypes...> type;

template<typename... TParameters>
struct VarTypeDict
{
    template<typename... TTypes>
    struct Values
    {

        Values() = default;

        Values( std::shared_ptr<void> ( &&input )[sizeof...( TTypes )] )
        {
            for ( size_t i = 0; i != sizeof...( TTypes ); ++i ) {
                m_tuple[i] = std::move( input[i] );
            }
        }

        template<typename TTag, typename TVal>
        auto Set( TVal && val ) &&
        {
            using namespace NSMultiTypeDict;
            constexpr static size_t TagPos = Tag2ID<TTag, TParameters...>;
            using rawVal = std::decay_t<TVal>;
            rawVal * tmp = new rawVal( std::forward<TVal>( val ) );
            m_tuple[TagPos] = std::shared_ptr<void>( tmp, []( void * ptr ) {
                rawVal * nptr = static_cast<rawVal *>( ptr );
                delete nptr;
            } );

            using new_type = NewTupleType<rawVal, TagPos, Values<>, TTypes...>;
            return new_type( std::move( m_tuple ) );
        }
        template<typename TTag>
        const auto & Get() const;

    private:
        std::shared_ptr<void> m_tuple[sizeof...( TTypes )];
    };

    public： static auto Create()
    {
        using namespace NSVarTypeDict;
        using type = typename Create_<sizeof...( TParameters ), Values>::type;
        return type {};
    }
};

} // namespace NSVarTypeDict
