#include "calculator.hpp"

#include <print>

#include "../global.hpp"
#include "calculator_constants.hpp"
#include "calculator_types.hpp"

expected<chrono::hours, calculator_error_t> get_estimated_time_to_breakthrough(
    const calculator_client_t &client) {
    if (!is_valid_client(client)) return unexpected(calculator_error_t::INVALID_CLIENT);

    chrono::hours hours_to_breakthrough{0};

    // determine the amount of exp needed to reach the next major stage
    double exp_needed{0.0};
    for (size_t i{client.minor_stage}; i <= LATE; ++i)
        exp_needed += STAGE_EXP_REQ[client.major_stage][i];
    exp_needed -=
        client.percent_progress / 100 * STAGE_EXP_REQ[client.major_stage][client.minor_stage];
    if (exp_needed <= 0.0) return unexpected(calculator_error_t::OVERFLOW_EXP);

    // calculate time to next daily reset rounded to nearest hour
    const chrono::time_point now{chrono::utc_clock::now()};
    const chrono::hh_mm_ss time_of_day{now - chrono::floor<chrono::days>(now)};

    chrono::hours hours_to_next_reset{DAILY_RESET_TIME - time_of_day.hours()};
    if (hours_to_next_reset < chrono::hours{0}) {
        // if the time of day is past the daily reset time, add 24 hours
        hours_to_next_reset += chrono::hours{24};
    }

    if (DEBUG) println(cerr, "Hours to next reset: {}", hours_to_next_reset);

    // calculate exp generate per hour by cultivation and aura gem
    const double cosmosapsis_and_aura_gem{client.cosmosapsis *
                                          (1 + AURA_GEM_MULT[client.aura_gem_quality])};

    if (DEBUG) {
        println(cerr, "Cosmosapsis: {}", client.cosmosapsis);
        println(cerr, "Aura gem quality: {}", QUALITY_STR[client.aura_gem_quality]);
        println(cerr, "Cosmosapsis and aura gem: {}", cosmosapsis_and_aura_gem);
        println(cerr, "Number of cosmosapsis per hour: {}", NUM_COSMOSAPSIS_PER_HOUR);
    }

    const double cultivation_exp_per_hour{cosmosapsis_and_aura_gem * NUM_COSMOSAPSIS_PER_HOUR};

    if (DEBUG) println(cerr, "Cultivation exp per hour: {}", cultivation_exp_per_hour);

    if (const double initial_day_passive_exp{cultivation_exp_per_hour *
                                             hours_to_next_reset.count()};
        initial_day_passive_exp > exp_needed)
        return chrono::hours{static_cast<int>(exp_needed / cultivation_exp_per_hour)};
    else {
        exp_needed -= initial_day_passive_exp;
        hours_to_breakthrough += hours_to_next_reset;
    }

    // calculate exp generate per day by cultivation and aura gem
    const double cultivation_exp_per_day{cultivation_exp_per_hour * 24.0};

    if (DEBUG) println(cerr, "Cultivation exp per day: {}", cultivation_exp_per_day);

    // calculate myrimom fruit exp
    const expected<double, calculator_error_t> calculate_fruit_exp_result{
        calculate_myrimon_fruit_exp(client)};
    if (!calculate_fruit_exp_result) return unexpected(calculate_fruit_exp_result.error());
    const double myrimon_fruit_exp{calculate_fruit_exp_result.value()};

    if (DEBUG) println(cerr, "Myrimon fruit exp: {}", myrimon_fruit_exp);

    exp_needed -= client.fruit_quantity * myrimon_fruit_exp;
    if (exp_needed <= 0.0) return unexpected(calculator_error_t::MYRIMON_NOW);

    // calculate respira exp per day
    const double expected_respira_exp_per_day{
        RESPIRA_BASE_EXP[client.major_stage] * (client.respira_bonus / 100.0) *
        EXPECTED_RESPIRA_MULT * client.daily_respira_attempts};

    if (DEBUG) println(cerr, "Expected respira exp per day: {}", expected_respira_exp_per_day);

    // calculate pill exp per day
    double expected_pill_exp_per_day{0.0};
    for (const auto &[idx, quantity] : client.pill_quantity | views::enumerate)
        // offset index by 2 because we do not count common and uncommon pills (start at rare)
        expected_pill_exp_per_day += PILL_BASE_EXP[client.major_stage][idx + 2] * quantity *
                                     (1.0 + client.pill_bonus / 100.0);

    if (DEBUG) println(cerr, "Expected pill exp per day: {}", expected_pill_exp_per_day);

    // calculate artifact exp per day
    const double expected_artifact_exp_per_day{calculate_artifact_exp(client)};

    if (DEBUG) println(cerr, "Expected artifact exp per day: {}", expected_artifact_exp_per_day);

    const double total_exp_per_day{cultivation_exp_per_day + expected_respira_exp_per_day +
                                   expected_pill_exp_per_day + expected_artifact_exp_per_day};
    chrono::days days_to_breakthrough{static_cast<int>(exp_needed / total_exp_per_day)};
    exp_needed -= days_to_breakthrough.count() * total_exp_per_day;
    if (exp_needed <= 0.0) return hours_to_breakthrough + days_to_breakthrough;

    exp_needed -=
        expected_respira_exp_per_day + expected_pill_exp_per_day + expected_artifact_exp_per_day;

    if (exp_needed <= 0.0)
        return hours_to_breakthrough + days_to_breakthrough;
    else
        return hours_to_breakthrough + days_to_breakthrough +
               chrono::hours{static_cast<int>(exp_needed / cultivation_exp_per_hour)};
}

bool is_valid_client(const calculator_client_t &client) {
    // if (client.major_stage == INVALID_MAJOR_STAGE || client.minor_stage == INVALID_MINOR_STAGE ||
    //     client.percent_progress == INVALID_DOUBLE_VAL || client.cosmosapsis == INVALID_DOUBLE_VAL
    //     || client.aura_gem_quality == INVALID_QUALITY || client.respira_bonus ==
    //     INVALID_DOUBLE_VAL || client.daily_respira_attempts == INVALID_UNSIGNED_VAL ||
    //     client.daily_pill_attempts == INVALID_UNSIGNED_VAL ||
    //     (client.pill_quantity[0] == INVALID_UNSIGNED_VAL &&
    //      client.pill_quantity[1] == INVALID_UNSIGNED_VAL &&
    //      client.pill_quantity[2] == INVALID_UNSIGNED_VAL) ||
    //     (client.pill_quantity[0] + client.pill_quantity[1] + client.pill_quantity[2]) !=
    //         client.daily_pill_attempts ||
    //     client.pill_bonus == INVALID_DOUBLE_VAL || client.extractor_quality == INVALID_QUALITY ||
    //     client.extractor_major_stage_bonus == INVALID_BINARY_VAL ||
    //     client.node_levels[0] == INVALID_EXTRACTOR_NODE_LVL ||
    //     client.node_levels[1] == INVALID_EXTRACTOR_NODE_LVL ||
    //     client.node_levels[2] == INVALID_EXTRACTOR_NODE_LVL ||
    //     client.fruit_quantity == INVALID_UNSIGNED_VAL)

    // {
    //     if (DEBUG) println(cerr, "Client basic info invalid");
    //     return false;
    // }

    if (client.major_stage == INVALID_MAJOR_STAGE) {
        if (DEBUG) println(cerr, "Major stage is invalid");
        return false;
    }

    if (client.minor_stage == INVALID_MINOR_STAGE) {
        if (DEBUG) println(cerr, "Minor stage is invalid");
        return false;
    }

    if (client.percent_progress == INVALID_DOUBLE_VAL) {
        if (DEBUG) println(cerr, "Percent progress is invalid");
        return false;
    }

    if (client.cosmosapsis == INVALID_DOUBLE_VAL) {
        if (DEBUG) println(cerr, "Cosmosapsis is invalid");
        return false;
    }

    if (client.aura_gem_quality == INVALID_QUALITY) {
        if (DEBUG) println(cerr, "Aura gem quality is invalid");
        return false;
    }

    if (client.respira_bonus == INVALID_DOUBLE_VAL) {
        if (DEBUG) println(cerr, "Respira bonus is invalid");
        return false;
    }

    if (client.daily_respira_attempts == INVALID_UNSIGNED_VAL) {
        if (DEBUG) println(cerr, "Daily respira attempts is invalid");
        return false;
    }

    if (client.daily_pill_attempts == INVALID_UNSIGNED_VAL) {
        if (DEBUG) println(cerr, "Daily pill attempts is invalid");
        return false;
    }

    if ((client.pill_quantity[RARE_IDX] == INVALID_UNSIGNED_VAL &&
         client.pill_quantity[EPIC_IDX] == INVALID_UNSIGNED_VAL &&
         client.pill_quantity[LEGENDARY_IDX] == INVALID_UNSIGNED_VAL) ||
        (client.pill_quantity[RARE_IDX] + client.pill_quantity[EPIC_IDX] +
         client.pill_quantity[LEGENDARY_IDX]) != client.daily_pill_attempts) {
        if (DEBUG) println(cerr, "Pill quantity is invalid");
        return false;
    }

    if (client.pill_bonus == INVALID_DOUBLE_VAL) {
        if (DEBUG) println(cerr, "Pill bonus is invalid");
        return false;
    }

    if (client.extractor_quality == INVALID_QUALITY) {
        if (DEBUG) println(cerr, "Extractor quality is invalid");
        return false;
    }

    if (client.extractor_major_stage_bonus == INVALID_BINARY_VAL) {
        if (DEBUG) println(cerr, "Extractor major stage bonus is invalid");
        return false;
    }

    if (client.node_levels[CULTIXP_NODE] == INVALID_EXTRACTOR_NODE_LVL ||
        client.node_levels[QUALITY_NODE] == INVALID_EXTRACTOR_NODE_LVL ||
        client.node_levels[GUSH_NODE] == INVALID_EXTRACTOR_NODE_LVL) {
        if (DEBUG) println(cerr, "Extractor node levels are invalid");
        return false;
    }

    if (client.fruit_quantity == INVALID_UNSIGNED_VAL) {
        if (DEBUG) println(cerr, "Fruit quantity is invalid");
        return false;
    }

    if (client.vase.has_value())
        if (client.vase->star == INVALID_STAR ||
            client.vase->daily_recharge == INVALID_BINARY_VAL ||
            client.vase_transmog == INVALID_BINARY_VAL) {
            if (DEBUG) println(cerr, "Vase info invalid");
            return false;
        }

    if (client.mirror.has_value())
        if (client.mirror->star == INVALID_STAR ||
            client.mirror->daily_recharge == INVALID_BINARY_VAL) {
            if (DEBUG) println(cerr, "Mirror info invalid");
            return false;
        }

    return true;
}

double consolidated_gush_chance(double gush_chance) {
    return 1. / ((2. - gush_chance) * (3. + (gush_chance - 3.) * gush_chance) *
                 (1. + (gush_chance - 1.) * gush_chance));
}

double calculate_myrimon_fruit_exp(const calculator_client_t &client) {
    // get base exp of the fruit based on its rank
    const fruit_rank_t fruit_rank{get_fruit_rank(client.major_stage)};

    const exp_t base_exp{FRUIT_BASE_EXP[fruit_rank]};

    // get probability distribution of quality
    array<double, NUM_QUALITIES> orb_quality_probabilities{
        NODE_QUALITY_CHANCE[client.node_levels[QUALITY_NODE]]};
    // The aura extractor adds a 30% chance to produce an orb of the same quality as itself
    orb_quality_probabilities[client.extractor_quality] += 0.3;

    if (DEBUG)
        assert(abs(accumulate(orb_quality_probabilities.begin(), orb_quality_probabilities.end(),
                              0.0) -
                   1.0) < 1e-10);

    // calculate multiplier from CultiXP node
    // A conditional 20% exp bonus is applied to exp orb of a certain quality if the "CultiXP"
    // extractor node is at or above this quality
    const quality_t cultixp_node_quality{
        get_extractor_node_quality(client.node_levels[CULTIXP_NODE])};
    double prob_cultixp_node_quality_higher_or_equal_to_orb{0.0};
    for (const auto &[quality, prob] : orb_quality_probabilities | views::enumerate)
        if (cultixp_node_quality >= quality)
            prob_cultixp_node_quality_higher_or_equal_to_orb += prob;
    const double cultixp_node_level_mult{client.node_levels[CULTIXP_NODE] *
                                         MULT_PER_CULTIXP_NODE_LEVEL};
    const double expected_conditional_20_percent_exp_bonus{
        prob_cultixp_node_quality_higher_or_equal_to_orb * 0.2};
    const double expected_cultixp_mult{1.0 + cultixp_node_level_mult +
                                       expected_conditional_20_percent_exp_bonus};

    // calculate multiplier from Quality node
    const double expected_quality_mult{inner_product(orb_quality_probabilities.begin(),
                                                     orb_quality_probabilities.end(),
                                                     QUALITY_MULT.begin(), 0.0)};

    // calculate multiplier from
    const double gush_chance{0.1 + static_cast<double>(client.extractor_quality) *
                                       GUSH_CHANCE_PER_QUALITY};
    const double gush_mult{1.5 + EXP_GUSH_MULT_PER_NODE_LVL * client.node_levels[GUSH_NODE]};

    const double expected_gush_mult{consolidated_gush_chance(gush_chance) * gush_mult};

    const double major_stage_bonus{
        client.extractor_major_stage_bonus == YES ? 1.0 + MAJOR_STAGE_BONUS : 1.0};

    return static_cast<double>(base_exp) * expected_cultixp_mult * major_stage_bonus *
           expected_quality_mult * expected_gush_mult;
}

double calculate_artifact_exp(const calculator_client_t &client) {
    // one energy recovery cycle is 15 mins
    static const int NUM_ENERGY_RECOVERY_CYCLE_PER_DAY{24 * 60 / 15};

    if (!client.vase) return 0.0;

    // calculate the total energy recovery for vase per day
    const double vase_energy_per_day{
        ARTIFACT_ENERGY_RECOVERY_RATE[client.vase->star] * NUM_ENERGY_RECOVERY_CYCLE_PER_DAY +
        (client.vase->daily_recharge == YES ? ARTIFACT_DAILY_ENERGY_RECHARGE : 0.0)};

    // calculate exp per mythic pill
    double mythic_bonus;
    switch (client.vase->star) {
        // no mythic bonus
        case 0:
            mythic_bonus = 0.0;
            break;
        // 1 and 2 stars
        case 1:
        case 2:
            mythic_bonus = VASE_1_STAR_PILL_BONUS;
            break;
        // 3 stars and above
        default:
            mythic_bonus = VASE_3_STAR_PILL_BONUS;
            break;
    }
    if (client.vase_transmog == YES)
        mythic_bonus += VASE_TRANSMOG_PILL_BONUS;  // transmog adds a flat bonus

    const double mythic_pill_exp{PILL_BASE_EXP[client.major_stage][MYTHIC] *
                                 (1.0 + mythic_bonus + client.pill_bonus / 100.0)};

    // calculate expected exp from mythic pills per day
    const double expected_vase_mythic_pill_exp_per_day{
        client.vase->star == 5 ?
                               // if vase is 5 star, add correction for costless chance
            mythic_pill_exp / (1.0 - VASE_5_STAR_COSTLESS_CHANCE) * vase_energy_per_day /
                VASE_MYTHIC_PILL_COST
                               :
                               // if below 5 star, calculate normally
            mythic_pill_exp * vase_energy_per_day / VASE_MYTHIC_PILL_COST};

    if (!client.mirror) return expected_vase_mythic_pill_exp_per_day;

    const double mirror_energy_per_day{
        ARTIFACT_ENERGY_RECOVERY_RATE[client.mirror->star] * NUM_ENERGY_RECOVERY_CYCLE_PER_DAY +
        (client.mirror->daily_recharge == YES ? ARTIFACT_DAILY_ENERGY_RECHARGE : 0.0)};

    double mirror_duplication_cost{MIRROR_DUPLICATE_MYTHIC_PILL_COST};
    switch (client.mirror->star) {
        // no cost reduction
        case 0:
            break;
        // 1 and 2 stars
        case 1:
        case 2:
            mirror_duplication_cost *= (1.0 - MIRROR_1_STAR_COST_DEDUCTION);
            break;
        // 3 stars and above
        default:
            mirror_duplication_cost *= (1.0 - MIRROR_3_STAR_COST_DEDUCTION);
            break;
    }

    static const double NUM_DUP_FROM_5_STAR_MIRROR{(1.0 - MIRROR_5_STAR_CHANCE_EXTRA_DUPLICATE) *
                                                       1.0 +
                                                   MIRROR_5_STAR_CHANCE_EXTRA_DUPLICATE * 2.0};

    const double num_dup_mythic_pill_per_cost{client.mirror->star == 5 ? NUM_DUP_FROM_5_STAR_MIRROR
                                                                       : 1.0};

    const double mirror_mythic_pill_dup_per_day{NUM_ENERGY_RECOVERY_CYCLE_PER_DAY /
                                                MIRROR_DUPLICATE_MYTHIC_PILL_COST *
                                                num_dup_mythic_pill_per_cost};

    const double expected_mirror_mythic_pill_exp_per_day{mirror_mythic_pill_dup_per_day *
                                                         mythic_pill_exp};

    return expected_vase_mythic_pill_exp_per_day + expected_mirror_mythic_pill_exp_per_day;
}