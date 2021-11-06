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

	constexpr shared_ptr() noexcept = default;
	constexpr explicit shared_ptr(std::nullptr_t) noexcept {}

	template <class Y>
	constexpr explicit shared_ptr(Y* ptr)
	{
		auto tmp = new control_block_ptr(ptr);
		set_values(tmp->m_data, tmp);
	}

	shared_ptr(shared_ptr&& r) noexcept :
		m_data(std::exchange(r.m_data, nullptr)), m_count(std::exchange(r.m_count, nullptr))
	{
	}

	template <class Y>
	shared_ptr(shared_ptr<Y>&& r) noexcept :
		m_data(std::exchange(r.m_data, nullptr)), m_count(std::exchange(r.m_count, nullptr))
	{
	}

	shared_ptr& operator=(shared_ptr r) noexcept
	{
		r.swap(*this);
		return *this;
	}

	shared_ptr(const shared_ptr& r) noexcept : m_data(r.m_data), m_count(r.m_count)
	{
		inc_count_if_valid();
	}

	template <class Y>
	shared_ptr(const shared_ptr<Y>& r) noexcept : m_data(r.m_data), m_count(r.m_count)
	{
		inc_count_if_valid();
	}

	~shared_ptr() { decrement_and_del_if_last(); }

	T& operator*() const noexcept { return *m_data; }
	pointer operator->() const noexcept { return m_data; }

	explicit operator bool() const noexcept { return m_data; }

	[[nodiscard]] int64_t use_count() const noexcept { return m_count ? m_count->get() : 0; }

	[[nodiscard]] pointer get() const noexcept { return m_data; }

	void reset() { shared_ptr().swap(*this); }

	template <class Y>
	void reset(Y* ptr)
	{
		shared_ptr(ptr).swap(*this);
	}

	void swap(shared_ptr& r) noexcept
	{
		T* t_tmp = m_data;
		m_data = r.m_data;
		r.m_data = t_tmp;

		ref_counter* ref_tmp = m_count;
		m_count = r.m_count;
		r.m_count = ref_tmp;
	}

	template <class TT>
	friend void swap(T& a, T& b) noexcept;

	template <class TT, class... Args>
	friend shared_ptr<TT> make_shared(Args&&... args);

	template <class Y>
	friend class shared_ptr;

private:
	void decrement_and_del_if_last()
	{
		if (!m_count)
			return;

		--(*m_count);

		if (m_count->get() == 0)
			delete m_count;
	}

	void inc_count_if_valid()
	{
		if (m_count)
			++(*m_count);
	}

	void set_values(T* data, ref_counter* count)
	{
		m_data = data;
		m_count = count;
	}

	T* m_data = nullptr;
	ref_counter* m_count = nullptr;
};

template <class TT, class... Args>
shared_ptr<TT> make_shared(Args&&... args)
{
	auto tmp = new control_block_object<TT>(std::forward<Args>(args)...);

	shared_ptr<TT> ptr;
	ptr.m_data = &tmp->m_data;
	ptr.m_count = tmp;

	return ptr;
}

template <class TT>
void swap(TT& a, TT& b) noexcept
{
	a.swap(b);
}

} // namespace simple

#endif // SHARED_PTR_H
