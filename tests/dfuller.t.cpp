#include "dfuller/dfuller.hpp"

#include <catch2/catch.hpp>

namespace dfuller {
namespace testing {

TEST_CASE("message construction", "[message]" )
{
    REQUIRE(message(std::string{}) == "Hello");
    REQUIRE(message("World") == "Hello, World");
}

} /* end namespace 'testing' */
} /* end namespace 'dfuller' */
