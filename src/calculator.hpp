#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

#include "global.hpp"
#include "calculator_constants.hpp"

/**
 * A struct that stores the necessary information for an artifact.
 *
 * Invariants:
 * - star is in the range [0, MAX_ARTIFACT_STAR]
 */
struct artifact_t {
    unsigned star;
    bool daily_recharge;
};

/**
 * A struct that stores the necessary information for a calculator client.
 *
 * Invariants:
 * - percent_progress > 0
 * - pill_bonus > 0
 * - sum(pill_quantity) == daily_pill_attempts
 * - (vase == null) => (mirror == null)
 */
struct calculator_client_t {
    // culitivation progress
    major_stage_t major_stage;
    minor_stage_t minor_stage;
    optional<unsigned> gate;
    double percent_progress;

    // aura gem
    quality_t aura_gem_quality;

    // respira
    exp_t respira_exp;
    unsigned daily_respira_attempts;

    // pills
    unsigned daily_pill_attempts;
    unsigned pill_quantity[3]; // 0: rare, 1: epic, 2: legendary
    double pill_bonus; // needs to be calculated because game does not show it

    // extractor
    quality_t extractor_quality;
    unsigned node_levels[3]; // 0: cultiXP, 1: quality, 2: gush

    // myrimon fruit
    unsigned fruit_quantity;

    // artifact
    optional<artifact_t> vase;
    bool vase_transmog;
    optional<artifact_t> mirror;
};

/**
 * Represents ownership of calculator sessions
 * maps user-id to (message-id, calculator_client_t)
 */
inline unordered_map<snowflake, pair<snowflake, calculator_client_t>> calc_sessions;

enum calc_event_t {
    CALC_CANCEL,
    CALC_ASK_STAGE,
    CALC_ASK_PERCENT_PROGRESS,

    NUM_CALC_EVENTS
};

constexpr inline string_view const CALC_EVENT_IDS[NUM_CALC_EVENTS]{
    "calc_cancel",
    "calc_ask_stage",
    "calc_ask_percent_progress"
};

enum calc_select_t {
    CALC_SELECT_MAJOR_STAGE,
    CALC_SELECT_MINOR_STAGE,

    NUM_CALC_SELECTS
};

constexpr inline string_view const CALC_SELECT_IDS[NUM_CALC_SELECTS]{
    "calc_select_major_stage",
    "calc_select_minor_stage"
};

task<void> start_interactive_calculator(const slashcommand_t& event);

task<void> calculator_button_click_handler(const button_click_t& event);
task<void> calculator_select_click_handler(const select_click_t& event);

/**
 * @brief verify whether the user causing the interaction is the owner of the session
 *
 * @returns an iterator to the session if the user is the owner, otherwise returns nullopt
 */
template<std::derived_from<interaction_create_t> T>
task<optional<unordered_map<snowflake, pair<snowflake, calculator_client_t>>::iterator>> verify_user(const T& event);

task<void> calc_cancel(const button_click_t& event);

task<void> calc_ask_stage(const button_click_t& event);

task<void> calc_ask_percent_progress(const button_click_t& event);

#endif // CALCULATOR_HPP