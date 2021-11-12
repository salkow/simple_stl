#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <cstddef>

#include "pair.h"

namespace simple
{
template <class InputIt, class UnaryFunction>
UnaryFunction for_each(InputIt first, InputIt last, UnaryFunction f)
{
	for (; first != last; ++first)
		f(*first);

	return f;
}

template <class InputIt, class Size, class UnaryFunction>
InputIt for_each_n(InputIt first, Size n, UnaryFunction f)
{
	for (Size i = 0; i < n; ++i, first++)
		f(*first);

	return first;
}

template <class InputIt, class UnaryPredicate>
bool all_of(InputIt first, InputIt last, UnaryPredicate p)
{
	for (; first != last; ++first)
	{
		if (!p(*first))
			return false;
	}

	return true;
}

template <class InputIt, class UnaryPredicate>
bool any_of(InputIt first, InputIt last, UnaryPredicate p)
{
	for (; first != last; ++first)
	{
		if (p(*first))
			return true;
	}

	return false;
}

template <class InputIt, class UnaryPredicate>
bool none_of(InputIt first, InputIt last, UnaryPredicate p)
{
	for (; first != last; ++first)
	{
		if (p(*first))
			return false;
	}

	return true;
}

template <class InputIt, class T>
typename std::ptrdiff_t count(InputIt first, InputIt last, const T& value)
{
	std::ptrdiff_t sum = 0;

	for (; first != last; ++first)
	{
		if (*first == value)
			sum++;
	}

	return sum;
}

template <class InputIt, class UnaryPredicate>
std::ptrdiff_t count_if(InputIt first, InputIt last, UnaryPredicate p)
{
	std::ptrdiff_t sum = 0;

	for (; first != last; ++first)
	{
		if (p(*first))
			sum++;
	}

	return sum;
}

template <class InputIt1, class InputIt2>
pair<InputIt1, InputIt2> mismatch(InputIt1 first1, InputIt1 last1, InputIt2 first2)
{
	while (first1 != last1 && *first1 == *first2)
	{
		first1++;
		first2++;
	}

	return pair(first1, first2);
}

template <class InputIt1, class InputIt2>
pair<InputIt1, InputIt2> mismatch(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2)
{
	while (first1 != last1 && first2 != last2 && *first1 == *first2)
	{
		first1++;
		first2++;
	}

	return pair(first1, first2);
}

template <class InputIt1, class InputIt2, class BinaryPredicate>
pair<InputIt1, InputIt2> mismatch(InputIt1 first1, InputIt1 last1, InputIt2 first2,
								  BinaryPredicate p)
{
	while (first1 != last1 && p(*first1, *first2))
	{
		first1++;
		first2++;
	}

	return pair(first1, first2);
}

template <class InputIt1, class InputIt2, class BinaryPredicate>
pair<InputIt1, InputIt2> mismatch(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2,
								  BinaryPredicate p)
{
	while (first1 != last1 && first2 != last2 && p(*first1, *first2))
	{
		first1++;
		first2++;
	}

	return pair(first1, first2);
}

template <class InputIt, class T>
InputIt find(InputIt first, InputIt last, const T& value)
{
	for (; first != last; ++first)
	{
		if (*first == value)
			return first;
	}

	return last;
}

template <class InputIt, class UnaryPredicate>
InputIt find_if(InputIt first, InputIt last, UnaryPredicate p)
{
	for (; first != last; ++first)
	{
		if (p(*first))
			return first;
	}

	return last;
}

template <class InputIt, class UnaryPredicate>
InputIt find_if_not(InputIt first, InputIt last, UnaryPredicate q)
{
	for (; first != last; ++first)
	{
		if (!q(*first))
			return first;
	}

	return last;
}

} // namespace simple

#endif // ALGORITHM_H
