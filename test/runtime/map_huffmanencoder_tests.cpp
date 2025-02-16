#include <catch2/catch.hpp>
#include <string>

#include <squeeze/squeeze.h>

using Catch::Matchers::Equals;
using namespace squeeze;

enum class Key {
    String_1,
    String_2,
    String_3
};

static auto buildMapStrings = [] {
    return std::to_array<KeyedStringView<Key>> ({
        // out of order and missing a value
        {Key::String_3, "Third String"},
        {Key::String_1, "First String"},
    });
};


SCENARIO("StringMap<HuffmanEncoder> Can provide the correct strings", "[StringMap][HuffmanEncoder]")
{
    GIVEN("A runtime initialised StringMap<HuffmanEncoder>") {
        auto const map = StringMap<Key, HuffmanEncoder>(buildMapStrings);

        THEN("The number of strings should be correct") {
            REQUIRE(map.count() == 2);
        }

        AND_GIVEN("The String_1 key is contained in the map") {
            REQUIRE(map.contains(Key::String_1));

            WHEN("The String_1 string is retrieved") {
                // convert to strings so Catch can use them
                auto s = map.get(Key::String_1);
                auto t = std::string{s.begin(), s.end()};

                THEN("The string should match the source data") {
                    REQUIRE_THAT( t, Equals("First String") );
                }
            }
        }

        AND_GIVEN("The String_2 key is absent from the map"){
            REQUIRE_FALSE(map.contains(Key::String_2));

            WHEN("The String_2 string is retrieved") {
                THEN("An exception should be thrown") {
                    REQUIRE_THROWS(map.get(Key::String_2));
                }
            }
        }

        AND_GIVEN("The String_3 key is contained in the map") {
            REQUIRE(map.contains(Key::String_3));

            WHEN("The String_3 string is retrieved") {
                // convert to strings so Catch can use them
                auto s = map.get(Key::String_3);
                auto t = std::string{s.begin(), s.end()};

                THEN("The string should match the source data") {
                    REQUIRE_THAT( t, Equals("Third String") );
                }
            }
        }

    }
}
