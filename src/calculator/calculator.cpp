#include "calculator.hpp"

#include "../global.hpp"
#include "calculator_constants.hpp"
#include "calculator_types.hpp"

expected<chrono::hours, calculator_error_t> get_estimated_time_to_breakthrough(
    const calculator_client_t &client) {
    if (!is_valid_client(client)) return unexpected(calculator_error_t::INVALID_CLIENT);

    // determine the amount of exp needed to reach the next major stage
    double exp_needed{0.0};
    for (size_t i{LATE}; i >= client.minor_stage; --i)
        exp_needed += STAGE_EXP_REQ[client.major_stage][i];
    exp_needed -=
        client.percent_progress / 100 * STAGE_EXP_REQ[client.major_stage][client.minor_stage];
    if (exp_needed <= 0.0) return unexpected(calculator_error_t::OVERFLOW_EXP);

    // calculate time to next daily reset rounded to nearest hour
    const auto now{chrono::utc_clock::now()};
    const auto next_reset{DAILY_RESET_TIME +
                          chrono::days{(now.time_since_epoch().count() / 86400) + 1}};
    const auto hours_to_next_reset{chrono::duration_cast<chrono::hours>(next_reset - now)};

    // calculate exp generate per hour by cultivation and aura gem
    double cosmosapsis_and_aura_gem{client.cosmosapsis *
                                    (1 + AURA_GEM_MULT[client.aura_gem_quality])};
    double exp_per_hour{cosmosapsis_and_aura_gem / NUM_COSMOSAPSIS_PER_HOUR};
    if (double initial_day_passive_exp{exp_per_hour * hours_to_next_reset.count()};
        initial_day_passive_exp > exp_needed)
        return chrono::hours{static_cast<int>(exp_needed / exp_per_hour)};
    else
        exp_needed -= initial_day_passive_exp;

    // calculate myrimom fruit exp

    // placeholder return value
    return chrono::hours{0};
}

bool is_valid_client(const calculator_client_t &client) {
    if (client.major_stage == INVALID_MAJOR_STAGE || client.minor_stage == INVALID_MINOR_STAGE ||
        client.percent_progress == INVALID_DOUBLE_VAL || client.cosmosapsis == INVALID_DOUBLE_VAL ||
        client.aura_gem_quality == INVALID_QUALITY || client.respira_bonus == INVALID_DOUBLE_VAL ||
        client.daily_respira_attempts == INVALID_UNSIGNED_VAL ||
        client.daily_pill_attempts == INVALID_UNSIGNED_VAL ||
        (client.pill_quantity[0] == INVALID_UNSIGNED_VAL &&
         client.pill_quantity[1] == INVALID_UNSIGNED_VAL &&
         client.pill_quantity[2] == INVALID_UNSIGNED_VAL) ||
        (client.pill_quantity[0] + client.pill_quantity[1] + client.pill_quantity[2]) !=
            client.daily_pill_attempts ||
        client.pill_bonus == INVALID_DOUBLE_VAL || client.extractor_quality == INVALID_QUALITY ||
        client.node_levels[0] == INVALID_EXTRACTOR_NODE_LVL ||
        client.node_levels[1] == INVALID_EXTRACTOR_NODE_LVL ||
        client.node_levels[2] == INVALID_EXTRACTOR_NODE_LVL ||
        client.fruit_quantity == INVALID_UNSIGNED_VAL)

        return false;

    if (client.vase.has_value())
        if (client.vase->star == INVALID_STAR ||
            client.vase->daily_recharge == INVALID_BINARY_VAL ||
            client.vase_transmog == INVALID_BINARY_VAL)
            return false;

    if (client.mirror.has_value())
        if (client.mirror->star == INVALID_STAR ||
            client.mirror->daily_recharge == INVALID_BINARY_VAL)
            return false;

    return true;
}

double consolidated_gush_chance(double gush_chance) {
    return 1. / ((2. - gush_chance) * (3. + (gush_chance - 3.) * gush_chance) *
                 (1. + (gush_chance - 1.) * gush_chance));
}

double calculate_myrimon_fruit_exp(const calculator_client_t &client) {
    // get probability distribution of quality
    array<double, NUM_QUALITIES> quality_probabilities{
        NODE_QUALITY_CHANCE[client.node_levels[QUALITY_NODE]]};
    // The aura extractor adds a 30% chance to produce an orb of the same quality as itself
    quality_probabilities[client.extractor_quality] += 0.3;
    
    if (DEBUG)
        assert(abs(accumulate(quality_probabilities.begin(), quality_probabilities.end(), 0.0) -
                   1.0) < 1e-10);


}