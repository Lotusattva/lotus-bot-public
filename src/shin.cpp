#include "shin.hpp"

#include <string>

#include "global.hpp"

using namespace std;

void shin(const message_create_t &event) {
    if (event.msg.author.is_bot()) return;
    if (event.msg.content.contains("Shin") || event.msg.content.contains("shin"))
        bot.message_create(message{event.msg.channel_id, "Shin is gae"});
}