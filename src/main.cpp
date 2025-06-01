#include <dpp/dpp.h>
#include <dpp/message.h>

#include "calculator.hpp"
#include "commands.hpp"
#include "global.hpp"
#include "scheduler.hpp"

void sigint_handler(int _) {
    cout << "\nSIGINT received, shutting down..." << endl;
    exit(0);
}

int main() {
    signal(SIGINT, sigint_handler);

    bot.on_log(utility::cout_logger());

    bot.on_slashcommand([](const slashcommand_t& event) -> task<void> {
        command_interaction cmd_data{event.command.get_command_interaction()};

        if (event.command.get_command_name() == "debug") {
            const auto& subcommand{cmd_data.options[0]};
            if (subcommand.name == "poll") {
                const auto& action{subcommand.options[0]};
                if (action.name == "start")
                    co_await start_debug_poll(event);
                else if (action.name == "end")
                    co_await end_debug_poll(event);
            } else if (subcommand.name == "calc") {
                co_await calculator_subcommand_handler(event, subcommand.options[0]);
            }
        }
    });

    bot.on_ready([](const ready_t& event) {
        if (run_once<struct register_bot_commands>()) {
            slashcommand debug_cmd{
                slashcommand("debug", "debug commands", bot.me.id)
                    .add_option(
                        // for testing polls
                        command_option(co_sub_command_group, "poll",
                                       "sect clash array sign up polls")
                            .add_option(command_option(co_sub_command, "start", "start a poll"))
                            .add_option(command_option(co_sub_command, "end", "end a poll")))
                    .add_option(calculator_commands())};

            bot.global_bulk_command_create({debug_cmd});
        }
    });

    bot.on_button_click([](const button_click_t& event) -> task<void> {
        co_await event.co_reply(ir_deferred_update_message, "");
        // if event.custom_id starts with "calc_", redirect to calculator handler
        if (event.custom_id.starts_with("calc_"))
            co_await calculator_button_click_handler(event);
        else {
            // do nothing on client side
            if (DEBUG) cerr << "Unhandled button click event: " << event.custom_id << endl;
        }
    });

    bot.on_select_click([](const select_click_t& event) -> task<void> {
        co_await event.co_reply(ir_deferred_update_message, "");
        // if event.custom_id starts with "calc_", redirect to calculator handler
        if (event.custom_id.starts_with("calc_"))
            co_await calculator_select_click_handler(event);
        else {
            // do nothing on client side
            if (DEBUG) cerr << "Unhandled select click event: " << event.custom_id << endl;
        }
    });

    bot.on_message_create([](const message_create_t& event) {
        // do nothing if message was sent by a bot
        if (event.msg.author.is_bot()) return;

        static emoji lotus_feet{"LotusFeet", 1357213347761885325};

        // // Count feet mentions
        int feet_count{0};

        string lowercase_no_space{
            string_view{event.msg.content} | views::filter([](const char& c) { return c != ' '; }) |
            views::transform([](const char& c) { return std::tolower(c); }) | ranges::to<string>()};

        size_t pos{0};
        while ((pos = lowercase_no_space.find("feet", pos)) != string::npos) {
            ++feet_count;
            pos += 4;  // Move past the word "feet"

            // check if consecutive characters are digits
            if (pos < lowercase_no_space.size() && std::isdigit(lowercase_no_space[pos])) {
                feet_count--; // Decrement feet_count if a number follows "feet"
                size_t end_pos{pos};
                while (end_pos < lowercase_no_space.size() &&
                       std::isdigit(lowercase_no_space[end_pos]))
                    ++end_pos;

                // Convert the number to an integer and add to feet_count
                feet_count += max(std::stoi(lowercase_no_space.substr(pos, end_pos - pos)),
                                  0);  // Use max() to prevent integer overflow
                pos = end_pos;         // Move past the number
            }
        }

        feet_count = min(feet_count, 20);

        if (feet_count > 0) {
            string response{};
            while (feet_count > 0) {
                response += lotus_feet.get_mention();
                --feet_count;
            }
            bot.message_create(message{event.msg.channel_id, response});
        }
    });

    if (DEBUG) {
        // nothing for now
    } else {
        auto now{chrono::system_clock::now()};
        auto now_time_t{chrono::system_clock::to_time_t(now)};
        tm* now_tm{std::gmtime(&now_time_t)};

        // Set target time to 19:00 on the next Friday
        now_tm->tm_hour = 19;
        now_tm->tm_min = 0;
        now_tm->tm_sec = 0;
        now_tm->tm_wday = 5;  // Friday (0 = Sunday, 5 = Friday)

        auto target_time{chrono::system_clock::from_time_t(mktime(now_tm))};
        if (target_time < now)  // If the target time is in the past, move to the next Friday
            target_time += chrono::days(1);

        // seconds to target time
        auto seconds_to_target{chrono::duration_cast<chrono::seconds>(target_time - now)};

        // Execute the poll scheduler initialization
        bot.start_timer(&create_polls, seconds_to_target.count(), &schedule_next_create);

        bot.start_timer(&process_poll_results,
                        seconds_to_target.count() + 24 * 60 * 60,  // 24 hours later
                        &schedule_next_process);
    }

    bot.start(st_wait);
}
