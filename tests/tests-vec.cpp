#include <catch2/catch.hpp>

#include <string>
#include <algorithm>
#include "../src/vector.h"

using std::string;
using namespace simple;

TEST_CASE("Non pointer tests", "[non_pointer_vec]")
{
	auto my_vec_1 = vector<int>();
	my_vec_1.push_back(5);
	my_vec_1.push_back(6);
	my_vec_1.emplace_back(7);

	try
	{
		my_vec_1.at(9);
	}
	catch (const std::out_of_range& e)
	{
	}

	REQUIRE(my_vec_1.size() == 3);
	REQUIRE(my_vec_1.capacity() == 4);

	REQUIRE(my_vec_1[0] == 5);
	REQUIRE(my_vec_1[1] == 6);
	REQUIRE(my_vec_1[2] == 7);

	my_vec_1[1] = 8;
	REQUIRE(my_vec_1[1] == 8);
	my_vec_1.at(2) = 9;
	REQUIRE(my_vec_1.at(2) == 9);

	my_vec_1.pop_back();
	REQUIRE(my_vec_1.size() == 2);

	REQUIRE(my_vec_1[0] == 5);
	REQUIRE(my_vec_1[1] == 8);

	my_vec_1.clear();

	REQUIRE(my_vec_1.size() == 0);

	auto my_vec_2 = vector<int>(2);
	my_vec_2.reserve(2);
	my_vec_2.push_back(15);
	my_vec_2.push_back(16);
	my_vec_2.push_back(17);
	my_vec_2.push_back(18);

	my_vec_2[1] = 26;
	REQUIRE(my_vec_2[1] == 26);

	REQUIRE(my_vec_2.size() == 6);
	REQUIRE(my_vec_2.capacity() == 7);

	my_vec_2.reserve(10);
	REQUIRE(my_vec_2.capacity() == 10);
	REQUIRE(my_vec_2[5] == 18);

	my_vec_2.pop_back();
	REQUIRE(my_vec_2.size() == 5);

	REQUIRE(my_vec_2[0] == 0);
	REQUIRE(my_vec_2[1] == 26);
	REQUIRE(my_vec_2[2] == 15);

	my_vec_2.clear();

	REQUIRE(my_vec_2.size() == 0);

	auto my_vec_3 = vector<int>(3, 5);
	REQUIRE(my_vec_3.size() == 3);
	REQUIRE(my_vec_3[0] == 5);
	REQUIRE(my_vec_3[1] == 5);
	REQUIRE(my_vec_3[2] == 5);
}

TEST_CASE("Pointer tests", "[non_pointer_vec]")
{
	auto my_vec_1 = vector<int*>();
	my_vec_1.push_back(new int(5));
	my_vec_1.push_back(new int(6));
	my_vec_1.emplace_back(new int(7));

	try
	{
		my_vec_1.at(15);
	}
	catch (const std::out_of_range& e)
	{
	}

	REQUIRE(my_vec_1.size() == 3);
	REQUIRE(my_vec_1.capacity() == 4);

	REQUIRE(*(my_vec_1[0]) == 5);
	REQUIRE(*(my_vec_1[1]) == 6);
	REQUIRE(*(my_vec_1[2]) == 7);

	my_vec_1.reserve(9);
	REQUIRE(my_vec_1.capacity() == 9);
	REQUIRE(*(my_vec_1[2]) == 7);

	delete my_vec_1[1];

	my_vec_1[1] = new int(8);
	REQUIRE(*(my_vec_1[1]) == 8);

	delete my_vec_1[2];

	my_vec_1.at(2) = new int(9);
	REQUIRE(*(my_vec_1.at(2)) == 9);

	delete my_vec_1[my_vec_1.size() - 1];
	my_vec_1.pop_back();
	REQUIRE(my_vec_1.size() == 2);

	REQUIRE(*(my_vec_1[0]) == 5);
	REQUIRE(*(my_vec_1[1]) == 8);

	for (unsigned int i = 0; i < my_vec_1.size(); i++)
	{
		delete my_vec_1[i];
	}

	my_vec_1.clear();

	REQUIRE(my_vec_1.size() == 0);

	auto my_vec_2 = vector<int*>();
	my_vec_2.reserve(2);
	my_vec_2.push_back(new int(15));
	my_vec_2.push_back(new int(16));
	my_vec_2.push_back(new int(17));
	my_vec_2.push_back(new int(18));

	delete my_vec_2[1];

	my_vec_2[1] = new int(26);
	REQUIRE(*(my_vec_2[1]) == 26);

	REQUIRE(my_vec_2.size() == 4);
	REQUIRE(my_vec_2.capacity() == 4);

	delete my_vec_2[my_vec_2.size() - 1];

	my_vec_2.pop_back();
	REQUIRE(my_vec_2.size() == 3);

	REQUIRE(*(my_vec_2[0]) == 15);
	REQUIRE(*(my_vec_2[1]) == 26);
	REQUIRE(*(my_vec_2[2]) == 17);

	for (unsigned int i = 0; i < my_vec_2.size(); i++)
	{
		delete my_vec_2[i];
	}

	my_vec_2.clear();

	REQUIRE(my_vec_2.size() == 0);
}

TEST_CASE("Iterator tests", "[iterator_vec]")
{
	{
		auto my_vec = vector<int>();

		REQUIRE(my_vec.begin() == my_vec.end());

		my_vec.push_back(1);

		auto my_iter = my_vec.end();
		my_iter--;

		REQUIRE(my_iter == my_vec.begin());
		REQUIRE(my_vec.begin() != my_vec.end());

		my_vec.pop_back();

		REQUIRE(my_vec.begin() == my_vec.end());

		my_vec.push_back(1);
		my_vec.emplace_back(2);

		my_iter = my_vec.begin();
		int value = *my_iter++;

		REQUIRE(value == 1);

		value = *my_iter--;
		REQUIRE(value == 2);

		my_vec.push_back(3);
		my_vec.push_back(4);
		my_vec.push_back(5);

		my_iter = my_vec.begin();
		value = my_iter[4];

		REQUIRE(value == 5);

		auto counter = std::distance(my_vec.begin(), my_vec.end());

		REQUIRE(counter == 5);

		my_vec.pop_back();

		counter = 0;
		for (int iter_value : my_vec)
		{
			iter_value++;
			counter++;
		}

		REQUIRE(counter == 4);
	}

	{
		auto my_vec = vector<string>();

		REQUIRE(my_vec.begin() == my_vec.end());

		my_vec.push_back("1");

		auto my_iter = my_vec.end();
		my_iter--;

		REQUIRE(my_iter == my_vec.begin());
		REQUIRE(my_vec.begin() != my_vec.end());

		my_vec.pop_back();

		REQUIRE(my_vec.begin() == my_vec.end());

		my_vec.push_back("1");
		my_vec.push_back("2");

		my_iter = my_vec.begin();
		string value = *my_iter++;

		REQUIRE(value == "1");

		value = *my_iter--;
		REQUIRE(value == "2");

		my_vec.push_back("3");
		my_vec.push_back("4");
		my_vec.push_back("5");

		my_iter = my_vec.begin();
		value = my_iter[4];
		REQUIRE(value == "5");

		auto counter = std::distance(my_vec.begin(), my_vec.end());
		REQUIRE(counter == 5);

		my_vec.pop_back();

		counter = 0;
		for (string iter_value : my_vec)
		{
			counter++;
		}

		REQUIRE(counter == 4);

		my_iter = my_vec.begin();

		my_iter += 2;

		value = *my_iter;

		REQUIRE(value == "3");

		my_iter -= 2;

		value = *my_iter;

		REQUIRE(value == "1");

		my_iter = my_iter + 1;

		value = *my_iter;

		REQUIRE(value == "2");

		my_iter = my_iter - 1;

		value = *my_iter;

		REQUIRE(value == "1");

		REQUIRE(my_vec.front() == "1");
		REQUIRE(my_vec.back() == "4");

		std::string* data = my_vec.data();
		REQUIRE(data[2] == "3");
	}
}

TEST_CASE("push_back, emplace_back", "[push_back_emplace_back]")
{
	{
		auto my_vec = vector<int>();

		my_vec.push_back(111111);
		my_vec.emplace_back(222222);
		my_vec.push_back(333333);

		my_vec.pop_back();

		REQUIRE(my_vec.back() == 222222);

		my_vec.clear();

		my_vec.push_back(234);

		REQUIRE(my_vec.front() == 234);
	}

	{
		auto my_vec = vector<string>();

		my_vec.push_back(string("111111111111111111111111111111111111111111111111"));
		my_vec.emplace_back("22222222222222222222222222222222222222222222222222222222");
		my_vec.push_back(string("33333333333333333333333333333333333333333333333333333333"));

		my_vec.pop_back();

		REQUIRE(my_vec.back() == "22222222222222222222222222222222222222222222222222222222");

		my_vec.clear();

		my_vec.push_back(string("2, 3, 4"));

		REQUIRE(my_vec.front() == "2, 3, 4");
	}
}

TEST_CASE("move_copy_swap", "[move_copy_swap]")
{
	auto my_vec_1 = vector<int>();
	my_vec_1.push_back(10);
	my_vec_1.push_back(100);
	my_vec_1.push_back(1000);

	vector<int> my_vec_2;
	my_vec_2 = my_vec_1;

	REQUIRE(my_vec_1 == my_vec_2);

	my_vec_1.pop_back();

	REQUIRE(my_vec_1 != my_vec_2);

	REQUIRE(my_vec_1.size() == 2);
	REQUIRE(my_vec_2.size() == 3);

	auto my_vec_3 = vector<string>();
	my_vec_3.push_back("10");
	my_vec_3.push_back("100");
	my_vec_3.push_back("1000");

	auto my_vec_4 = my_vec_3;

	my_vec_4.pop_back();

	REQUIRE(my_vec_4.size() == 2);
	REQUIRE(my_vec_3.size() == 3);

	auto my_vec_5 = vector<string>();
	my_vec_5.push_back("10");
	my_vec_5.push_back("100");
	my_vec_5.push_back("1000");

	auto my_vec_6 = std::move(my_vec_5);
	REQUIRE(my_vec_6.size() == 3);
	REQUIRE(my_vec_5.data() == nullptr);
}
