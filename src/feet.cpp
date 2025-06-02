#include "feet.hpp"

void feet(const message_create_t& event) {
        // do nothing if message was sent by a bot
        if (event.msg.author.is_bot()) return;

        static emoji lotus_feet{"LotusFeet", 1357213347761885325};

        // // Count feet mentions
        int feet_count{0};

        string lowercase_no_space{
            string_view{event.msg.content} | views::filter([](const char& c) { return !(c == ' ' || c == '\n'); }) |
            views::transform([](const char& c) { return std::tolower(c); }) | ranges::to<string>()};

        size_t pos{0};
        while ((pos = lowercase_no_space.find("feet", pos)) != string::npos) {
            ++feet_count;
            pos += 4;  // Move past the word "feet"

            // check if consecutive characters are digits
            if (pos < lowercase_no_space.size() && std::isdigit(lowercase_no_space[pos])) {
                feet_count--; // Decrement feet_count if a number follows "feet"
                size_t end_pos{pos};
                while (end_pos < lowercase_no_space.size() &&
                       std::isdigit(lowercase_no_space[end_pos]))
                    ++end_pos;

                // Convert the number to an integer and add to feet_count
                feet_count += max(std::stoi(lowercase_no_space.substr(pos, end_pos - pos)),
                                  0);  // Use max() to prevent integer overflow
                pos = end_pos;         // Move past the number
            }
        }

        feet_count = min(feet_count, 20);

        if (feet_count > 0) {
            if (DEBUG)
                cerr << "Printing " << feet_count << " feet in response to message: "<< event.msg.content << endl;
            
            string response{};
            while (feet_count > 0) {
                response += lotus_feet.get_mention();
                --feet_count;
            }
            bot.message_create(message{event.msg.channel_id, response});
            
        }
    }