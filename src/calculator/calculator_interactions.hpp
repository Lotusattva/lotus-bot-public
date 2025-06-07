#ifndef CALCULATOR_INTERACTIONS_HPP
#define CALCULATOR_INTERACTIONS_HPP

#include <string_view>

using namespace std;

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
    CALC_BUTTON_PILL,
    CALC_BUTTON_EXTRACTOR_QUALITY,
    CALC_BUTTON_EXTRACTOR_NODE,
    CALC_BUTTON_MYRIMON,
    CALC_BUTTON_VASE_YES,
    CALC_BUTTON_VASE_NO,
    CALC_BUTTON_VASE_DETAIL,
    CALC_BUTTON_MIRROR_YES,
    CALC_BUTTON_MIRROR_NO,
    CALC_BUTTON_MIRROR_DETAIL,

    NUM_CALC_BUTTONS
};

constexpr inline string_view const CALC_BUTTON_IDS[NUM_CALC_BUTTONS]{
    "calc_start",          "calc_cancel",        "calc_stage",
    "calc_percent",        "calc_cosmosapsis",   "calc_aura_gem",
    "calc_respira",        "calc_pill",          "calc_extractor_quality",
    "calc_extractor_node", "calc_myrimon",       "calc_vase_yes",
    "calc_vase_no",        "calc_vase_detail",   "calc_mirror_yes",
    "calc_mirror_no",      "calc_mirror_detail",
};

#define SUBCOMMAND_AND_DESCRIPTION 2

constexpr inline string_view CALC_MAIN_COMMAND[SUBCOMMAND_AND_DESCRIPTION]{
    "calc", "cultivation calculator"};

enum calc_subcmd_t {
    CALC_SUBCMD_START,
    CALC_SUBCMD_PERCENT,
    CALC_SUBCMD_COSMOSAPSIS,
    CALC_SUBCMD_RESPIRA,
    CALC_SUBCMD_PILL,
    CALC_SUBCMD_EXTRACTOR,
    CALC_SUBCMD_MYRIMON,

    NUM_CALC_SUBCMDS
};

constexpr inline string_view CALC_SUBCMDS[NUM_CALC_SUBCMDS][SUBCOMMAND_AND_DESCRIPTION]{
    {"start", "start an interactive cultivation calculator session"},
    {"percent", "report percent progress during an interactive calc session"},
    {"cosmosapsis", "report cosmosapsis during an interactive calc session"},
    {"respira", "report respira during an interactive calc session"},
    {"pill", "report pill data during an interactive calc session"},
    {"extractor", "report extractor node levels during an interactive calc session"},
    {"myrimon", "report myrimon fruit data during an interactive calc session"}};

constexpr inline unsigned short CALC_SUBCMD_NUM_PARAM[NUM_CALC_SUBCMDS]{
    0,  // start
    1,  // percent
    1,  // cosmoapsis
    2,  // respira
    5,  // pill
    3,  // extractor
    1,  // myrimon
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
    (string_view * [CALC_SUBCMD_NUM_PARAM[CALC_SUBCMD_PILL]]){
        (string_view[SUBCOMMAND_AND_DESCRIPTION]){"num_daily_pill_attempts",
                                                  "number of daily pill attempts"},
        (string_view[SUBCOMMAND_AND_DESCRIPTION]){"num_rare_pills",
                                                  "number of rare pills consumed"},
        (string_view[SUBCOMMAND_AND_DESCRIPTION]){"num_epic_pills",
                                                  "number of epic pills consumed"},
        (string_view[SUBCOMMAND_AND_DESCRIPTION]){"num_legendary_pills",
                                                  "number of legendary pills consumed"},
        (string_view[SUBCOMMAND_AND_DESCRIPTION]){"pill_exp_bonus",
                                                  "pill experience bonus in percent"}},
    (string_view * [CALC_SUBCMD_NUM_PARAM[CALC_SUBCMD_EXTRACTOR]]){
        (string_view[SUBCOMMAND_AND_DESCRIPTION]){"culti_xp", "level of the cultivation xp node"},
        (string_view[SUBCOMMAND_AND_DESCRIPTION]){"quality", "level of the extractor quality node"},
        (string_view[SUBCOMMAND_AND_DESCRIPTION]){"gush", "level of the extractor gush node"}},
    (string_view * [CALC_SUBCMD_NUM_PARAM[CALC_SUBCMD_MYRIMON]]){
        (string_view[SUBCOMMAND_AND_DESCRIPTION]){"num_myrimon", "number of myrimon fruits"}}};

#endif  // CALCULATOR_INTERACTIONS_HPP