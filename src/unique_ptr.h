#ifndef UNIQUE_PTR_H
#define UNIQUE_PTR_H

#include <utility>

namespace simple
{

template <typename T>
class unique_ptr
{
public:
	using pointer = T*;

	constexpr unique_ptr() noexcept = default;
	explicit unique_ptr(pointer p) : m_data(p) {}
	constexpr explicit unique_ptr(std::nullptr_t) noexcept {}

	unique_ptr(unique_ptr&& u) noexcept
	{
		reset(u.get());
		u.m_data = nullptr;
	}

	unique_ptr& operator=(unique_ptr&& u) noexcept
	{
		reset(u.get());
		u.m_data = nullptr;
	}

	unique_ptr& operator=(std::nullptr_t) noexcept
	{
		reset();
		return *this;
	}

	unique_ptr(const unique_ptr& p) = delete;
	T& operator=(const unique_ptr& p) = delete;

	~unique_ptr() { delete m_data; }

	pointer get() const noexcept { return m_data; }

	pointer release() noexcept
	{
		pointer tmp = m_data;
		m_data = nullptr;
		return tmp;
	}

	void reset(pointer ptr = pointer()) noexcept
	{
		delete m_data;
		m_data = ptr;
	}

private:
	void move(unique_ptr&& u)
	{
		reset(u.get());
		u.m_data = nullptr;
	}

	T* m_data = nullptr;
};

} // namespace simple

#endif // UNIQUE_PTR_H
