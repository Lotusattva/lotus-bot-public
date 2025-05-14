#ifndef CALCULATOR_CLIENT_HPP
#define CALCULATOR_CLIENT_HPP

#include "calculator_constants.hpp"
#include <optional>
using std::optional;

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

#endif