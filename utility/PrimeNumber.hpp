/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_UTILITY_PRIME_NUMBER_HPP_
#define QUICKSTEP_UTILITY_PRIME_NUMBER_HPP_

#include <cstddef>

namespace quickstep {

/** \addtogroup Utility
 *  @{
 */

/**
 * @brief Find the least prime number that is greater than or equal to the
 *        argument.
 *
 * @param number The number to compute.
 * @return The least prime number greater than or equal to the input number.
 **/
std::size_t get_next_prime_number(std::size_t number);

/**
 * @brief Find the greatest prime number that is less than or equal to the
 *        argument.
 *
 * @param number The number to compute.
 * @return The greatest prime number less than or equal the input number, or 0
 *         if no such prime exists (i.e. if the input is 0 or 1).
 **/
std::size_t get_previous_prime_number(std::size_t number);

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_UTILITY_PRIME_NUMBER_HPP_
