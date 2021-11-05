#ifndef VECTOR_H
#define VECTOR_H

#include <type_traits>
#include <utility>
#include <iterator>
#include <exception>

namespace simple
{

constexpr std::size_t CAPACITY_INCREASE_FACTOR = 2;
using size_type = std::size_t;

template <typename T>
class vector_iterator
{
public:
	using value_type = T;
	using pointer = value_type*;
	using const_pointer = const value_type*;
	using reference = value_type&;
	using const_reference = const value_type&;
	using iterator_category = std::random_access_iterator_tag;
	using difference_type = std::ptrdiff_t;

	vector_iterator() = default;
	constexpr explicit vector_iterator(pointer ptr) : m_ptr(ptr) {}

	constexpr vector_iterator& operator++()
	{
		m_ptr++;
		return *this;
	}

	constexpr vector_iterator operator++(int)
	{
		vector_iterator it(*this);
		++m_ptr;
		return it;
	}

	constexpr vector_iterator& operator--()
	{
		m_ptr--;
		return *this;
	}

	constexpr vector_iterator operator--(int)
	{
		vector_iterator it(*this);
		--m_ptr;
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
		vector_iterator it(*this);
		return it += offset;
	}

	constexpr vector_iterator operator-(size_type offset) const
	{
		vector_iterator it(*this);
		return it -= offset;
	}

	difference_type operator-(const vector_iterator& other) const { return m_ptr - other.m_ptr; }

	constexpr reference operator[](size_type index) { return *(m_ptr + index); }
	constexpr const_reference operator[](size_type index) const { return *(m_ptr + index); }

	constexpr pointer operator->() { return m_ptr; }
	constexpr const_pointer operator->() const { return m_ptr; }

	constexpr reference operator*() { return *m_ptr; }
	constexpr const_reference operator*() const { return *m_ptr; }

	constexpr bool operator==(const vector_iterator& other) const { return m_ptr == other.m_ptr; }
	constexpr bool operator!=(const vector_iterator& other) const { return m_ptr != other.m_ptr; }
	constexpr bool operator<(const vector_iterator& other) const { return m_ptr < other.m_ptr; }
	constexpr bool operator<=(const vector_iterator& other) const { return m_ptr <= other.m_ptr; }
	constexpr bool operator>(const vector_iterator& other) const { return m_ptr > other.m_ptr; }
	constexpr bool operator>=(const vector_iterator& other) const { return m_ptr >= other.m_ptr; }

private:
	pointer m_ptr = nullptr;
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

	constexpr explicit vector(size_type num_of_elements, const T& value)
	{
		reallocate(num_of_elements);
		for (size_type i = 0; i < num_of_elements; ++i)
			emplace_back(value);
	}

	constexpr explicit vector(size_type num_of_elements)
	{
		reallocate(num_of_elements);
		for (size_type i = 0; i < num_of_elements; ++i)
			emplace_back(T());
	}

	constexpr vector(vector&& other) noexcept :
		m_elements(std::exchange(other.m_elements, nullptr)),
		m_capacity(std::exchange(other.m_capacity, 0)), m_size(std::exchange(other.m_size, 0))
	{
	}

	constexpr vector(vector const& other) { copy(other); }

	~vector()
	{
		destruct_elements();
		::operator delete(m_elements);
	}

	constexpr vector& operator=(vector const& other)
	{
		if (this == &other)
			return *this;

		destruct_elements();

		copy(other);
		return *this;
	}

	constexpr vector& operator=(vector&& other) noexcept
	{
		destruct_elements();
		::operator delete(m_elements);

		m_elements = std::exchange(other.m_elements, nullptr);
		m_capacity = std::exchange(other.m_capacity, 0);
		m_size = std::exchange(other.m_size, 0);

		return *this;
	}

	constexpr bool operator==(vector const& other) const
	{
		if (m_size != other.m_size)
			return false;

		auto this_it = this->begin();
		auto other_it = other.begin();

		for (; this_it != this->end(); ++this_it, ++other_it)
		{
			if (*this_it != *other_it)
				return false;
		}

		return true;
	}

	constexpr bool operator!=(vector const& other) const { return !(*this == other); }

	constexpr reference operator[](size_type pos) noexcept { return m_elements[pos]; }
	constexpr const_reference operator[](size_type pos) const noexcept { return m_elements[pos]; }

	[[nodiscard]] constexpr reference at(size_type pos)
	{
		if (pos < m_size)
			return m_elements[pos];

		throw std::out_of_range("Index out of range");
	}

	[[nodiscard]] constexpr const_reference at(size_type pos) const
	{
		if (pos < m_size)
			return m_elements[pos];

		throw std::out_of_range("Index out of range");
	}

	constexpr void reserve(size_type new_cap)
	{
		if (m_capacity < new_cap)
			reallocate(new_cap);
	}

	constexpr void push_back(const T& value) { emplace_back(value); }

	constexpr void push_back(T&& value) { emplace_back(std::move(value)); }

	template <class... Args>
	constexpr reference emplace_back(Args&&... args)
	{
		if (m_size == m_capacity)
			reallocate(get_increased_capacity());

		new (&m_elements[m_size]) T(std::forward<Args>(args)...);

		return m_elements[m_size++];
	}

	constexpr void insert(iterator first, iterator last)
	{
		size_type new_capacity = static_cast<size_type>(last - first) + m_size;

		reserve(new_capacity);

		for (; first != last; first++)
			emplace_back(*first);
	}

	constexpr void pop_back() noexcept
	{
		--m_size;
		m_elements[m_size].~T();
	}

	constexpr void clear() noexcept
	{
		destruct_elements();
		m_size = 0;
	}

	[[nodiscard]] constexpr iterator begin() noexcept { return iterator(m_elements); }
	[[nodiscard]] constexpr iterator begin() const noexcept { return iterator(m_elements); }
	[[nodiscard]] constexpr iterator cbegin() const noexcept { return iterator(m_elements); }

	[[nodiscard]] constexpr iterator end() noexcept { return iterator(m_elements + m_size); }
	[[nodiscard]] constexpr iterator end() const noexcept { return iterator(m_elements + m_size); }
	[[nodiscard]] constexpr iterator cend() const noexcept { return iterator(m_elements + m_size); }

	[[nodiscard]] constexpr reference front() noexcept { return m_elements[0]; }
	[[nodiscard]] constexpr const_reference front() const noexcept { return m_elements[0]; }

	[[nodiscard]] constexpr reference back() noexcept { return m_elements[m_size - 1]; }
	[[nodiscard]] constexpr const_reference back() const noexcept { return m_elements[m_size - 1]; }

	[[nodiscard]] constexpr pointer data() noexcept { return m_elements; }
	[[nodiscard]] constexpr const_pointer data() const noexcept { return m_elements; }

	[[nodiscard]] constexpr size_type size() const noexcept { return m_size; }
	[[nodiscard]] constexpr size_type capacity() const noexcept { return m_capacity; }
	[[nodiscard]] constexpr size_type empty() const noexcept { return m_size == 0; }

private:
	constexpr void reallocate(size_type new_cap)
	{
		T* new_block = allocate_new_blocks(new_cap);
		transfer_items_to_new_block(new_block);

		destruct_elements();
		::operator delete(m_elements);

		m_elements = new_block;
		m_capacity = new_cap;
	}

	constexpr void transfer_items_to_new_block(T* new_block)
	{
		if constexpr (std::is_nothrow_move_constructible_v<T>)
		{
			for (size_type i = 0; i < m_size; ++i)
				new (&new_block[i]) T(std::move(m_elements[i]));
		}

		else
		{
			for (size_type i = 0; i < m_size; ++i)
				new (&new_block[i]) T(m_elements[i]);
		}
	}

	constexpr void copy(vector const& other)
	{
		if (m_capacity < other.m_size)
		{
			::operator delete(m_elements);
			m_elements = allocate_new_blocks(other.m_size);
			m_capacity = other.m_size;
		}

		m_size = other.m_size;

		for (size_type i = 0; i < m_size; ++i)
			new (&m_elements[i]) T(other.m_elements[i]);
	}

	constexpr void destruct_elements() const
	{
		if constexpr (!std::is_trivially_destructible_v<T>)
		{
			for (auto& i : *this)
				i.~T();
		}
	}

	[[nodiscard]] constexpr size_type get_increased_capacity() const
	{
		return m_capacity * CAPACITY_INCREASE_FACTOR + 1;
	}

	[[nodiscard]] constexpr T* allocate_new_blocks(size_type size) const
	{
		return static_cast<T*>(::operator new(sizeof(T) * size));
	}

	T* m_elements = nullptr;
	size_type m_capacity = 0;
	size_type m_size = 0;
};

} // namespace simple

#endif // VECTOR_H
