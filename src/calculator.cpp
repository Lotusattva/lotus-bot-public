#include "calculator.hpp"

task<optional<unordered_map<snowflake, snowflake>::iterator>> verify_user(const button_click_t& event) {
    if (DEBUG)
        cerr << "Verifying user..." << endl;

    snowflake user_id{ event.command.usr.id };
    auto it{ calc_sessions.find(user_id) };
    if (it == calc_sessions.end()) {
        if (DEBUG)
            cerr << "User ID: " << user_id << " is not the owner of this session." << endl;
        co_return nullopt;
    }

    if (DEBUG)
        cerr << "querying message ID..." << endl;

    confirmation_callback_t callback{ co_await event.co_get_original_response() };
    if (callback.is_error()) {
        cerr << "Error: " << callback.get_error().message << endl;
        co_return nullopt;
    }
    snowflake msg_id{ callback.get<message>().id };

    if (DEBUG)
        cerr << "Message ID: " << msg_id << endl;

    if (it->second != msg_id) {
        if (DEBUG)
            cerr << "User ID: " << user_id << " is not the owner of this session." << endl;
        co_return nullopt;
    }

    if (DEBUG)
        cerr << "User ID: " << user_id << " is the owner of this session." << endl;
    co_return it;
}

task<void> cancel_calc(const button_click_t& event) {
    co_await event.co_reply(ir_deferred_update_message, "");

    if (DEBUG)
        cerr << "Canceling calculator session..." << endl;

    optional<unordered_map<snowflake, snowflake>::iterator> it{ co_await verify_user(event) };

    if (!it.has_value())
        co_return;

    if (DEBUG)
        cerr << "Owner verified, removing session..." << endl;

    static message session_cancel_message{ message()
        .set_flags(m_using_components_v2)
        .add_component_v2(component()
            // a container
            .set_type(cot_container)
            // ...with a text display
            .add_component_v2(component()
                .set_type(cot_text_display)
                .set_content("You have cancelled the session.")
            )
        )
    };

    co_await event.co_edit_response(session_cancel_message);

    calc_sessions.erase(it.value());
    co_return;
}

task<void> ask_stage(const button_click_t& event) {
    // do nothing for now
    co_await event.co_reply(ir_pong, "");
}