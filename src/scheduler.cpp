#include "scheduler.hpp"
#include "poll_helper.hpp"

task<void> create_polls(timer _) {
    static message POLL{
       message().set_poll(poll()
           .set_question("Sign up here for arrays")
           .add_answer(ArrayRoleStr[MAGICAL_DRIVER])
           .add_answer(ArrayRoleStr[MAGICAL_PASSENGER])
           .add_answer(ArrayRoleStr[PHYSICAL_DRIVER])
           .add_answer(ArrayRoleStr[PHYSICAL_PASSENGER])
           .set_duration(__DEBUG_MODE__ ? 1 : 24)) };

    for (auto i{ 0 }; i < MANAGED_GUILDS; ++i) {
        message my_poll{ POLL };

        if (__DEBUG_MODE__)
            cerr << "Creating poll for channel: " << CHANNEL_IDS[i] << endl;

        my_poll.set_channel_id(CHANNEL_IDS[i]);
        my_poll.set_guild_id(GUILD_IDS[i]);
        confirmation_callback_t confirmation{ co_await bot.co_message_create(my_poll) };
        if (confirmation.is_error()) {
            cerr << "Error: " << confirmation.get_error().message << endl;
            continue;
        }
        polls[CHANNEL_IDS[i]] = confirmation.get<message>();
    }

    co_return;
}

task<void> process_poll_results(timer _) {
    for (const auto& [_, msg] : polls) {
        confirmation_callback_t confirmation{ co_await bot.co_poll_end(msg) };
        if (confirmation.is_error()) {
            cerr << "Error: " << confirmation.get_error().message << endl;
            continue;
        }

        confirmation = co_await bot.co_guild_get_members(msg.guild_id, 1000, 0);
        guild_member_map members;
        if (confirmation.is_error())
            cerr << "Error: " << confirmation.get_error().message << endl;
        else
            members = confirmation.get<guild_member_map>();

        optional<map<string, vector<string>>> voters{ co_await get_voters(msg, members) };
        if (!voters.has_value()) {
            cerr << "Error: " << confirmation.get_error().message << endl;
            message error_msg{ message().set_content("Failed to fetch poll results.").set_channel_id(msg.channel_id) };
            co_await bot.co_message_create(error_msg);
            continue;
        }

        map<string, vector<string>> role_selections{ voters.value() };
        make_selections(role_selections);
        string post{ "# Selections: \n" + print_role_selections(role_selections) };
        message result_msg{ message().set_content(post).set_channel_id(msg.channel_id) };
        co_await bot.co_message_create(result_msg);
    }

    co_return;
}

task<void> schedule_next_create(timer _) {
    if (__DEBUG_MODE__)
        bot.start_timer(&create_polls, 60, &schedule_next_create);
    else
        bot.start_timer(&create_polls, SECONDS_IN_WEEK, &schedule_next_create);

    co_return;
}

task<void> schedule_next_process(timer _) {
    if (__DEBUG_MODE__)
        bot.start_timer(&process_poll_results, 60, &schedule_next_process);
    else
        bot.start_timer(&process_poll_results, SECONDS_IN_WEEK, &schedule_next_process);

    co_return;
}

task<void> mock_reminder(timer _) {
    for (auto i{ 0 }; i < MANAGED_GUILDS; ++i) {
        message reminder{ message()
            .set_content("This is a mock reminder to test the automated scheduler.")
            .set_channel_id(CHANNEL_IDS[i]) };

        confirmation_callback_t confirmation{ co_await bot.co_message_create(reminder) };
        if (confirmation.is_error()) {
            cerr << "Error: " << confirmation.get_error().message << endl;
            continue;
        }
    }

    co_return;
}

task<void> schedule_next_mock_reminder(timer _) {
    bot.start_timer(&mock_reminder, 60, &schedule_next_mock_reminder);

    co_return;
}