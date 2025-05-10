#include "calculator.hpp"

task<optional<unordered_map<snowflake, snowflake>::iterator>> verify_user(const button_click_t& event) {
    if (__DEBUG_MODE__)
        cerr << "Verifying user..." << endl;

    snowflake user_id{ event.command.usr.id };
    auto it{ calc_sessions.find(user_id) };
    if (it == calc_sessions.end()) {
        if (__DEBUG_MODE__)
            cerr << "User ID: " << user_id << " is not the owner of this session." << endl;
        co_return nullopt;
    }

    if (__DEBUG_MODE__)
        cerr << "querying message ID..." << endl;

    confirmation_callback_t callback{ co_await event.co_get_original_response() };
    if (callback.is_error()) {
        cerr << "Error: " << callback.get_error().message << endl;
        co_return nullopt;
    }
    snowflake msg_id{ callback.get<message>().id };

    if (__DEBUG_MODE__)
        cerr << "Message ID: " << msg_id << endl;

    if (it->second != msg_id) {
        if (__DEBUG_MODE__)
            cerr << "User ID: " << user_id << " is not the owner of this session." << endl;
        co_return nullopt;
    }

    if (__DEBUG_MODE__)
        cerr << "User ID: " << user_id << " is the owner of this session." << endl;
    co_return it;
}

task<void> cancel_calc(const button_click_t& event) {
    co_await event.co_reply(ir_deferred_update_message, "");
    
    if (__DEBUG_MODE__)
        cerr << "Canceling calculator session..." << endl;

    optional<unordered_map<snowflake, snowflake>::iterator> it{ co_await verify_user(event) };

    if (!it.has_value())
        co_return;

    if (__DEBUG_MODE__)
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
            // ... and an action row
            .add_component_v2(component()
                .set_type(cot_action_row)
                // ... that contains a button
                .add_component_v2(component()
                    .set_type(cot_button)
                    .set_label("DELETE MESSAGE")
                    .set_id("delete_calc_message")
                    .set_style(cos_secondary)
                )
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