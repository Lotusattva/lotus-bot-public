#include "calculator.hpp"

static const component cancel_button{component()
                                         .set_type(cot_button)
                                         .set_style(cos_danger)
                                         .set_label("CANCEL")
                                         .set_id(CALC_EVENT_IDS[CALC_CANCEL])};

command_option calculator_commands() {
    return command_option{co_sub_command_group, string{CALC_MAIN_COMMAND[0]},
                          string{CALC_MAIN_COMMAND[1]}}
        .add_option(command_option{co_sub_command,
                                   string{CALC_SUBCMDS[CALC_SUBCMD_START][0]},
                                   string{CALC_SUBCMDS[CALC_SUBCMD_START][1]}})
        .add_option(
            command_option{co_sub_command,
                           string{CALC_SUBCMDS[CALC_SUBCMD_PERCENT][0]},
                           string{CALC_SUBCMDS[CALC_SUBCMD_PERCENT][1]}}
                .add_option(command_option{
                    co_number,
                    string{CALC_SUBCMD_PARAM[CALC_SUBCMD_PERCENT][0][0]},
                    string{CALC_SUBCMD_PARAM[CALC_SUBCMD_PERCENT][0][1]}, true}
                                .set_min_value(0.0)
                                .set_max_value(1000.0)))
        .add_option(
            command_option{co_sub_command,
                           string{CALC_SUBCMDS[CALC_SUBCMD_COSMOSAPSIS][0]},
                           string{CALC_SUBCMDS[CALC_SUBCMD_COSMOSAPSIS][1]}}
                .add_option(command_option{
                    co_number,
                    string{CALC_SUBCMD_PARAM[CALC_SUBCMD_COSMOSAPSIS][0][0]},
                    string{CALC_SUBCMD_PARAM[CALC_SUBCMD_COSMOSAPSIS][0][1]},
                    true}
                                .set_min_value(0.0)))
        .add_option(
            command_option{co_sub_command,
                           string{CALC_SUBCMDS[CALC_SUBCMD_RESPIRA][0]},
                           string{CALC_SUBCMDS[CALC_SUBCMD_RESPIRA][1]}}
                .add_option(command_option{
                    co_integer,
                    string{CALC_SUBCMD_PARAM[CALC_SUBCMD_RESPIRA][0][0]},
                    string{CALC_SUBCMD_PARAM[CALC_SUBCMD_RESPIRA][0][1]}, true}
                                .set_min_value(0))
                .add_option(command_option{
                    co_integer,
                    string{CALC_SUBCMD_PARAM[CALC_SUBCMD_RESPIRA][1][0]},
                    string{CALC_SUBCMD_PARAM[CALC_SUBCMD_RESPIRA][1][1]}, true}
                                .set_min_value(0)));
}

task<void> start_interactive_calculator(const slashcommand_t &event) {
    static component calc_overview_text{
        component()
            .set_type(cot_text_display)
            .set_content(
                "This is a cultivation calculator that produces an estimate "
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

    static message calc_msg{
        message()
            .set_flags(m_using_components_v2)
            .add_component_v2(component()
                                  .set_type(cot_container)
                                  .add_component_v2(calc_overview_text)
                                  .add_component_v2(action_row))};

    co_await event.co_reply(calc_msg);

    if (DEBUG) cerr << "Caching session..." << endl;

    snowflake user_id{event.command.usr.id};

    if (DEBUG) cerr << "User ID: " << user_id << endl;

    confirmation_callback_t callback{co_await event.co_get_original_response()};
    if (callback.is_error()) {
        cerr << "Error: " << callback.get_error().message << endl;
        co_return;
    }
    const message &to_cache_msg{callback.get<message>()};

    calc_sessions.insert(
        {user_id, make_pair(to_cache_msg, calculator_client_t{})});
    co_return;
}

task<void> calculator_subcommand_handler(
    const slashcommand_t &event, const command_data_option &subcommand) {
    if (subcommand.name == "interactive")
        co_await start_interactive_calculator(event);
    else if (subcommand.name == "percent")
        co_await process_percent_progress(event);
    else if (subcommand.name == "cosmosapsis")
        co_await process_cosmosapsis(event);
    else if (subcommand.name == "respira")
        co_await process_respira(event);
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
        if (client.major_stage == NUM_MAJOR_STAGES ||
            client.minor_stage == NUM_MINOR_STAGES)
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
    } else
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
    } else if (event.custom_id ==
               CALC_SELECT_IDS[CALC_SELECT_AURA_GEM_QUALITY]) {
        if (DEBUG)
            cerr << "Aura gem quality selected: " << event.values[0] << endl;

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
        if (DEBUG)
            cerr << "this user is not the owner of this session." << endl;
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
                 << " does not match the cached message ID: "
                 << it->second.first.id << endl;
        co_return nullopt;
    }

    if (DEBUG)
        cerr << "User ID: " << user_id << " is the owner of this session."
             << endl;
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
                    .add_component_v2(
                        component()
                            .set_type(cot_text_display)
                            .set_content("You have cancelled the session.")))};

    co_await event.co_edit_response(session_cancel_message);

    calc_sessions.erase(event.command.usr.id);
    co_return;
}

component major_stage_selectmenu_factory() {
    component major_stage_selectmenu{
        component()
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
    component minor_stage_selectmenu{
        component()
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

    //////////// Customize UI elements here! ////////////////

    static component text_display{
        component()
            .set_type(cot_text_display)
            .set_content("Please select your major and minor stage")};

    static component major_stage_selectmenu{
        component()
            .set_type(cot_action_row)
            .add_component_v2(major_stage_selectmenu_factory())};

    static component minor_stage_selectmenu{
        component()
            .set_type(cot_action_row)
            .add_component_v2(minor_stage_selectmenu_factory())};

    static component next_button{
        component()
            .set_type(cot_button)
            .set_style(cos_primary)
            .set_label("NEXT")
            .set_id(CALC_EVENT_IDS[CALC_ASK_PERCENT_PROGRESS])};

    static message calc_ask_stage_message{
        message()
            .set_flags(m_using_components_v2)
            .add_component_v2(
                component()
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

    confirmation_callback_t confirmation{
        co_await event.co_edit_response(calc_ask_stage_message)};

    if (confirmation.is_error()) {
        cerr << "Error: " << confirmation.get_error().message << endl;
        co_return;
    }

    co_return;
}

task<void> calc_ask_percent_progress(const button_click_t &event) {
    if (DEBUG) cerr << "Asking for percent progress..." << endl;

    calculator_client_t &client{calc_sessions[event.command.usr.id].second};
    string client_info{
        "You have entered:\n"
        "- Major Stage: " +
        string{MAJOR_STAGE_STR[client.major_stage]} +
        "\n"
        "- Minor Stage: " +
        string{MINOR_STAGE_STR[client.minor_stage]} + "\n\n"};

    component text_display{
        component()
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
                    .add_component_v2(component()
                                          .set_type(cot_action_row)
                                          .add_component_v2(cancel_button))));

    co_return;
}

task<void> process_percent_progress(const slashcommand_t &event) {
    snowflake user_id{event.command.usr.id};
    auto it{calc_sessions.find(user_id)};
    if (it == calc_sessions.end()) {
        if (DEBUG)
            cerr << "User ID: " << user_id << " is not in any session." << endl;

        co_await event.co_reply(
            message("This command should be used in an interactive calculator "
                    "session."
                    "Please start a new session with `/debug calc "
                    "interactive`.")
                .set_flags(m_ephemeral));
        co_return;
    }

    // acknowledge the slash command
    auto reply{event.co_reply("input received, processing...")};

    calculator_client_t &client{it->second.second};
    client.percent_progress = get<double>(event.get_parameter("percentage"));

    component text_display{
        component()
            .set_type(cot_text_display)
            .set_content("Your percent progress is set to " +
                         to_string(client.percent_progress) +
                         "%.\n\n"
                         "Please click **NEXT** to continue, or use `/calc "
                         "percent` to update your progress.\n\n")};

    static component next_button{
        component()
            .set_type(cot_button)
            .set_style(cos_primary)
            .set_label("NEXT")
            .set_id(CALC_EVENT_IDS[CALC_ASK_COSMOSAPSIS])};

    message msg{calc_sessions[user_id].first};

    // edit the message
    msg.components.clear();
    msg.add_component_v2(
        component()
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

    calculator_client_t &client{calc_sessions[event.command.usr.id].second};
    string client_info{
        "You have entered:\n"
        "- Major Stage: " +
        string{MAJOR_STAGE_STR[client.major_stage]} +
        "\n"
        "- Minor Stage: " +
        string{MINOR_STAGE_STR[client.minor_stage]} +
        "\n"
        "- Percent Progress: " +
        to_string(client.percent_progress) + "%\n\n"};

    component text_display{
        component()
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
                    .add_component_v2(component()
                                          .set_type(cot_action_row)
                                          .add_component_v2(cancel_button))));

    co_return;
}

task<void> process_cosmosapsis(const slashcommand_t &event) {
    snowflake user_id{event.command.usr.id};
    auto it{calc_sessions.find(user_id)};
    if (it == calc_sessions.end()) {
        if (DEBUG)
            cerr << "User ID: " << user_id << " is not in any session." << endl;

        co_await event.co_reply(
            message("This command should be used in an interactive calculator "
                    "session."
                    "Please start a new session with `/debug calc "
                    "interactive`.")
                .set_flags(m_ephemeral));
        co_return;
    }

    // acknowledge the slash command
    auto reply{event.co_reply("input received, processing...")};

    calculator_client_t &client{it->second.second};
    client.cosmosapsis = get<double>(event.get_parameter("cosmosapsis_val"));

    component text_display{
        component()
            .set_type(cot_text_display)
            .set_content("Your cosmosapsis is set to " +
                         to_string(client.cosmosapsis) +
                         ".\n\n"
                         "Please click **NEXT** to continue, or use `/calc "
                         "cosmosapsis` to update your progress.\n\n")};

    static component next_button{
        component()
            .set_type(cot_button)
            .set_style(cos_primary)
            .set_label("NEXT")
            .set_id(CALC_EVENT_IDS[CALC_ASK_AURA_GEM])};

    message msg{calc_sessions[user_id].first};

    // edit the message
    msg.components.clear();
    msg.add_component_v2(
        component()
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
    component aura_gem_quality_selectmenu{
        component()
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

    string client_info{
        "You have entered:\n"
        "- Major Stage: " +
        string{MAJOR_STAGE_STR[calc_sessions[event.command.usr.id]
                                   .second.major_stage]} +
        "\n"
        "- Minor Stage: " +
        string{MINOR_STAGE_STR[calc_sessions[event.command.usr.id]
                                   .second.minor_stage]} +
        "\n"
        "- Percent Progress: " +
        to_string(calc_sessions[event.command.usr.id].second.percent_progress) +
        "%\n"
        "- Cosmosapsis: " +
        to_string(calc_sessions[event.command.usr.id].second.cosmosapsis) +
        "\n\n"};

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
            .set_content(client_info +
                         "Please select the quality of your aura gem.")};

    if (DEBUG) cerr << "Sending aura gem selection message..." << endl;

    co_await event.co_edit_response(
        message()
            .set_flags(m_using_components_v2)
            // a container
            .add_component_v2(
                component()
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

    calculator_client_t &client{calc_sessions[event.command.usr.id].second};

    string client_info{
        "You have entered:\n"
        "- Major Stage: " +
        string{MAJOR_STAGE_STR[client.major_stage]} +
        "\n"
        "- Minor Stage: " +
        string{MINOR_STAGE_STR[client.minor_stage]} +
        "\n"
        "- Percent Progress: " +
        to_string(client.percent_progress) +
        "%\n"
        "- Cosmosapsis: " +
        to_string(client.cosmosapsis) +
        "\n"
        "- Aura Gem Quality: " +
        string{QUALITY_STR[client.aura_gem_quality]} + "\n\n"};

    component text_display{
        component()
            .set_type(cot_text_display)
            .set_content(
                client_info +
                "Please input the amount of exp per respira attempt and "
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
                    .add_component_v2(component()
                                          .set_type(cot_action_row)
                                          .add_component_v2(cancel_button))));

    co_return;
}

task<void> process_respira(const slashcommand_t &event) {
    snowflake user_id{event.command.usr.id};
    auto it{calc_sessions.find(user_id)};
    if (it == calc_sessions.end()) {
        if (DEBUG)
            cerr << "User ID: " << user_id << " is not in any session." << endl;

        co_await event.co_reply(
            message("This command should be used in an interactive calculator "
                    "session."
                    "Please start a new session with `/debug calc "
                    "interactive`.")
                .set_flags(m_ephemeral));
        co_return;
    }

    // acknowledge the slash command
    auto reply{event.co_reply("input received, processing...")};

    calculator_client_t &client{it->second.second};
    client.respira_exp =
        static_cast<exp_t>(get<int64_t>(event.get_parameter("respira_exp")));
    client.daily_respira_attempts =
        static_cast<exp_t>(get<int64_t>(event.get_parameter("daily_attempts")));

    component text_display{
        component()
            .set_type(cot_text_display)
            .set_content("Your respira exp is set to " +
                         to_string(client.respira_exp) +
                         ", "
                         "and daily attempts are set to " +
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
    msg.add_component_v2(
        component()
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

auto add_two_things(auto a, auto b) { return a + b; }