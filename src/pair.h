#ifndef PAIR_H
#define PAIR_H

#include <utility>

namespace simple
{

template <class T1, class T2>
struct pair
{
	pair() = default;
	pair(T1 x, T2 y) : first(std::move(x)), second(std::move(y)) {}

	T1 first;
	T2 second;

	constexpr friend bool operator==(const std::pair<T1, T2>& lhs, const std::pair<T1, T2>& rhs)
	{
		return lhs.first == rhs.first && lhs.second == rhs.second;
	}

	constexpr friend bool operator!=(const std::pair<T1, T2>& lhs, const std::pair<T1, T2>& rhs)
	{
		return !(lhs == rhs);
	}
};

} // namespace simple

#endif // PAIR_H
