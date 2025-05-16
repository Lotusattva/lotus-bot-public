#ifndef POLL_HELPER_HPP
#define POLL_HELPER_HPP

#include "global.hpp"

task<optional<map<string_view, vector<string>>>> get_voters(const message& msg, const members_container& members);
void select(vector<string>& vec, unsigned limit);
void make_selections(map<string_view, vector<string>>& role_selections);
string print_role_selections(const map<string_view, vector<string>>& role_selections);
string print_single_role_selection(const string_view& role, const vector<string>& vec);

#endif // POLL_HELPER_HPP