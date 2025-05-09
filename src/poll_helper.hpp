#ifndef POLL_HELPER_HPP
#define POLL_HELPER_HPP

#include "global.hpp"

task<optional<map<string, vector<string>>>> get_voters(const message& msg, const members_container& members);
void select(vector<string>& vec, unsigned limit);
void make_selections(map<string, vector<string>>& role_selections);
string print_role_selections(const map<string, vector<string>>& role_selections);
string print_single_role_selection(const string& role, const vector<string>& vec);

#endif // POLL_HELPER_HPP