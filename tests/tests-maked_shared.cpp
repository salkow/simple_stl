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
