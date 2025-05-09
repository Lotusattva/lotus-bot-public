#include "commands.hpp"
#include "poll_helper.hpp"

task<void> debug_poll_start_command(const slashcommand_t& event) {
    // remove finalized polls
    erase_if(debug_polls, [](const auto& pair) {
        auto results{ pair.second.get_poll().results };
        return results && results->is_finalized;
        });

    const snowflake& channel_id{ event.command.channel_id };
    if (debug_polls.find(channel_id) != debug_polls.end()) {
        co_await event.co_reply("A poll is already running in this channel.");
        co_return;
    }

    static const message POLL{
        message().set_poll(poll()
            .set_question("Sign up here for arrays")
            .add_answer(ArrayRoleStr[MAGICAL_DRIVER])
            .add_answer(ArrayRoleStr[MAGICAL_PASSENGER])
            .add_answer(ArrayRoleStr[PHYSICAL_DRIVER])
            .add_answer(ArrayRoleStr[PHYSICAL_PASSENGER])
            .set_duration(24)) };

    co_await event.co_reply(POLL);

    confirmation_callback_t callback{ co_await event.co_get_original_response() };
    if (callback.is_error()) {
        cerr << "Error: " << callback.get_error().message << endl;
        co_return;
    }
    const message& msg{ callback.get<message>() };
    debug_polls[msg.channel_id] = msg;
    co_return;
}



task<void> debug_poll_end_command(const slashcommand_t& event) {
    const snowflake& channel_id{ event.command.channel_id };
    const members_container& members{ event.command.get_guild().members };


    auto it{ debug_polls.find(channel_id) };
    if (it == debug_polls.end()) {
        co_await event.co_reply("No poll is running in this channel.");
        co_return;
    }
    if (auto results{ it->second.get_poll().results }; results && results->is_finalized) {
        debug_polls.erase(it);
        co_await event.co_reply("No poll is running in this channel.");
        co_return;
    }

    message msg{ it->second };

    dpp::async thinking = event.co_thinking();

    confirmation_callback_t callback{ co_await bot.co_poll_end(msg) };
    if (callback.is_error()) {
        cerr << "Poll failed to end. Error: " << callback.get_error().message << endl;
        co_await thinking;
        event.edit_original_response(message {"Poll failed to end."});
        co_return;
    }

    debug_polls.erase(it);

    msg = callback.get<message>();
    optional<map<string, vector<string>>> voters{ co_await get_voters(msg, members) };
    if (!voters.has_value()) {
        co_await thinking;
        event.edit_original_response(message {"Could not fetch poll results."});
        co_return;
    }

    map<string, vector<string>> role_selections{ voters.value() };
    make_selections(role_selections);
    string post{ "# Selections: \n" + print_role_selections(role_selections) };
    co_await thinking;
    event.edit_original_response(message { post });
}


