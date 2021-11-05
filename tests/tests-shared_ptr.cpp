#include "../lib/include/catch2/catch.hpp"
#include "../src/shared_ptr.h"

#include <string>

using simple::shared_ptr;
using std::string;

TEST_CASE("Construct empty shared_ptr", "[construct_empty_shared_ptr]")
{
	shared_ptr<string> x_1;
	REQUIRE(!x_1.get());

	shared_ptr<string> x_2;
	REQUIRE(!x_2);
}

TEST_CASE("Construct shared_ptr", "[construct_shared_ptr]")
{
	shared_ptr<string> x(new string("Very very very very long string....."));

	REQUIRE(*x == string("Very very very very long string....."));
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

TEST_CASE("shared_ptr reset", "[shared_ptr_reset]")
{
	shared_ptr<int> x_1;
	x_1.reset();
	REQUIRE(!x_1);

	shared_ptr<int> x_2(new int(9));
	x_2.reset();
	REQUIRE(!x_2);

	shared_ptr<int> x_3;
	x_3.reset(new int(8));
	REQUIRE(*x_3 == 8);

	shared_ptr<int> x_4(new int(7));
	x_3.reset(new int(8));
	REQUIRE(*x_3 == 8);
}
