#ifndef SHARED_PTR_H
#define SHARED_PTR_H

#include <utility>

namespace simple
{

class ref_counter
{
public:
	ref_counter() = default;
	virtual ~ref_counter() = default;

	ref_counter(const ref_counter& other) = delete;
	ref_counter(ref_counter&& other) = delete;
	ref_counter& operator=(const ref_counter& other) = delete;
	ref_counter& operator=(ref_counter&& other) = delete;

	int64_t& operator++()
	{
		m_ref_counter++;
		return m_ref_counter;
	}

	int64_t& operator--()
	{
		m_ref_counter--;
		return m_ref_counter;
	}

	[[nodiscard]] int64_t get() const { return m_ref_counter; }

private:
	int64_t m_ref_counter = 1;
};

template <class T>
class control_block_ptr : public ref_counter
{
public:
	explicit control_block_ptr(T* ptr) : m_data(ptr) {}
	~control_block_ptr() override { delete m_data; }

	control_block_ptr(const control_block_ptr& other) = delete;
	control_block_ptr(control_block_ptr&& other) = delete;
	control_block_ptr& operator=(const control_block_ptr& other) = delete;
	control_block_ptr& operator=(control_block_ptr&& other) = delete;

	T* m_data;
};

template <class T>
class control_block_object : public ref_counter
{
public:
	template <class... Args>
	explicit control_block_object(Args&&... args) : m_data(std::forward<Args>(args)...)
	{
	}

	T m_data;
};

template <typename T>
class shared_ptr
{
public:
	using pointer = T*;

	template <class TT, class... Args>
	friend shared_ptr<TT> make_shared(Args&&... args);

	constexpr shared_ptr() noexcept = default;
	constexpr explicit shared_ptr(std::nullptr_t) noexcept {}

	template <class Y>
	constexpr explicit shared_ptr(Y* p)
	{
		auto tmp = new control_block_ptr(p);
		m_data = tmp->m_data;
		m_count = tmp;
	}

	shared_ptr(shared_ptr&& u) noexcept :
		m_data(std::exchange(u.m_data, nullptr)), m_count(std::exchange(u.m_count, nullptr))
	{
	}

	shared_ptr& operator=(shared_ptr&& u) noexcept
	{
		decrement_and_del_if_last();

		m_data = std::exchange(u.m_data, nullptr);
		m_count = std::exchange(u.m_count, nullptr);
		return *this;
	}

	shared_ptr(const shared_ptr& p) noexcept : m_data(p.m_data), m_count(p.m_count)
	{
		if (m_count)
			++(*m_count);
	}

	shared_ptr& operator=(const shared_ptr& p)
	{
		if (this == &p || m_count == p.m_count)
			return *this;

		decrement_and_del_if_last();

		m_data = p.m_data;
		m_count = p.m_count;

		if (m_count)
			++(*m_count);

		return *this;
	}

	~shared_ptr() { decrement_and_del_if_last(); }

	T& operator*() const noexcept { return *m_data; }
	pointer operator->() const noexcept { return m_data; }

	explicit operator bool() const noexcept { return m_data; }

	[[nodiscard]] int64_t use_count() const noexcept { return m_count ? m_count->get() : 0; }

	[[nodiscard]] pointer get() const noexcept { return m_data; }

	void reset()
	{
		decrement_and_del_if_last();
		m_data = nullptr;
		m_count = nullptr;
	}

	void reset(T* p)
	{
		decrement_and_del_if_last();

		auto tmp = new control_block_ptr(p);
		m_data = tmp->m_data;
		m_count = tmp;
	}

private:
	void decrement_and_del_if_last()
	{
		if (!m_count)
			return;

		--(*m_count);

		if (m_count->get() == 0)
			delete m_count;
	}

	T* m_data = nullptr;
	ref_counter* m_count = nullptr;
};

template <class T, class... Args>
shared_ptr<T> make_shared(Args&&... args)
{
	auto tmp = new control_block_object<T>(std::forward<Args>(args)...);

	shared_ptr<T> ptr;
	ptr.m_data = &tmp->m_data;
	ptr.m_count = tmp;

	return ptr;
}

} // namespace simple

#endif // SHARED_PTR_H
