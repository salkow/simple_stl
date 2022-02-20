#include "my_string.h"

simple::string::string() : m_elem(create_string(STARTING_CAPACITY)) {}

simple::string::string(simple::size_type count, char ch) :
	m_size(count), m_elem(create_string(count)), m_capacity(count)
{
	memset(m_elem, ch, count);
}

simple::string::string(const simple::string& other) :
	m_size(other.m_size), m_elem(create_string(other.m_size)), m_capacity(m_size)
{
	std::memcpy(m_elem, other.m_elem, m_size);
}

simple::string::string(simple::string&& other) noexcept
{
	m_size = std::exchange(other.m_size, 0);
	m_elem = std::exchange(other.m_elem, nullptr);
	m_capacity = std::exchange(other.m_capacity, 0);
}

simple::string::string(const char* other) :
	m_size(strlen(other)), m_elem(create_string(m_size)), m_capacity(m_size)
{
	std::memcpy(m_elem, other, m_size);
}

simple::string::string(const char* other, size_type count) :
	m_size(count), m_elem(create_string(count)), m_capacity(count)
{
	std::memcpy(m_elem, other, count);
}

simple::string& simple::string::operator=(simple::string other)
{
	other.swap(*this);
	return *this;
}

simple::string& simple::string::operator=(const char* other)
{
	m_size = strlen(other);
	fit(m_size);
	std::memcpy(m_elem, other, m_size);
	m_elem[m_size] = '\0';

	return *this;
}

simple::string::~string() { delete_string(); }

bool simple::string::empty() const { return m_size == 0; }

simple::size_type simple::string::size() const { return m_size; }

simple::size_type simple::string::length() const { return m_size; }

simple::string::iterator simple::string::begin() const noexcept { return iterator(m_elem); }

simple::string::iterator simple::string::end() const noexcept { return iterator(m_elem + m_size); }

simple::string::value_type& simple::string::front() noexcept { return m_elem[0]; }

const simple::string::value_type& simple::string::front() const noexcept { return m_elem[0]; }

simple::string::value_type& simple::string::back() noexcept { return m_elem[m_size - 1]; }

const simple::string::value_type& simple::string::back() const noexcept { return m_elem[m_size - 1]; }

simple::string::pointer simple::string::data() noexcept { return m_elem; }

simple::string::pointer simple::string::data() const noexcept { return m_elem; }

bool simple::string::operator==(const simple::string& other) const
{
	if (m_size != other.m_size)
		return false;

	return !(std::memcmp(m_elem, other.m_elem, m_size));
}

bool simple::string::operator==(const char* other) const { return !(std::strcmp(m_elem, other)); }

bool simple::string::operator!=(const simple::string& other) const { return !(*this == other); }

bool simple::string::operator!=(const char* other) const { return !(this->m_elem == other); }

simple::string::value_type& simple::string::operator[](simple::size_type pos) noexcept
{
	return m_elem[pos];
}

const simple::string::value_type& simple::string::operator[](simple::size_type pos) const noexcept
{
	return m_elem[pos];
}

simple::string::value_type& simple::string::at(simple::size_type pos)
{
	if (pos < m_size)
		return m_elem[pos];

	throw std::out_of_range("Index out of range");
}

const simple::string::value_type& simple::string::at(simple::size_type pos) const
{
	if (pos < m_size)
		return m_elem[pos];

	throw std::out_of_range("Index out of range");
}

void simple::string::reserve(simple::size_type new_cap)
{
	if (m_capacity < new_cap)
		reallocate(new_cap);
}

simple::size_type simple::string::capacity() const { return m_capacity; }

void simple::string::clear() noexcept
{
	m_elem[0] = '\0';
	m_size = 0;
}

const char* simple::string::c_str() const { return m_elem; }

void simple::string::push_back(char ch)
{
	if (m_size == m_capacity)
		reallocate(get_increased_capacity());

	m_elem[m_size] = ch;
	m_size++;
	m_elem[m_size] = '\0';
}

void simple::string::pop_back()
{
	m_elem[m_size - 1] = '\0';
	m_size--;
}

void simple::string::reallocate(simple::size_type new_cap)
{
	auto* tmp_string = create_string(new_cap);
	std::memcpy(tmp_string, m_elem, m_size);

	delete_string();

	m_elem = tmp_string;
	m_elem[m_size] = '\0';
	m_capacity = new_cap;
}

void simple::string::fit(simple::size_type new_cap)
{
	if (m_capacity < new_cap)
	{
		delete_string();
		m_elem = create_string(new_cap);
		m_capacity = new_cap;
	}
}

void simple::string::delete_string() { delete[] m_elem; }

char* simple::string::create_string(simple::size_type size)
{
	auto* tmp = new char[size + 1];
	tmp[size] = '\0';
	return tmp;
}

simple::size_type simple::string::get_increased_capacity() const
{
	return m_capacity * CAPACITY_INCREASE_FACTOR + 1;
}
