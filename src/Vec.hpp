#ifndef VEC_HPP
#define VEC_HPP

#include <stdexcept>
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

	constexpr VecIterator(pointer ptr) : m_ptr(ptr) {}

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

	constexpr Vec(size_type capacity = DEFAULT_CAPACITY) :
		m_capacity(capacity), m_elements(static_cast<T*>(::operator new(sizeof(T) * capacity)))
	{
	}

	Vec(Vec&& other) noexcept : m_capacity(0), m_elements(nullptr)
	{
		m_capacity = other.m_capacity;
		m_elements = other.m_elements;
		m_size = other.m_size;
	}

	Vec& operator=(Vec&& other) noexcept
	{
		m_capacity = other.m_capacity;
		m_elements = other.m_elements;
		m_size = other.m_size;

		return *this;
	}

	~Vec()
	{
		clear();
		::operator delete(m_elements, m_capacity * sizeof(T));
	}

	void swap(Vec& other) noexcept
	{
		using std::swap;
		swap(m_capacity, other.m_capacity);
		swap(m_size, other.m_size);
		swap(m_elements, other.m_elements);
	}

	constexpr void push_back(const T& value)
	{
		if (m_size >= m_capacity)
			realloc(get_increased_capacity());

		new (&m_elements[m_size]) T(value);

		m_size++;
	}

	constexpr void push_back(T&& value)
	{
		if (m_size >= m_capacity)
			realloc(get_increased_capacity());

		new (&m_elements[m_size]) T(std::move(value));

		m_size++;
	}

	template <typename... Args>
	constexpr reference emplace_back(Args&&... args)
	{
		if (m_size >= m_capacity)
			realloc(get_increased_capacity());

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

	constexpr reference at(size_type pos)
	{
		if (pos >= m_size)
			throw std::out_of_range("Index out of range");

		return m_elements[pos];
	}

	constexpr const_reference at(size_type pos) const
	{
		if (pos >= m_size)
			throw std::out_of_range("Index out of range");

		return m_elements[pos];
	}

	constexpr void clear()
	{
		internal_clear<T>();
		m_size = 0;
	}

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
	constexpr void realloc(size_type new_capacity)
	{
		auto new_block = static_cast<T*>(::operator new(new_capacity * sizeof(T)));

		for (size_type i = 0; i < m_capacity; i++)
		{
			new (&new_block[i]) T(std::move(m_elements[i]));
		}

		internal_clear<T>();

		::operator delete(m_elements, m_capacity * sizeof(T));

		m_elements = new_block;
		m_capacity = new_capacity;
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
