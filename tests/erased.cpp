#include <catch2/catch_all.hpp>
#include "erased.hpp"

using namespace light;

TEST_CASE("Testing erased_ptr class", "[erased_ptr]") {
    SECTION("Create erased_ptr from shared_ptr") {
        auto sharedPtr = std::make_shared<int>(42);
        erased_ptr p{ sharedPtr };

        REQUIRE_NOTHROW(p.get<int>());
        REQUIRE(p.get<int>() == 42);
    }

    SECTION("Create erased_ptr from unique_ptr") {
        auto uniquePtr = std::make_unique<int>(56);
        erased_ptr p{ std::move(uniquePtr) };

        REQUIRE_NOTHROW(p.get<int>());
        REQUIRE(p.get<int>() == 56);
    }

    SECTION("Create erased_ptr from raw pointer") {
        auto rawPtr = new int(99);
        erased_ptr p{ rawPtr };

        REQUIRE_NOTHROW(p.get<int>());
        REQUIRE(p.get<int>() == 99);
    }

    SECTION("Type mismatch throws bad_cast") {
        auto sharedString = std::make_shared<std::string>("test");
        erased_ptr p{ sharedString };

        REQUIRE_THROWS_AS(p.get<int>(), std::bad_cast);
    }

    SECTION("Erased pointer shares ownership with shared_ptr") {
        auto sharedPtr = std::make_shared<int>(123);
        erased_ptr p{ sharedPtr };

        sharedPtr.reset();

        REQUIRE_NOTHROW(p.get<int>());
        REQUIRE(p.get<int>() == 123);
    }

    SECTION("Erased_ptr stores and retrieves non-primitive type") {
        struct Dummy {
            int x;
            double y;

            Dummy(int xVal, double yVal) : x(xVal), y(yVal) {}
        };

        auto dummyPtr = std::make_shared<Dummy>(10, 3.14);
        erased_ptr p{ dummyPtr };

        REQUIRE_NOTHROW(p.get<Dummy>());
        REQUIRE(p.get<Dummy>().x == 10);
        REQUIRE(p.get<Dummy>().y == 3.14);
    }

    SECTION("Create erased_ptr using make_erased") {
        struct Dummy {
            int x;
            double y;

            Dummy(int xVal, double yVal) : x(xVal), y(yVal) {}
        };

        auto p = make_erased<Dummy>(10, 3.14);

        REQUIRE_NOTHROW(p.get<Dummy>());
        REQUIRE(p.get<Dummy>().x == 10);
        REQUIRE(p.get<Dummy>().y == 3.14);
    }

    SECTION("Move semantics with erased_ptr") {
        auto uniquePtr = std::make_unique<int>(101);
        auto first = erased_ptr{ std::move(uniquePtr) };
        auto second = std::move(first);

        REQUIRE_NOTHROW(second.get<int>());
        REQUIRE(second.get<int>() == 101);

        REQUIRE_THROWS_AS(first.get<int>(), std::bad_cast);
    }

    SECTION("Copying erased_ptr") {
        auto sharedPtr = std::make_shared<int>(78);
        auto original = erased_ptr{ sharedPtr };
        auto copy = original;

        REQUIRE_NOTHROW(copy.get<int>());
        REQUIRE(copy.get<int>() == 78);

        sharedPtr.reset();

        REQUIRE(original.get<int>() == 78);
        REQUIRE(copy.get<int>() == 78);
    }
}