#include "commands.hpp"
#include "poll_helper.hpp"
#include "calculator.hpp"

task<void> start_debug_poll(const slashcommand_t& event) {
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
            .add_answer(ARRAY_ROLE_STR[MAGICAL_DRIVER])
            .add_answer(ARRAY_ROLE_STR[MAGICAL_PASSENGER])
            .add_answer(ARRAY_ROLE_STR[PHYSICAL_DRIVER])
            .add_answer(ARRAY_ROLE_STR[PHYSICAL_PASSENGER])
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



task<void> end_debug_poll(const slashcommand_t& event) {
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

task<void> start_interactive_calculator(const slashcommand_t& event) {
    static component calc_overview_text{ component()
        .set_type(cot_text_display)
        .set_content(
            "This is a cultivation calculator that produces an estimate of "
            "the time it takes to breakthrough to the next major stage.\n\n"
            "I will walk you through a step-by-step process to collect the "
            "necessary in-game data to perform the calculation. Please grab "
            "your mobile device and launch the game. \n\n"
            "Click **NEXT** if you are ready proceed! "
            "Otherwise, click **CANCEL** to end this session."
        )
    };

    static component next_button{ component()
        .set_type(cot_button)
        .set_style(cos_primary)
        .set_label("NEXT")
        .set_id("calc_ask_stage")
    };

    static component action_row{ component()
        .set_type(cot_action_row)
        .add_component_v2(next_button)
        .add_component_v2(cancel_button)
    };

    static message msg{ message()
        .set_flags(m_using_components_v2)
        .add_component_v2(component()
            .set_type(cot_container)
            .add_component_v2(calc_overview_text)
            .add_component_v2(action_row)
        )
    };

    co_await event.co_reply(msg);

    // logs session
    snowflake user_id{ event.command.usr.id };

    if (DEBUG)
        cerr << "User ID: " << user_id << endl;

    confirmation_callback_t callback{ co_await event.co_get_original_response() };
    if (callback.is_error()) {
        cerr << "Error: " << callback.get_error().message << endl;
        co_return;
    }
    snowflake msg_id{ callback.get<message>().id };

    if (DEBUG)
        cerr << "Message ID: " << msg_id << endl;

    calc_sessions.insert({ user_id, make_pair(msg_id, calculator_client_t{}) });
    co_return;
}