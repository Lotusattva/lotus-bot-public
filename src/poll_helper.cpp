#include "poll_helper.hpp"

#include <random>

void select(vector<string>& vec, unsigned limit) {
    if (vec.size() > limit) {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(vec.begin(), vec.end(), g);
        vec.resize(limit);
    }
}

task<optional<map<string, vector<string>>>> get_voters(const message& msg, const members_container& members) {
    map<string, vector<string>> role_selections{
        { ArrayRoleStr[MAGICAL_DRIVER], {} },
        { ArrayRoleStr[MAGICAL_PASSENGER], {} },
        { ArrayRoleStr[PHYSICAL_DRIVER], {} },
        { ArrayRoleStr[PHYSICAL_PASSENGER], {} },
    };

    const poll& poll{ msg.get_poll() };

    for (const auto& [_, answer] : poll.answers) {
        confirmation_callback_t callback{ co_await bot.co_poll_get_answer_voters(msg, answer.id, 0, 100) };
        if (callback.is_error()) {
            cerr << "Error: " << callback.get_error().message << endl;
            co_return{};
        }
        user_map voters{ callback.get<user_map>() };
        for (const auto& [_, user] : voters) {
            string name{ user.global_name };
            if (auto member{ members.find(user.id) }; member != members.end()) {
                string nickname{ member->second.get_nickname() };
                if (!nickname.empty()) name = nickname;
            }
            role_selections[answer.media.text].push_back(name);
        }

    }

    co_return role_selections;
}

void make_selections(map<string, vector<string>>& role_selections) {
    for (auto i{ 0 }; i < NUM_ARRAY_ROLES; ++i)
        select(role_selections[ArrayRoleStr[i]], ArrayRoleLimit[i]);
}

string print_role_selections(const map<string, vector<string>>& role_selections) {
    string result;
    for (const auto& [role, names] : role_selections)
        result += print_single_role_selection(role, names);
    return result;
}

string print_single_role_selection(const string& role, const vector<string>& vec) {
    string result{ "## " + role + "\n" };
    if (vec.empty())
        result += "*No one signed up for this role.*\n";
    else for (const auto& name : vec)
        result += "> - " + name + "\n";
    return result;
}