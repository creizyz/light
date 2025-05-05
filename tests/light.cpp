#include <catch2/catch_all.hpp>

#include "light.hpp"

using namespace light;

TEST_CASE("Test Light class functionality", "[Light]") {
    SECTION("Verify the default constructor") {
        Light obj;

        // Check that the constructor initializes the correct value
        REQUIRE(obj.get_number() == 6);
    }

    SECTION("Simple number test with Light object") {
        Light obj;

        // Ensure the number is not modified unexpectedly
        REQUIRE(obj.get_number() == 6);
        REQUIRE_FALSE(obj.get_number() == 0);
    }
}