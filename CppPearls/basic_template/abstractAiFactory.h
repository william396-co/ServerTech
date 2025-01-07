#pragma once

#include <type_traits>
#include <concepts>

template<typename...Ts>
struct AbstractorAIFActory
	: Ts...{using Ts::operator()...;};

template<typename...Ts> AbstractorAIFActory(Ts...) -> AbstractorAIFActory<Ts...>;

template<typename T,typename U>
concept IsAbstractAI = std::same_as<T, U>;


template<typename T>
static constexpr auto AIFactory = AbstractorAIFActory{
	[]()requires IsAbstractAI<T,Lux> {
		return new LuxEasy;}

	[]()requires IsAbstractAI<T,Ziggs> {
		return new ZiggsEasy;}

	[]()requires IsAbstractAI<T,Teemo> {
		return new TeemoEasy;}
};


#define CONCRETE_AI_FACTORY(x) \
	[]() requires IsAbstractAI<T,x>{ \
	return new GMP_CONCAT(x,Easy);}