#include "../lib/include/catch2/catch.hpp"
#include "../src/algorithm.h"
#include "../src/vector.h"

using simple::pair;
using simple::vector;

TEST_CASE("for_each.", "[for_each]")
{
	vector<int> vec;
	vec.emplace_back(1);
	vec.emplace_back(2);
	vec.emplace_back(3);

	auto add_one = [](int& x) { x++; };

	simple::for_each(vec.begin(), vec.end(), add_one);

	REQUIRE(vec[0] == 2);
	REQUIRE(vec[1] == 3);
	REQUIRE(vec[2] == 4);
}

TEST_CASE("for_each_n.", "[for_each_n]")
{
	vector<int> vec;
	vec.emplace_back(1);
	vec.emplace_back(2);
	vec.emplace_back(3);

	auto add_one = [](int& x) { x++; };

	simple::for_each_n(vec.begin(), 2, add_one);

	REQUIRE(vec[0] == 2);
	REQUIRE(vec[1] == 3);
	REQUIRE(vec[2] == 3);
}

TEST_CASE("all_of.", "[all_of]")
{
	vector even_vec(4, 2);

	auto is_even = [](int x) { return x % 2 == 0; };

	REQUIRE(simple::all_of(even_vec.begin(), even_vec.end(), is_even));

	vector odd_vec(4, 3);

	REQUIRE(!simple::all_of(odd_vec.begin(), odd_vec.end(), is_even));
}

TEST_CASE("any_of.", "[any_of]")
{
	vector vec(4, 3);

	auto is_even = [](int x) { return x % 2 == 0; };

	REQUIRE(!simple::any_of(vec.begin(), vec.end(), is_even));

	vec.emplace_back(2);

	REQUIRE(simple::any_of(vec.begin(), vec.end(), is_even));
}

TEST_CASE("none_of.", "[none_of]")
{
	vector vec(4, 3);

	auto is_even = [](int x) { return x % 2 == 0; };

	REQUIRE(simple::none_of(vec.begin(), vec.end(), is_even));

	vec.emplace_back(2);

	REQUIRE(!simple::none_of(vec.begin(), vec.end(), is_even));
}

TEST_CASE("count.", "[count]")
{
	vector<int> vec;
	vec.emplace_back(1);
	vec.emplace_back(1);
	vec.emplace_back(2);
	vec.emplace_back(3);

	REQUIRE(simple::count(vec.begin(), vec.end(), 1) == 2);
	REQUIRE(simple::count(vec.begin(), vec.end(), 2) == 1);
	REQUIRE(simple::count(vec.begin(), vec.end(), 4) == 0);
}

TEST_CASE("count_if.", "[count_if]")
{
	vector<int> vec;
	vec.emplace_back(1);
	vec.emplace_back(2);
	vec.emplace_back(3);
	vec.emplace_back(4);
	vec.emplace_back(5);

	auto is_even = [](int x) { return x % 2 == 0; };
	auto is_odd = [](int x) { return x % 2 == 1; };

	REQUIRE(simple::count_if(vec.begin(), vec.end(), is_even) == 2);
	REQUIRE(simple::count_if(vec.begin(), vec.end(), is_odd) == 3);
}

TEST_CASE("mismatch.", "[mismatch]")
{
	vector<int> vec1;
	vec1.emplace_back(1);
	vec1.emplace_back(2);
	vec1.emplace_back(3);

	vector<int> vec2;
	vec2.emplace_back(1);
	vec2.emplace_back(2);
	vec2.emplace_back(7);

	auto mismatch = simple::mismatch(vec1.begin(), vec1.end(), vec2.begin());

	REQUIRE(*mismatch.first == 3);
	REQUIRE(*mismatch.second == 7);

	auto are_odd = [](const int& x, const int& y) { return x % 2 == 1 && y % 2 == 1; };

	mismatch = simple::mismatch(vec1.begin(), vec1.end(), vec2.begin(), are_odd);

	REQUIRE(*mismatch.first == 2);
	REQUIRE(*mismatch.second == 2);

	vec2.pop_back();

	mismatch = simple::mismatch(vec1.begin(), vec1.end(), vec2.begin(), vec2.end());

	REQUIRE(*mismatch.first == 3);
	REQUIRE(mismatch.second == vec2.end());

	mismatch = simple::mismatch(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), are_odd);

	REQUIRE(*mismatch.first == 2);
	REQUIRE(*mismatch.second == 2);
}

TEST_CASE("find", "[find]")
{
	vector<int> vec;
	vec.emplace_back(1);
	vec.emplace_back(2);
	vec.emplace_back(3);

	REQUIRE(*simple::find(vec.begin(), vec.end(), 2) == 2);
	REQUIRE(simple::find(vec.begin(), vec.end(), 4) == vec.end());
}

TEST_CASE("find_if", "[find_if]")
{
	vector<int> vec;
	vec.emplace_back(1);
	vec.emplace_back(2);
	vec.emplace_back(3);

	auto is_two = [](int x) { return x == 2; };
	auto is_four = [](int x) { return x == 4; };

	REQUIRE(*simple::find_if(vec.begin(), vec.end(), is_two) == 2);
	REQUIRE(simple::find_if(vec.begin(), vec.end(), is_four) == vec.end());
}

TEST_CASE("find_if_not", "[find_if_not]")
{
	vector<int> vec;
	vec.emplace_back(2);
	vec.emplace_back(2);
	vec.emplace_back(2);
	vec.emplace_back(2);
	vec.emplace_back(3);

	auto is_two = [](int x) { return x == 2; };

	REQUIRE(*simple::find_if_not(vec.begin(), vec.end(), is_two) == 3);

	vec.pop_back();
	REQUIRE(simple::find_if_not(vec.begin(), vec.end(), is_two) == vec.end());
}
