#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

#include "global.hpp"

/**
 * @brief verify whether the user causing the interaction is the owner of the session
 *
 * @returns an iterator to the session if the user is the owner, otherwise returns nullopt
 */
task<optional<unordered_map<snowflake, snowflake>::iterator>> verify_user(const button_click_t& event);

task<void> cancel_calc(const button_click_t& event);

task<void> ask_stage(const button_click_t& event);



/////////////// Define calculator constants /////////////////////

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

inline const vector<const unsigned long> EXP_REQ[__MAJOR_STAGE_COUNT__][__MINOR_STAGE_COUNT__]{
    { // FOUNDATION
        { // EARLY
            12376ul, 15624ul
        },
        { // MIDDLE
            15708ul, 19873ul, 23919ul
        },
        { // LATE
            36180ul, 45426ul, 55074ul, 64320ul
        }
    },
    { // VIRTUOSO
        { // EARLY
            39438ul, 49517ul, 60033ul, 70112ul
        },
        { // MIDDLE
            60915ul, 77190ul, 93003ul, 108810ul, 125085ul
        },
        { // LATE
            158500ul, 199301ul, 241672ul, 280905ul, 324845ul,
            364077ul
        }
    },
    { // NASCENT
        { // EARLY
            183679ul, 230962ul, 280065ul, 325529ul, 376450ul,
            421915ul
        },
        { // MIDDLE
            308712ul, 389749ul, 470786ul, 551823ul, 632859ul,
            710038ul, 794933ul
        },
        { // LATE
            719563ul, 918827ul, 1095905ul, 1295213ul, 1472337ul,
            1671600ul, 1848723ul, 2047987ul
        }
    },
    { // INCARNATION
        { // EARLY
            801210ul, 1023083ul, 1220304ul, 1442177ul, 1639398ul,
            1861271ul, 2058492ul, 2280365ul
        },
        { // MIDDLE
            1412392ul, 1804723ul, 2170898ul, 2537074ul, 2903249ul,
            3269425ul, 3635601ul, 4027931ul, 4394107ul
        },
        { // LATE
            1483013ul, 1853766ul, 2286312ul, 2595272ul, 3027818ul,
            3460363ul, 3769324ul, 4201870ul, 4572623ul, 4943376ul,
            5252337ul, 5623090ul, 5932051ul, 6241012ul, 6549973ul
        }
    },
    { // VOIDBREAK
        { // EARLY
            1020222ul, 1292280ul, 1496324ul, 1836398ul, 2040442ul,
            2380515ul, 2584560ul, 2856619ul, 3128678ul, 3400737ul,
            3604781ul, 3808825ul, 4080884ul, 4216914ul, 4488973ul,
            4761031ul, 4897061ul, 5169120ul, 5373164ul, 5577208ul
        },
        { // MIDDLE
            2131409ul, 2699785ul, 3126066ul, 3836536ul, 4262817ul,
            4973288ul, 5399569ul, 5967944ul, 6536321ul, 7104696ul,
            7530977ul, 7957260ul, 8525635ul, 8809823ul, 9378199ul,
            9946574ul, 10230762ul, 10799138ul, 11225420ul, 11651701ul
        },
        { // LATE
            4615726ul, 5846585ul, 6769731ul, 8308306ul, 9231451ul,
            10770025ul, 11693172ul, 12924031ul, 14154891ul, 15385751ul,
            16308897ul, 17232041ul, 18462902ul, 19078331ul, 20309192ul,
            21540052ul, 22155482ul, 23386342ul, 24309487ul, 25232632ul
        }
    },
    { // WHOLENESS
        { // EARLY
            2802204ul, 3549458ul, 4109898ul, 5043967ul, 5604407ul,
            6538475ul, 7098915ul, 7846170ul, 8593424ul, 9340679ul,
            9901119ul, 10461560ul, 11208814ul, 11582441ul, 12329696ul,
            13076950ul, 13450577ul, 14197831ul, 14758272ul, 15318712ul
        },
        { // MIDDLE
            3959269ul, 5015074ul, 5806927ul, 7126683ul, 7918537ul,
            9238293ul, 10030147ul, 11085952ul, 12141757ul, 13197562ul,
            13989415ul, 14781270ul, 15837074ul, 16364976ul, 17420782ul,
            18476587ul, 19004489ul, 20060294ul, 20852147ul, 21644001ul
        },
        { // LATE
            4566759ul, 5784560ul, 6697912ul, 8220165ul, 9133517ul,
            10655769ul, 11569121ul, 12786923ul, 14004725ul, 15222528ul,
            16135879ul, 17049231ul, 18267033ul, 18875934ul, 20093737ul,
            21311538ul, 21920440ul, 23138242ul, 24051593ul, 24964954ul
        }
    },
    { // PERFECTION
        { // EARLY
            7047835ul, 8927256ul, 10336824ul, 12686101ul, 14095669ul,
            16444946ul, 17854513ul, 19733936ul, 21613358ul, 23492780ul,
            24902348ul, 26311914ul, 28191337ul, 29131048ul, 31010470ul,
            32889893ul, 33829604ul, 35709026ul, 37118593ul, 38528160ul
        },
        { // MIDDLE
            10361621ul, 13124720ul, 15197044ul, 18650918ul, 20723242ul,
            24177115ul, 26249440ul, 29012538ul, 31775638ul, 34538736ul,
            36611061ul, 38683384ul, 41446484ul, 42828033ul, 45591132ul,
            48354231ul, 49735780ul, 52498879ul, 54571204ul, 56643527ul
        },
        { // LATE
            13731909ul, 17393750ul, 20140132ul, 24717435ul, 27463816ul,
            32041119ul, 34787501ul, 38449343ul, 42111185ul, 45773027ul,
            48519409ul, 51265790ul, 54927633ul, 56758553ul, 60420396ul,
            64082238ul, 65913160ul, 69575001ul, 72321383ul, 75067764ul
        }
    },
    { // NIRVANA
        { // EARLY
            16786018ul, 21262289ul, 24619493ul, 30214832ul, 33572035ul,
            39167375ul, 42524578ul, 47000849ul, 51477121ul, 55953392ul,
            59310596ul, 62667799ul, 67144071ul, 69382206ul, 73858478ul,
            78334749ul, 80572885ul, 85049156ul, 88406360ul, 91763563ul
        },
        { // MIDDLE
            21455765ul, 27177300ul, 31468452ul, 38620374ul, 42911526ul,
            50063448ul, 54354600ul, 60076137ul, 65797673ul, 71519211ul,
            75810363ul, 80101516ul, 85823053ul, 88683821ul, 94405358ul,
            100126894ul, 102987663ul, 108709201ul, 113000352ul, 117291505ul
        },
        { // LATE
            26926091ul, 34106381ul, 39491600ul, 48466963ul, 53852181ul,
            62827545ul, 68212762ul, 75393054ul, 82573344ul, 89753635ul,
            95138854ul, 100524071ul, 107704362ul, 111294508ul, 118474798ul,
            125655089ul, 129245235ul, 136425525ul, 141810744ul, 147195961ul
        }
    },
    { // CELESTIAL
        { // EARLY
            43853989ul, 55548385ul, 64319182ul, 78937179ul, 87707976ul,
            102325973ul, 111096770ul, 122791166ul, 134485564ul, 146179961ul,
            154950758ul, 163721555ul, 175415953ul, 181263151ul, 192957548ul,
            204651945ul, 210499143ul, 222193540ul, 230964337ul, 239735135ul
        },
        { // MIDDLE
            57847086ul, 73272975ul, 84842392ul, 104124754ul, 115694171ul,
            134976533ul, 146545951ul, 161971839ul, 177397729ul, 192823619ul,
            204393035ul, 215962453ul, 231388342ul, 239101287ul, 254527177ul,
            269953066ul, 277666011ul, 293091900ul, 304661317ul, 316230734ul
        },
        { // LATE
            78231251ul, 99092916ul, 114739167ul, 140816251ul, 156462500ul,
            182539583ul, 198185834ul, 219047500ul, 239909167ul, 260770834ul,
            276417084ul, 292063334ul, 312925000ul, 323355834ul, 344217501ul,
            365079167ul, 375510001ul, 396371667ul, 412017917ul, 427664167ul
        }
    },
    { // ETERNAL
        { // EARLY
            126409270ul, 160118408ul, 185400262ul, 227536685ul, 252818538ul,
            294954962ul, 320236816ul, 353945954ul, 387655093ul, 421364232ul,
            446646085ul, 471927939ul, 505637078ul, 522491646ul, 556200786ul,
            589909924ul, 606764493ul, 640473632ul, 665755485ul, 691037339ul
        },
        { // MIDDLE
            147741734ul, 187139530ul, 216687876ul, 265935121ul, 295483467ul,
            344730713ul, 374279058ul, 413676855ul, 453074650ul, 492472446ul,
            522020793ul, 551569139ul, 590966936ul, 610665833ul, 650063628ul,
            689461425ul, 709160322ul, 748558118ul, 778106464ul, 807654811ul
        },
        { // LATE
            192195528ul, 243447668ul, 281886773ul, 345951950ul, 384391054ul,
            448456231ul, 486895336ul, 538147477ul, 589399617ul, 640651758ul,
            679090863ul, 717529969ul, 768782109ul, 794408180ul, 845660321ul,
            896912461ul, 922538531ul, 973790672ul, 1012229778ul, 1050668882ul
        }
    }
};



#endif // CALCULATOR_HPP