#ifndef POLL_HPP
#define POLL_HPP

#include "global.hpp"

command_option poll_commands();

task<void> poll_subcommand_handler(const slashcommand_t& event, const command_data_option& options);

task<void> start_debug_poll(const slashcommand_t& event);

task<void> end_debug_poll(const slashcommand_t& event);

task<optional<map<string_view, vector<string>>>> get_voters(const message& msg,
                                                            const members_container& members);
void select(vector<string>& vec, unsigned limit);

void make_selections(map<string_view, vector<string>>& role_selections);

string print_role_selections(const map<string_view, vector<string>>& role_selections);

string print_single_role_selection(const string_view& role, const vector<string>& vec);

#endif  // POLL_HPP