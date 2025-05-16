#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

#include "global.hpp"
#include "calculator_client.hpp"

/**
 * Represents ownership of calculator sessions
 * maps user-id to (message-id, calculator_client_t)
 */
inline unordered_map<snowflake, pair<snowflake, calculator_client_t>> calc_sessions;

/**
 * @brief verify whether the user causing the interaction is the owner of the session
 *
 * @returns an iterator to the session if the user is the owner, otherwise returns nulllopt
 */
task<optional<unordered_map<snowflake, pair<snowflake, calculator_client_t>>::iterator>> verify_user(const button_click_t& event);

task<void> cancel_calc(const button_click_t& event);

task<void> ask_stage(const button_click_t& event);

inline const component cancel_button{ component()
    .set_type(cot_button)
    .set_style(cos_danger)
    .set_label("CANCEL")
    .set_id("cancel_calc")
};

task<void> ask_percent_progress(const button_click_t& event);

#endif // CALCULATOR_HPP