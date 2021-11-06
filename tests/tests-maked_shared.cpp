#include "../lib/include/catch2/catch.hpp"
#include "../src/shared_ptr.h"

using simple::make_shared;
using simple::shared_ptr;

TEST_CASE("shared_ptr with make_shared no parameter", "[shared_ptr_make_shared_no_parameter]")
{
	auto x_1 = make_shared<int>();
	REQUIRE(x_1.get());
	REQUIRE(x_1);
	REQUIRE(x_1.use_count() == 1);
}

TEST_CASE("shared_ptr move operations make_shared", "[shared_ptr_move_operations_make_shared]")
{
	auto x_1 = make_shared<int>(4);
	REQUIRE(*x_1 == 4);
	REQUIRE(x_1.use_count() == 1);

	auto x_2 = std::move(x_1);
	REQUIRE(*x_2 == 4);
	REQUIRE(x_2.use_count() == 1);

	shared_ptr<int> x_3;
	x_3 = std::move(x_2);
	REQUIRE(*x_3 == 4);

	shared_ptr<int> x_4(std::move(x_3));
	x_4 = make_shared<int>(15);
	REQUIRE(*x_4 == 15);
	REQUIRE(*(x_4.get()) == 15);
}

TEST_CASE("shared_ptr copy operations make_shared", "[shared_ptr_copy_operations_make_shared]")
{
	auto x_1 = make_shared<int>(4);
	REQUIRE(*x_1 == 4);
	REQUIRE(x_1.use_count() == 1);

	shared_ptr<int> x_2(x_1);
	REQUIRE(*x_2 == 4);
	REQUIRE(x_1.use_count() == 2);
	REQUIRE(x_2.use_count() == 2);
	REQUIRE(x_1);

	shared_ptr<int> x_3;
	x_3 = x_2;
	REQUIRE(*x_3 == 4);
	REQUIRE(x_3.use_count() == 3);

	shared_ptr<int> x_4(x_3);
	REQUIRE(x_4.use_count() == 4);

	auto x_5 = make_shared<int>(9);
	x_4 = x_5;
	REQUIRE(*x_4 == 9);
	REQUIRE(*(x_4.get()) == 9);
	REQUIRE(x_4.use_count() == 2);
	REQUIRE(x_3.use_count() == 3);
}

TEST_CASE("shared_ptr self assignment in 2 steps make_shared",
		  "[shared_ptr_self_assignment_2_steps_make_shared]")
{
	auto x_1 = make_shared<int>(9);
	shared_ptr<int> x_2(x_1);
	x_1 = x_2;

	REQUIRE(x_1.use_count() == 2);
}

class Base
{
public:
	explicit Base(int y) : m_y(y) {}

	int m_y;
};

class Derived : public Base
{
public:
	explicit Derived(int y) : Base(y) {}
};

TEST_CASE("Derived class pointer to a base class pointer shared_ptr make_shared",
		  "[derived_pointer_to_base_pointer_shared_ptr_make_shared]")
{
	shared_ptr<Base> x = make_shared<Base>(9);
	REQUIRE(x->m_y == 9);

	shared_ptr<Derived> derived = make_shared<Derived>(8);

	shared_ptr<Base> base = derived;

	REQUIRE(base->m_y == 8);

	shared_ptr<Base> other_base = make_shared<Base>(9);
	other_base = std::move(base);

	REQUIRE(other_base->m_y == 8);

	shared_ptr<Derived> third_base = make_shared<Derived>(8);
	shared_ptr<Base> forth_base = make_shared<Base>(18);
	forth_base = std::move(third_base);
	REQUIRE(forth_base->m_y == 8);

	shared_ptr<Derived> fifth_base = make_shared<Derived>(28);
	shared_ptr<Base> sixth(std::move(fifth_base));
	REQUIRE(sixth->m_y == 28);

	shared_ptr<Derived> seventh_base = make_shared<Derived>(39);
	shared_ptr<Base> eighth;
	eighth = seventh_base;
	REQUIRE(eighth->m_y == 39);
}

TEST_CASE("Reset shared_ptr make_shared", "[reset_shared_ptr_make_shared]")
{
	auto x = make_shared<int>(7);
	x.reset(new int(3));
	REQUIRE(*x == 3);

	x.reset();
	REQUIRE(x.get() == nullptr);

	auto x_1 = make_shared<int>(7);
	REQUIRE(*x_1 == 7);

	shared_ptr<Base> base;
	base.reset(new Derived(1));
	REQUIRE(base->m_y == 1);
}
