#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

#include <dpp/appcommand.h>

#include <string>

#include "calculator_constants.hpp"
#include "global.hpp"

enum calc_state_t {
    CALC_CANCEL,
    CALC_ASK_STAGE,
    CALC_ASK_PERCENT_PROGRESS,
    CALC_ASK_COSMOSAPSIS,
    CALC_ASK_AURA_GEM,
    CALC_ASK_RESPIRA,  // both attempt and exp
    CALC_ASK_PILL,     // attempt, how many of each quality, bonus
    CALC_ASK_EXTRACTOR_QUALITY,
    CALC_ASK_EXTRACTOR_NODE_LVL,
    CALC_ASK_MYRIMON_FRUIT,
    CALC_ASK_VASE_OWN,  // whether the user owns a vase
    CALC_ASK_VASE_DETAIL,
    CALC_ASK_MIRROR_OWN,  // whether the user owns a mirror
    CALC_ASK_MIRROR_DETAIL,

    NUM_CALC_STATES
};

constexpr inline string_view const CALC_STATE_IDS[NUM_CALC_STATES]{
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
    calc_state_t calc_state{NUM_CALC_STATES};

    // culitivation progress
    major_stage_t major_stage{NUM_MAJOR_STAGES};  // default to invalid
    minor_stage_t minor_stage{NUM_MINOR_STAGES};  // default to invalid
    optional<unsigned> gate;
    double percent_progress{-1.0};  // default to invalid, should be a non-negative number

    // cosmosapsis and aura gem
    double cosmosapsis{-1.0};  // default to invalid, should be a non-negative number
    quality_t aura_gem_quality{NUM_QUALITIES};  // default to invalid

    // respira
    exp_t respira_exp{0};                // default to 0, should be a positive number
    unsigned daily_respira_attempts{0};  // default to 0, should be a positive number

    // pills
    unsigned daily_pill_attempts{0};     // default to 0, should be a positive number
    unsigned pill_quantity[3]{0, 0, 0};  // 0: rare, 1: epic, 2: legendary
    double pill_bonus{0.0};              // needs to be calculated because game does not show it

    // extractor
    quality_t extractor_quality{NUM_QUALITIES};  // default to invalid

    // 0: cultiXP, 1: quality, 2: gush; default to 31, should be in the range [0, 30]
    unsigned node_levels[3]{31, 31, 31};

    // myrimon fruit
    unsigned fruit_quantity{0};  // default to 0, should be a positive number

    // artifact
    optional<artifact_t> vase{nullopt};
    bool vase_transmog{false};
    optional<artifact_t> mirror{nullopt};
};

typedef unordered_map<snowflake, pair<message, calculator_client_t>> calc_session_map;

/**
 * Represents ownership of calculator sessions
 * maps user-id to (message, calculator_client_t)
 */
inline calc_session_map calc_sessions;

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

enum calc_button_t {
    CALC_BUTTON_START,
    CALC_BUTTON_CANCEL,
    CALC_BUTTON_STAGE,
    CALC_BUTTON_PERCENT,
    CALC_BUTTON_COSMOSAPSIS,
    CALC_BUTTON_AURA_GEM,
    CALC_BUTTON_RESPIRA,
    CALC_BUTTON_EXTRACTOR_QUALITY,
    CALC_BUTTON_VASE_YES,
    CALC_BUTTON_VASE_NO,
    CALC_BUTTON_VASE_DETAIL,
    CALC_BUTTON_MIRROR_YES,
    CALC_BUTTON_MIRROR_NO,
    CALC_BUTTON_MIRROR_DETAIL,

    NUM_CALC_BUTTONS
};

constexpr inline string_view const CALC_BUTTON_IDS[NUM_CALC_BUTTONS]{
    "calc_start",
    "calc_cancel",
    "calc_stage",
    "calc_percent",
    "calc_cosmosapsis",
    "calc_aura_gem",
    "calc_respira",
    "calc_extractor_quality",
    "calc_vase_yes",
    "calc_vase_no",
    "calc_vase_detail",
    "calc_mirror_yes",
    "calc_mirror_no",
    "calc_mirror_detail",
};

#define SUBCOMMAND_AND_DESCRIPTION 2

constexpr inline string_view CALC_MAIN_COMMAND[SUBCOMMAND_AND_DESCRIPTION]{
    "calc", "cultivation calculator"};

enum CALC_SUBCMD_t {
    CALC_SUBCMD_START,
    CALC_SUBCMD_PERCENT,
    CALC_SUBCMD_COSMOSAPSIS,
    CALC_SUBCMD_RESPIRA,

    NUM_CALC_SUBCMDS
};

constexpr inline string_view CALC_SUBCMDS[NUM_CALC_SUBCMDS][SUBCOMMAND_AND_DESCRIPTION]{
    {"start", "start an interactive cultivation calculator session"},
    {"percent", "report percent progress during an interactive calc session"},
    {"cosmosapsis", "report cosmosapsis during an interactive calc session"},
    {"respira", "report respira during an interactive calc session"}};

constexpr inline unsigned short CALC_SUBCMD_NUM_PARAM[NUM_CALC_SUBCMDS]{
    0,  // start
    1,  // percent
    1,  // cosmoapsis
    2,  // respira
};

constexpr inline string_view **CALC_SUBCMD_PARAM[NUM_CALC_SUBCMDS]{
    (string_view * [CALC_SUBCMD_NUM_PARAM[CALC_SUBCMD_START]]){},
    (string_view *
     [CALC_SUBCMD_NUM_PARAM[CALC_SUBCMD_PERCENT]]){(string_view[SUBCOMMAND_AND_DESCRIPTION]){
        "percentage_val", "percentage displayed on top left corner of the screen"}},
    (string_view *
     [CALC_SUBCMD_NUM_PARAM[CALC_SUBCMD_COSMOSAPSIS]]){(string_view[SUBCOMMAND_AND_DESCRIPTION]){
        "cosmosapsis_val", "cosmosapsis displayed in your abode"}},
    (string_view * [CALC_SUBCMD_NUM_PARAM[CALC_SUBCMD_RESPIRA]]){
        (string_view[SUBCOMMAND_AND_DESCRIPTION]){"respira_exp_per_attempt",
                                                  "respira exp per attempt"},
        (string_view[SUBCOMMAND_AND_DESCRIPTION]){"num_daily_respira_attempts",
                                                  "number of daily respira attempts"}},
};

command_option calculator_commands();

task<void> start_interactive_calculator(const slashcommand_t &event);

task<void> calculator_subcommand_handler(const slashcommand_t &event,
                                         const command_data_option &options);
task<void> calculator_button_click_handler(const button_click_t &event);
task<void> calculator_select_click_handler(const select_click_t &event);

/**
 * @brief verify whether the user causing the interaction is the owner of the
 * session
 *
 * @returns an iterator to the session if the user is the owner, otherwise
 * returns nullopt
 */
template <std::derived_from<interaction_create_t> T>
task<optional<pair<message, calculator_client_t> *>> verify_user(const T &event);
task<optional<pair<message, calculator_client_t> *>> verify_user(const slashcommand_t &event);
string print_client_info(const calculator_client_t &client);

task<void> non_session_interaction(const slashcommand_t &event);

task<void> calc_cancel(const button_click_t &event);

task<void> calc_ask_stage(const button_click_t &event);

task<void> calc_ask_percent_progress(const button_click_t &event);
task<void> process_percent_progress(const slashcommand_t &event);

task<void> calc_ask_cosmosapsis(const button_click_t &event);
task<void> process_cosmosapsis(const slashcommand_t &event);

task<void> calc_ask_aura_gem(const button_click_t &event);

task<void> calc_ask_respira(const button_click_t &event);
task<void> process_respira(const slashcommand_t &event);

task<void> calc_ask_pill(const button_click_t &event);
task<void> process_pill(const slashcommand_t &event);

task<void> calc_ask_extractor_quality(const button_click_t &event);

task<void> calc_ask_extractor_node_lvl(const button_click_t &event);
task<void> process_extractor_node_lvl(const slashcommand_t &event);

task<void> calc_ask_myrimon_fruit(const button_click_t &event);
task<void> process_myrimon_fruit(const slashcommand_t &event);

task<void> calc_ask_vase_own(const button_click_t &event);
task<void> calc_ask_vase_detail(const button_click_t &event);

task<void> calc_ask_mirror_own(const button_click_t &event);
task<void> calc_ask_mirror_detail(const button_click_t &event);

task<void> calc_under_construction(const button_click_t &event);

#endif  // CALCULATOR_HPP