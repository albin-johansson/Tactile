#include "vector_map.hpp"

#include <catch.hpp>
#include <string>
#include <string_view>

using namespace tactile;

TEST_CASE("small_map defaults", "[vector_map]")
{
  const vector_map<int, int> map;
  CHECK(map.capacity() == 0);
  CHECK(map.size() == 0);  // NOLINT
  CHECK(map.empty());
}

TEST_CASE("vector_map::emplace", "[vector_map]")
{
  using namespace std::string_view_literals;

  vector_map<int, std::string> map;
  const auto i = 7;

  SECTION("Emplacing one string")
  {
    auto& [key, value] = map.emplace(i, "hello");
    CHECK(key == i);
    CHECK(value == "hello"sv);
  }

  SECTION("Overwriting the string with the same key")
  {
    auto& [key, value] = map.emplace(i, "world");
    CHECK(map.size() == 1);
    CHECK(key == i);
    CHECK(value == "world"sv);
  }
}

TEST_CASE("vector_map::erase", "[vector_map]")
{
  vector_map<int, std::string> map;
  CHECK_NOTHROW(map.erase(0));

  map.emplace(0, "foo");
  map.emplace(1, "bar");
  CHECK(map.contains(0));
  CHECK(map.contains(1));
  CHECK(!map.empty());

  map.erase(0);
  CHECK(!map.contains(0));
  CHECK(map.contains(1));
  CHECK(!map.empty());

  map.erase(1);
  CHECK(!map.contains(1));
  CHECK(map.empty());
}

TEST_CASE("vector_map::reserve", "[vector_map]")
{
  vector_map<int, int> map;
  const auto n = 10;

  map.reserve(n);
  CHECK(map.capacity() == n);
}

TEST_CASE("vector_map::clear", "[vector_map]")
{
  vector_map<int, int> map;

  map.emplace(0, 10);
  map.emplace(1, 20);
  map.emplace(2, 30);

  map.clear();

  CHECK(map.empty());
}

TEST_CASE("vector_map::at", "[vector_map]")
{
  using namespace std::string_view_literals;

  vector_map<int, std::string> map;
  CHECK_THROWS_AS(map.at(0), std::out_of_range);

  map.emplace(0, "foo");
  const auto& str = map.at(0);
  CHECK(str == "foo"sv);
}

TEST_CASE("vector_map::operator[]", "[vector_map]")
{
  using namespace std::string_view_literals;

  vector_map<int, std::string> map;
  CHECK_THROWS_AS(map[0], std::out_of_range);

  map.emplace(0, "foo");
  const auto& str = map[0];
  CHECK(str == "foo"sv);
}

TEST_CASE("small_map begin/end", "[vector_map]")
{
  vector_map<int, std::string> map;
  map.emplace(1, "abc");
  map.emplace(2, "def");
  map.emplace(3, "ghi");

  CHECK(std::distance(map.begin(), map.end()) == 3);
}