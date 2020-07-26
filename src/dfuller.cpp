/**
 **************************************************
 * @file     dfuller.cpp
 * @author   Michael Wagner
 * @date     2020-07-26
 * @brief Some brief description about this module.
 * @details
 * Some long description about this module.
 *
 */

#include <dfuller/dfuller.hpp>

namespace dfuller {

std::string message(const std::string& recipient)
{
    const std::string greeting { "Hello" };
    return recipient.empty() ? greeting : greeting + ", " + recipient;
}

} /* end namespace 'dfuller' */
