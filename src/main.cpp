#include <iostream>
#include <vector>
#include <string>

#include "Vec.hpp"

using Simple::Vec;
using std::string;

struct Vector3
{

	Vector3() { m_MemoryBlock = new int[5]; }

	Vector3(float scalar) : m_x(scalar), m_y(scalar), m_z(scalar) { m_MemoryBlock = new int[5]; }

	Vector3(float x, float y, float z) : m_x(x), m_y(y), m_z(z) { m_MemoryBlock = new int[5]; }

	~Vector3()
	{
		std::cout << "Destroy\n";
		delete[] m_MemoryBlock;
		m_MemoryBlock = nullptr;
	}

	Vector3(const Vector3& other) = delete;

	Vector3(Vector3&& other) : m_x(other.m_x), m_y(other.m_y), m_z(other.m_z)
	{
		std::cout << "Move\n";

		m_MemoryBlock = other.m_MemoryBlock;
		other.m_MemoryBlock = nullptr;
	}

	Vector3& operator=(const Vector3& other) = delete;

	Vector3& operator=(Vector3&& other)
	{
		std::cout << "Move\n";
		m_x = other.m_x;
		m_y = other.m_y;
		m_z = other.m_z;

		m_MemoryBlock = other.m_MemoryBlock;
		other.m_MemoryBlock = nullptr;

		return *this;
	}

	float m_x = 0.0f, m_y = 0.0f, m_z = 0.0f;
	int* m_MemoryBlock = nullptr;
};

class Foo
{
public:
	Foo()
	{
		std::cout << "The constructor was called.\n";
		y = new int[10];
	}
	~Foo()
	{
		std::cout << "The destructor was called.\n";
		delete[] y;
	}

	int* y;
};

void non_pointer();

void non_pointer()
{
	// auto my_vec = std::vector<std::string>();

	auto my_vec = Vec<std::string>(2);
	// auto my_vec = Vec<int>();

	auto x = std::string();
	my_vec.push_back(x);

	auto x_1 = std::string();
	my_vec.push_back(x_1);

	// auto x_2 = std::string();
	// my_vec.push_back(x_2);

	// auto x_3 = std::string();

	// my_vec.clear();

	// my_vec.push_back(x_3);
}

void pointer();
void pointer()
{
	// auto my_vec = std::vector<Foo*>();

	auto my_vec = Vec<Foo*>();

	auto x = new Foo();

	my_vec.push_back(x);

	my_vec.clear();

	delete my_vec[0];
}

void iter();
void iter()
{
	// auto my_vec = std::vector<int>();
	auto my_vec = Vec<int>();

	my_vec.push_back(10);
	my_vec.push_back(20);
	my_vec.push_back(30);
	my_vec.push_back(40);
	my_vec.push_back(50);

	auto it_begin = my_vec.begin();
	auto it_end = my_vec.end();

	it_begin++;
	it_begin--;

	for (auto it = my_vec.begin(); it != my_vec.end(); it++)
	{
		std::cout << *it << '\n';
	}

	for (auto value : my_vec)
		std::cout << value << '\n';

	it_begin = my_vec.begin();
	it_begin += 2;
	it_begin -= 2;
	std::cout << *it_begin << '\n';
}

void foo();
void foo()
{
	auto my_vec_1 = Vec<Vector3>();
	my_vec_1.push_back(12);

	auto my_vec_2 = std::move(my_vec_1);
}

int main() { foo(); }
