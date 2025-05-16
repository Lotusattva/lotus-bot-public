#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "global.hpp"

task<void> start_debug_poll(const slashcommand_t& event);

task<void> end_debug_poll(const slashcommand_t& event);

#endif // COMMANDS_HPP