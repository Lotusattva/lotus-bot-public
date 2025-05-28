#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include "global.hpp"

/**
 * @brief Create polls in all managed channels
 */
task<void> create_polls(timer _);

/**
 * @brief Process poll results and send the final selections to the channel.
 */
task<void> process_poll_results(timer _);

constexpr inline const uint64_t SECONDS_IN_WEEK{7 * 24 * 60 * 60};

task<void> schedule_next_create(timer _);

task<void> schedule_next_process(timer _);

task<void> mock_reminder(timer _);

task<void> schedule_next_mock_reminder(timer _);

#endif // SCHEDULER_HPP