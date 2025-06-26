#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

#include <chrono>
#include <expected>

#include "calculator_types.hpp"

// game's daily reset happens at 8:00 am UTC-5
// constexpr inline chrono::utc_time DAILY_RESET_TIME{chrono::hours{8 + 5}}; // 8 am + 5 hours offset

enum class calculator_error_t {
    INVALID_CLIENT,  // The calculator client is not fully initialized or has invalid data.
    OVERFLOW_EXP, // The player already has enough exp to breakthrough to the next major stage.
    MYRIMON_NOW, // The player can breakthrough to the next major stage now if they consume the amount of myrimon fruits they have set in the calculator client.
    MYRIMOM_CALCULATION_ERROR, // An error occurred while calculating the myrimon fruit exp.
};

/**
 * @brief Get the estimated time to breakthrough to the next major cultivation stage.
 *
 * Assumptions at the time of calling this function:
 * - The player has used all their daily pill attempts this server day (pill attempts for this server day are not included in the calculation).
 * - The player has used all their daily respira attempts this server day (respira attempts for this server day are not included in the calculation).
 * - The player has used their artifact daily recharges this server day (artifact recharges for this server day are not included in the calculation).
 * - The player's artifact charges are at 0.
 * - The player will consume the number of pills they have set in the calculator client every day.
 * - The player will consume their myrimon fruits by the number they have set in the calculator client in the near future.
 *
 * @param client The calculator client.
 * @return An expected value containing the estimated time to breakthrough in hours, or an error if the calculation fails.
 */
// expected<chrono::hours, calculator_error_t> get_estimated_time_to_breakthrough(const calculator_client_t &client);

bool is_valid_client(const calculator_client_t &client);

double consolidated_gush_chance(double gush_chance);

double calculate_myrimon_fruit_exp(const calculator_client_t &client);

double calculate_artifact_exp(const calculator_client_t &client);

#endif  // CALCULATOR_HPP