#ifndef CALCULATOR_STATES_HPP
#define CALCULATOR_STATES_HPP

#include <string_view>
#include <array>

using namespace std;

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
    CALC_ASK_MYRIMON,
    CALC_ASK_VASE_OWN,  // whether the user owns a vase
    CALC_ASK_VASE_DETAIL,
    CALC_ASK_MIRROR_OWN,  // whether the user owns a mirror
    CALC_ASK_MIRROR_DETAIL,

    NUM_CALC_STATES
};

constexpr inline array<string_view, NUM_CALC_STATES> CALC_STATE_IDS{
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

#endif // CALCULATOR_STATES_HPP