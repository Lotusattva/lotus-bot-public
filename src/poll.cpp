#include "poll.hpp"
#include <dpp/appcommand.h>

#include <random>

command_option poll_commands() {
    return command_option(co_sub_command_group, "poll", "sect clash array sign up polls")
        .add_option(command_option(co_sub_command, "start", "start a poll"))
        .add_option(command_option(co_sub_command, "end", "end a poll"));
}

task<void> poll_subcommand_handler(const slashcommand_t& event,
                                   const command_data_option& options) {
    const command_data_option& subcommand{options.options[0]};

    if (subcommand.name == "start")
        co_await start_debug_poll(event);
    else if (subcommand.name == "end")
        co_await end_debug_poll(event);

    co_return;
}

task<void> start_debug_poll(const slashcommand_t& event) {
    // remove finalized polls
    erase_if(debug_polls, [](const auto& pair) {
        auto results{pair.second.get_poll().results};
        return results && results->is_finalized;
    });

    const snowflake& channel_id{event.command.channel_id};
    if (debug_polls.find(channel_id) != debug_polls.end()) {
        co_await event.co_reply("A poll is already running in this channel.");
        co_return;
    }

    static const message POLL{message().set_poll(poll()
                                                     .set_question("Sign up here for arrays")
                                                     .add_answer(ARRAY_ROLE_STR[MAGICAL_DRIVER])
                                                     .add_answer(ARRAY_ROLE_STR[MAGICAL_PASSENGER])
                                                     .add_answer(ARRAY_ROLE_STR[PHYSICAL_DRIVER])
                                                     .add_answer(ARRAY_ROLE_STR[PHYSICAL_PASSENGER])
                                                     .set_duration(24))};

    co_await event.co_reply(POLL);

    confirmation_callback_t callback{co_await event.co_get_original_response()};
    if (callback.is_error()) {
        cerr << "Error: " << callback.get_error().message << endl;
        co_return;
    }
    const message& msg{callback.get<message>()};
    debug_polls[msg.channel_id] = msg;
    co_return;
}

task<void> end_debug_poll(const slashcommand_t& event) {
    const snowflake& channel_id{event.command.channel_id};
    const members_container& members{event.command.get_guild().members};

    auto it{debug_polls.find(channel_id)};
    if (it == debug_polls.end()) {
        co_await event.co_reply("No poll is running in this channel.");
        co_return;
    }
    if (auto results{it->second.get_poll().results}; results && results->is_finalized) {
        debug_polls.erase(it);
        co_await event.co_reply("No poll is running in this channel.");
        co_return;
    }

    message msg{it->second};

    dpp::async thinking = event.co_thinking();

    confirmation_callback_t callback{co_await bot.co_poll_end(msg)};
    if (callback.is_error()) {
        cerr << "Poll failed to end. Error: " << callback.get_error().message << endl;
        co_await thinking;
        event.edit_original_response(message{"Poll failed to end."});
        co_return;
    }

    debug_polls.erase(it);

    msg = callback.get<message>();
    optional<map<string_view, vector<string>>> voters{co_await get_voters(msg, members)};
    if (!voters.has_value()) {
        co_await thinking;
        event.edit_original_response(message{"Could not fetch poll results."});
        co_return;
    }

    map<string_view, vector<string>> role_selections{voters.value()};
    make_selections(role_selections);
    string post{"# Selections: \n" + print_role_selections(role_selections)};
    co_await thinking;
    event.edit_original_response(message{post});
}

void select(vector<string>& vec, unsigned limit) {
    if (vec.size() > limit) {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(vec.begin(), vec.end(), g);
        vec.resize(limit);
    }
}

task<optional<map<string_view, vector<string>>>> get_voters(const message& msg,
                                                            const members_container& members) {
    map<string_view, vector<string>> role_selections{
        {ARRAY_ROLE_STR[MAGICAL_DRIVER], {}},
        {ARRAY_ROLE_STR[MAGICAL_PASSENGER], {}},
        {ARRAY_ROLE_STR[PHYSICAL_DRIVER], {}},
        {ARRAY_ROLE_STR[PHYSICAL_PASSENGER], {}},
    };

    const poll& poll{msg.get_poll()};

    for (const auto& [_, answer] : poll.answers) {
        confirmation_callback_t callback{
            co_await bot.co_poll_get_answer_voters(msg, answer.id, 0, 100)};
        if (callback.is_error()) {
            cerr << "Error: " << callback.get_error().message << endl;
            co_return {};
        }
        user_map voters{callback.get<user_map>()};
        for (const auto& [_, user] : voters) {
            string name{user.global_name};
            if (auto member{members.find(user.id)}; member != members.end()) {
                string nickname{member->second.get_nickname()};
                if (!nickname.empty()) name = nickname;
            }
            role_selections[answer.media.text].push_back(name);
        }
    }

    co_return role_selections;
}

void make_selections(map<string_view, vector<string>>& role_selections) {
    for (auto i{0}; i < NUM_ARRAY_ROLES; ++i)
        select(role_selections[ARRAY_ROLE_STR[i]], ARRAY_ROLE_LIMIT[i]);
}

string print_role_selections(const map<string_view, vector<string>>& role_selections) {
    string result;
    for (const auto& [role, names] : role_selections)
        result += print_single_role_selection(role, names);
    return result;
}

string print_single_role_selection(const string_view& role, const vector<string>& vec) {
    string result{"## " + string{role} + "\n"};
    if (vec.empty())
        result += "*No one signed up for this role.*\n";
    else
        for (const auto& name : vec) result += "> - " + name + "\n";
    return result;
}