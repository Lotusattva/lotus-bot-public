#include <dpp/dpp.h>
#include <dpp/message.h>

#include <iostream>
#include <print>

#include "calculator/calculator_interface.hpp"
#include "feet.hpp"
#include "global.hpp"
#include "poll.hpp"
#include "shin.hpp"

void sigint_handler(int _) {
    cout << "\nSIGINT received, shutting down..." << endl;
    exit(0);
}

int main() {
    signal(SIGINT, sigint_handler);

    bot.on_log(utility::cout_logger());

    bot.on_slashcommand([](const slashcommand_t& event) -> task<void> {
        const command_interaction cmd_interaction{event.command.get_command_interaction()};

        if (event.command.get_command_name() == "debug") {
            const command_data_option& options{cmd_interaction.options[0]};
            if (options.name == "poll")
                co_await poll_subcommand_handler(event, options);
            else if (options.name == "calc")
                co_await calculator_subcommand_handler(event, options);
        }
    });

    bot.on_ready([](const ready_t& event) {
        if (run_once<struct register_bot_commands>()) {
            slashcommand debug_cmd{
                slashcommand("debug", "debug commands", bot.me.id).add_option(poll_commands())};

            bot.global_bulk_command_create({debug_cmd, calculator_commands()});
        }
    });

    bot.on_button_click([](const button_click_t& event) -> task<void> {
        co_await event.co_reply(ir_deferred_update_message, "");
        // if event.custom_id starts with "calc_", redirect to calculator handler
        if (event.custom_id.starts_with("calc_"))
            co_await calculator_button_click_handler(event);
        else {
            // do nothing on client side
            if (DEBUG) println(cerr, "Unhandled button click event: {}", event.custom_id);
        }
    });

    bot.on_select_click([](const select_click_t& event) -> task<void> {
        co_await event.co_reply(ir_deferred_update_message, "");
        // if event.custom_id starts with "calc_", redirect to calculator handler
        if (event.custom_id.starts_with("calc_"))
            co_await calculator_select_click_handler(event);
        else {
            // do nothing on client side
            if (DEBUG) println(cerr, "Unhandled select click event: {}", event.custom_id);
        }
    });

    bot.on_message_create(&feet);
    bot.on_message_create(&shin);

    if (DEBUG) {
        // calculator_client_t client;
        // client.major_stage = PERFECTION;
        // client.minor_stage = EARLY;
        // client.percent_progress = 30.0;
        // client.cosmosapsis = 900.0;
        // client.aura_gem_quality = MYTHIC;
        // client.respira_bonus = 222.0;
        // client.daily_respira_attempts = 16;
        // client.daily_pill_attempts = 27;
        // client.pill_quantity[RARE_IDX] = 15;
        // client.pill_quantity[EPIC_IDX] = 10;
        // client.pill_quantity[LEGENDARY_IDX] = 2;
        // client.pill_bonus = 47.8;
        // client.extractor_quality = MYTHIC;
        // client.extractor_major_stage_bonus = YES;
        // client.node_levels[CULTIXP_NODE] = 30;
        // client.node_levels[QUALITY_NODE] = 30;
        // client.node_levels[GUSH_NODE] = 30;
        // client.fruit_quantity = 100;
        // client.vase.emplace();
        // client.vase->star = 3;
        // client.vase->daily_recharge = YES;
        // client.vase_transmog = YES;
        // client.mirror.emplace();
        // client.mirror->star = 3;
        // client.mirror->daily_recharge = YES;

        // auto result{get_estimated_time_to_breakthrough(client)};
        // if (result)
        //     println(cout, "Estimated time to breakthrough: {} days and {} hours.",
        //             result.value().count() / 24, result.value().count() % 24);
        // else
        //     println(cerr, "Error calculating estimated time to breakthrough: {}",
        //             static_cast<int>(result.error()));
    } else {
        // auto now{chrono::system_clock::now()};
        // auto now_time_t{chrono::system_clock::to_time_t(now)};
        // tm* now_tm{std::gmtime(&now_time_t)};

        // // Set target time to 19:00 on the next Friday
        // now_tm->tm_hour = 19;
        // now_tm->tm_min = 0;
        // now_tm->tm_sec = 0;
        // now_tm->tm_wday = 5;  // Friday (0 = Sunday, 5 = Friday)

        // auto target_time{chrono::system_clock::from_time_t(mktime(now_tm))};
        // if (target_time < now)  // If the target time is in the past, move to the next Friday
        //     target_time += chrono::days(1);

        // // seconds to target time
        // auto seconds_to_target{chrono::duration_cast<chrono::seconds>(target_time - now)};

        // // Execute the poll scheduler initialization
        // bot.start_timer(&create_polls, seconds_to_target.count(), &schedule_next_create);

        // bot.start_timer(&process_poll_results,
        //                 seconds_to_target.count() + 24 * 60 * 60,  // 24 hours later
        //                 &schedule_next_process);
    }

    bot.start(st_wait);
}