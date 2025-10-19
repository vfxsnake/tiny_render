#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../vec.hpp"
#include "../model.h"

TEST_CASE("Vector tests") {
    SECTION("Vector addition") {
        Vec2i v1(1, 2);
        Vec2i v2(3, 4);
        Vec2i sum = v1 + v2;
        REQUIRE(sum.x == 4);
        REQUIRE(sum.y == 6);
    }

    SECTION("Vector subtraction") {
        Vec2i v1(1, 2);
        Vec2i v2(3, 4);
        Vec2i diff = v1 - v2;
        REQUIRE(diff.x == -2);
        REQUIRE(diff.y == -2);
    }
}

TEST_CASE("Model tests") {
    SECTION("Model loading") {
        Model model = loadModel("../obj/african_head/african_head.obj");
        REQUIRE(model.numberOfVertices() == 1258);
        REQUIRE(model.numberOfFaces() == 2492);
    }

    SECTION("Invalid model loading") {
        REQUIRE_THROWS(loadModel("non_existent_file.obj"));
    }
}