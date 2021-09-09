#ifndef VEC_H
#define VEC_H

#include <algorithm>
#include <type_traits>

namespace Simple
{

const std::size_t DEFAULT_CAPACITY = 2;

template <typename Vec>
class VecIterator
{
public:
	using value_type = typename Vec::value_type;
	using pointer = value_type*;
	using reference = value_type&;
	using const_reference = value_type const&;
	using size_type = std::size_t;

	constexpr explicit VecIterator(pointer ptr) : m_ptr(ptr) {}

	constexpr VecIterator& operator++()
	{
		m_ptr++;
		return *this;
	}

	constexpr VecIterator operator++(int)
	{
		VecIterator it = *this;
		++(*this);
		return it;
	}

	constexpr VecIterator& operator--()
	{
		m_ptr--;
		return *this;
	}

	constexpr VecIterator operator--(int)
	{
		VecIterator it = *this;
		--(*this);
		return it;
	}

	constexpr VecIterator& operator+=(size_type offset)
	{
		m_ptr += offset;
		return *this;
	}

	constexpr VecIterator& operator-=(size_type offset)
	{
		m_ptr -= offset;
		return *this;
	}

	constexpr VecIterator operator+(size_type offset) const
	{
		VecIterator it = *this;
		return it += offset;
	}

	constexpr VecIterator operator-(size_type offset) const
	{
		VecIterator it = *this;
		return it -= offset;
	}

	constexpr reference operator[](size_type index) { return *(m_ptr + index); }
	constexpr const_reference operator[](size_type index) const { return *(m_ptr + index); }

	constexpr pointer operator->() { return m_ptr; }
	constexpr pointer operator->() const { return m_ptr; }

	constexpr reference operator*() { return *m_ptr; }
	constexpr const_reference operator*() const { return *m_ptr; }

	constexpr bool operator==(const VecIterator& other) const { return m_ptr == other.m_ptr; }
	constexpr bool operator!=(const VecIterator& other) const { return !(*this == other); }
	constexpr bool operator<(const VecIterator& other) const { return m_ptr < other.m_ptr; }
	constexpr bool operator<=(const VecIterator& other) const { return m_ptr <= other.m_ptr; }
	constexpr bool operator>(const VecIterator& other) const { return m_ptr > other.m_ptr; }
	constexpr bool operator>=(const VecIterator& other) const { return m_ptr >= other.m_ptr; }

private:
	pointer m_ptr;
};

template <class T>
class Vec
{
public:
	using value_type = T;
	using iterator = T*;
	using const_iterator = T const*;
	using pointer = T*;
	using const_pointer = T const*;
	using reference = value_type&;
	using const_reference = value_type const&;
	using size_type = std::size_t;

	constexpr explicit Vec(size_type capacity = DEFAULT_CAPACITY) :
		m_capacity(capacity), m_elements(static_cast<T*>(::operator new(sizeof(T) * capacity)))
	{
	}

	constexpr Vec(Vec&& other) noexcept : m_capacity(0), m_elements(nullptr)
	{
		move(std::move(other));
	}

	constexpr Vec& operator=(Vec&& other) noexcept
	{
		if (this == &other)
			return *this;

		move(std::move(other));

		return *this;
	}

	constexpr Vec(Vec const& other) : m_capacity(0), m_elements(nullptr) { copy(other); }

	constexpr Vec& operator=(Vec const& other)
	{
		if (this == &other)
			return *this;

		copy(other);
		return *this;
	}

	~Vec()
	{
		clear();
		::operator delete(m_elements);
	}

	constexpr void swap(Vec& other) noexcept
	{
		using std::swap;
		swap(m_capacity, other.m_capacity);
		swap(m_size, other.m_size);
		swap(m_elements, other.m_elements);
	}

	constexpr void push_back(const T& value)
	{
		if (m_size >= m_capacity)
			reallocate(get_increased_capacity());

		new (&m_elements[m_size]) T(value);

		m_size++;
	}

	constexpr void push_back(T&& value)
	{
		if (m_size >= m_capacity)
			reallocate(get_increased_capacity());

		new (&m_elements[m_size]) T(std::move(value));

		m_size++;
	}

	template <typename... Args>
	constexpr reference emplace_back(Args&&... args)
	{
		if (m_size >= m_capacity)
			reallocate(get_increased_capacity());

		new (&m_elements[m_size]) T(std::forward<Args>(args)...);

		return m_elements[m_size++];
	}

	constexpr void pop_back()
	{
		m_size--;
		m_elements[m_size].~T();
	}

	constexpr reference operator[](size_type pos) { return m_elements[pos]; }
	constexpr const_reference operator[](size_type pos) const { return m_elements[pos]; }

	constexpr void clear()
	{
		internal_clear<T>();
		m_size = 0;
	}

	constexpr bool operator==(Vec const& other) const
	{
		return (size() == other.size()) && std::equal(begin(), end(), other.begin());
	}
	constexpr bool operator!=(Vec const& other) const { return !(*this == other); }

	constexpr iterator begin() { return iterator(m_elements); }
	constexpr const_iterator begin() const { return iterator(m_elements); }

	constexpr iterator end() { return iterator(m_elements + m_size); }
	constexpr const_iterator end() const { return iterator(m_elements + m_size); }

	constexpr reference front() { return m_elements[0]; }
	constexpr const_reference front() const { return m_elements[0]; }

	constexpr reference back() { return m_elements[m_size - 1]; }
	constexpr const_reference back() const { return m_elements[m_size - 1]; }

	constexpr pointer data() { return m_elements; }
	constexpr const_pointer data() const { return m_elements; }

	constexpr size_type size() const { return m_size; }
	constexpr size_type capacity() const { return m_capacity; }
	constexpr size_type empty() const { return m_size == 0; }

private:
	constexpr void reallocate(size_type new_capacity)
	{
		auto* new_block = static_cast<T*>(::operator new(new_capacity * sizeof(T)));

		for (size_t i = 0; i < m_capacity; i++)
			new (&new_block[i]) T(std::move(m_elements[i]));

		internal_clear<T>();

		::operator delete(m_elements);

		m_elements = new_block;
		m_capacity = new_capacity;
	}

	constexpr void move(Vec&& other)
	{
		m_capacity = other.m_capacity;
		m_elements = other.m_elements;
		m_size = other.m_size;

		other.m_capacity = 0;
		other.m_elements = nullptr;
		other.m_size = 0;
	}

	constexpr void copy(Vec const& other)
	{
		clear();
		::operator delete(m_elements);

		m_capacity = other.m_capacity;
		m_elements = static_cast<T*>(::operator new(sizeof(T) * m_capacity));
		m_size = 0;

		for (auto& elem : other)
			push_back(elem);
	}

	template <typename X>
	typename std::enable_if<
		!std::is_trivially_destructible<X>::value>::type constexpr internal_clear()
	{
		for (size_type i = 0; i < m_size; i++)
			m_elements[i].~T();
	}

	template <typename X>
	typename std::enable_if<
		std::is_trivially_destructible<X>::value>::type constexpr internal_clear()
	{
	}

	constexpr size_type get_increased_capacity() { return m_capacity + m_capacity / 2 + 1; }

	size_type m_capacity; // Available space for elements.
	size_type m_size = 0; // Number of elements.
	T* m_elements;
};

} // namespace Simple

#endif // VEC_HPP
