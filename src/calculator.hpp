#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

#include "global.hpp"
#include "calculator_client.hpp"

task<void> calculator_handler(const button_click_t& event);

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

constexpr inline const char* const CALC_EVENT_IDS[NUM_CALC_EVENTS]{
    "calc_cancel",
    "calc_ask_stage",
    "calc_ask_percent_progress"
};

/**
 * @brief verify whether the user causing the interaction is the owner of the session
 *
 * @returns an iterator to the session if the user is the owner, otherwise returns nulllopt
 */
task<optional<unordered_map<snowflake, pair<snowflake, calculator_client_t>>::iterator>> verify_user(const button_click_t& event);

task<void> calc_cancel(const button_click_t& event);

task<void> calc_ask_stage(const button_click_t& event);

inline const component cancel_button{ component()
    .set_type(cot_button)
    .set_style(cos_danger)
    .set_label("CANCEL")
    .set_id("calc_cancel")
};

task<void> calc_ask_percent_progress(const button_click_t& event);

#endif // CALCULATOR_HPP