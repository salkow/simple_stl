#ifndef VEC_H
#define VEC_H

#include <algorithm>
#include <type_traits>

namespace simple
{

constexpr float CAPACITY_INCREASE_FACTOR = 1.5;
using size_type = std::size_t;

template <typename T>
class vector_iterator
{
public:
	using difference_type = T;
	using value_type = T;
	using pointer = value_type*;
	using reference = value_type&;
	using const_reference = value_type const&;
	using iterator_category = std::forward_iterator_tag;

	constexpr explicit vector_iterator(pointer ptr) : m_ptr(ptr) {}

	constexpr vector_iterator& operator++()
	{
		m_ptr++;
		return *this;
	}

	constexpr vector_iterator operator++(int)
	{
		vector_iterator it = *this;
		++(*this);
		return it;
	}

	constexpr vector_iterator& operator--()
	{
		m_ptr--;
		return *this;
	}

	constexpr vector_iterator operator--(int)
	{
		vector_iterator it = *this;
		--(*this);
		return it;
	}

	constexpr vector_iterator& operator+=(size_type offset)
	{
		m_ptr += offset;
		return *this;
	}

	constexpr vector_iterator& operator-=(size_type offset)
	{
		m_ptr -= offset;
		return *this;
	}

	constexpr vector_iterator operator+(size_type offset) const
	{
		vector_iterator it = *this;
		return it += offset;
	}

	constexpr vector_iterator operator-(size_type offset) const
	{
		vector_iterator it = *this;
		return it -= offset;
	}

	constexpr reference operator[](size_type index) { return *(m_ptr + index); }
	constexpr const_reference operator[](size_type index) const { return *(m_ptr + index); }

	constexpr pointer operator->() { return m_ptr; }
	constexpr pointer operator->() const { return m_ptr; }

	constexpr reference operator*() { return *m_ptr; }
	constexpr const_reference operator*() const { return *m_ptr; }

	constexpr bool operator==(const vector_iterator& other) const { return m_ptr == other.m_ptr; }
	constexpr bool operator!=(const vector_iterator& other) const { return m_ptr != other.m_ptr; }
	constexpr bool operator<(const vector_iterator& other) const { return m_ptr < other.m_ptr; }
	constexpr bool operator<=(const vector_iterator& other) const { return m_ptr <= other.m_ptr; }
	constexpr bool operator>(const vector_iterator& other) const { return m_ptr > other.m_ptr; }
	constexpr bool operator>=(const vector_iterator& other) const { return m_ptr >= other.m_ptr; }

private:
	pointer m_ptr;
};

template <class T>
class vector
{
public:
	using value_type = T;
	using iterator = vector_iterator<T>;
	using pointer = T*;
	using const_pointer = T const*;
	using reference = value_type&;
	using const_reference = value_type const&;

	constexpr explicit vector() = default;

	constexpr explicit vector(size_type num_of_elements, const T& value = T())
	{
		reallocate(num_of_elements);
		for (size_type i = 0; i < num_of_elements; ++i)
			emplace_back(value);
	}

	constexpr vector(vector&& other) noexcept { move(std::move(other)); }

	constexpr vector& operator=(vector&& other) noexcept
	{
		move(std::move(other));

		return *this;
	}

	constexpr vector(vector const& other) { copy(other); }

	constexpr vector& operator=(vector const& other)
	{
		if (this == &other)
			return *this;

		copy(other);
		return *this;
	}

	~vector()
	{
		clear();
		::operator delete(m_elements);
	}

	constexpr void reserve(size_type new_cap)
	{
		if (m_capacity < new_cap)
			reallocate(new_cap);
	}

	constexpr void swap(vector& other) noexcept
	{
		std::swap(m_capacity, other.m_capacity);
		std::swap(m_size, other.m_size);
		std::swap(m_elements, other.m_elements);
	}

	constexpr void push_back(const T& value) { emplace_back(value); }

	constexpr void push_back(T&& value) { emplace_back(std::move(value)); }

	template <typename... Args>
	constexpr reference emplace_back(Args&&... args)
	{
		if (m_size == m_capacity)
			reallocate(get_increased_capacity());

		new (&m_elements[m_size]) T(std::forward<Args>(args)...);

		return m_elements[m_size++];
	}

	constexpr void pop_back()
	{
		--m_size;
		m_elements[m_size].~T();
	}

	constexpr reference operator[](size_type pos) { return m_elements[pos]; }
	constexpr const_reference operator[](size_type pos) const { return m_elements[pos]; }

	constexpr void clear()
	{
		destruct_elements();
		m_size = 0;
	}

	constexpr bool operator==(vector const& other) const
	{
		return (m_size == other.m_size) && std::equal(begin(), end(), other.begin());
	}
	constexpr bool operator!=(vector const& other) const { return !(*this == other); }

	constexpr iterator begin() { return iterator(m_elements); }
	constexpr iterator begin() const { return iterator(m_elements); }

	constexpr iterator end() { return iterator(m_elements + m_size); }
	constexpr iterator end() const { return iterator(m_elements + m_size); }

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
	constexpr void reallocate(size_type new_cap)
	{
		T* new_block = allocate_new_T_block(new_cap);
		transfer_items_to_new_block<T>(new_block);

		destruct_elements();
		::operator delete(m_elements);

		m_elements = new_block;
		m_capacity = new_cap;
	}

	template <typename X>
	typename std::enable_if<!std::is_move_assignable<X>::value>::
		type constexpr transfer_items_to_new_block(X* new_block) const
	{
		for (size_type i = 0; i < m_capacity; ++i)
			new (&new_block[i]) T(m_elements[i]);
	}

	template <typename X>
	typename std::enable_if<
		std::is_move_assignable<X>::value>::type constexpr transfer_items_to_new_block(X* new_block)
	{
		for (size_type i = 0; i < m_capacity; ++i)
			new (&new_block[i]) T(std::move(m_elements[i]));
	}

	constexpr void move(vector&& other)
	{
		m_capacity = std::exchange(other.m_capacity, 0);
		m_elements = std::exchange(other.m_elements, nullptr);
		m_size = std::exchange(other.m_size, 0);
	}

	constexpr void copy(vector const& other)
	{
		destruct_elements();

		if (m_capacity < other.m_size)
		{
			::operator delete(m_elements);
			m_elements = allocate_new_T_block(other.m_size);
			m_capacity = other.m_size;
		}

		m_size = other.m_size;

		for (size_type i = 0; i < m_size; ++i)
			new (&m_elements[i]) T(other.m_elements[i]);
	}

	constexpr void destruct_elements()
	{
		for (size_type i = 0; i < m_size; i++)
			m_elements[i].~T();
	}

	constexpr size_type get_increased_capacity() const
	{
		return m_capacity * CAPACITY_INCREASE_FACTOR + 1;
	}

	constexpr T* allocate_new_T_block(size_type size) const
	{
		return static_cast<T*>(::operator new(sizeof(T) * size));
	}

	size_type m_capacity = 0;
	size_type m_size = 0;
	T* m_elements = nullptr;
};

} // namespace simple

#endif // VEC_HPP
