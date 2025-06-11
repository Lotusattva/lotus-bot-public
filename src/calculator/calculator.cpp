#include "calculator.hpp"

#include "calculator_constants.hpp"
#include "calculator_types.hpp"

expected<chrono::hours, calculator_error_t> get_estimated_time_to_breakthrough(
    const calculator_client_t &client) {
    if (!is_valid_client(client)) return unexpected(calculator_error_t::INVALID_CLIENT);

    // determine the amount of exp needed to reach the next major stage

    // placeholder return value
    return unexpected(calculator_error_t::INVALID_CLIENT);
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