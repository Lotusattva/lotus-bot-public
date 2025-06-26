#include <dpp/dpp.h>
#include <dpp/message.h>

#include "calculator/calculator.hpp"
#include "calculator/calculator_constants.hpp"
#include "feet.hpp"
#include "global.hpp"
#include "poll.hpp"
#include "scheduler.hpp"

void sigint_handler(int _) {
    cout << "\nSIGINT received, shutting down..." << endl;
    exit(0);
}

// int main() {
//     signal(SIGINT, sigint_handler);

//     bot.on_log(utility::cout_logger());

//     bot.on_slashcommand([](const slashcommand_t& event) -> task<void> {
//         const command_interaction cmd_interaction{event.command.get_command_interaction()};

//         if (event.command.get_command_name() == "debug") {
//             const command_data_option& options{cmd_interaction.options[0]};
//             if (options.name == "poll")
//                 co_await poll_subcommand_handler(event, options);
//             else if (options.name == "calc")
//                 co_await calculator_subcommand_handler(event, options);
//         }
//     });

//     bot.on_ready([](const ready_t& event) {
//         if (run_once<struct register_bot_commands>()) {
//             slashcommand debug_cmd{slashcommand("debug", "debug commands", bot.me.id)
//                                        .add_option(poll_commands())
//                                        .add_option(calculator_commands())};

//             bot.global_bulk_command_create({debug_cmd});
//         }
//     });

//     bot.on_button_click([](const button_click_t& event) -> task<void> {
//         co_await event.co_reply(ir_deferred_update_message, "");
//         // if event.custom_id starts with "calc_", redirect to calculator handler
//         if (event.custom_id.starts_with("calc_"))
//             co_await calculator_button_click_handler(event);
//         else {
//             // do nothing on client side
//             if (DEBUG) cerr << "Unhandled button click event: " << event.custom_id << endl;
//         }
//     });

//     bot.on_select_click([](const select_click_t& event) -> task<void> {
//         co_await event.co_reply(ir_deferred_update_message, "");
//         // if event.custom_id starts with "calc_", redirect to calculator handler
//         if (event.custom_id.starts_with("calc_"))
//             co_await calculator_select_click_handler(event);
//         else {
//             // do nothing on client side
//             if (DEBUG) cerr << "Unhandled select click event: " << event.custom_id << endl;
//         }
//     });

//     bot.on_message_create(&feet);

//     if (DEBUG) {
//         // nothing for now
//     } else {
//         auto now{chrono::system_clock::now()};
//         auto now_time_t{chrono::system_clock::to_time_t(now)};
//         tm* now_tm{std::gmtime(&now_time_t)};

//         // Set target time to 19:00 on the next Friday
//         now_tm->tm_hour = 19;
//         now_tm->tm_min = 0;
//         now_tm->tm_sec = 0;
//         now_tm->tm_wday = 5;  // Friday (0 = Sunday, 5 = Friday)

//         auto target_time{chrono::system_clock::from_time_t(mktime(now_tm))};
//         if (target_time < now)  // If the target time is in the past, move to the next Friday
//             target_time += chrono::days(1);

//         // seconds to target time
//         auto seconds_to_target{chrono::duration_cast<chrono::seconds>(target_time - now)};

//         // Execute the poll scheduler initialization
//         bot.start_timer(&create_polls, seconds_to_target.count(), &schedule_next_create);

//         bot.start_timer(&process_poll_results,
//                         seconds_to_target.count() + 24 * 60 * 60,  // 24 hours later
//                         &schedule_next_process);
//     }

//     bot.start(st_wait);
// }


int main() {
    calculator_client_t client;
    client.major_stage = WHOLENESS;
    client.minor_stage = LATE;
    client.extractor_quality = MYTHIC;
    client.extractor_major_stage_bonus = YES;
    client.node_levels[CULTIXP_NODE] = 30;
    client.node_levels[QUALITY_NODE] = 30;
    client.node_levels[GUSH_NODE] = 30;

    cout << "Start calculation" << endl;
    double fruit_result {calculate_myrimon_fruit_exp(client)};
    if (fruit_result) {
        cout << "Myrimon fruit exp (no gush): " << fruit_result << endl;
    }
}