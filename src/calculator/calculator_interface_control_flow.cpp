#include "../global.hpp"
#include "calculator_interactions.hpp"
#include "calculator_interface.hpp"
#include "calculator_types.hpp"

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
                            co_number, string{CALC_SUBCMD_PARAM[CALC_SUBCMD_RESPIRA][0][0]},
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
                                .set_min_value(0)))

        // subcommand for reporting extractor node levels
        .add_option(command_option{co_sub_command, string{CALC_SUBCMDS[CALC_SUBCMD_EXTRACTOR][0]},
                                   string{CALC_SUBCMDS[CALC_SUBCMD_EXTRACTOR][1]}}
                        // Parameters
                        // 1. cultivation xp node level
                        .add_option(command_option{
                            co_integer, string{CALC_SUBCMD_PARAM[CALC_SUBCMD_EXTRACTOR][0][0]},
                            string{CALC_SUBCMD_PARAM[CALC_SUBCMD_EXTRACTOR][0][1]}, true}
                                        .set_min_value(0)
                                        .set_max_value(30))
                        // 2. quality node level
                        .add_option(command_option{
                            co_integer, string{CALC_SUBCMD_PARAM[CALC_SUBCMD_EXTRACTOR][1][0]},
                            string{CALC_SUBCMD_PARAM[CALC_SUBCMD_EXTRACTOR][1][1]}, true}
                                        .set_min_value(0)
                                        .set_max_value(30))
                        // 3. gush node level
                        .add_option(command_option{
                            co_integer, string{CALC_SUBCMD_PARAM[CALC_SUBCMD_EXTRACTOR][2][0]},
                            string{CALC_SUBCMD_PARAM[CALC_SUBCMD_EXTRACTOR][2][1]}, true}
                                        .set_min_value(0)
                                        .set_max_value(30)))

        // subcommand for reporting number of myrimon fruit
        .add_option(command_option{co_sub_command, string{CALC_SUBCMDS[CALC_SUBCMD_MYRIMON][0]},
                                   string{CALC_SUBCMDS[CALC_SUBCMD_MYRIMON][1]}}
                        // Parameters
                        // 1. number of myrimon fruit
                        .add_option(command_option{
                            co_integer, string{CALC_SUBCMD_PARAM[CALC_SUBCMD_MYRIMON][0][0]},
                            string{CALC_SUBCMD_PARAM[CALC_SUBCMD_MYRIMON][0][1]}, true}
                                        .set_min_value(0)))

        /////// end of subcommands
        ;
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
    else if (subcommand.name == CALC_SUBCMDS[CALC_SUBCMD_EXTRACTOR][0])
        co_await process_extractor_node(event);
    else if (subcommand.name == CALC_SUBCMDS[CALC_SUBCMD_MYRIMON][0])
        co_await process_myrimon(event);
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
        else if (client.respira_bonus == INVALID_DOUBLE_VAL ||
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
            co_await calc_ask_extractor(event);
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
            co_await calc_ask_extractor(event);
    } else if (id == CALC_BUTTON_IDS[CALC_BUTTON_PILL])
        co_await calc_ask_extractor(event);
    else if (id == CALC_BUTTON_IDS[CALC_BUTTON_EXTRACTOR_QUALITY]) {
        if (client.extractor_quality == INVALID_QUALITY)
            co_return;  // do nothing since user has not selected extractor quality
        else if (client.node_levels[0] == INVALID_EXTRACTOR_NODE_LVL ||
                 client.node_levels[1] == INVALID_EXTRACTOR_NODE_LVL ||
                 client.node_levels[2] == INVALID_EXTRACTOR_NODE_LVL)
            // ask for extractor node levels if not set
            co_await calc_ask_extractor_node(event);
        else if (client.fruit_quantity == INVALID_UNSIGNED_VAL)
            // ask for myrimon fruit quantity if not set
            co_await calc_ask_myrimon(event);
        else
            // ask for vase ownership if all previous data are set
            co_await calc_ask_vase_own(event);
    } else if (id == CALC_BUTTON_IDS[CALC_BUTTON_EXTRACTOR_NODE]) {
        if (client.fruit_quantity == INVALID_UNSIGNED_VAL)
            // ask for myrimon fruit quantity if not set
            co_await calc_ask_myrimon(event);
        else
            // ask for vase ownership if all previous data are set
            co_await calc_ask_vase_own(event);
    } else if (id == CALC_BUTTON_IDS[CALC_BUTTON_MYRIMON])
        co_await calc_ask_vase_own(event);
    else if (id == CALC_BUTTON_IDS[CALC_BUTTON_VASE_YES])
        // ask about vase details if user owns a vase
        co_await calc_ask_vase_detail(event);
    else if (id == CALC_BUTTON_IDS[CALC_BUTTON_VASE_NO])
        // skip asking mirror
        co_await calc_under_construction(event);  // TODO: replace with calc_result
    else if (id == CALC_BUTTON_IDS[CALC_BUTTON_VASE_DETAIL]) {
        if (client.vase.has_value() && client.vase->star != INVALID_STAR &&
            client.vase->daily_recharge != INVALID_BINARY_VAL)
            // ask about mirror ownership if vase details are set
            co_await calc_ask_mirror_own(event);
        else
            co_return;  // do nothing since user has not set vase details
    } else if (id == CALC_BUTTON_IDS[CALC_BUTTON_MIRROR_YES])
        // ask about mirror details if user owns a mirror
        co_await calc_ask_mirror_detail(event);
    else if (id == CALC_BUTTON_IDS[CALC_BUTTON_MIRROR_NO])
        // skip asking mirror details
        co_await calc_result(event);
    else if (id == CALC_BUTTON_IDS[CALC_BUTTON_MIRROR_DETAIL]) {
        if (client.mirror.has_value() && client.mirror->star != INVALID_STAR &&
            client.mirror->daily_recharge != INVALID_BINARY_VAL)
            // all data are set, then calculate result
            co_await calc_result(event);
    } else {
        if (DEBUG) cerr << "Unhandled calculator button click event: " << id << endl;
        co_await calc_under_construction(event);
    }

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
    } else if (event.custom_id == CALC_SELECT_IDS[CALC_SELECT_EXTRACTOR_QUALITY]) {
        if (DEBUG) cerr << "Extractor quality selected: " << event.values[0] << endl;

        client.extractor_quality = get_quality(event.values[0]);
    } else if (event.custom_id == CALC_SELECT_IDS[CALC_SELECT_EXTRACTOR_MAJOR_STAGE_BONUS]) {
        if (DEBUG) cerr << "Extractor major stage bonus selected: " << event.values[0] << endl;

        client.extractor_major_stage_bonus = get_binary_val(event.values[0]);
    } else if (event.custom_id == CALC_SELECT_IDS[CALC_SELECT_VASE_STAR]) {
        if (DEBUG) cerr << "Vase star selected: " << event.values[0] << endl;

        if (!client.vase) client.vase.emplace();
        client.vase->star = get_artifact_star(event.values[0]);
    } else if (event.custom_id == CALC_SELECT_IDS[CALC_SELECT_VASE_DAILY_RECHARGE]) {
        if (DEBUG) cerr << "Vase daily recharge selected: " << event.values[0] << endl;

        if (!client.vase) client.vase.emplace();
        client.vase->daily_recharge = get_binary_val(event.values[0]);
    } else if (event.custom_id == CALC_SELECT_IDS[CALC_SELECT_VASE_TRANSMOG]) {
        if (DEBUG) cerr << "Vase transmog selected: " << event.values[0] << endl;

        client.vase_transmog = get_binary_val(event.values[0]);
    } else if (event.custom_id == CALC_SELECT_IDS[CALC_SELECT_MIRROR_STAR]) {
        if (DEBUG) cerr << "Mirror star selected: " << event.values[0] << endl;

        if (!client.mirror) client.mirror.emplace();
        client.mirror->star = get_artifact_star(event.values[0]);
    } else if (event.custom_id == CALC_SELECT_IDS[CALC_SELECT_MIRROR_DAILY_RECHARGE]) {
        if (DEBUG) cerr << "Mirror daily recharge selected: " << event.values[0] << endl;

        if (!client.mirror) client.mirror.emplace();
        client.mirror->daily_recharge = get_binary_val(event.values[0]);
    } else
        cerr << "Unhandled select event: " << event.custom_id << endl;

    co_return;
}