#include "../lib/include/catch2/catch.hpp"

#include "../src/unique_ptr.h"

#include <utility>

using simple::unique_ptr;

TEST_CASE("Construct empty unique_ptr", "[construct_empty_unique_ptr]")
{
	unique_ptr<int> empty_1;
	unique_ptr<int> empty_2(nullptr);
}

TEST_CASE("Construct unique_ptr", "[construct_unique_ptr]") { unique_ptr<int> x(new int(3)); }

TEST_CASE("Move operations", "[move_operations")
{
	unique_ptr<int> x_1(new int(4));

	unique_ptr<int> x_2(std::move(x_1));

	unique_ptr<int> x_3 = std::move(x_2);
}

TEST_CASE("Reset unique_ptr", "[reset_unique_ptr]")
{
	unique_ptr<int> x(new int(5));

	x.reset(new int(3));

	x.reset();

	unique_ptr<int> x_1(new int(7));
	x_1 = nullptr;
}

TEST_CASE("Release", "[release]")
{
	unique_ptr<int> x(new int(9));

	int* x_ptr = x.release();

	delete x_ptr;
}
