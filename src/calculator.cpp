#include "calculator.hpp"
#include <dpp/appcommand.h>

#include <string>

static const component cancel_button{component()
                                         .set_type(cot_button)
                                         .set_style(cos_danger)
                                         .set_label("CANCEL")
                                         .set_id(CALC_EVENT_IDS[CALC_CANCEL])};

command_option calculator_commands() {
    return command_option{co_sub_command_group, string{CALC_MAIN_COMMAND[0]},
                          string{CALC_MAIN_COMMAND[1]}}
        .add_option(command_option{co_sub_command, string{CALC_SUBCMDS[CALC_SUBCMD_START][0]},
                                   string{CALC_SUBCMDS[CALC_SUBCMD_START][1]}})
        .add_option(command_option{co_sub_command, string{CALC_SUBCMDS[CALC_SUBCMD_PERCENT][0]},
                                   string{CALC_SUBCMDS[CALC_SUBCMD_PERCENT][1]}}
                        .add_option(command_option{
                            co_number, string{CALC_SUBCMD_PARAM[CALC_SUBCMD_PERCENT][0][0]},
                            string{CALC_SUBCMD_PARAM[CALC_SUBCMD_PERCENT][0][1]}, true}
                                        .set_min_value(0.0)
                                        .set_max_value(1000.0)))
        .add_option(command_option{co_sub_command, string{CALC_SUBCMDS[CALC_SUBCMD_COSMOSAPSIS][0]},
                                   string{CALC_SUBCMDS[CALC_SUBCMD_COSMOSAPSIS][1]}}
                        .add_option(command_option{
                            co_number, string{CALC_SUBCMD_PARAM[CALC_SUBCMD_COSMOSAPSIS][0][0]},
                            string{CALC_SUBCMD_PARAM[CALC_SUBCMD_COSMOSAPSIS][0][1]}, true}
                                        .set_min_value(0.0)))
        .add_option(command_option{co_sub_command, string{CALC_SUBCMDS[CALC_SUBCMD_RESPIRA][0]},
                                   string{CALC_SUBCMDS[CALC_SUBCMD_RESPIRA][1]}}
                        .add_option(command_option{
                            co_integer, string{CALC_SUBCMD_PARAM[CALC_SUBCMD_RESPIRA][0][0]},
                            string{CALC_SUBCMD_PARAM[CALC_SUBCMD_RESPIRA][0][1]}, true}
                                        .set_min_value(0))
                        .add_option(command_option{
                            co_integer, string{CALC_SUBCMD_PARAM[CALC_SUBCMD_RESPIRA][1][0]},
                            string{CALC_SUBCMD_PARAM[CALC_SUBCMD_RESPIRA][1][1]}, true}
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
                                     .set_id(CALC_EVENT_IDS[CALC_ASK_STAGE])};

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

    co_await event.co_reply(calc_msg);

    if (DEBUG) cerr << "Caching session..." << endl;

    snowflake user_id{event.command.usr.id};

    if (calc_sessions.find(user_id) != calc_sessions.end()) {
        if (DEBUG) cerr << "User ID: " << user_id << " already has an active session." << endl;
        co_await event.co_reply(
            message("You already have an active calculator session. Please finish it first.")
                .set_flags(m_ephemeral));
        co_return;
    }

    if (DEBUG) cerr << "User ID: " << user_id << endl;

    confirmation_callback_t callback{co_await event.co_get_original_response()};
    if (callback.is_error()) {
        cerr << "Error: " << callback.get_error().message << endl;
        co_return;
    }
    const message &to_cache_msg{callback.get<message>()};

    calc_sessions.insert({user_id, make_pair(to_cache_msg, calculator_client_t{})});
    co_return;
}

task<void> calculator_subcommand_handler(const slashcommand_t &event,
                                         const command_data_option &options) {
    const command_data_option & subcommand{options.options[0]};

    if (DEBUG) cerr << "Handling calculator subcommand: " << subcommand.name << endl;

    if (subcommand.name == CALC_SUBCMDS[CALC_SUBCMD_START][0])
        co_await start_interactive_calculator(event);
    else if (subcommand.name == CALC_SUBCMDS[CALC_SUBCMD_PERCENT][0])
        co_await process_percent_progress(event);
    else if (subcommand.name == CALC_SUBCMDS[CALC_SUBCMD_COSMOSAPSIS][0])
        co_await process_cosmosapsis(event);
    else if (subcommand.name == CALC_SUBCMDS[CALC_SUBCMD_RESPIRA][0])
        co_await process_respira(event);
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

    if (id == CALC_EVENT_IDS[CALC_CANCEL])
        co_await calc_cancel(event);
    else if (id == CALC_EVENT_IDS[CALC_ASK_STAGE])
        co_await calc_ask_stage(event);
    else if (id == CALC_EVENT_IDS[CALC_ASK_PERCENT_PROGRESS]) {
        calculator_client_t &client{calc_sessions[event.command.usr.id].second};
        if (client.major_stage == NUM_MAJOR_STAGES || client.minor_stage == NUM_MINOR_STAGES)
            // do nothing and wait for user to finish selection
            co_return;
        else
            // else continue to next step
            co_await calc_ask_percent_progress(event);
    } else if (id == CALC_EVENT_IDS[CALC_ASK_COSMOSAPSIS])
        co_await calc_ask_cosmosapsis(event);
    else if (id == CALC_EVENT_IDS[CALC_ASK_AURA_GEM])
        co_await calc_ask_aura_gem(event);
    else if (id == CALC_EVENT_IDS[CALC_ASK_RESPIRA]) {
        calculator_client_t &client{calc_sessions[event.command.usr.id].second};
        if (client.aura_gem_quality == NUM_QUALITIES)
            // do nothing and wait for user to finish selection
            co_return;
        else
            // else continue to next step
            co_await calc_ask_respira(event);
    } else if (id == CALC_EVENT_IDS[CALC_ASK_PILL]) {
        co_await calc_ask_pill(event);
    } else if (id == CALC_EVENT_IDS[CALC_ASK_EXTRACTOR_QUALITY])
        co_await calc_ask_extractor_quality(event);
    else if (id == CALC_EVENT_IDS[CALC_ASK_EXTRACTOR_NODE_LVL])
        co_await calc_ask_extractor_node_lvl(event);
    else if (id == CALC_EVENT_IDS[CALC_ASK_MYRIMON_FRUIT])
        co_await calc_ask_myrimon_fruit(event);
    else if (id == CALC_EVENT_IDS[CALC_ASK_VASE_OWN])
        co_await calc_ask_vase_own(event);
    else if (id == CALC_EVENT_IDS[CALC_ASK_VASE_DETAIL])
        co_await calc_ask_vase_detail(event);
    else if (id == CALC_EVENT_IDS[CALC_ASK_MIRROR_OWN])
        co_await calc_ask_mirror_own(event);
    else if (id == CALC_EVENT_IDS[CALC_ASK_MIRROR_DETAIL])
        co_await calc_ask_mirror_detail(event);

    else
        cerr << "Unhandled calculator event: " << id << endl;

    co_return;
}

task<void> calculator_select_click_handler(const select_click_t &event) {
    auto it{co_await verify_user(event)};
    if (!it) co_return;

    calculator_client_t &client{it.value()->second.second};

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
task<optional<calc_session_map::iterator>> verify_user(const T &event) {
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
    co_return it;
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
                                     .set_id(CALC_EVENT_IDS[CALC_ASK_PERCENT_PROGRESS])};

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
    if (client.major_stage != NUM_MAJOR_STAGES)
        info += "- Major Stage: " + string{MAJOR_STAGE_STR[client.major_stage]} + "\n";
    if (client.minor_stage != NUM_MINOR_STAGES)
        info += "- Minor Stage: " + string{MINOR_STAGE_STR[client.minor_stage]} + "\n";

    // TODO: handle gate

    if (client.percent_progress != -1.0)
        info += "- Percent Progress: " + to_string(client.percent_progress) + "\n";
    if (client.cosmosapsis != -1.0)
        info += "- Cosmosapsis: " + to_string(client.cosmosapsis) + "\n";

    if (client.aura_gem_quality != NUM_QUALITIES)
        info += "- Aura Gem Quality: " + string{QUALITY_STR[client.aura_gem_quality]} + "\n";

    if (client.respira_exp != 0) info += "- Respira Exp: " + to_string(client.respira_exp) + "\n";

    if (client.daily_respira_attempts != 0)
        info += "- Daily Respira Attempts: " + to_string(client.daily_respira_attempts) + "\n";

    if (client.daily_pill_attempts != 0)
        info += "- Daily Pill Attempts: " + to_string(client.daily_pill_attempts) + "\n";

    if (client.pill_quantity[0] != 0 || client.pill_quantity[1] != 0 ||
        client.pill_quantity[2] != 0)
        info += "- Pill Quantity: " + to_string(client.pill_quantity[0]) + " (Rare), " +
                to_string(client.pill_quantity[1]) + " (Epic), " +
                to_string(client.pill_quantity[2]) + " (Legendary)\n";

    if (client.pill_bonus != 0.0) info += "- Pill Bonus: " + to_string(client.pill_bonus) + "%\n";

    if (client.extractor_quality != NUM_QUALITIES)
        info += "- Extractor Quality: " + string{QUALITY_STR[client.extractor_quality]} + "\n";

    if (client.node_levels[0] != 31 && client.node_levels[1] != 31 && client.node_levels[2] != 31)
        info += "- Node Levels: " + to_string(client.node_levels[0]) + " (Cultivation XP), " +
                to_string(client.node_levels[1]) + " (Quality), " +
                to_string(client.node_levels[2]) + " (Gush)\n";

    if (client.fruit_quantity != 0)
        info += "- Myrimon Fruit Quantity: " + to_string(client.fruit_quantity) + "\n";

    // TODO: handle artifacts

    return info + "\n\n";
}

task<void> calc_ask_percent_progress(const button_click_t &event) {
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
    snowflake user_id{event.command.usr.id};
    auto it{calc_sessions.find(user_id)};
    if (it == calc_sessions.end()) {
        if (DEBUG) cerr << "User ID: " << user_id << " is not in any session." << endl;

        co_await event.co_reply(message("This command should be used in an interactive calculator "
                                        "session."
                                        "Please start a new session with `/debug calc "
                                        "interactive`.")
                                    .set_flags(m_ephemeral));
        co_return;
    }

    // acknowledge the slash command
    auto reply{event.co_reply("input received, processing...")};

    calculator_client_t &client{it->second.second};
    client.percent_progress =
        get<double>(event.get_parameter(string{CALC_SUBCMD_PARAM[CALC_SUBCMD_PERCENT][0][0]}));

    component text_display{component()
                               .set_type(cot_text_display)
                               .set_content("Your percent progress is set to " +
                                            to_string(client.percent_progress) +
                                            "%.\n\n"
                                            "Please click **NEXT** to continue, or use `/calc "
                                            "percent` to update your progress.\n\n")};

    static component next_button{component()
                                     .set_type(cot_button)
                                     .set_style(cos_primary)
                                     .set_label("NEXT")
                                     .set_id(CALC_EVENT_IDS[CALC_ASK_COSMOSAPSIS])};

    message msg{calc_sessions[user_id].first};

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

    auto edit_msg{bot.co_message_edit(msg)};

    co_await reply;
    co_await event.co_delete_original_response();
    co_await edit_msg;

    co_return;
}

task<void> calc_ask_cosmosapsis(const button_click_t &event) {
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
    snowflake user_id{event.command.usr.id};
    auto it{calc_sessions.find(user_id)};
    if (it == calc_sessions.end()) {
        if (DEBUG) cerr << "User ID: " << user_id << " is not in any session." << endl;

        co_await event.co_reply(
            message("This command should be used in an interactive calculator session."
                    "Please start a new session with `/debug calc interactive`.")
                .set_flags(m_ephemeral));
        co_return;
    }

    // acknowledge the slash command
    auto reply{event.co_reply("input received, processing...")};

    calculator_client_t &client{it->second.second};
    client.cosmosapsis =
        get<double>(event.get_parameter(string{CALC_SUBCMD_PARAM[CALC_SUBCMD_COSMOSAPSIS][0][0]}));

    component text_display{component()
                               .set_type(cot_text_display)
                               .set_content("Your cosmosapsis is set to " +
                                            to_string(client.cosmosapsis) +
                                            ".\n\n"
                                            "Please click **NEXT** to continue, or use `/calc "
                                            "cosmosapsis` to update your progress.\n\n")};

    static component next_button{component()
                                     .set_type(cot_button)
                                     .set_style(cos_primary)
                                     .set_label("NEXT")
                                     .set_id(CALC_EVENT_IDS[CALC_ASK_AURA_GEM])};

    message msg{calc_sessions[user_id].first};

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

    auto edit_msg{bot.co_message_edit(msg)};

    co_await reply;
    co_await event.co_delete_original_response();
    co_await edit_msg;

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
                                     .set_id(CALC_EVENT_IDS[CALC_ASK_RESPIRA])};

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
    snowflake user_id{event.command.usr.id};
    auto it{calc_sessions.find(user_id)};
    if (it == calc_sessions.end()) {
        if (DEBUG) cerr << "User ID: " << user_id << " is not in any session." << endl;

        co_await event.co_reply(message("This command should be used in an interactive calculator "
                                        "session."
                                        "Please start a new session with `/debug calc "
                                        "interactive`.")
                                    .set_flags(m_ephemeral));
        co_return;
    }

    // acknowledge the slash command
    auto reply{event.co_reply("input received, processing...")};

    calculator_client_t &client{it->second.second};
    client.respira_exp = static_cast<exp_t>(
        get<int64_t>(event.get_parameter(string{CALC_SUBCMD_PARAM[CALC_SUBCMD_RESPIRA][0][0]})));
    client.daily_respira_attempts = static_cast<exp_t>(
        get<int64_t>(event.get_parameter(string{CALC_SUBCMD_PARAM[CALC_SUBCMD_RESPIRA][1][0]})));

    component text_display{component()
                               .set_type(cot_text_display)
                               .set_content("Your respira exp is set to " +
                                            to_string(client.respira_exp) +
                                            ", and daily attempts are set to " +
                                            to_string(client.daily_respira_attempts) +
                                            ".\n\n"
                                            "Please click **NEXT** to continue, or use `/calc "
                                            "respira` to update your progress.\n\n")};

    static component next_button{component()
                                     .set_type(cot_button)
                                     .set_style(cos_primary)
                                     .set_label("NEXT")
                                     .set_id(CALC_EVENT_IDS[CALC_ASK_PILL])};

    message msg{calc_sessions[user_id].first};

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

    auto edit_msg{bot.co_message_edit(msg)};

    co_await reply;
    co_await event.co_delete_original_response();
    co_await edit_msg;

    co_return;
}

task<void> calc_ask_pill(const button_click_t &event) {
    // TODO

    co_await calc_under_construction(event);
    co_return;
}

task<void> process_pill(const slashcommand_t &event) {
    co_await event.co_reply(message("This command is not implemented yet.").set_flags(m_ephemeral));
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