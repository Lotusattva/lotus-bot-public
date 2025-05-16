#include "calculator.hpp"
#include "calculator_constants.hpp"

task<void> calculator_button_click_handler(const button_click_t& event) {
    if (!co_await verify_user(event))
        co_return;

    const string& id{ event.custom_id };

    if (DEBUG)
        cerr << "Handling calculator event: " << id << endl;

    if (id == CALC_EVENT_IDS[CALC_CANCEL])
        co_await calc_cancel(event);
    else if (id == CALC_EVENT_IDS[CALC_ASK_STAGE])
        co_await calc_ask_stage(event);
    else if (id == CALC_EVENT_IDS[CALC_ASK_PERCENT_PROGRESS])
        co_await calc_ask_percent_progress(event);
    else
        cerr << "Unhandled calculator event: " << id << endl;

    co_return;
}

task<void> calculator_select_click_handler(const select_click_t& event) {
    auto it{ co_await verify_user(event) };
    if (!it.has_value())
        co_return;

    calculator_client_t& client{ it.value()->second.second };

    if (event.custom_id == CALC_SELECT_IDS[CALC_SELECT_MAJOR_STAGE]) {
        if (DEBUG)
            cerr << "Major stage selected: " << event.values[0] << endl;

        client.major_stage = get_major_stage(event.values[0]);
    } else if (event.custom_id == CALC_SELECT_IDS[CALC_SELECT_MINOR_STAGE]) {
        if (DEBUG)
            cerr << "Minor stage selected: " << event.values[0] << endl;

        client.minor_stage = get_minor_stage(event.values[0]);
    } else
        cerr << "Unhandled select event: " << event.custom_id << endl;

    co_return;
}

template<derived_from<interaction_create_t> T>
task<optional<unordered_map<snowflake, pair<snowflake, calculator_client_t>>::iterator>> verify_user(const T& event) {
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

    if (it->second.first != msg_id) {
        if (DEBUG)
            cerr << "User ID: " << user_id << " is not the owner of this session." << endl;
        co_return nullopt;
    }

    if (DEBUG)
        cerr << "User ID: " << user_id << " is the owner of this session." << endl;
    co_return it;
}

task<void> calc_cancel(const button_click_t& event) {
    if (DEBUG)
        cerr << "Canceling calculator session..." << endl;

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

    calc_sessions.erase(event.command.usr.id);
    co_return;
}

component major_stage_selectmenu_factory() {
    component major_stage_selectmenu{ component()
        .set_type(cot_selectmenu)
        .set_id(CALC_SELECT_IDS[CALC_SELECT_MAJOR_STAGE])
        .set_placeholder("Select your major stage")
        .set_required(true)
    };

    for (size_t i{ 0 }; i < NUM_MAJOR_STAGES; ++i)
        major_stage_selectmenu.add_select_option(select_option(MAJOR_STAGE_STR[i], MAJOR_STAGE_STR[i], ""));

    return major_stage_selectmenu;
}

component minor_stage_selectmenu_factory() {
    component minor_stage_selectmenu{ component()
        .set_type(cot_selectmenu)
        .set_id(CALC_SELECT_IDS[CALC_SELECT_MINOR_STAGE])
        .set_placeholder("Select your minor stage")
        .set_required(true)
    };

    for (size_t i{ 0 }; i < NUM_MINOR_STAGES; ++i)
        minor_stage_selectmenu.add_select_option(select_option(MINOR_STAGE_STR[i], MINOR_STAGE_STR[i], ""));

    return minor_stage_selectmenu;
}

task<void> calc_ask_stage(const button_click_t& event) {
    if (DEBUG)
        cerr << "Asking for cultivation stage..." << endl;

    static component text_display{ component()
        .set_type(cot_text_display)
        .set_content("Please select your major and minor stage")
    };

    static component major_stage_selectmenu{ component()
        .set_type(cot_action_row)
        .add_component_v2(major_stage_selectmenu_factory()) };
        
    static component minor_stage_selectmenu{ component()
        .set_type(cot_action_row)
        .add_component_v2(minor_stage_selectmenu_factory()) };

    static component next_button{ component()
        .set_type(cot_button)
        .set_style(cos_primary)
        .set_label("NEXT")
        .set_id("calc_ask_percent_progress")
    };

    static message calc_ask_stage_message{ message()
        .set_flags(m_using_components_v2)
        .add_component_v2(component()
            // a container
            .set_type(cot_container)
            // ...with a text display
            .add_component_v2(text_display)
            .add_component_v2(major_stage_selectmenu)
            .add_component_v2(minor_stage_selectmenu)
            .add_component_v2(component()
                .set_type(cot_action_row)
                .add_component_v2(next_button)
                .add_component_v2(cancel_button)
            )
        )
    };

    if (DEBUG)
        cerr << "Sending cultivation stage selection message..." << endl;

    confirmation_callback_t confirmation{ co_await event.co_edit_response(calc_ask_stage_message) };

    if (confirmation.is_error()) {
        cerr << "Error: " << confirmation.get_error().message << endl;
        co_return;
    }

    co_return;
}

task<void> calc_ask_percent_progress(const button_click_t& event) {
    if (DEBUG)
        cerr << "Asking for percent progress..." << endl;

    // TODO

    co_return;
}