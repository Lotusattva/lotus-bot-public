#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "global.hpp"

task<void> debug_poll_start_command(const slashcommand_t& event);

task<void> debug_poll_end_command(const slashcommand_t& event);

#endif // COMMANDS_HPP