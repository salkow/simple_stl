#ifndef SHARED_PTR_H
#define SHARED_PTR_H

#include <utility>

namespace simple
{

template <typename T>
class shared_ptr
{
public:
	using pointer = T*;

	constexpr shared_ptr() noexcept = default;
	constexpr explicit shared_ptr(std::nullptr_t) noexcept {}

	constexpr explicit shared_ptr(T* p) : m_data(p), m_count(new int64_t(1)) {}

	shared_ptr(shared_ptr&& u) noexcept :
		m_data(std::exchange(u.m_data, nullptr)), m_count(std::exchange(u.m_count, nullptr))
	{
	}

	shared_ptr& operator=(shared_ptr&& u) noexcept
	{
		decrement_and_delete_if_last();

		m_data = std::exchange(u.m_data, nullptr);
		m_count = std::exchange(u.m_count, nullptr);
		return *this;
	}

	shared_ptr(const shared_ptr& p) : m_data(p.m_data), m_count(p.m_count)
	{
		if (m_count)
			++(*m_count);
	}

	shared_ptr& operator=(const shared_ptr& p)
	{
		if (this == &p || m_count == p.m_count)
			return *this;

		decrement_and_delete_if_last();

		m_data = p.m_data;
		m_count = p.m_count;

		if (m_count)
			(*m_count)++;

		return *this;
	}

	~shared_ptr() { decrement_and_delete_if_last(); }

	T& operator*() const noexcept { return *m_data; }
	pointer operator->() const noexcept { return m_data; }

	explicit operator bool() const noexcept { return m_data; }

	[[nodiscard]] int64_t use_count() const noexcept { return m_count ? *m_count : 0; }

	[[nodiscard]] pointer get() const noexcept { return m_data; }

	void reset()
	{
		decrement_and_delete_if_last();
		m_data = nullptr;
		m_count = nullptr;
	}

	void reset(T* p)
	{
		decrement_and_delete_if_last();
		m_data = p;
		m_count = new int64_t(1);
	}

private:
	void decrement_and_delete_if_last()
	{
		if (!m_count)
			return;

		--(*m_count);

		if (*m_count == 0)
		{
			delete m_data;
			delete m_count;
		}
	}

	T* m_data = nullptr;
	int64_t* m_count = nullptr;
};

} // namespace simple

#endif // SHARED_PTR_H
