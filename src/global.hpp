#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include <dpp/dpp.h>

constexpr inline bool DEBUG = true;

using namespace std;
using namespace dpp;

inline const char* BOT_TOKEN{ getenv("BOT_TOKEN") };

inline cluster bot(BOT_TOKEN, intents::i_all_intents); // global bot instance

inline message_map polls; // global poll message storage
inline message_map debug_polls; // global debug poll message storage

/**
 * Represents ownership of calculator sessions
 * maps user-id to message-id
 */
inline unordered_map<snowflake, snowflake> calc_sessions; 

#define NUM_ARRAY_ROLES 4

enum ArrayRole {
    MAGICAL_DRIVER = 0,
    MAGICAL_PASSENGER = 1,
    PHYSICAL_DRIVER = 2,
    PHYSICAL_PASSENGER = 3,
};

constexpr inline const char* const ArrayRoleStr[]{
    "Magical driver",
    "Magical passenger",
    "Physical driver",
    "Physical passenger",
};

constexpr inline const unsigned ArrayRoleLimit[]{
    2,
    8,
    1,
    4,
};

#define MANAGED_GUILDS 2

constexpr inline const snowflake GUILD_IDS[MANAGED_GUILDS]{
    1331102316018929705, // Abyss
    1188528789928554596, // Hermitage
};

// TODO: change channel IDs later
constexpr inline const snowflake CHANNEL_IDS[MANAGED_GUILDS]{
    1366138167958306816, // Abyss
    1365845286181802034, // Hermitage
};

#endif // GLOBAL_HPP