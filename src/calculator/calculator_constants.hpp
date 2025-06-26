#ifndef CALCULATOR_CONSTANTS_HPP
#define CALCULATOR_CONSTANTS_HPP

#include <array>
#include <cassert>
#include <chrono>
#include <cstdint>
#include <numeric>
#include <string_view>

using namespace std;

////////////// Utility functions //////////////

// Compile-time string hash
constexpr uint64_t hash_string(const string_view &str) {
    uint64_t hash = 5381;
    for (char c : str) {
        hash = ((hash << 5) + hash) + static_cast<unsigned char>(c);
    }
    return hash;
}

/////////////////// Item rarities ////////////////

enum quality_t {
    COMMON,
    UNCOMMON,
    RARE,
    EPIC,
    LEGENDARY,
    MYTHIC,

    NUM_QUALITIES
};

// constexpr inline string_view QUALITY_STR[NUM_QUALITIES]
constexpr inline array<string_view, NUM_QUALITIES> QUALITY_STR{"Common", "Uncommon",  "Rare",
                                                               "Epic",   "Legendary", "Mythic"};

constexpr quality_t get_quality(const string_view &name) {
    const uint64_t hash{hash_string(name)};

    switch (hash) {
        case hash_string(QUALITY_STR[COMMON]):
            return COMMON;
        case hash_string(QUALITY_STR[UNCOMMON]):
            return UNCOMMON;
        case hash_string(QUALITY_STR[RARE]):
            return RARE;
        case hash_string(QUALITY_STR[EPIC]):
            return EPIC;
        case hash_string(QUALITY_STR[LEGENDARY]):
            return LEGENDARY;
        case hash_string(QUALITY_STR[MYTHIC]):
            return MYTHIC;
        default:
            return NUM_QUALITIES;  // An invalid quality, indicates an error
    }
}

/////////////////// Cosmosapsis ///////////////////

// one cosmosapsis is 8 seconds
constexpr inline chrono::seconds COSMOSAPSIS{8};
constexpr inline int NUM_COSMOSAPSIS_PER_HOUR{3600 / COSMOSAPSIS.count()};

///////////////// Cultivation stages and exp requirements /////////////////

enum major_stage_t {

    /* Exclude the following two outlier stages */
    // NOVICE,
    // CONNECTION,
    /* Most people get through these stages within a couple hours anyway */

    FOUNDATION,
    VIRTUOSO,
    NASCENT,
    INCARNATION,
    VOIDBREAK,
    WHOLENESS,
    PERFECTION,
    NIRVANA,
    CELESTIAL,
    ETERNAL,

    NUM_MAJOR_STAGES
};

// constexpr inline string_view MAJOR_STAGE_STR[NUM_MAJOR_STAGES]
constexpr inline array<string_view, NUM_MAJOR_STAGES> MAJOR_STAGE_STR{
    "Foundation", "Virtuoso",   "Nascence", "Incarnation", "Voidbreak",
    "Wholeness",  "Perfection", "Nirvana",  "Celestial",   "Eternal"};

constexpr major_stage_t get_major_stage(const string_view &name) {
    const uint64_t hash{hash_string(name)};

    switch (hash) {
        case hash_string(MAJOR_STAGE_STR[FOUNDATION]):
            return FOUNDATION;
        case hash_string(MAJOR_STAGE_STR[VIRTUOSO]):
            return VIRTUOSO;
        case hash_string(MAJOR_STAGE_STR[NASCENT]):
            return NASCENT;
        case hash_string(MAJOR_STAGE_STR[INCARNATION]):
            return INCARNATION;
        case hash_string(MAJOR_STAGE_STR[VOIDBREAK]):
            return VOIDBREAK;
        case hash_string(MAJOR_STAGE_STR[WHOLENESS]):
            return WHOLENESS;
        case hash_string(MAJOR_STAGE_STR[PERFECTION]):
            return PERFECTION;
        case hash_string(MAJOR_STAGE_STR[NIRVANA]):
            return NIRVANA;
        case hash_string(MAJOR_STAGE_STR[CELESTIAL]):
            return CELESTIAL;
        case hash_string(MAJOR_STAGE_STR[ETERNAL]):
            return ETERNAL;
        default:
            return NUM_MAJOR_STAGES;  // An invalid stage, indicates an error
    }
}

enum minor_stage_t {
    EARLY,
    MIDDLE,
    LATE,

    NUM_MINOR_STAGES
};

// constexpr inline string_view MINOR_STAGE_STR[NUM_MINOR_STAGES]
constexpr inline array<string_view, NUM_MINOR_STAGES> MINOR_STAGE_STR{"Early", "Middle", "Late"};

constexpr minor_stage_t get_minor_stage(const string_view &name) {
    const uint64_t hash{hash_string(name)};

    switch (hash) {
        case hash_string(MINOR_STAGE_STR[EARLY]):
            return EARLY;
        case hash_string(MINOR_STAGE_STR[MIDDLE]):
            return MIDDLE;
        case hash_string(MINOR_STAGE_STR[LATE]):
            return LATE;
        default:
            return NUM_MINOR_STAGES;  // An invalid stage, indicates an error
    }
}

/**
 * A 2d array describing the number of gates in each stage
 *
 * - first dimension: major stage
 * - second dimension: minor stage
 */
constexpr inline array<array<unsigned, NUM_MINOR_STAGES>, NUM_MAJOR_STAGES> NUM_GATES{
    2u,  3u,  4u,   // FOUNDATION
    4u,  5u,  6u,   // VIRTUOSO
    6u,  7u,  8u,   // NASCENT
    8u,  9u,  15u,  // INCARNATION
    20u, 20u, 20u,  // VOIDBREAK
    20u, 20u, 20u,  // WHOLENESS
    20u, 20u, 20u,  // PERFECTION
    20u, 20u, 20u,  // NIRVANA
    20u, 20u, 20u,  // CELESTIAL
    20u, 20u, 20u   // ETERNAL
};

// constexpr inline const unsigned NUM_GATES[NUM_MAJOR_STAGES][NUM_MINOR_STAGES]{
//     {2u, 3u, 4u}, // FOUNDATION
//     {4u, 5u, 6u}, // VIRTUOSO
//     {6u, 7u, 8u}, // NASCENT
//     {8u, 9u, 15u}, // INCARNATION
//     {20u, 20u, 20u}, // VOIDBREAK
//     {20u, 20u, 20u}, // WHOLENESS
//     {20u, 20u, 20u}, // PERFECTION
//     {20u, 20u, 20u}, // NIRVANA
//     {20u, 20u, 20u}, // CELESTIAL
//     {20u, 20u, 20u}, // ETERNAL
// };

typedef unsigned long long exp_t;

/*

// UNUSED: exp req for Novice and Connection

constexpr inline const exp_t NOVICE_EXP_REQ{ 2 };

#define NUM_CONNECTION_STAGES 10

constexpr inline const exp_t CONNECTION_EXP_REQ[NUM_CONNECTION_STAGES]{
    13, 73, 176, 500, 1000, 1600, 2500, 3000, 3500, 4500
};

*/

/**
 * A 3d array describing the experience required to complete each gate
 *
 * - first dimension: major stage
 * - second dimension: minor stage
 * - third dimension: gate
 *
 * Access exp required for a gate by:
 * `GATE_EXP_REQ[major_stage][minor_stage][gate]`
 */
// constexpr inline const exp_t* const GATE_EXP_REQ[NUM_MAJOR_STAGES][NUM_MINOR_STAGES]
constexpr inline array<array<const exp_t *, NUM_MINOR_STAGES>, NUM_MAJOR_STAGES> GATE_EXP_REQ{
    // FOUNDATION
    (const exp_t[]){// EARLY
                    12376, 15624},
    (const exp_t[]){// MIDDLE
                    15708, 19873, 23919},
    (const exp_t[]){// LATE
                    36180, 45426, 55074, 64320},

    // VIRTUOSO
    (const exp_t[]){// EARLY
                    39438, 49517, 60033, 70112},
    (const exp_t[]){// MIDDLE
                    60915, 77190, 93000, 108810, 125085},
    (const exp_t[]){// LATE
                    158500, 199301, 241672, 280905, 324845, 364077},

    // NASCENT
    (const exp_t[]){// EARLY
                    183679, 230962, 280065, 325529, 376450, 421915},
    (const exp_t[]){// MIDDLE
                    308712, 389749, 470786, 551823, 632859, 710038, 794933},
    (const exp_t[]){// LATE
                    719563, 918827, 1095950, 1295213, 1472337, 1671600, 1848723, 2047987},

    // INCARNATION
    (const exp_t[]){// EARLY
                    801210, 1023083, 1220304, 1442177, 1639398, 1861271, 2058492, 2280365},
    (const exp_t[]){
        // MIDDLE
        1412392, 1804723, 2170898, 2537074, 2903249, 3269425, 3635601, 4027931, 4394107},
    (const exp_t[]){// LATE
                    1483013, 1853766, 2286312, 2595272, 3027818, 3460363, 3769324, 4201870, 4572623,
                    4943376, 5252337, 5623090, 5932051, 6241012, 6549973},

    // VOIDBREAK
    (const exp_t[]){// EARLY
                    1020222, 1292280, 1496324, 1836398, 2040442, 2380515, 2584560,
                    2856619, 3128678, 3400737, 3604781, 3808825, 4080884, 4216914,
                    4488973, 4761031, 4897061, 5169120, 5373164, 5577208},
    (const exp_t[]){// MIDDLE
                    2131409, 2699785, 3126066,  3836536,  4262817,  4973288, 5399569,
                    5967944, 6536321, 7104696,  7530977,  7957260,  8525635, 8809823,
                    9378199, 9946574, 10230762, 10799138, 11225420, 11651701},
    (const exp_t[]){// LATE
                    4615726,  5846585,  6769731,  8308306,  9231451,  10770025, 11693172,
                    12924031, 14154891, 15385751, 16308897, 17232041, 18462902, 19078331,
                    20309192, 21540052, 22155482, 23386342, 24309487, 25232632},

    // WHOLENESS
    (const exp_t[]){// EARLY
                    2802204,  3549458,  4109898,  5043967,  5604407,  6538475,  7098915,
                    7846170,  8593424,  9340679,  9901119,  10461560, 11208814, 11582441,
                    12329696, 13076950, 13450577, 14197831, 14758272, 15318712},
    (const exp_t[]){// MIDDLE
                    3959269,  5015074,  5806927,  7126683,  7918537,  9238293,  10030147,
                    11085952, 12141757, 13197562, 13989415, 14781270, 15837074, 16364976,
                    17420782, 18476587, 19004489, 20060294, 20852147, 21644001},
    (const exp_t[]){// LATE
                    4566759,  5784560,  6697912,  8220165,  9133517,  10655769, 11569121,
                    12786923, 14004725, 15222528, 16135879, 17049231, 18267033, 18875934,
                    20093737, 21311538, 21920440, 23138242, 24051593, 24964954},

    // PERFECTION
    (const exp_t[]){// EARLY
                    7047835,  8927256,  10336824, 12686101, 14095669, 16444946, 17854513,
                    19733936, 21613358, 23492780, 24902348, 26311914, 28191337, 29131048,
                    31010470, 32889893, 33829604, 35709026, 37118593, 38528160},
    (const exp_t[]){// MIDDLE
                    10361621, 13124720, 15197044, 18650918, 20723242, 24177115, 26249440,
                    29012538, 31775638, 34538736, 36611061, 38683384, 41446484, 42828033,
                    45591132, 48354231, 49735780, 52498879, 54571204, 56643527},
    (const exp_t[]){// LATE
                    13731909, 17393750, 20140132, 24717435, 27463816, 32041119, 34787501,
                    38449343, 42111185, 45773027, 48519409, 51265790, 54927633, 56758553,
                    60420396, 64082238, 65913160, 69575001, 72321383, 75067764},

    // NIRVANA
    (const exp_t[]){// EARLY
                    16786018, 21262289, 24619493, 30214832, 33572035, 39167375, 42524578,
                    47000849, 51477121, 55953392, 59310596, 62667799, 67144071, 69382206,
                    73858478, 78334749, 80572885, 85049156, 88406360, 91763563},
    (const exp_t[]){// MIDDLE
                    21455765, 27177300,  31468452,  38620374,  42911526,  50063448, 54354600,
                    60076137, 65797673,  71519211,  75810363,  80101516,  85823053, 88683821,
                    94405358, 100126894, 102987663, 108709201, 113000352, 117291505},
    (const exp_t[]){// LATE
                    26926091,  34106381,  39491600,  48466963,  53852181,  62827545,  68212762,
                    75393054,  82573344,  89753635,  95138854,  100524071, 107704362, 111294508,
                    118474798, 125655089, 129245235, 136425525, 141810744, 147195961},

    // CELESTIAL
    (const exp_t[]){// EARLY
                    43853989,  55548385,  64319182,  78937179,  87707976,  102325973, 111096770,
                    122791166, 134485564, 146179961, 154950758, 163721555, 175415953, 181263151,
                    192957548, 204651945, 210499143, 222193540, 230964337, 239735135},
    (const exp_t[]){// MIDDLE
                    57847086,  73272975,  84842392,  104124754, 115694171, 134976533, 146545951,
                    161971839, 177397729, 192823619, 204393035, 215962453, 231388342, 239101287,
                    254527177, 269953066, 277666011, 293091900, 304661317, 316230734},
    (const exp_t[]){// LATE
                    78231251,  99092916,  114739167, 140816251, 156462500, 182539583, 198185834,
                    219047500, 239909167, 260770834, 276417084, 292063334, 312925000, 323355834,
                    344217501, 365079167, 375510001, 396371667, 412017917, 427664167},

    // ETERNAL
    (const exp_t[]){// EARLY
                    126409270, 160118408, 185400262, 227536685, 252818538, 294954962, 320236816,
                    353945954, 387655093, 421364232, 446646085, 471927939, 505637078, 522491646,
                    556200786, 589909924, 606764493, 640473632, 665755485, 691037339},
    (const exp_t[]){// MIDDLE
                    147741734, 187139530, 216687876, 265935121, 295483467, 344730713, 374279058,
                    413676855, 453074650, 492472446, 522020793, 551569139, 590966936, 610665833,
                    650063628, 689461425, 709160322, 748558118, 778106464, 807654811},
    (const exp_t[]){// LATE
                    192195528, 243447668, 281886773, 345951950, 384391054,  448456231, 486895336,
                    538147477, 589399617, 640651758, 679090863, 717529969,  768782109, 794408180,
                    845660321, 896912461, 922538531, 973790672, 1012229778, 1050668882}};

constexpr array<array<exp_t, NUM_MINOR_STAGES>, NUM_MAJOR_STAGES> stage_exp_req_factory() noexcept {
    array<array<exp_t, NUM_MINOR_STAGES>, NUM_MAJOR_STAGES> STAGE_EXP_REQ{};

    for (size_t major_stage{0}; major_stage < NUM_MAJOR_STAGES; ++major_stage)
        for (size_t minor_stage{0}; minor_stage < NUM_MINOR_STAGES; ++minor_stage)
            STAGE_EXP_REQ[major_stage][minor_stage] = accumulate(
                GATE_EXP_REQ[major_stage][minor_stage],
                GATE_EXP_REQ[major_stage][minor_stage] + NUM_GATES[major_stage][minor_stage], 0ull);

    return STAGE_EXP_REQ;
}

constexpr inline array<array<exp_t, NUM_MINOR_STAGES>, NUM_MAJOR_STAGES> STAGE_EXP_REQ{
    stage_exp_req_factory()};

///////////////////////// Aura gem ////////////////////////////

/**
 * Aura gem absoprtion rate multipliers per quality
 */
// constexpr inline const double AURA_GEM_MULT[NUM_QUALITIES]
constexpr inline array<double, NUM_QUALITIES> AURA_GEM_MULT{
    0.1,   // COMMON
    0.13,  // UNCOMMON
    0.16,  // RARE
    0.2,   // EPIC
    0.24,  // LEGENDARY
    0.28   // MYTHIC
};

////////////////////////// Artifacts ///////////////////////////

enum artifact_star_t {
    STAR_0,
    STAR_1,
    STAR_2,
    STAR_3,
    STAR_4,
    STAR_5,

    NUM_ARTIFACT_STARS
};

// constexpr inline string_view const ARTIFACT_STAR_STR[NUM_ARTIFACT_STARS]
constexpr inline array<string_view, NUM_ARTIFACT_STARS> ARTIFACT_STAR_STR{
    "0_star", "1_star", "2_star", "3_star", "4_star", "5_star"};

constexpr artifact_star_t get_artifact_star(const string_view &name) {
    const uint64_t hash{hash_string(name)};

    switch (hash) {
        case hash_string(ARTIFACT_STAR_STR[STAR_0]):
            return STAR_0;
        case hash_string(ARTIFACT_STAR_STR[STAR_1]):
            return STAR_1;
        case hash_string(ARTIFACT_STAR_STR[STAR_2]):
            return STAR_2;
        case hash_string(ARTIFACT_STAR_STR[STAR_3]):
            return STAR_3;
        case hash_string(ARTIFACT_STAR_STR[STAR_4]):
            return STAR_4;
        case hash_string(ARTIFACT_STAR_STR[STAR_5]):
            return STAR_5;
        default:
            return NUM_ARTIFACT_STARS;  // An invalid star, indicates an error
    }
}

/**
 * Artifact energy recovery rate per star
 * Unit: points per 15 mins
 */
// constexpr inline const double ARTIFACT_ENERGY_RECOVERY_RATE[NUM_ARTIFACT_STARS]
constexpr inline array<double, NUM_ARTIFACT_STARS> ARTIFACT_ENERGY_RECOVERY_RATE{
    1.,   // 0 star
    1.3,  // 1 star
    1.6,  // 2 star
    2.,   // 3 star
    2.4,  // 4 star
    3.    // 5 star
};

#define VASE_TRANSMOG_PILL_BONUS 0.08  // additive
#define VASE_1_STAR_PILL_BONUS 0.1
#define VASE_3_STAR_PILL_BONUS 0.2        // not additive! overrides VASE_1_STAR_PILL_BONUS
#define VASE_5_STAR_COSTLESS_CHANCE 0.15  // chance to not cost energy when purifying a pill
#define VASE_MYTHIC_PILL_COST 100.0       // cost to purify a mythic pill

#define MIRROR_1_STAR_COST_DEDUCTION 0.05
#define MIRROR_3_STAR_COST_DEDUCTION 0.1  // not additive! overrides MIRROR_1_STAR_COST_DEDUCTION
#define MIRROR_5_STAR_CHANCE_EXTRA_DUPLICATE \
    0.15  // chance to make an extra copy when duplicating an item
#define MIRROR_DUPLICATE_MYTHIC_PILL_COST 200.0 

#define ARTIFACT_DAILY_ENERGY_RECHARGE 100.0  // paid daily energy recharge for artifacts

///////////////////// Respira ///////////////////////

// constexpr inline const int RESPIRA_MULT[]
constexpr inline array<int, 4> RESPIRA_MULT{1, 2, 5, 10};

// constexpr inline const double RESPIRA_MULT_CHANCE[]
constexpr inline array<double, 4> RESPIRA_MULT_CHANCE{0.55, 0.3, 0.1475, 0.0025};

// RESPIRA_MULT_CHANCE need to add up to 1
// static_assert(accumulate(RESPIRA_MULT_CHANCE.begin(), RESPIRA_MULT_CHANCE.end(), 0.0) == 1.0);

static_assert(abs(accumulate(RESPIRA_MULT_CHANCE.begin(), RESPIRA_MULT_CHANCE.end(), 0.0) - 1.0) <
                  1e-10,
              "RESPIRA_MULT_CHANCE must sum to 1.0");

constexpr inline const double EXPECTED_RESPIRA_MULT{
    inner_product(RESPIRA_MULT.begin(), RESPIRA_MULT.end(), RESPIRA_MULT_CHANCE.begin(), 0.0)};

/**
 * Respira base exp per major stage
 *
 * Players' respira exp is usually a lot higher than the base exp because of various bonuses
 *
 * TODO: these values need to be verified
 */
// constexpr inline const exp_t RESPIRA_BASE_EXP[NUM_MAJOR_STAGES]
constexpr inline array<exp_t, NUM_MAJOR_STAGES> RESPIRA_BASE_EXP{
    // 6, // NOVICE
    // 20, // CONNECTION

    110,    // FOUNDATION
    650,    // VIRTUOSO
    3200,   // NASCENT
    5300,   // INCARNATION
    7800,   // VOIDBREAK
    10500,  // WHOLENESS
    13500,  // PERFECTION
    25000,  // NIRVANA
    37500,  // CELESTIAL
};

// these two arrays need to be equal length
static_assert(sizeof(RESPIRA_MULT) / sizeof(RESPIRA_MULT[0]) ==
                  sizeof(RESPIRA_MULT_CHANCE) / sizeof(RESPIRA_MULT_CHANCE[0]),
              "RESPIRA_MULT and RESPIRA_MULT_CHANCE must have the same size");

//////////////////// Pills /////////////////////

/**
 * Pill base exp per quality
 *
 * NOTE: base exp for common and uncommon pills are not used and are set to 0
 *
 * TODO: these values need to be verified
 */
// constexpr inline const exp_t PILL_BASE_EXP[NUM_MAJOR_STAGES][NUM_QUALITIES]
constexpr inline array<array<exp_t, NUM_QUALITIES>, NUM_MAJOR_STAGES> PILL_BASE_EXP{
    /*

    // UNUSED: pill base exp for Novice and Connection

     // NOVICE
        0, // COMMON
        0, // UNCOMMON
        0, // RARE
        0, // EPIC
        0, // LEGENDARY
        0, // MYTHIC


     // CONNECTION R1
        125,  // COMMON
        250,  // UNCOMMON
        400,  // RARE
        750,  // EPIC
        1500, // LEGENDARY
        3000, // MYTHIC



    */

    // FOUNDATION R2
    625,    // COMMON
    1250,   // UNCOMMON
    2000,   // RARE
    3750,   // EPIC
    7500,   // LEGENDARY
    15000,  // MYTHIC

    // VIRTUOSO R3
    1900,   // COMMON
    3800,   // UNCOMMON
    6080,   // RARE
    11400,  // EPIC
    22800,  // LEGENDARY
    45600,  // MYTHIC

    // NASCENT R4
    5000,    // COMMON
    10000,   // UNCOMMON
    16000,   // RARE
    30000,   // EPIC
    60000,   // LEGENDARY
    120000,  // MYTHIC

    // INCARNATION R5
    8000,    // COMMON
    16000,   // UNCOMMON
    25600,   // RARE
    48000,   // EPIC
    96000,   // LEGENDARY
    192000,  // MYTHIC

    // VOIDBREAK R6
    12000,   // COMMON
    24000,   // UNCOMMON
    38400,   // RARE
    72000,   // EPIC
    144000,  // LEGENDARY
    288000,  // MYTHIC

    // WHOLENESS R7
    20500,   // COMMON
    41000,   // UNCOMMON
    65600,   // RARE
    123000,  // EPIC
    246000,  // LEGENDARY
    492000,  // MYTHIC

    // PERFECTION R8
    31000,   // COMMON
    62000,   // UNCOMMON
    99200,   // RARE
    186000,  // EPIC
    372000,  // LEGENDARY
    744000,  // MYTHIC

    // NIRVANA R9
    57000,    // COMMON
    114000,   // UNCOMMON
    182400,   // RARE
    342000,   // EPIC
    684000,   // LEGENDARY
    1368000,  // MYTHIC

    // CELESTIAL R10
    128375,   // COMMON
    256750,   // UNCOMMON
    410800,   // RARE
    770250,   // EPIC
    1540500,  // LEGENDARY
    3081000,  // MYTHIC

    // ETERNAL R11
    0, 0, 0, 0, 0, 0  // no data available at this time

};

//////////////////// Myrimon fruits and extractor /////////////////////////

/**
 * TODO: these values need to be verified
 */
enum fruit_rank_t {
    R3,   // Incarnation (?)
    R6,   // Voidbreak
    R7,   // Wholeness
    R8,   // Perfection
    R9,   // Nirvana
    R10,  // Celestial
    R11,  // Eternal

    NUM_FRUIT_RANKS
};

constexpr fruit_rank_t get_fruit_rank(major_stage_t major_stage) {
    switch (major_stage) {
        case INCARNATION:
            return R3;  // R3 fruit rank
        case VOIDBREAK:
            return R6;  // R6 fruit rank
        case WHOLENESS:
            return R7;  // R7 fruit rank
        case PERFECTION:
            return R8;  // R8 fruit rank
        case NIRVANA:
            return R9;  // R9 fruit rank
        case CELESTIAL:
            return R10;  // R10 fruit rank
        case ETERNAL:
            return R11;  // R11 fruit rank
        default:
            return NUM_FRUIT_RANKS;  // An invalid rank, indicates an error
    }
}

// constexpr inline const exp_t FRUIT_BASE_EXP[NUM_FRUIT_RANKS]
constexpr inline array<exp_t, NUM_FRUIT_RANKS> FRUIT_BASE_EXP{
    65000,   // R3
    96000,   // R6
    130000,  // R7
    240000,  // R8
    420000,  // R9
    800000,  // R10
    1810000  // R11
};

// constexpr inline const double QUALITY_MULT[NUM_QUALITIES]
constexpr inline array<double, NUM_QUALITIES> QUALITY_MULT{
    1.0,  // COMMON
    1.3,  // UNCOMMON
    1.6,  // RARE
    2.0,  // EPIC
    2.4,  // LEGENDARY
    3.0,  // MYTHIC
};

#define MAX_EXTRACTOR_NODE_LVL 30

constexpr quality_t get_extractor_node_quality(unsigned short node_level) {
    if (node_level < 0 || node_level > MAX_EXTRACTOR_NODE_LVL)
        return NUM_QUALITIES;  // An invalid quality, indicates an error

    if (node_level < 6)
        return COMMON;
    else if (node_level < 11)
        return UNCOMMON;
    else if (node_level < 16)
        return RARE;
    else if (node_level < 21)
        return EPIC;
    else if (node_level < 26)
        return LEGENDARY;
    else
        return MYTHIC;
}

#define EXP_GUSH_BASE_MULT 1.5
#define EXP_GUSH_MULT_PER_NODE_LVL 0.04
#define GUSH_BASE_CHANCE 0.1
#define GUSH_CHANCE_PER_QUALITY 0.05
#define MAJOR_STAGE_BONUS 0.5
#define MULT_PER_CULTIXP_NODE_LEVEL 0.02


/**
 * Chance of generating an orb of a certain quality based on the extractor "Quality" node level
 *
 * ex. get chance of getting a EPIC orb if "Quality" node is at level 10:
 * NODE_QUALITY_CHANCE[10][EPIC]
 */
constexpr inline array<array<double, NUM_QUALITIES>, MAX_EXTRACTOR_NODE_LVL + 1>
    NODE_QUALITY_CHANCE{
        // Common, Uncommon, Rare, Epic, Legendary, Mythic
        0.7,  0.0,  0.0, 0.0, 0.0, 0.0,  // lvl 0
        0.65, 0.05, 0.0, 0.0, 0.0, 0.0,  // lvl 1
        0.6,  0.1,  0.0, 0.0, 0.0, 0.0,  // lvl 2
        0.55, 0.15, 0.0, 0.0, 0.0, 0.0,  // lvl 3
        0.5,  0.2,  0.0, 0.0, 0.0, 0.0,  // lvl 4
        0.45, 0.25, 0.0, 0.0, 0.0, 0.0,  // lvl 5
        0.2,  0.5,  0.0, 0.0, 0.0, 0.0,  // lvl 6
        0.15, 0.55, 0.0, 0.0, 0.0, 0.0,  // lvl 7
        0.1,  0.6,  0.0, 0.0, 0.0, 0.0,  // lvl 8
        0.05, 0.65, 0.0, 0.0, 0.0, 0.0,  // lvl 9
        0.0,  0.7,  0.0, 0.0, 0.0, 0.0,  // lvl 10
        0.0,  0.4,  0.3, 0.0, 0.0, 0.0,  // lvl 11
        0.0,  0.3,  0.4, 0.0, 0.0, 0.0,  // lvl 12
        0.0,  0.2,  0.5, 0.0, 0.0, 0.0,  // lvl 13
        0.0,  0.1,  0.6, 0.0, 0.0, 0.0,  // lvl 14
        0.0,  0.0,  0.7, 0.0, 0.0, 0.0,  // lvl 15
        0.0,  0.0,  0.4, 0.3, 0.0, 0.0,  // lvl 16
        0.0,  0.0,  0.3, 0.4, 0.0, 0.0,  // lvl 17
        0.0,  0.0,  0.2, 0.5, 0.0, 0.0,  // lvl 18
        0.0,  0.0,  0.1, 0.6, 0.0, 0.0,  // lvl 19
        0.0,  0.0,  0.0, 0.7, 0.0, 0.0,  // lvl 20
        0.0,  0.0,  0.0, 0.4, 0.3, 0.0,  // lvl 21
        0.0,  0.0,  0.0, 0.3, 0.4, 0.0,  // lvl 22
        0.0,  0.0,  0.0, 0.2, 0.5, 0.0,  // lvl 23
        0.0,  0.0,  0.0, 0.1, 0.6, 0.0,  // lvl 24
        0.0,  0.0,  0.0, 0.0, 0.7, 0.0,  // lvl 25
        0.0,  0.0,  0.0, 0.0, 0.4, 0.3,  // lvl 26
        0.0,  0.0,  0.0, 0.0, 0.3, 0.4,  // lvl 27
        0.0,  0.0,  0.0, 0.0, 0.2, 0.5,  // lvl 28
        0.0,  0.0,  0.0, 0.0, 0.1, 0.6,  // lvl 29
        0.0,  0.0,  0.0, 0.0, 0.0, 0.7,  // lvl 30
    };

// static assert for NODE_QUALITY_CHANCE to check each row adds up to 0.7
static_assert(
    [] consteval {
        return none_of(NODE_QUALITY_CHANCE.begin(), NODE_QUALITY_CHANCE.end(), [](const auto &row) {
            return abs(accumulate(row.begin(), row.end(), 0.0) - 0.7) > 1e-10;
        });
    }(),
    "Each row in NODE_QUALITY_CHANCE must sum to 0.7");

/*
### Quality chance calculation:

For the chance of getting orb of a certain quality `Q`,
when the "Quality" extractor node is at level `N`,
and the extractor is at quality `R`:

chance = NODE_QUALITY_CHANCE[Q][N] + (R == Q ? 0.3 : 0.0)

assert(SUM(chance of all Q) == 1.0) at a given N and R.
- TODO: implement testing for this assertion

*/

/*

### EXP calculation:

base_mult = [(1 + exp_node_bonus) + conditional_20%] * quality_multiplier

exp_no_gush = base_mult * base_exp

note: exp_node_bonus will be different depending on whether you are in mortal world or spirit world
where:
- base_mult = the multiplier applied to the base exp of the fruit without gush

- exp_no_gush = exp of the orb produced when consuming a myrimon fruit without triggering gush

- base_exp = base exp of the fruit based on its rank (R6, R7, etc)

- exp_node_bonus_sw = 0.02 * <level of the "CultiXP" extractor node> (Spirit World)

- exp_node_bonus_mw = 0.04 * <level of the "CultiXP" extractor node> (Mortal World)

- conditional_20% = A conditional 20% exp bonus is applied to exp orb of a certain quality
    if the "CultiXP" extractor node is at or above this quality (ex. if the orb is RARE, and
    the "CultiXP" extractor node is at RARE or higher, then the 20% bonus applies)

- quality_multiplier = the multiplier applied to the base exp of the fruit based on its quality
    (ex. 1.0 for COMMON, 1.3 for UNCOMMON, etc)

### Gush calculation:

gush_mult = EXP_GUSH_BASE_MULT
    + (EXP_GUSH_MULT_PER_NODE_LVL * <level of the "Gush" extractor node>)

Gush calculation equation :

exp_with_gush = base_exp * base_mult * gush_mult

*/

#endif  // CALCULATOR_CONSTANTS_HPP
