#ifndef CALCULATOR_INTERFACE_HPP
#define CALCULATOR_INTERFACE_HPP

#include <dpp/dpp.h>

#include <string>

#include "calculator_interactions.hpp"
#include "calculator_types.hpp"
#include "../global.hpp"

using namespace dpp;

typedef unordered_map<snowflake, pair<message, calculator_client_t>> calc_session_map;

/**
 * Represents ownership of calculator sessions
 * maps user-id to (message, calculator_client_t)
 */
inline calc_session_map calc_sessions;

// Register slashcommands for the calculator
command_option calculator_commands();

// Control flow for calculator subcommands
task<void> calculator_subcommand_handler(const slashcommand_t &event,
                                         const command_data_option &options);

// Control flow for calculator button clicks
task<void> calculator_button_click_handler(const button_click_t &event);

// Control flow for calculator select clicks
task<void> calculator_select_click_handler(const select_click_t &event);

/**
 * @brief verify whether the user causing the interaction is the owner of the
 * session
 *
 * @returns an iterator to the session if the user is the owner, otherwise
 * returns nullopt
 */
template <derived_from<interaction_create_t> T>
task<optional<pair<message, calculator_client_t> *>> verify_user(const T &event) {
    snowflake user_id{event.command.usr.id};

    if (DEBUG) cerr << "Verifying user: " << user_id << endl;

    auto it{calc_sessions.find(user_id)};
    if (it == calc_sessions.end()) {
        if (DEBUG) cerr << "this user is not the owner of this session." << endl;
        co_return nullopt;
    }

    if (DEBUG) cerr << "querying message and channel ID..." << endl;

    confirmation_callback_t callback{co_await event.co_get_original_response()};
    if (callback.is_error()) {
        cerr << "Error: " << callback.get_error().message << endl;
        co_return nullopt;
    }
    message msg{callback.get<message>()};

    if (DEBUG) {
        cerr << "Message ID: " << msg.id << endl;
        cerr << "Channel ID: " << msg.channel_id << endl;
    }

    if (it->second.first.id != msg.id) {
        if (DEBUG)
            cerr << "Message ID: " << msg.id
                 << " does not match the cached message ID: " << it->second.first.id << endl;
        co_return nullopt;
    }

    if (DEBUG) cerr << "User ID: " << user_id << " is the owner of this session." << endl;
    co_return &it->second;
}

task<optional<pair<message, calculator_client_t> *>> verify_user(const slashcommand_t &event);
string print_client_info(const calculator_client_t &client);

// Notify non-users that they should only use certain commands while in a session
task<void> non_session_interaction(const slashcommand_t &event);

// component factories

inline const component cancel_button{component()
                                         .set_type(cot_button)
                                         .set_style(cos_danger)
                                         .set_label("CANCEL")
                                         .set_id(CALC_BUTTON_IDS[CALC_BUTTON_CANCEL])};


component selectmenu_factory(const string_view &id, const string_view &placeholder,
                             const span<const string_view> &options);

///////// Below are calculator events/states

task<void> start_interactive_calculator(const slashcommand_t &event);

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

task<void> calc_ask_extractor(const button_click_t &event);

task<void> calc_ask_extractor_node(const button_click_t &event);
task<void> process_extractor_node(const slashcommand_t &event);

task<void> calc_ask_myrimon(const button_click_t &event);
task<void> process_myrimon(const slashcommand_t &event);

task<void> calc_ask_vase_own(const button_click_t &event);
task<void> calc_ask_vase_detail(const button_click_t &event);

task<void> calc_ask_mirror_own(const button_click_t &event);
task<void> calc_ask_mirror_detail(const button_click_t &event);

task<void> calc_under_construction(const button_click_t &event);

#endif  // CALCULATOR_INTERFACE_HPP