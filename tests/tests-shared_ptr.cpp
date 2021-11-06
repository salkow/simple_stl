#include "../lib/include/catch2/catch.hpp"
#include "../src/shared_ptr.h"

using simple::shared_ptr;
using simple::swap;

TEST_CASE("Construct empty shared_ptr", "[construct_empty_shared_ptr]")
{
	shared_ptr<int> x_1;
	REQUIRE(!x_1.get());

	shared_ptr<int> x_2;
	REQUIRE(!x_2);
}

TEST_CASE("shared_ptr simple move operations", "[shared_simple_ptr_move_operations]")
{
	shared_ptr<int> x_1(new int(4));
	REQUIRE(*x_1 == 4);
	REQUIRE(x_1.use_count() == 1);

	shared_ptr<int> x_2(std::move(x_1));
	REQUIRE(*x_2 == 4);
	REQUIRE(x_2.use_count() == 1);

	shared_ptr<int> x_3;
	x_3 = std::move(x_2);
	REQUIRE(*x_3 == 4);

	shared_ptr<int> x_4(std::move(x_3));
	x_4 = shared_ptr<int>(new int(15));
	REQUIRE(*x_4 == 15);
	REQUIRE(*(x_4.get()) == 15);
}

TEST_CASE("shared_ptr simple copy operations", "[shared_ptr_simple_copy_operations]")
{
	shared_ptr<int> x_1(new int(4));
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

	shared_ptr<int> x_5(new int(9));
	x_4 = x_5;
	REQUIRE(*x_4 == 9);
	REQUIRE(*(x_4.get()) == 9);
	REQUIRE(x_4.use_count() == 2);
	REQUIRE(x_3.use_count() == 3);
}

TEST_CASE("shared_ptr copy/move empty object", "[shared_ptr_copy_move_empty_object]")
{
	shared_ptr<int> x_1;
	shared_ptr<int> x_2(x_1);

	shared_ptr<int> x_3;
	x_3 = x_2;

	shared_ptr<int> x_4(std::move(x_3));

	REQUIRE(!x_1);
	REQUIRE(x_1.use_count() == 0);

	REQUIRE(!x_2);
	REQUIRE(!x_4.get());
}

TEST_CASE("shared_ptr self assignment in 2 steps", "[shared_ptr_self_assignment_2_steps]")
{
	shared_ptr<int> x_1(new int(11));
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

TEST_CASE("Derived class pointer to a base class pointer shared_ptr",
		  "[derived_pointer_to_base_pointer_shared_ptr]")
{
	shared_ptr<Base> x(new Derived(9));

	REQUIRE(x->m_y == 9);

	shared_ptr<Derived> derived(new Derived(8));

	shared_ptr<Base> base = derived;

	REQUIRE(base->m_y == 8);

	shared_ptr<Base> other_base(new Base(9));
	other_base = std::move(base);

	REQUIRE(other_base->m_y == 8);

	shared_ptr<Derived> third_base(new Derived(8));
	shared_ptr<Base> forth_base(new Base(18));
	forth_base = std::move(third_base);
	REQUIRE(forth_base->m_y == 8);

	shared_ptr<Derived> fifth_base(new Derived(28));
	shared_ptr<Base> sixth(std::move(fifth_base));
	REQUIRE(sixth->m_y == 28);

	shared_ptr<Derived> seventh_base(new Derived(39));
	shared_ptr<Base> eighth;
	eighth = seventh_base;
	REQUIRE(eighth->m_y == 39);
}

TEST_CASE("Reset shared_ptr", "[reset_shared_ptr]")
{
	shared_ptr<int> x(new int(5));
	REQUIRE(*x == 5);

	x.reset(new int(3));
	REQUIRE(*x == 3);

	x.reset();
	REQUIRE(x.get() == nullptr);

	shared_ptr<int> x_1(new int(7));
	REQUIRE(*x_1 == 7);

	shared_ptr<Base> base;
	base.reset(new Derived(1));
	REQUIRE(base->m_y == 1);
}

TEST_CASE("Swap shared_ptr", "[swap_shared_ptr]")
{
	shared_ptr<int> x_1(new int(5));
	shared_ptr<int> x_2(new int(9));

	x_1.swap(x_2);

	REQUIRE(*x_1 == 9);
	REQUIRE(*x_2 == 5);

	swap(x_2, x_1);

	REQUIRE(*x_1 == 5);
	REQUIRE(*x_2 == 9);
}
