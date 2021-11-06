#ifndef ARRAY_H
#define ARRAY_H

#include <type_traits>
#include <utility>
#include <iterator>
#include <exception>

namespace simple
{

using size_type = std::size_t;

template <typename T>
class array_iterator
{
public:
	using value_type = T;
	using pointer = value_type*;
	using const_pointer = const value_type*;
	using reference = value_type&;
	using const_reference = const value_type&;
	using iterator_category = std::random_access_iterator_tag;
	using difference_type = std::ptrdiff_t;

	array_iterator() = default;
	constexpr explicit array_iterator(pointer ptr) : m_ptr(ptr) {}

	constexpr array_iterator& operator++()
	{
		m_ptr++;
		return *this;
	}

	constexpr array_iterator operator++(int)
	{
		array_iterator it(*this);
		++m_ptr;
		return it;
	}

	constexpr array_iterator& operator--()
	{
		m_ptr--;
		return *this;
	}

	constexpr array_iterator operator--(int)
	{
		array_iterator it(*this);
		--m_ptr;
		return it;
	}

	constexpr array_iterator& operator+=(size_type offset)
	{
		m_ptr += offset;
		return *this;
	}

	constexpr array_iterator& operator-=(size_type offset)
	{
		m_ptr -= offset;
		return *this;
	}

	constexpr array_iterator operator+(size_type offset) const
	{
		array_iterator it(*this);
		return it += offset;
	}

	constexpr array_iterator operator-(size_type offset) const
	{
		array_iterator it(*this);
		return it -= offset;
	}

	difference_type operator-(const array_iterator& other) const { return m_ptr - other.m_ptr; }

	constexpr reference operator[](size_type index) { return *(m_ptr + index); }
	constexpr const_reference operator[](size_type index) const { return *(m_ptr + index); }

	constexpr pointer operator->() { return m_ptr; }
	constexpr const_pointer operator->() const { return m_ptr; }

	constexpr reference operator*() { return *m_ptr; }
	constexpr const_reference operator*() const { return *m_ptr; }

	constexpr bool operator==(const array_iterator& other) const { return m_ptr == other.m_ptr; }
	constexpr bool operator!=(const array_iterator& other) const { return m_ptr != other.m_ptr; }
	constexpr bool operator<(const array_iterator& other) const { return m_ptr < other.m_ptr; }
	constexpr bool operator<=(const array_iterator& other) const { return m_ptr <= other.m_ptr; }
	constexpr bool operator>(const array_iterator& other) const { return m_ptr > other.m_ptr; }
	constexpr bool operator>=(const array_iterator& other) const { return m_ptr >= other.m_ptr; }

private:
	pointer m_ptr = nullptr;
};

template <class T, size_type N>
struct array
{
	using value_type = T;
	using iterator = array_iterator<T>;
	using pointer = value_type*;
	using reference = value_type&;
	using const_reference = const value_type&;
	using iterator_category = std::random_access_iterator_tag;
	using difference_type = std::ptrdiff_t;

	constexpr reference operator[](size_type pos) { return m_elements[pos]; }
	constexpr const_reference operator[](size_type pos) const { return m_elements[pos]; }

	[[nodiscard]] constexpr reference at(size_type pos)
	{
		if (pos < N)
			return m_elements[pos];

		throw std::out_of_range("Index out of range");
	}

	[[nodiscard]] constexpr const_reference at(size_type pos) const
	{
		if (pos < N)
			return m_elements[pos];

		throw std::out_of_range("Index out of range");
	}

	[[nodiscard]] constexpr iterator begin() noexcept { return iterator(m_elements); }
	[[nodiscard]] constexpr iterator begin() const noexcept { return iterator(m_elements); }
	[[nodiscard]] constexpr iterator cbegin() const noexcept { return iterator(m_elements); }

	[[nodiscard]] constexpr iterator end() noexcept { return iterator(m_elements + N); }
	[[nodiscard]] constexpr iterator end() const noexcept { return iterator(m_elements + N); }
	[[nodiscard]] constexpr iterator cend() const noexcept { return iterator(m_elements + N); }

	[[nodiscard]] constexpr reference front() { return m_elements[0]; }
	[[nodiscard]] constexpr const_reference front() const { return m_elements[0]; }

	[[nodiscard]] constexpr reference back() { return m_elements[N - 1]; }
	[[nodiscard]] constexpr const_reference back() const { return m_elements[N - 1]; }

	[[nodiscard]] constexpr T* data() noexcept { return m_elements; }
	[[nodiscard]] constexpr const T* data() const noexcept { return m_elements; }

	[[nodiscard]] constexpr bool empty() const { return false; }
	[[nodiscard]] constexpr size_type size() const { return N; }

	void fill(const T& value)
	{
		for (auto& i : *this)
		{
			i = value;
		}
	}

	T m_elements[N];
};

template <class T>
struct array<T, 0>
{
	using value_type = T;
	using iterator = array_iterator<T>;
	using pointer = value_type*;
	using reference = value_type&;
	using const_reference = const value_type&;
	using iterator_category = std::random_access_iterator_tag;
	using difference_type = std::ptrdiff_t;

	constexpr reference operator[](size_type /*pos*/) { return m_elements[0]; }
	constexpr const_reference operator[](size_type /*pos*/) const { return m_elements[0]; }

	[[nodiscard]] constexpr reference at(size_type /*pos*/)
	{
		throw std::out_of_range("Index out of range");
	}

	[[nodiscard]] constexpr const_reference at(size_type /*pos*/) const
	{
		throw std::out_of_range("Index out of range");
	}

	[[nodiscard]] constexpr iterator begin() noexcept { return iterator(); }
	[[nodiscard]] constexpr iterator begin() const noexcept { return iterator(); }
	[[nodiscard]] constexpr iterator cbegin() const noexcept { return iterator(); }

	[[nodiscard]] constexpr iterator end() noexcept { return iterator(); }
	[[nodiscard]] constexpr iterator end() const noexcept { return iterator(); }
	[[nodiscard]] constexpr iterator cend() const noexcept { return iterator(); }

	[[nodiscard]] constexpr reference front() { return m_elements[0]; }
	[[nodiscard]] constexpr const_reference front() const { return m_elements[0]; }

	[[nodiscard]] constexpr reference back() { return m_elements[0]; }
	[[nodiscard]] constexpr const_reference back() const { return m_elements[0]; }

	[[nodiscard]] constexpr T* data() noexcept { return nullptr; }
	[[nodiscard]] constexpr const T* data() const noexcept { return nullptr; }

	[[nodiscard]] constexpr bool empty() const { return true; }
	[[nodiscard]] constexpr size_type size() const { return 0; }

	void fill(const T& /*value*/) const {}

	T m_elements[1];
};

} // namespace simple

#endif // ARRAY_H
