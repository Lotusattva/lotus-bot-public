#include "calculator.hpp"

#include <dpp/appcommand.h>
#include <dpp/restresults.h>

#include <string>

#include "calculator_constants.hpp"

static const component cancel_button{component()
                                         .set_type(cot_button)
                                         .set_style(cos_danger)
                                         .set_label("CANCEL")
                                         .set_id(CALC_BUTTON_IDS[CALC_BUTTON_CANCEL])};

command_option calculator_commands() {
    return command_option{co_sub_command_group, string{CALC_MAIN_COMMAND[0]},
                          string{CALC_MAIN_COMMAND[1]}}
        // starting an interactive calculator session
        .add_option(command_option{co_sub_command, string{CALC_SUBCMDS[CALC_SUBCMD_START][0]},
                                   string{CALC_SUBCMDS[CALC_SUBCMD_START][1]}})
        // subcommand for reporting percent progress
        .add_option(command_option{co_sub_command, string{CALC_SUBCMDS[CALC_SUBCMD_PERCENT][0]},
                                   string{CALC_SUBCMDS[CALC_SUBCMD_PERCENT][1]}}
                        // Parameters
                        // 1. percentage value
                        .add_option(command_option{
                            co_number, string{CALC_SUBCMD_PARAM[CALC_SUBCMD_PERCENT][0][0]},
                            string{CALC_SUBCMD_PARAM[CALC_SUBCMD_PERCENT][0][1]}, true}
                                        .set_min_value(0.0)))
        // subcommand for reporting cosmosapsis
        .add_option(command_option{co_sub_command, string{CALC_SUBCMDS[CALC_SUBCMD_COSMOSAPSIS][0]},
                                   string{CALC_SUBCMDS[CALC_SUBCMD_COSMOSAPSIS][1]}}
                        // Parameters
                        // 1. cosmosapsis value
                        .add_option(command_option{
                            co_number, string{CALC_SUBCMD_PARAM[CALC_SUBCMD_COSMOSAPSIS][0][0]},
                            string{CALC_SUBCMD_PARAM[CALC_SUBCMD_COSMOSAPSIS][0][1]}, true}
                                        .set_min_value(0.0)))
        // subcommand for reporting respira
        .add_option(command_option{co_sub_command, string{CALC_SUBCMDS[CALC_SUBCMD_RESPIRA][0]},
                                   string{CALC_SUBCMDS[CALC_SUBCMD_RESPIRA][1]}}
                        // Parameters
                        // 1. respira exp per attempt
                        .add_option(command_option{
                            co_integer, string{CALC_SUBCMD_PARAM[CALC_SUBCMD_RESPIRA][0][0]},
                            string{CALC_SUBCMD_PARAM[CALC_SUBCMD_RESPIRA][0][1]}, true}
                                        .set_min_value(1))
                        // 2. number of daily respira attempts
                        .add_option(command_option{
                            co_integer, string{CALC_SUBCMD_PARAM[CALC_SUBCMD_RESPIRA][1][0]},
                            string{CALC_SUBCMD_PARAM[CALC_SUBCMD_RESPIRA][1][1]}, true}
                                        .set_min_value(1)))
        // subcommand for reporting pill data
        .add_option(
            command_option{co_sub_command, string{CALC_SUBCMDS[CALC_SUBCMD_PILL][0]},
                           string{CALC_SUBCMDS[CALC_SUBCMD_PILL][1]}}
                // Parameters
                // 1. number of daily pill attempts
                .add_option(command_option{co_integer,
                                           string{CALC_SUBCMD_PARAM[CALC_SUBCMD_PILL][0][0]},
                                           string{CALC_SUBCMD_PARAM[CALC_SUBCMD_PILL][0][1]}, true}
                                .set_min_value(1)
                                .set_max_value(30))
                // 2. number of rare pills consumed per day
                .add_option(command_option{co_integer,
                                           string{CALC_SUBCMD_PARAM[CALC_SUBCMD_PILL][1][0]},
                                           string{CALC_SUBCMD_PARAM[CALC_SUBCMD_PILL][1][1]}, true}
                                .set_min_value(0)
                                .set_max_value(30))
                // 3. number of epic pills consumed per day
                .add_option(command_option{co_integer,
                                           string{CALC_SUBCMD_PARAM[CALC_SUBCMD_PILL][2][0]},
                                           string{CALC_SUBCMD_PARAM[CALC_SUBCMD_PILL][2][1]}, true}
                                .set_min_value(0)
                                .set_max_value(30))
                // 4. number of legendary pills consumed per day
                .add_option(command_option{co_integer,
                                           string{CALC_SUBCMD_PARAM[CALC_SUBCMD_PILL][3][0]},
                                           string{CALC_SUBCMD_PARAM[CALC_SUBCMD_PILL][3][1]}, true}
                                .set_min_value(0)
                                .set_max_value(30))
                // 5. pill experience bonus in percent
                .add_option(command_option{co_number,
                                           string{CALC_SUBCMD_PARAM[CALC_SUBCMD_PILL][4][0]},
                                           string{CALC_SUBCMD_PARAM[CALC_SUBCMD_PILL][4][1]}, true}
                                .set_min_value(0)));
}

task<void> start_interactive_calculator(const slashcommand_t &event) {
    static component calc_overview_text{
        component()
            .set_type(cot_text_display)
            .set_content("This is a cultivation calculator that produces an estimate "
                         "of "
                         "the time it takes to breakthrough to the next major "
                         "stage.\n\n"
                         "I will walk you through a step-by-step process to collect "
                         "the "
                         "necessary in-game data to perform the calculation. Please "
                         "grab "
                         "your mobile device and launch the game. \n\n"
                         "Click **NEXT** if you are ready proceed! "
                         "Otherwise, click **CANCEL** to end this session.")};

    static component next_button{component()
                                     .set_type(cot_button)
                                     .set_style(cos_primary)
                                     .set_label("NEXT")
                                     .set_id(CALC_BUTTON_IDS[CALC_BUTTON_START])};

    static component action_row{component()
                                    .set_type(cot_action_row)
                                    .add_component_v2(next_button)
                                    .add_component_v2(cancel_button)};

    static message calc_msg{message()
                                .set_flags(m_using_components_v2)
                                .add_component_v2(component()
                                                      .set_type(cot_container)
                                                      .add_component_v2(calc_overview_text)
                                                      .add_component_v2(action_row))};

    snowflake user_id{event.command.usr.id};

    if (calc_sessions.find(user_id) != calc_sessions.end()) {
        if (DEBUG) cerr << "User ID: " << user_id << " already has an active session." << endl;
        co_await event.co_reply(
            message("You already have an active calculator session. Please finish it first.")
                .set_flags(m_ephemeral));
        co_return;
    }

    co_await event.co_reply(calc_msg);

    if (DEBUG) cerr << "User ID: " << user_id << endl;

    confirmation_callback_t callback{co_await event.co_get_original_response()};
    if (callback.is_error()) {
        cerr << "Error: " << callback.get_error().message << endl;
        co_return;
    }
    const message &to_cache_msg{callback.get<message>()};

    if (DEBUG) cerr << "Caching session..." << endl;
    calc_sessions.insert({user_id, make_pair(to_cache_msg, calculator_client_t{})});
    co_return;
}

task<void> calculator_subcommand_handler(const slashcommand_t &event,
                                         const command_data_option &options) {
    const command_data_option &subcommand{options.options[0]};

    if (DEBUG) cerr << "Handling calculator subcommand: " << subcommand.name << endl;

    if (subcommand.name == CALC_SUBCMDS[CALC_SUBCMD_START][0]) {
        co_await start_interactive_calculator(event);
        co_return;
    }

    if (subcommand.name == CALC_SUBCMDS[CALC_SUBCMD_PERCENT][0])
        co_await process_percent_progress(event);
    else if (subcommand.name == CALC_SUBCMDS[CALC_SUBCMD_COSMOSAPSIS][0])
        co_await process_cosmosapsis(event);
    else if (subcommand.name == CALC_SUBCMDS[CALC_SUBCMD_RESPIRA][0])
        co_await process_respira(event);
    else if (subcommand.name == CALC_SUBCMDS[CALC_SUBCMD_PILL][0])
        co_await process_pill(event);
    else {
        if (DEBUG) cerr << "Unhandled calculator subcommand: " << subcommand.name << endl;

        co_await event.co_reply(
            message("This command is not implemented yet.").set_flags(m_ephemeral));
    }

    co_return;
}

task<void> calculator_button_click_handler(const button_click_t &event) {
    if (!co_await verify_user(event)) co_return;

    const string &id{event.custom_id};

    if (DEBUG) cerr << "Handling calculator event: " << id << endl;

    if (id == CALC_BUTTON_IDS[CALC_BUTTON_START]) {
        co_await calc_ask_stage(event);
        co_return;
    }

    if (id == CALC_BUTTON_IDS[CALC_BUTTON_CANCEL]) {
        co_await calc_cancel(event);
        co_return;
    }

    const calculator_client_t &client{calc_sessions[event.command.usr.id].second};
    if (id == CALC_BUTTON_IDS[CALC_BUTTON_STAGE]) {
        if (client.major_stage == INVALID_MAJOR_STAGE || client.minor_stage == INVALID_MINOR_STAGE)
            co_return;  // do nothing since user has not finished selecting both stages
        else {
            // both stages are set, then...
            if (client.percent_progress == INVALID_DOUBLE_VAL)
                // ask for percent progress if not set
                co_await calc_ask_percent_progress(event);
            else if (client.cosmosapsis == INVALID_DOUBLE_VAL)
                // ask for cosmosapsis if not set
                co_await calc_ask_cosmosapsis(event);
            else
                // ask for aura gem quality if both percent progress and cosmosapsis are set
                co_await calc_ask_aura_gem(event);
        }
    } else if (id == CALC_BUTTON_IDS[CALC_BUTTON_PERCENT]) {
        if (client.cosmosapsis == INVALID_DOUBLE_VAL)
            // ask for cosmosapsis if not set
            co_await calc_ask_cosmosapsis(event);
        else
            // ask for aura gem quality if both percent progress and cosmosapsis are set
            co_await calc_ask_aura_gem(event);

    } else if (id == CALC_BUTTON_IDS[CALC_BUTTON_COSMOSAPSIS])
        co_await calc_ask_aura_gem(event);
    else if (id == CALC_BUTTON_IDS[CALC_BUTTON_AURA_GEM]) {
        if (client.aura_gem_quality == INVALID_QUALITY)
            co_return;  // do nothing since user has not selected aura gem quality
        else if (client.respira_exp == INVALID_UNSIGNED_VAL ||
                 client.daily_respira_attempts == INVALID_UNSIGNED_VAL)
            // ask for respira if not set
            co_await calc_ask_respira(event);
        else if (client.daily_pill_attempts == INVALID_UNSIGNED_VAL ||
                 (client.pill_quantity[0] == INVALID_UNSIGNED_VAL &&
                  client.pill_quantity[1] == INVALID_UNSIGNED_VAL &&
                  client.pill_quantity[2] == INVALID_UNSIGNED_VAL) ||
                 client.pill_bonus == INVALID_DOUBLE_VAL)
            // ask for pill if not set
            co_await calc_ask_pill(event);
        else
            // ask for extractor quality if all previous data are set
            co_await calc_ask_extractor_quality(event);
    } else if (id == CALC_BUTTON_IDS[CALC_BUTTON_RESPIRA]) {
        if (client.daily_pill_attempts == INVALID_UNSIGNED_VAL ||
            (client.pill_quantity[0] == INVALID_UNSIGNED_VAL &&
             client.pill_quantity[1] == INVALID_UNSIGNED_VAL &&
             client.pill_quantity[2] == INVALID_UNSIGNED_VAL) ||
            client.pill_bonus == INVALID_DOUBLE_VAL)
            // ask for pill if not set
            co_await calc_ask_pill(event);
        else
            // ask for extractor quality if all previous data are set
            co_await calc_ask_extractor_quality(event);
    } else if (id == CALC_BUTTON_IDS[CALC_BUTTON_EXTRACTOR_QUALITY]) {
        if (client.extractor_quality == INVALID_QUALITY)
            co_return;  // do nothing since user has not selected extractor quality
        else if (client.node_levels[0] == INVALID_EXTRACTOR_NODE_LVL ||
                 client.node_levels[1] == INVALID_EXTRACTOR_NODE_LVL ||
                 client.node_levels[2] == INVALID_EXTRACTOR_NODE_LVL)
            // ask for extractor node levels if not set
            co_await calc_ask_extractor_node_lvl(event);
        else if (client.fruit_quantity == INVALID_UNSIGNED_VAL)
            // ask for myrimon fruit quantity if not set
            co_await calc_ask_myrimon_fruit(event);
        else
            // ask for vase ownership if all previous data are set
            co_await calc_ask_vase_own(event);
    } else
        co_await calc_under_construction(event);

    co_return;
}

task<void> calculator_select_click_handler(const select_click_t &event) {
    optional<pair<message, calculator_client_t> *> user{co_await verify_user(event)};
    if (!user) co_return;

    calculator_client_t &client{user.value()->second};

    if (event.custom_id == CALC_SELECT_IDS[CALC_SELECT_MAJOR_STAGE]) {
        if (DEBUG) cerr << "Major stage selected: " << event.values[0] << endl;

        client.major_stage = get_major_stage(event.values[0]);
    } else if (event.custom_id == CALC_SELECT_IDS[CALC_SELECT_MINOR_STAGE]) {
        if (DEBUG) cerr << "Minor stage selected: " << event.values[0] << endl;

        client.minor_stage = get_minor_stage(event.values[0]);
    } else if (event.custom_id == CALC_SELECT_IDS[CALC_SELECT_AURA_GEM_QUALITY]) {
        if (DEBUG) cerr << "Aura gem quality selected: " << event.values[0] << endl;

        client.aura_gem_quality = get_quality(event.values[0]);
    } else
        cerr << "Unhandled select event: " << event.custom_id << endl;

    co_return;
}

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

task<void> calc_cancel(const button_click_t &event) {
    if (DEBUG) cerr << "Canceling calculator session..." << endl;

    static message session_cancel_message{
        message()
            .set_flags(m_using_components_v2)
            .add_component_v2(
                component()
                    // a container
                    .set_type(cot_container)
                    // ...with a text display
                    .add_component_v2(component()
                                          .set_type(cot_text_display)
                                          .set_content("You have cancelled the session.")))};

    co_await event.co_edit_response(session_cancel_message);

    calc_sessions.erase(event.command.usr.id);
    co_return;
}

component major_stage_selectmenu_factory() {
    component major_stage_selectmenu{component()
                                         .set_type(cot_selectmenu)
                                         .set_id(CALC_SELECT_IDS[CALC_SELECT_MAJOR_STAGE])
                                         .set_placeholder("Select your major stage")
                                         .set_required(true)};

    for (size_t i{0}; i < NUM_MAJOR_STAGES; ++i)
        major_stage_selectmenu.add_select_option(
            select_option(MAJOR_STAGE_STR[i], MAJOR_STAGE_STR[i], ""));

    return major_stage_selectmenu;
}

component minor_stage_selectmenu_factory() {
    component minor_stage_selectmenu{component()
                                         .set_type(cot_selectmenu)
                                         .set_id(CALC_SELECT_IDS[CALC_SELECT_MINOR_STAGE])
                                         .set_placeholder("Select your minor stage")
                                         .set_required(true)};

    for (size_t i{0}; i < NUM_MINOR_STAGES; ++i)
        minor_stage_selectmenu.add_select_option(
            select_option(MINOR_STAGE_STR[i], MINOR_STAGE_STR[i], ""));

    return minor_stage_selectmenu;
}

task<void> calc_ask_stage(const button_click_t &event) {
    calc_sessions[event.command.usr.id].second.calc_state = CALC_ASK_STAGE;

    if (DEBUG) cerr << "Asking for cultivation stage..." << endl;

    static component text_display{component()
                                      .set_type(cot_text_display)
                                      .set_content("Please select your major and minor stage")};

    static component major_stage_selectmenu{
        component().set_type(cot_action_row).add_component_v2(major_stage_selectmenu_factory())};

    static component minor_stage_selectmenu{
        component().set_type(cot_action_row).add_component_v2(minor_stage_selectmenu_factory())};

    static component next_button{component()
                                     .set_type(cot_button)
                                     .set_style(cos_primary)
                                     .set_label("NEXT")
                                     .set_id(CALC_BUTTON_IDS[CALC_BUTTON_STAGE])};

    static message calc_ask_stage_message{
        message()
            .set_flags(m_using_components_v2)
            .add_component_v2(component()
                                  // a container
                                  .set_type(cot_container)
                                  // ...with a text display
                                  .add_component_v2(text_display)
                                  // ...and two select menus
                                  .add_component_v2(major_stage_selectmenu)
                                  .add_component_v2(minor_stage_selectmenu)
                                  // ...and an action row with two buttons
                                  .add_component_v2(component()
                                                        .set_type(cot_action_row)
                                                        .add_component_v2(next_button)
                                                        .add_component_v2(cancel_button)))};

    if (DEBUG) cerr << "Sending cultivation stage selection message..." << endl;

    confirmation_callback_t confirmation{co_await event.co_edit_response(calc_ask_stage_message)};

    if (confirmation.is_error()) {
        cerr << "Error: " << confirmation.get_error().message << endl;
        co_return;
    }

    co_return;
}

string print_client_info(const calculator_client_t &client) {
    string info{"You have entered:\n"};
    if (client.major_stage != INVALID_MAJOR_STAGE)
        info += "- Major Stage: " + string{MAJOR_STAGE_STR[client.major_stage]} + "\n";
    if (client.minor_stage != INVALID_MINOR_STAGE)
        info += "- Minor Stage: " + string{MINOR_STAGE_STR[client.minor_stage]} + "\n";

    // TODO: handle gate

    if (client.percent_progress != INVALID_DOUBLE_VAL)
        info += "- Percent Progress: " + to_string(client.percent_progress) + "%\n";
    if (client.cosmosapsis != INVALID_DOUBLE_VAL)
        info += "- Cosmosapsis: " + to_string(client.cosmosapsis) + "\n";

    if (client.aura_gem_quality != INVALID_QUALITY)
        info += "- Aura Gem Quality: " + string{QUALITY_STR[client.aura_gem_quality]} + "\n";

    if (client.respira_exp != INVALID_UNSIGNED_VAL)
        info += "- Respira Exp: " + to_string(client.respira_exp) + "\n";

    if (client.daily_respira_attempts != INVALID_UNSIGNED_VAL)
        info += "- Daily Respira Attempts: " + to_string(client.daily_respira_attempts) + "\n";

    if (client.daily_pill_attempts != INVALID_UNSIGNED_VAL)
        info += "- Daily Pill Attempts: " + to_string(client.daily_pill_attempts) + "\n";

    if (!(client.pill_quantity[0] == INVALID_UNSIGNED_VAL &&
          client.pill_quantity[1] == INVALID_UNSIGNED_VAL &&
          client.pill_quantity[2] == INVALID_UNSIGNED_VAL))
        info += "- Pill Quantity: " + to_string(client.pill_quantity[0]) + " (Rare), " +
                to_string(client.pill_quantity[1]) + " (Epic), " +
                to_string(client.pill_quantity[2]) + " (Legendary)\n";

    if (client.pill_bonus != INVALID_DOUBLE_VAL)
        info += "- Pill Bonus: " + to_string(client.pill_bonus) + "%\n";

    if (client.extractor_quality != INVALID_QUALITY)
        info += "- Extractor Quality: " + string{QUALITY_STR[client.extractor_quality]} + "\n";

    if (client.node_levels[0] != INVALID_EXTRACTOR_NODE_LVL &&
        client.node_levels[1] != INVALID_EXTRACTOR_NODE_LVL &&
        client.node_levels[2] != INVALID_EXTRACTOR_NODE_LVL)
        info += "- Node Levels: " + to_string(client.node_levels[0]) + " (Cultivation XP), " +
                to_string(client.node_levels[1]) + " (Quality), " +
                to_string(client.node_levels[2]) + " (Gush)\n";

    if (client.fruit_quantity != INVALID_UNSIGNED_VAL)
        info += "- Myrimon Fruit Quantity: " + to_string(client.fruit_quantity) + "\n";

    // TODO: handle artifacts

    return info + "\n\n";
}

task<void> non_session_interaction(const slashcommand_t &event) {
    co_await event.co_reply(message("This command should be used in an interactive calculator "
                                    "session."
                                    "Please start a new session with `/" +
                                    string{CALC_MAIN_COMMAND[0]} + " " +
                                    string{CALC_SUBCMDS[CALC_SUBCMD_START][0]} + "`")
                                .set_flags(m_ephemeral));
}

task<optional<pair<message, calculator_client_t> *>> verify_user(const slashcommand_t &event) {
    snowflake user_id{event.command.usr.id};
    auto it{calc_sessions.find(user_id)};

    if (it == calc_sessions.end()) {
        if (DEBUG) cerr << "User ID: " << user_id << " is not in any session." << endl;
        co_await non_session_interaction(event);
        co_return nullopt;
    }

    co_return &it->second;
}

task<void> calc_ask_percent_progress(const button_click_t &event) {
    calc_sessions[event.command.usr.id].second.calc_state = CALC_ASK_PERCENT_PROGRESS;

    if (DEBUG) cerr << "Asking for percent progress..." << endl;

    string client_info{print_client_info(calc_sessions[event.command.usr.id].second)};

    component text_display{component()
                               .set_type(cot_text_display)
                               .set_content(client_info +
                                            "Please input your percent progress (top left corner "
                                            "of the screen) via `/calc percent` command.\n\n"
                                            "We will continue once your input is received.")};

    co_await event.co_edit_response(
        message()
            .set_flags(m_using_components_v2)
            .add_component_v2(
                component()
                    // a container
                    .set_type(cot_container)
                    // ...with a text display
                    .add_component_v2(text_display)  // ...and a button
                    .add_component_v2(
                        component().set_type(cot_action_row).add_component_v2(cancel_button))));

    co_return;
}

task<void> process_percent_progress(const slashcommand_t &event) {
    optional<pair<message, calculator_client_t> *> user{co_await verify_user(event)};

    if (!user) co_return;

    // acknowledge the slash command
    auto reply{event.co_reply("input received, processing...")};

    calculator_client_t &client{user.value()->second};
    client.percent_progress =
        get<double>(event.get_parameter(string{CALC_SUBCMD_PARAM[CALC_SUBCMD_PERCENT][0][0]}));

    if (client.calc_state == CALC_ASK_PERCENT_PROGRESS) {
        // if the user is in the percent progress state, then we can edit the message
        string client_info{print_client_info(client)};

        component text_display{component()
                                   .set_type(cot_text_display)
                                   .set_content(client_info +
                                                "Please click **NEXT** to continue, or use `/calc "
                                                "percent` to update your progress.\n\n")};

        static component next_button{component()
                                         .set_type(cot_button)
                                         .set_style(cos_primary)
                                         .set_label("NEXT")
                                         .set_id(CALC_BUTTON_IDS[CALC_BUTTON_PERCENT])};

        message msg{user.value()->first};

        // edit the message
        msg.components.clear();
        msg.add_component_v2(component()
                                 // a container
                                 .set_type(cot_container)
                                 // ...with a text display
                                 .add_component_v2(text_display)
                                 // ...and two buttons
                                 .add_component_v2(component()
                                                       .set_type(cot_action_row)
                                                       .add_component_v2(next_button)
                                                       .add_component_v2(cancel_button)));

        co_await bot.co_message_edit(msg);
    }

    // otherwise proceed silently
    co_await reply;
    event.co_delete_original_response();

    co_return;
}

task<void> calc_ask_cosmosapsis(const button_click_t &event) {
    calc_sessions[event.command.usr.id].second.calc_state = CALC_ASK_COSMOSAPSIS;

    if (DEBUG) cerr << "Asking for cosmosapsis..." << endl;

    string client_info{print_client_info(calc_sessions[event.command.usr.id].second)};

    component text_display{component()
                               .set_type(cot_text_display)
                               .set_content(client_info +
                                            "Please input your cosmosapsis via `/calc "
                                            "cosmosapsis` command.\n\n"
                                            "We will continue once your input is received.")};

    co_await event.co_edit_response(
        message()
            .set_flags(m_using_components_v2)
            .add_component_v2(
                component()
                    // a container
                    .set_type(cot_container)
                    // ...with a text display
                    .add_component_v2(text_display)
                    // ...and a button
                    .add_component_v2(
                        component().set_type(cot_action_row).add_component_v2(cancel_button))));

    co_return;
}

task<void> process_cosmosapsis(const slashcommand_t &event) {
    optional<pair<message, calculator_client_t> *> user{co_await verify_user(event)};

    if (!user) co_return;

    // acknowledge the slash command
    auto reply{event.co_reply("input received, processing...")};

    calculator_client_t &client{user.value()->second};
    client.cosmosapsis =
        get<double>(event.get_parameter(string{CALC_SUBCMD_PARAM[CALC_SUBCMD_COSMOSAPSIS][0][0]}));

    if (client.calc_state == CALC_ASK_COSMOSAPSIS) {
        // if the user is in the cosmosapsis state, then we can edit the message
        string client_info{print_client_info(calc_sessions[event.command.usr.id].second)};

        component text_display{component()
                                   .set_type(cot_text_display)
                                   .set_content(client_info +
                                                "Please click **NEXT** to continue, or use `/calc "
                                                "cosmosapsis` to update your progress.\n\n")};

        static component next_button{component()
                                         .set_type(cot_button)
                                         .set_style(cos_primary)
                                         .set_label("NEXT")
                                         .set_id(CALC_BUTTON_IDS[CALC_BUTTON_COSMOSAPSIS])};

        message msg{user.value()->first};

        // edit the message
        msg.components.clear();
        msg.add_component_v2(component()
                                 // a container
                                 .set_type(cot_container)
                                 // ...with a text display
                                 .add_component_v2(text_display)
                                 // ...and two buttons
                                 .add_component_v2(component()
                                                       .set_type(cot_action_row)
                                                       .add_component_v2(next_button)
                                                       .add_component_v2(cancel_button)));

        co_await bot.co_message_edit(msg);
    }

    // otherwise proceed silently
    co_await reply;
    event.co_delete_original_response();

    co_return;
}

component aura_gem_quality_selectmenu_factory() {
    component aura_gem_quality_selectmenu{component()
                                              .set_type(cot_selectmenu)
                                              .set_id(CALC_SELECT_IDS[CALC_SELECT_AURA_GEM_QUALITY])
                                              .set_placeholder("Select quality of your aura gem")
                                              .set_required(true)

    };
    for (size_t i{0}; i < NUM_QUALITIES; ++i)
        aura_gem_quality_selectmenu.add_select_option(
            select_option{QUALITY_STR[i], QUALITY_STR[i], ""});

    return aura_gem_quality_selectmenu;
}

task<void> calc_ask_aura_gem(const button_click_t &event) {
    if (DEBUG) cerr << "Asking for aura gem quality" << endl;

    string client_info{print_client_info(calc_sessions[event.command.usr.id].second)};

    static component aura_gem_quality_selectmenu{
        component()
            .set_type(cot_action_row)
            .add_component_v2(aura_gem_quality_selectmenu_factory())};

    static component next_button{component()
                                     .set_type(cot_button)
                                     .set_style(cos_primary)
                                     .set_label("NEXT")
                                     .set_id(CALC_BUTTON_IDS[CALC_BUTTON_AURA_GEM])};

    component text_display{
        component()
            .set_type(cot_text_display)
            .set_content(client_info + "Please select the quality of your aura gem.")};

    if (DEBUG) cerr << "Sending aura gem selection message..." << endl;

    co_await event.co_edit_response(
        message()
            .set_flags(m_using_components_v2)
            // a container
            .add_component_v2(component()
                                  .set_type(cot_container)
                                  // ...with a text display
                                  .add_component_v2(text_display)
                                  // ...and a select menu for aura gem quality
                                  .add_component_v2(aura_gem_quality_selectmenu)
                                  // ...and an action row with two buttons
                                  .add_component_v2(component()
                                                        .set_type(cot_action_row)
                                                        .add_component_v2(next_button)
                                                        .add_component_v2(cancel_button))));

    co_return;
}

task<void> calc_ask_respira(const button_click_t &event) {
    calc_sessions[event.command.usr.id].second.calc_state = CALC_ASK_RESPIRA;

    if (DEBUG) cerr << "Asking for respira" << endl;

    string client_info{print_client_info(calc_sessions[event.command.usr.id].second)};

    component text_display{
        component()
            .set_type(cot_text_display)
            .set_content(client_info + "Please input the amount of exp per respira attempt and "
                                       "the number of daily respira attempts via `/calc respira` "
                                       "command.\n\n"
                                       "We will continue once your input is received.")};

    co_await event.co_edit_response(
        message()
            .set_flags(m_using_components_v2)
            .add_component_v2(
                component()
                    // a container
                    .set_type(cot_container)
                    // ...with a text display
                    .add_component_v2(text_display)
                    // ...and a button
                    .add_component_v2(
                        component().set_type(cot_action_row).add_component_v2(cancel_button))));

    co_return;
}

task<void> process_respira(const slashcommand_t &event) {
    optional<pair<message, calculator_client_t> *> user{co_await verify_user(event)};

    if (!user) co_return;

    // acknowledge the slash command
    auto reply{event.co_reply("input received, processing...")};

    calculator_client_t &client{user.value()->second};
    client.respira_exp = static_cast<exp_t>(
        get<int64_t>(event.get_parameter(string{CALC_SUBCMD_PARAM[CALC_SUBCMD_RESPIRA][0][0]})));
    client.daily_respira_attempts = static_cast<unsigned>(
        get<int64_t>(event.get_parameter(string{CALC_SUBCMD_PARAM[CALC_SUBCMD_RESPIRA][1][0]})));

    if (client.calc_state == CALC_ASK_RESPIRA) {
        // if the user is in the respira state, then we can edit the message
        string client_info{print_client_info(calc_sessions[event.command.usr.id].second)};

        component text_display{component()
                                   .set_type(cot_text_display)
                                   .set_content(client_info +
                                                "Please click **NEXT** to continue, or use `/calc "
                                                "respira` to update your progress.\n\n")};

        static component next_button{component()
                                         .set_type(cot_button)
                                         .set_style(cos_primary)
                                         .set_label("NEXT")
                                         .set_id(CALC_BUTTON_IDS[CALC_BUTTON_RESPIRA])};

        message msg{user.value()->first};

        // edit the message
        msg.components.clear();
        msg.add_component_v2(component()
                                 // a container
                                 .set_type(cot_container)
                                 // ...with a text display
                                 .add_component_v2(text_display)
                                 // ...and two buttons
                                 .add_component_v2(component()
                                                       .set_type(cot_action_row)
                                                       .add_component_v2(next_button)
                                                       .add_component_v2(cancel_button)));

        co_await bot.co_message_edit(msg);
    }

    // otherwise proceed silently
    co_await reply;
    event.co_delete_original_response();

    co_return;
}

task<void> calc_ask_pill(const button_click_t &event) {
    calc_sessions[event.command.usr.id].second.calc_state = CALC_ASK_PILL;

    if (DEBUG) cerr << "Asking for pill data..." << endl;

    string client_info{print_client_info(calc_sessions[event.command.usr.id].second)};

    component text_display{
        component()
            .set_type(cot_text_display)
            .set_content(client_info + "Please input the following data: \n"
                                       "- Number of daily pill attempts \n"
                                       "- Average number of rare pills consumed everyday \n"
                                       "- Average number of epic pills consumed everyday \n"
                                       "- Average number of legendary pills consumed everyday \n"
                                       "- Pill exp bonus in percent \n"
                                       "via `/calc pill` command.\n\n"
                                       "Note that the number of rare, epic and legendary pills "
                                       "must add up to the number of daily pill attempts.\n\n"
                                       "We will continue once your input is received.")};

    co_await event.co_edit_response(
        message()
            .set_flags(m_using_components_v2)
            .add_component_v2(
                component()
                    // a container
                    .set_type(cot_container)
                    // ...with a text display
                    .add_component_v2(text_display)
                    // ...and a button
                    .add_component_v2(
                        component().set_type(cot_action_row).add_component_v2(cancel_button))));

    co_return;
}

task<void> process_pill(const slashcommand_t &event) {
    optional<pair<message, calculator_client_t> *> user{co_await verify_user(event)};

    if (!user) co_return;

    // acknowledge the slash command
    co_await event.co_reply("input received, processing...");

    calculator_client_t &client{user.value()->second};

    unsigned daily_pill_attempts{static_cast<unsigned>(
        get<int64_t>(event.get_parameter(string{CALC_SUBCMD_PARAM[CALC_SUBCMD_PILL][0][0]})))};
    unsigned num_rare{static_cast<unsigned>(
        get<int64_t>(event.get_parameter(string{CALC_SUBCMD_PARAM[CALC_SUBCMD_PILL][1][0]})))};
    unsigned num_epic{static_cast<unsigned>(
        get<int64_t>(event.get_parameter(string{CALC_SUBCMD_PARAM[CALC_SUBCMD_PILL][2][0]})))};
    unsigned num_legendary{static_cast<unsigned>(
        get<int64_t>(event.get_parameter(string{CALC_SUBCMD_PARAM[CALC_SUBCMD_PILL][3][0]})))};

    if (num_rare + num_epic + num_legendary != daily_pill_attempts) {
        confirmation_callback_t confirmation{co_await event.co_get_original_response()};
        if (confirmation.is_error()) {
            cerr << "Error: " << confirmation.get_error().message << endl;
            co_return;
        }
        message msg{confirmation.get<message>()};
        msg.set_content(
            "The number of rare, epic and legendary pills must add up to the number of daily "
            "pill attempts. Please try again.");
        co_await bot.co_message_edit(msg);
        co_return;
    }

    // else if the number of rare, epic and legendary pills add up to the number of daily pill]
    client.daily_pill_attempts = daily_pill_attempts;
    client.pill_quantity[0] = num_rare;
    client.pill_quantity[1] = num_epic;
    client.pill_quantity[2] = num_legendary;
    client.pill_bonus =
        get<double>(event.get_parameter(string{CALC_SUBCMD_PARAM[CALC_SUBCMD_PILL][4][0]}));

    if (client.calc_state == CALC_ASK_PILL) {
        // if the user is in the pill state, then we can edit the message
        string client_info{print_client_info(calc_sessions[event.command.usr.id].second)};

        component text_display{component()
                                   .set_type(cot_text_display)
                                   .set_content(client_info +
                                                "Please click **NEXT** to continue, or use `/calc "
                                                "pill` to update your progress.\n\n")};

        static component next_button{component()
                                         .set_type(cot_button)
                                         .set_style(cos_primary)
                                         .set_label("NEXT")
                                         .set_id(CALC_BUTTON_IDS[CALC_BUTTON_PILL])};

        message msg{user.value()->first};

        // edit the message
        msg.components.clear();
        msg.add_component_v2(component()
                                 // a container
                                 .set_type(cot_container)
                                 // ...with a text display
                                 .add_component_v2(text_display)
                                 // ...and two buttons
                                 .add_component_v2(component()
                                                       .set_type(cot_action_row)
                                                       .add_component_v2(next_button)
                                                       .add_component_v2(cancel_button)));

        co_await bot.co_message_edit(msg);
    }

    // otherwise proceed silently
    event.co_delete_original_response();

    co_return;
}

task<void> calc_ask_extractor_quality(const button_click_t &event) {
    co_await calc_under_construction(event);
    co_return;
}

task<void> calc_ask_extractor_node_lvl(const button_click_t &event) {
    co_await calc_under_construction(event);
    co_return;
}

task<void> process_extractor_node_lvl(const slashcommand_t &event) {
    co_await event.co_reply(message("This command is not implemented yet.").set_flags(m_ephemeral));
    co_return;
}

task<void> calc_ask_myrimon_fruit(const button_click_t &event) {
    co_await calc_under_construction(event);
    co_return;
}

task<void> process_myrimon_fruit(const slashcommand_t &event) {
    co_await event.co_reply(message("This command is not implemented yet.").set_flags(m_ephemeral));
    co_return;
}

task<void> calc_ask_vase_own(const button_click_t &event) {
    co_await calc_under_construction(event);
    co_return;
}

task<void> calc_ask_vase_detail(const button_click_t &event) {
    co_await calc_under_construction(event);
    co_return;
}

task<void> calc_ask_mirror_own(const button_click_t &event) {
    co_await calc_under_construction(event);
    co_return;
}

task<void> calc_ask_mirror_detail(const button_click_t &event) {
    co_await calc_under_construction(event);
    co_return;
}

task<void> calc_under_construction(const button_click_t &event) {
    if (DEBUG) cerr << "Informing user that this feature is under construction..." << endl;

    static message under_construction_message{
        message()
            .set_flags(m_using_components_v2)
            .add_component_v2(
                component()
                    // a container
                    .set_type(cot_container)
                    // ...with a text display
                    .add_component_v2(
                        component()
                            .set_type(cot_text_display)
                            .set_content("This part of the calculator is "
                                         "under construction. Please check back later.")))};

    co_await event.co_edit_response(under_construction_message);

    calc_sessions.erase(event.command.usr.id);

    co_return;
}