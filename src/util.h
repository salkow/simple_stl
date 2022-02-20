#ifndef UTIL_H
#define UTIL_H

template <typename T>
[[nodiscard]] bool compare_values(const T &first, const T &second)
{
	return first == second;
}

template <typename T>
[[nodiscard]] bool compare_values(T *first, T *second)
{
	return *first == *second;
}

namespace simple
{
	template <class InputIt, class T>
	constexpr InputIt find(InputIt first, InputIt last, const T &value)
	{
		for (; first != last; ++first)
		{
			if (*first == value)
				return first;
		}
		return last;
	}

	template <class InputIt, class UnaryPredicate>
	constexpr InputIt find_if(InputIt first, InputIt last, UnaryPredicate p)
	{
		for (; first != last; ++first)
		{
			if (p(*first))
				return first;
		}

		return last;
	}
} // namespace simple

#endif // UTIL_H
