#include <dpp/dpp.h>
#include <dpp/message.h>
#include <unordered_map>

#include "commands.hpp"
#include "global.hpp"
#include "scheduler.hpp"
#include "calculator.hpp"

void sigint_handler(int _) {
    cout << "\nSIGINT received, shutting down..." << endl;
    exit(0);
}

int main() {
    signal(SIGINT, sigint_handler);

    bot.on_log(utility::cout_logger());

    bot.on_slashcommand([](const slashcommand_t& event) -> task<void> {
        command_interaction cmd_data{ event.command.get_command_interaction() };

        if (event.command.get_command_name() == "debug") {
            const auto& subcommand{ cmd_data.options[0] };
            if (subcommand.name == "poll") {
                const auto& action{ subcommand.options[0] };
                if (action.name == "start")
                    co_await start_debug_poll(event);
                else if (action.name == "end")
                    co_await end_debug_poll(event);
            } else if (subcommand.name == "calc") {
                const auto& action{ subcommand.options[0] };
                if (action.name == "interactive")
                    co_await start_interactive_calculator(event);
            }
        }

        });

    bot.on_ready([](const ready_t& event) {
        if (run_once<struct register_bot_commands>()) {
            slashcommand debug_cmd{ slashcommand("debug", "debug commands", bot.me.id)
                .add_option(
                    // for testing polls
                    command_option(co_sub_command_group, "poll", "sect clash array sign up polls")
                    .add_option(command_option(co_sub_command, "start", "start a poll"))
                    .add_option(command_option(co_sub_command, "end", "end a poll"))
                ).add_option(
                    // for testing cultivation calculator
                    command_option(co_sub_command_group, "calc", "cultivation calculator")
                    .add_option(command_option(co_sub_command, "interactive", "interactive step-by-step calculator"))
                    // .add_option(
                    //     command_option(co_sub_command, "arg", "terminal-style calculator")
                    // )
                )
            };

            bot.global_bulk_command_create({ debug_cmd });
        }

        });

    bot.on_button_click([](const button_click_t& event) -> task<void> {
        // if event.custom_id starts with "calc_", redirect to calculator handler
        if (event.custom_id.starts_with("calc_"))
            co_await calculator_handler(event);
        else {
            co_await event.co_reply(ir_deferred_update_message, ""); // do nothing on client side
            if (DEBUG)
                cerr << "Unhandled button click event: " << event.custom_id << endl;
        }

        });

    bot.on_select_click([](const select_click_t& event) -> task<void> {
        // do nothing
        co_await event.co_reply(ir_deferred_update_message, "");
        });

    if (DEBUG) {
        // nothing for now
    } else {
        auto now{ chrono::system_clock::now() };
        auto now_time_t{ chrono::system_clock::to_time_t(now) };
        tm* now_tm{ std::gmtime(&now_time_t) };

        // Set target time to 19:00 on the next Friday
        now_tm->tm_hour = 19;
        now_tm->tm_min = 0;
        now_tm->tm_sec = 0;
        now_tm->tm_wday = 5; // Friday (0 = Sunday, 5 = Friday)

        auto target_time{ chrono::system_clock::from_time_t(mktime(now_tm)) };
        if (target_time < now) // If the target time is in the past, move to the next Friday
            target_time += chrono::days(1);

        // seconds to target time
        auto seconds_to_target{ chrono::duration_cast<chrono::seconds>(target_time - now) };

        // Execute the poll scheduler initialization
        bot.start_timer(&create_polls, seconds_to_target.count(), &schedule_next_create);

        bot.start_timer(&process_poll_results,
            seconds_to_target.count() + 24 * 60 * 60, // 24 hours later
            &schedule_next_process);
    }

    bot.start(st_wait);
}
