#include "feet.hpp"

#include <string>

#include "global.hpp"

using namespace std;

void feet(const message_create_t& event) {
    // do nothing if message was sent by a bot
    if (event.msg.author.is_bot()) return;

    static emoji lotus_feet{"LotusFeet", 1357213347761885325};

    // // Count feet mentions
    int feet_count{0};

    string lowercase_no_space{
        string_view{event.msg.content} |
        views::filter([](const char& c) { return !(c == ' ' || c == '\n'); }) |
        views::transform([](const char& c) { return std::tolower(c); }) | ranges::to<string>()};

    size_t pos{0};
    size_t left_bound{0};

    // find all occurrences of "feet" or "foot" in the message
    while ((pos = min(lowercase_no_space.find("feet", pos),
                      lowercase_no_space.find("foot", pos))) != string::npos) {
        bool digit_found{false};

        // Check if preceding characters are digits
        if (pos > 0 && std::isdigit(lowercase_no_space[pos - 1]) && pos > left_bound) {
            // If a digit precedes "feet" or "foot", decrement feet_count
            digit_found = true;
            size_t start_pos{pos - 1};  // start position of the number
            while (start_pos > 0 && std::isdigit(lowercase_no_space[start_pos - 1])) --start_pos;

            feet_count += max(std::stoi(lowercase_no_space.substr(start_pos, pos - start_pos)),
                              0);  // Use max() to prevent integer overflow
        }

        pos += 4;  // Move past the word "feet" or "foot"

        // check if consecutive characters are digits
        if (!digit_found && pos < lowercase_no_space.size() &&
            std::isdigit(lowercase_no_space[pos])) {
            digit_found = true;

            size_t end_pos{pos};  // end position of the number
            while (end_pos < lowercase_no_space.size() && std::isdigit(lowercase_no_space[end_pos]))
                ++end_pos;

            // Convert the number to an integer and add to feet_count
            feet_count += max(std::stoi(lowercase_no_space.substr(pos, end_pos - pos)),
                              0);  // Use max() to prevent integer overflow
            pos = end_pos;         // Move past the number
            left_bound = pos;      // Update left_bound to the end of the number
        }

        if (!digit_found)
            // If no digit was found before "feet" or "foot", increment feet_count by 1
            ++feet_count;
    }

    feet_count = min(feet_count, 20);

    if (feet_count > 0) {
        if (DEBUG)
            cerr << "Printing " << feet_count
                 << " feet in response to message: " << event.msg.content << endl;

        string response{};
        while (feet_count > 0) {
            response += lotus_feet.get_mention();
            --feet_count;
        }
        bot.message_create(message{event.msg.channel_id, response});
    }
}