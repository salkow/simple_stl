#include "../lib/include/catch2/catch.hpp"

#include "../src/unique_ptr.h"

#include <utility>
#include <string>

using simple::unique_ptr;

TEST_CASE("Construct empty unique_ptr", "[construct_empty_unique_ptr]")
{
	unique_ptr<int> empty_1;
	REQUIRE(empty_1.get() == nullptr);

	unique_ptr<int> empty_2(nullptr);
	REQUIRE(empty_2.get() == nullptr);
}

TEST_CASE("Construct unique_ptr", "[construct_unique_ptr]")
{
	unique_ptr<int> x(new int(3));
	REQUIRE(*x == 3);
}

TEST_CASE("Move operations", "[move_operations")
{
	unique_ptr<int> x_1(new int(4));
	REQUIRE(*x_1 == 4);

	unique_ptr<int> x_2(std::move(x_1));
	REQUIRE(*x_2 == 4);

	unique_ptr<int> x_3 = std::move(x_2);
	REQUIRE(*x_3 == 4);
}

TEST_CASE("Reset unique_ptr", "[reset_unique_ptr]")
{
	unique_ptr<int> x(new int(5));
	REQUIRE(*x == 5);

	x.reset(new int(3));
	REQUIRE(*x == 3);

	x.reset();
	REQUIRE(x.get() == nullptr);

	unique_ptr<int> x_1(new int(7));
	REQUIRE(*x_1 == 7);

	x_1 = nullptr;
	REQUIRE(x_1.get() == nullptr);
}

TEST_CASE("Release", "[release]")
{
	unique_ptr<int> x(new int(9));
	REQUIRE(*x == 9);

	int* x_ptr = x.release();
	REQUIRE(*x_ptr == 9);

	delete x_ptr;
}

TEST_CASE("Arrow operator", "[arrow_operator")
{
	unique_ptr<std::string> x(new std::string("123"));

	REQUIRE(x->size() == 3);
}

TEST_CASE("Dereference operator", "[dereference_operator")
{
	unique_ptr<int> x(new int(15));

	REQUIRE(*x == 15);
}
