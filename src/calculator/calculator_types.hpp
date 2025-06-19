#ifndef CALCULATOR_TYPES_HPP
#define CALCULATOR_TYPES_HPP

#include <optional>

#include "calculator_constants.hpp"
#include "calculator_states.hpp"

using namespace std;

#define INVALID_CALC_STATE calc_state_t::NUM_CALC_STATES
#define INVALID_MAJOR_STAGE major_stage_t::NUM_MAJOR_STAGES
#define INVALID_MINOR_STAGE minor_stage_t::NUM_MINOR_STAGES
#define INVALID_QUALITY quality_t::NUM_QUALITIES
#define INVALID_UNSIGNED_VAL 0
#define INVALID_DOUBLE_VAL -1.0
#define INVALID_EXTRACTOR_NODE_LVL 31
#define INVALID_STAR NUM_ARTIFACT_STARS  // default to invalid, should be in the range [0, 5]

enum binary_t {
    NO,
    YES,
    NUM_YES_NO  // used for invalid yes/no
};

#define INVALID_BINARY_VAL NUM_YES_NO

inline constexpr string_view BINARY_VAL_STR[NUM_YES_NO]{"no", "yes"};

constexpr binary_t get_binary_val(const string_view &id) {
    if (id == BINARY_VAL_STR[NO])
        return NO;
    else if (id == BINARY_VAL_STR[YES])
        return YES;
    else
        return NUM_YES_NO;  // invalid yes/no
}

/**
 * A struct that stores the necessary information for an artifact.
 *
 * Invariants:
 * - star is in the range [0, 5]
 */
struct artifact_t {
    unsigned short star{INVALID_STAR};  // default to invalid, should be in the range [0, 5]

    // 0: no daily recharge, 1: daily recharge, 2 (invalid binary value): not set
    binary_t daily_recharge{INVALID_BINARY_VAL};
};

enum extractor_node_t {
    CULTIXP_NODE,
    QUALITY_NODE,
    GUSH_NODE,

    NUM_EXTRACTOR_NODES
};

enum pill_quantity_index_t {
    RARE_IDX,
    EPIC_IDX,
    LEGENDARY_IDX,

    NUM_PILL_QUANTITIES_INDICES
};

/**
 * A struct that stores the necessary information for a calculator client.
 *
 * Invariants of a fully initialized calculator client:
 * - percent_progress > 0
 * - cosmosapsis > 0
 * - pill_bonus > 0
 * - daiily_respira_attempts > 0
 * - sum(pill_quantity) == daily_pill_attempts
 * - pill_bonus is non-negative
 * - node_levels[i] is in the range [0, 30] for i in [0, 1, 2]
 * - fruit_quantity > 0
 * - (vase == null) -> (trans_mog == false) AND (mirror == null)
 * - any field should not contain an invalid value
 */
struct calculator_client_t {
    calc_state_t calc_state{INVALID_CALC_STATE};  // default to invalid

    // cultivation progress
    major_stage_t major_stage{INVALID_MAJOR_STAGE};  // default to invalid
    minor_stage_t minor_stage{INVALID_MINOR_STAGE};  // default to invalid
    optional<unsigned short> gate;

    // default to invalid, should be a non-negative number
    double percent_progress{INVALID_DOUBLE_VAL};

    // cosmosapsis and aura gem
    double cosmosapsis{INVALID_DOUBLE_VAL};  // default to invalid, should be a non-negative number
    quality_t aura_gem_quality{INVALID_QUALITY};  // default to invalid

    // respira
    double respira_bonus{
        INVALID_DOUBLE_VAL};  // default to invalid, should be a non-negative number
    unsigned short daily_respira_attempts{
        INVALID_UNSIGNED_VAL};  // default to 0, should be a positive number

    // pills
    unsigned short daily_pill_attempts{
        INVALID_UNSIGNED_VAL};  // default to 0, should be a positive number

    // 0: rare, 1: epic, 2: legendary
    unsigned short pill_quantity[NUM_PILL_QUANTITIES_INDICES]{
        INVALID_UNSIGNED_VAL, INVALID_UNSIGNED_VAL, INVALID_UNSIGNED_VAL};
    double pill_bonus{INVALID_DOUBLE_VAL};  // needs to be calculated because game does not show it

    // extractor
    quality_t extractor_quality{INVALID_QUALITY};  // default to invalid

    // 0: cultiXP, 1: quality, 2: gush; default to 31, should be in the range [0, 30]
    unsigned short node_levels[NUM_EXTRACTOR_NODES]{
        INVALID_EXTRACTOR_NODE_LVL, INVALID_EXTRACTOR_NODE_LVL, INVALID_EXTRACTOR_NODE_LVL};

    // myrimon fruit
    unsigned short fruit_quantity{
        INVALID_UNSIGNED_VAL};  // default to 0, should be a positive number

    // artifacts
    optional<artifact_t> vase{nullopt};
    // 0: no transmog, 1: has vase transmog, 2 (invalid binary value): not set
    binary_t vase_transmog{INVALID_BINARY_VAL};
    optional<artifact_t> mirror{nullopt};
};

#endif  // CALCULATOR_TYPES_HPP