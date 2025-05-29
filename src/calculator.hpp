#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

#include "calculator_constants.hpp"
#include "global.hpp"

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
 * - cosmosapsis > 0
 * - pill_bonus > 0
 * - sum(pill_quantity) == daily_pill_attempts
 * - (vase == null) => (mirror == null)
 */
struct calculator_client_t {
    // culitivation progress
    major_stage_t major_stage{ NUM_MAJOR_STAGES }; // default to invalid
    minor_stage_t minor_stage{ NUM_MINOR_STAGES }; // default to invalid
    optional<unsigned> gate;
    double percent_progress;

    // cosmosapsis and aura gem
    double cosmosapsis;
    quality_t aura_gem_quality{ NUM_QUALITIES }; // default to invalid

    // respira
    exp_t respira_exp{ 0 };
    unsigned daily_respira_attempts{ 0 };

    // pills
    unsigned daily_pill_attempts{ 0 };
    unsigned pill_quantity[3]{ 0, 0, 0 }; // 0: rare, 1: epic, 2: legendary
    double pill_bonus{ 0.0 };         // needs to be calculated because game does not show it

    // extractor
    quality_t extractor_quality{ NUM_QUALITIES }; // default to invalid
    unsigned node_levels[3]{ 0, 0, 0 }; // 0: cultiXP, 1: quality, 2: gush

    // myrimon fruit
    unsigned fruit_quantity{ 0 };

    // artifact
    optional<artifact_t> vase{ nullopt };
    bool vase_transmog{ false };
    optional<artifact_t> mirror{ nullopt };
};

typedef unordered_map<snowflake, pair<message, calculator_client_t>> calc_session_map;

/**
 * Represents ownership of calculator sessions
 * maps user-id to (message-id, channel-id, calculator_client_t)
 */
inline calc_session_map calc_sessions;

enum calc_event_t {
    CALC_CANCEL,
    CALC_ASK_STAGE,
    CALC_ASK_PERCENT_PROGRESS,
    CALC_ASK_COSMOSAPSIS,
    CALC_ASK_AURA_GEM,
    CALC_ASK_RESPIRA, // both attempt and exp
    CALC_ASK_PILL,    // attempt, how many of each quality, bonus
    CALC_ASK_EXTRACTOR_QUALITY,
    CALC_ASK_EXTRACTOR_NODE_LVL,
    CALC_ASK_MYRIMON_FRUIT,
    CALC_ASK_VASE_OWN, // whether the user owns a vase
    CALC_ASK_VASE_DETAIL,
    CALC_ASK_MIRROR_OWN, // whether the user owns a mirror
    CALC_ASK_MIRROR_DETAIL,

    NUM_CALC_EVENTS
};

constexpr inline string_view const CALC_EVENT_IDS[NUM_CALC_EVENTS]{
    "calc_cancel",
    "calc_ask_stage",
    "calc_ask_percent_progress",
    "calc_ask_cosmosapsis",
    "calc_ask_aura_gem",
    "calc_ask_respira",
    "calc_ask_pill",
    "calc_ask_extractor_quality",
    "calc_ask_extractor_node_lvl",
    "calc_ask_myrimon_fruit",
    "calc_ask_vase_own",
    "calc_ask_vase_detail",
    "calc_ask_mirror_own",
    "calc_ask_mirror_detail",
};

enum calc_select_t {
    // stage
    CALC_SELECT_MAJOR_STAGE,
    CALC_SELECT_MINOR_STAGE,

    // aura gem
    CALC_SELECT_AURA_GEM_QUALITY,

    // extractor
    CALC_SELECT_EXTRACTOR_QUALITY,

    // vase detail
    CALC_SELECT_VASE_STAR,
    CALC_SELECT_VASE_DAILY_RECHARGE,
    CALC_SELECT_VASE_TRANSMOG,

    // mirror detail
    CALC_SELECT_MIRROR_STAR,
    CALC_SELECT_MIRROR_DAILY_RECHARGE,

    NUM_CALC_SELECTS
};

constexpr inline string_view const CALC_SELECT_IDS[NUM_CALC_SELECTS]{
    "calc_select_major_stage",       "calc_select_minor_stage", "calc_select_aura_gem_quality",
    "calc_select_extractor_quality", "calc_select_vase_star",   "calc_select_vase_daily_recharge",
    "calc_select_vase_transmog",     "calc_select_mirror_star", "calc_select_mirror_daily_recharge",
};

command_option calculator_commands();

task<void> start_interactive_calculator(const slashcommand_t& event);

task<void> calculator_subcommand_handler(const slashcommand_t& event, const command_data_option& subcommand);
task<void> calculator_button_click_handler(const button_click_t& event);
task<void> calculator_select_click_handler(const select_click_t& event);

/**
 * @brief verify whether the user causing the interaction is the owner of the session
 *
 * @returns an iterator to the session if the user is the owner, otherwise returns nullopt
 */
template <std::derived_from<interaction_create_t> T>
task<optional<calc_session_map::iterator>> verify_user(const T& event);

task<void> calc_cancel(const button_click_t& event);

task<void> calc_ask_stage(const button_click_t& event);

task<void> calc_ask_percent_progress(const button_click_t& event);
task<void> process_percent_progress(const slashcommand_t& event);

task<void> calc_ask_cosmosapsis(const button_click_t& event);
task<void> process_cosmosapsis(const slashcommand_t& event);

task<void> calc_ask_aura_gem(const button_click_t& event);

task<void> calc_ask_respira(const button_click_t& event);
task<void> process_respira(const slashcommand_t& event);

task<void> calc_ask_pill(const button_click_t& event);
task<void> process_pill(const slashcommand_t& event);

task<void> calc_ask_extractor_quality(const button_click_t& event);

task<void> calc_ask_extractor_node_lvl(const button_click_t& event);
task<void> process_extractor_node_lvl(const slashcommand_t& event);

task<void> calc_ask_myrimon_fruit(const button_click_t& event);
task<void> process_myrimon_fruit(const slashcommand_t& event);

task<void> calc_ask_vase_own(const button_click_t& event);
task<void> calc_ask_vase_detail(const button_click_t& event);

task<void> calc_ask_mirror_own(const button_click_t& event);
task<void> calc_ask_mirror_detail(const button_click_t& event);

#endif // CALCULATOR_HPP