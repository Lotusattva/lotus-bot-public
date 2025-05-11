#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

#include "global.hpp"

/**
 * @brief verify whether the user causing the interaction is the owner of the session
 *
 * @returns an iterator to the session if the user is the owner, otherwise returns nulllopt
 */
task<optional<unordered_map<snowflake, snowflake>::iterator>> verify_user(const button_click_t& event);

task<void> cancel_calc(const button_click_t& event);

task<void> ask_stage(const button_click_t& event);



/////////////// Define calcullator constants /////////////////////

enum MajorStage {

    // excluding novice and connection

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

    __MAJOR_STAGE_COUNT__
};

enum MinorStage {
    EARLY,
    MIDDLE,
    LATE,

    __MINOR_STAGE_COUNT__
};

/**
 * A 2d array describing the number of gates in each stage
 *
 * - first dimension: major stage
 * - second dimension: minor stage
 *
 */
constexpr inline const unsigned short GATE_COUNT[__MAJOR_STAGE_COUNT__][__MINOR_STAGE_COUNT__]{
    {2, 3, 4}, // FOUNDATION
    {4, 5, 6}, // VIRTUOSO
    {6, 7, 8}, // NASCENT
    {8, 9, 15}, // INCARNATION
    {20, 20, 20}, // VOIDBREAK
    {20, 20, 20}, // WHOLENESS
    {20, 20, 20}, // PERFECTION
    {20, 20, 20}, // NIRVANA
    {20, 20, 20}, // CELESTIAL
    {20, 20, 20}, // ETERNAL
};

typedef unsigned long long exp_t;

/**
 * A 3d array describing the experience required to complete each gate
 *
 * - first dimension: major stage
 * - second dimension: minor stage
 * - third dimension: gate
 *
 */
constexpr inline const exp_t const* EXP_REQ[__MAJOR_STAGE_COUNT__][__MINOR_STAGE_COUNT__]{
    { // FOUNDATION
        (const exp_t[]) { // EARLY
            12376ull, 15624ull
        },
        (const exp_t[]) { // MIDDLE
            15708ull, 19873ull, 23919ull
        },
        (const exp_t[]) { // LATE
            36180ull, 45426ull, 55074ull, 64320ull
        }
    },
    { // VIRTUOSO
        (const exp_t[]) { // EARLY
            39438ull, 49517ull, 60033ull, 70112ull
        },
        (const exp_t[]) { // MIDDLE
            60915ull, 77190ull, 93003ull, 108810ull, 125085ull
        },
        (const exp_t[]) { // LATE
            158500ull, 199301ull, 241672ull, 280905ull, 324845ull,
            364077ull
        }
    },
    { // NASCENT
        (const exp_t[]) { // EARLY
            183679ull, 230962ull, 280065ull, 325529ull, 376450ull,
            421915ull
        },
        (const exp_t[]) { // MIDDLE
            308712ull, 389749ull, 470786ull, 551823ull, 632859ull,
            710038ull, 794933ull
        },
        (const exp_t[]) { // LATE
            719563ull, 918827ull, 1095905ull, 1295213ull, 1472337ull,
            1671600ull, 1848723ull, 2047987ull
        }
    },
    { // INCARNATION
        (const exp_t[]) { // EARLY
            801210ull, 1023083ull, 1220304ull, 1442177ull, 1639398ull,
            1861271ull, 2058492ull, 2280365ull
        },
        (const exp_t[]) { // MIDDLE
            1412392ull, 1804723ull, 2170898ull, 2537074ull, 2903249ull,
            3269425ull, 3635601ull, 4027931ull, 4394107ull
        },
        (const exp_t[]) { // LATE
            1483013ull, 1853766ull, 2286312ull, 2595272ull, 3027818ull,
            3460363ull, 3769324ull, 4201870ull, 4572623ull, 4943376ull,
            5252337ull, 5623090ull, 5932051ull, 6241012ull, 6549973ull
        }
    },
    { // VOIDBREAK
        (const exp_t[]) { // EARLY
            1020222ull, 1292280ull, 1496324ull, 1836398ull, 2040442ull,
            2380515ull, 2584560ull, 2856619ull, 3128678ull, 3400737ull,
            3604781ull, 3808825ull, 4080884ull, 4216914ull, 4488973ull,
            4761031ull, 4897061ull, 5169120ull, 5373164ull, 5577208ull
        },
        (const exp_t[]) { // MIDDLE
            2131409ull, 2699785ull, 3126066ull, 3836536ull, 4262817ull,
            4973288ull, 5399569ull, 5967944ull, 6536321ull, 7104696ull,
            7530977ull, 7957260ull, 8525635ull, 8809823ull, 9378199ull,
            9946574ull, 10230762ull, 10799138ull, 11225420ull, 11651701ull
        },
        (const exp_t[]) { // LATE
            4615726ull, 5846585ull, 6769731ull, 8308306ull, 9231451ull,
            10770025ull, 11693172ull, 12924031ull, 14154891ull, 15385751ull,
            16308897ull, 17232041ull, 18462902ull, 19078331ull, 20309192ull,
            21540052ull, 22155482ull, 23386342ull, 24309487ull, 25232632ull
        }
    },
    { // WHOLENESS
        (const exp_t[]) { // EARLY
            2802204ull, 3549458ull, 4109898ull, 5043967ull, 5604407ull,
            6538475ull, 7098915ull, 7846170ull, 8593424ull, 9340679ull,
            9901119ull, 10461560ull, 11208814ull, 11582441ull, 12329696ull,
            13076950ull, 13450577ull, 14197831ull, 14758272ull, 15318712ull
        },
        (const exp_t[]) { // MIDDLE
            3959269ull, 5015074ull, 5806927ull, 7126683ull, 7918537ull,
            9238293ull, 10030147ull, 11085952ull, 12141757ull, 13197562ull,
            13989415ull, 14781270ull, 15837074ull, 16364976ull, 17420782ull,
            18476587ull, 19004489ull, 20060294ull, 20852147ull, 21644001ull
        },
        (const exp_t[]) { // LATE
            4566759ull, 5784560ull, 6697912ull, 8220165ull, 9133517ull,
            10655769ull, 11569121ull, 12786923ull, 14004725ull, 15222528ull,
            16135879ull, 17049231ull, 18267033ull, 18875934ull, 20093737ull,
            21311538ull, 21920440ull, 23138242ull, 24051593ull, 24964954ull
        }
    },
    { // PERFECTION
        (const exp_t[]) { // EARLY
            7047835ull, 8927256ull, 10336824ull, 12686101ull, 14095669ull,
            16444946ull, 17854513ull, 19733936ull, 21613358ull, 23492780ull,
            24902348ull, 26311914ull, 28191337ull, 29131048ull, 31010470ull,
            32889893ull, 33829604ull, 35709026ull, 37118593ull, 38528160ull
        },
        (const exp_t[]) { // MIDDLE
            10361621ull, 13124720ull, 15197044ull, 18650918ull, 20723242ull,
            24177115ull, 26249440ull, 29012538ull, 31775638ull, 34538736ull,
            36611061ull, 38683384ull, 41446484ull, 42828033ull, 45591132ull,
            48354231ull, 49735780ull, 52498879ull, 54571204ull, 56643527ull
        },
        (const exp_t[]) { // LATE
            13731909ull, 17393750ull, 20140132ull, 24717435ull, 27463816ull,
            32041119ull, 34787501ull, 38449343ull, 42111185ull, 45773027ull,
            48519409ull, 51265790ull, 54927633ull, 56758553ull, 60420396ull,
            64082238ull, 65913160ull, 69575001ull, 72321383ull, 75067764ull
        }
    },
    { // NIRVANA
        (const exp_t[]) { // EARLY
            16786018ull, 21262289ull, 24619493ull, 30214832ull, 33572035ull,
            39167375ull, 42524578ull, 47000849ull, 51477121ull, 55953392ull,
            59310596ull, 62667799ull, 67144071ull, 69382206ull, 73858478ull,
            78334749ull, 80572885ull, 85049156ull, 88406360ull, 91763563ull
        },
        (const exp_t[]) { // MIDDLE
            21455765ull, 27177300ull, 31468452ull, 38620374ull, 42911526ull,
            50063448ull, 54354600ull, 60076137ull, 65797673ull, 71519211ull,
            75810363ull, 80101516ull, 85823053ull, 88683821ull, 94405358ull,
            100126894ull, 102987663ull, 108709201ull, 113000352ull, 117291505ull
        },
        (const exp_t[]) { // LATE
            26926091ull, 34106381ull, 39491600ull, 48466963ull, 53852181ull,
            62827545ull, 68212762ull, 75393054ull, 82573344ull, 89753635ull,
            95138854ull, 100524071ull, 107704362ull, 111294508ull, 118474798ull,
            125655089ull, 129245235ull, 136425525ull, 141810744ull, 147195961ull
        }
    },
    { // CELESTIAL
        (const exp_t[]) { // EARLY
            43853989ull, 55548385ull, 64319182ull, 78937179ull, 87707976ull,
            102325973ull, 111096770ull, 122791166ull, 134485564ull, 146179961ull,
            154950758ull, 163721555ull, 175415953ull, 181263151ull, 192957548ull,
            204651945ull, 210499143ull, 222193540ull, 230964337ull, 239735135ull
        },
        (const exp_t[]) { // MIDDLE
            57847086ull, 73272975ull, 84842392ull, 104124754ull, 115694171ull,
            134976533ull, 146545951ull, 161971839ull, 177397729ull, 192823619ull,
            204393035ull, 215962453ull, 231388342ull, 239101287ull, 254527177ull,
            269953066ull, 277666011ull, 293091900ull, 304661317ull, 316230734ull
        },
        (const exp_t[]) { // LATE
            78231251ull, 99092916ull, 114739167ull, 140816251ull, 156462500ull,
            182539583ull, 198185834ull, 219047500ull, 239909167ull, 260770834ull,
            276417084ull, 292063334ull, 312925000ull, 323355834ull, 344217501ull,
            365079167ull, 375510001ull, 396371667ull, 412017917ull, 427664167ull
        }
    },
    { // ETERNAL
        (const exp_t[]) { // EARLY
            126409270ull, 160118408ull, 185400262ull, 227536685ull, 252818538ull,
            294954962ull, 320236816ull, 353945954ull, 387655093ull, 421364232ull,
            446646085ull, 471927939ull, 505637078ull, 522491646ull, 556200786ull,
            589909924ull, 606764493ull, 640473632ull, 665755485ull, 691037339ull
        },
        (const exp_t[]) { // MIDDLE
            147741734ull, 187139530ull, 216687876ull, 265935121ull, 295483467ull,
            344730713ull, 374279058ull, 413676855ull, 453074650ull, 492472446ull,
            522020793ull, 551569139ull, 590966936ull, 610665833ull, 650063628ull,
            689461425ull, 709160322ull, 748558118ull, 778106464ull, 807654811ull
        },
        (const exp_t[]) { // LATE
            192195528ull, 243447668ull, 281886773ull, 345951950ull, 384391054ull,
            448456231ull, 486895336ull, 538147477ull, 589399617ull, 640651758ull,
            679090863ull, 717529969ull, 768782109ull, 794408180ull, 845660321ull,
            896912461ull, 922538531ull, 973790672ull, 1012229778ull, 1050668882ull
        }
    }
};



#endif // CALCULATOR_HPP