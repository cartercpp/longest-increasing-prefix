#pragma once

#include <utility>
#include <type_traits>
#include <concepts>
#include <cstddef>

template <auto...>
struct value_sequence;

template <auto FirstValue, auto... OtherValues>
	requires (std::same_as<decltype(FirstValue), decltype(OtherValues)> && ...)
struct value_sequence<FirstValue, OtherValues...>
{
	template <auto Value>
	using append = value_sequence<FirstValue, OtherValues..., Value>;

	static constexpr std::size_t size = 1 + sizeof...(OtherValues);
};

template <typename, auto...>
struct lis_inner;

template <typename Accumulated, auto PrevValue, auto FirstValue, auto... OtherValues>
struct lis_inner<Accumulated, PrevValue, FirstValue, OtherValues...>
{
	using sequence = std::conditional_t<
		(PrevValue < FirstValue),
		lis_inner<typename Accumulated::template append<FirstValue>, FirstValue, OtherValues...>,
		lis_inner<Accumulated, PrevValue>
	>::sequence;
};

template <typename Accumulated, auto PrevValue>
struct lis_inner<Accumulated, PrevValue>
{
	using sequence = Accumulated;
};

template <auto...>
struct lis_outer;

template <auto FirstValue, auto SecondValue, auto... OtherValues>
struct lis_outer<FirstValue, SecondValue, OtherValues...>
{
	using inner_result = typename lis_inner<
		value_sequence<FirstValue>, FirstValue, SecondValue, OtherValues...>::sequence;

	using next_result = typename lis_outer<SecondValue, OtherValues...>::sequence;

	using sequence = std::conditional_t<
		(inner_result::size > next_result::size),
		inner_result,
		next_result
	>;
};

template <auto Value>
struct lis_outer<Value>
{
	using sequence = value_sequence<Value>;
};

template <auto... Values>
using lis = typename lis_outer<Values...>::sequence;
