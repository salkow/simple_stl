#include "hash_function.h"

using simple::string;

std::size_t simple::hash<int>::operator()(int value) const { return static_cast<size_t>(value); }

std::size_t simple::hash<unsigned int>::operator()(unsigned int value) const
{
	return static_cast<size_t>(value);
}

std::size_t simple::hash<string>::operator()(const string& value) const
{
	std::size_t h = 0;
	for (char letter : value)
		h = (h << 6) ^ (h >> 26) ^ static_cast<std::size_t>(letter);

	return h;
}

std::size_t simple::hash<string*>::operator()(const string* value) const
{
	hash<string> non_pointer_hash;
	return non_pointer_hash(*value);
}
