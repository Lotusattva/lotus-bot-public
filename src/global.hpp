#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include <dpp/dpp.h>

#define DEBUG false

using namespace std;
using namespace dpp;

inline string BOT_TOKEN{getenv("BOT_TOKEN")};

inline cluster bot(BOT_TOKEN, intents::i_all_intents);  // global bot instance

inline message_map polls;        // global poll message storage
inline message_map debug_polls;  // global debug poll message storage

enum ArrayRole {
    MAGICAL_DRIVER,
    MAGICAL_PASSENGER,
    PHYSICAL_DRIVER,
    PHYSICAL_PASSENGER,

    NUM_ARRAY_ROLES
};

constexpr inline string_view const ARRAY_ROLE_STR[]{
    "Magical driver",
    "Magical passenger",
    "Physical driver",
    "Physical passenger",
};

constexpr inline const unsigned ARRAY_ROLE_LIMIT[]{
    2,
    8,
    1,
    4,
};

enum Guild {
    ABYSS,
    HERMITAGE,

    NUM_GUILDS
};

constexpr inline const snowflake GUILD_IDS[NUM_GUILDS]{
    1331102316018929705,  // Abyss
    1188528789928554596,  // Hermitage
};

// TODO: change channel IDs later
constexpr inline const snowflake CHANNEL_IDS[NUM_GUILDS]{
    1366138167958306816,  // Abyss
    1365845286181802034,  // Hermitage
};

#endif  // GLOBAL_HPP