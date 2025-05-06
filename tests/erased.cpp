#include <catch2/catch_all.hpp>
#include "erased.hpp"

using namespace light;

struct Dummy {
    int x;
    double y;

    Dummy(int xVal, double yVal) : x(xVal), y(yVal) {}
};

TEST_CASE("Testing erased_ptr class", "[erased_ptr]") {

    SECTION("Create erased_ptr from unique_ptr") {
        auto data = std::make_unique<int>(56);
        erased_ptr p{ std::move(data) };

        REQUIRE_NOTHROW(p.get<int>());
        REQUIRE(p.get<int>() == 56);
    }

    SECTION("Create erased_ptr from raw pointer") {
        auto data = new int(99);
        erased_ptr p{ data };

        REQUIRE_NOTHROW(p.get<int>());
        REQUIRE(p.get<int>() == 99);
    }

    SECTION("Type mismatch throws bad_cast") {
        auto data = new std::string("test");
        erased_ptr p{ data };

        REQUIRE_THROWS_AS(p.get<int>(), std::bad_cast);
    }

    SECTION("Erased_ptr stores and retrieves non-primitive type") {
        auto data = new Dummy{ 10, 3.14 };
        erased_ptr p{ data };

        REQUIRE_NOTHROW(p.get<Dummy>());
        REQUIRE(p.get<Dummy>().x == 10);
        REQUIRE(p.get<Dummy>().y == 3.14);
    }

    SECTION("Create erased_ptr using make_erased") {
        auto p = make_erased<Dummy>(10, 3.14);

        REQUIRE_NOTHROW(p.get<Dummy>());
        REQUIRE(p.get<Dummy>().x == 10);
        REQUIRE(p.get<Dummy>().y == 3.14);
    }

    SECTION("Move semantics assignment with erased_ptr") {
        auto data = std::make_unique<int>(101);
        auto first = erased_ptr{ std::move(data) };
        erased_ptr second = std::move(first);

        REQUIRE_THROWS_AS(first.get<int>(), std::bad_cast);
        REQUIRE_NOTHROW(second.get<int>());
        REQUIRE(second.get<int>() == 101);
    }

    SECTION("Move semantics constructor with erased_ptr") {
        auto data = std::make_unique<int>(101);
        auto first = erased_ptr{ std::move(data) };
        erased_ptr second{ std::move(first) };

        REQUIRE_THROWS_AS(first.get<int>(), std::bad_cast);
        REQUIRE_NOTHROW(second.get<int>());
        REQUIRE(second.get<int>() == 101);
    }

    SECTION("Copy assignment with erased_ptr") {
        auto data = new int(78);
        auto original = erased_ptr{ data };
        erased_ptr copy = original;

        REQUIRE_NOTHROW(original.get<int>());
        REQUIRE(original.get<int>() == 78);

        REQUIRE_NOTHROW(copy.get<int>());
        REQUIRE(copy.get<int>() == 78);
    }

    SECTION("Copy constructor with erased_ptr") {
        auto data = new int(78);
        auto original = erased_ptr{ data };
        erased_ptr copy{ original };

        REQUIRE_NOTHROW(original.get<int>());
        REQUIRE(original.get<int>() == 78);

        REQUIRE_NOTHROW(copy.get<int>());
        REQUIRE(copy.get<int>() == 78);
    }

    SECTION("Reset an erased_ptr to the empty state") {
        auto p = make_erased<int>(78);
        p.reset();

        REQUIRE_THROWS_AS(p.get<int>(), std::bad_cast);
    }

    SECTION("Reset an erased_ptr to another value") {
        auto p = make_erased<int>(78);
        p.reset(new std::string("test"));

        REQUIRE_THROWS_AS(p.get<int>(), std::bad_cast);
        REQUIRE_NOTHROW(p.get<std::string>());
        REQUIRE(p.get<std::string>() == "test");
    }
}