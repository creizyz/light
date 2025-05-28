#include <catch2/catch_all.hpp>

#include "light.hpp"
#include "version.hpp"

using namespace light;

TEST_CASE("Test Light class functionality", "[Light]") {
    SECTION("Verify the default constructor")
    {
        Light obj;

        // Check that the constructor initializes the correct value
        REQUIRE(obj.get_number() == 6);
    }

    SECTION("Simple number test with Light object")
    {
        Light obj;

        // Ensure the number is not modified unexpectedly
        REQUIRE(obj.get_number() == 6);
        REQUIRE_FALSE(obj.get_number() == 0);
    }

    SECTION("Verify the project has a valid version")
    {
        REQUIRE(LIGHT_PROJECT_VERSION_MAJOR >= 0);
        REQUIRE(LIGHT_PROJECT_VERSION_MINOR >= 0);
        REQUIRE(LIGHT_PROJECT_VERSION_PATCH >= 0);
    }

    SECTION("Verify the last commit hash was successfully retrieved")
    {
        REQUIRE(strlen(LIGHT_PROJECT_COMMIT_HASH) != 0);
    }
}