/**
 * jcch_dbgen.c/h among other things contain the data originating from dists.dss and are therefore subject to the TPCH
 * license (see LICENSE)
 */

#include "jcch_dbgen.h"
#include "dss.h"
#include "rnd.h"
#include "dsstypes.h"

#include <math.h>
#include <stdlib.h>

extern double jcch_flt_scale;

set_member jcch_p_cntr_set_list[] = {
  {1, "SM CASE"},
  {2, "SM BOX"},
  {3, "SM BAG"},
  {4, "SM JAR"},
  {5, "SM PACK"},
  {6, "SM PKG"},
  {7, "SM CAN"},
  {8, "SM DRUM"},
  {9, "LG CASE"},
  {10, "LG BOX"},
  {11, "LG BAG"},
  {12, "LG JAR"},
  {13, "LG PACK"},
  {14, "LG PKG"},
  {15, "LG CAN"},
  {16, "LG DRUM"},
  {17, "MED CASE"},
  {18, "MED BOX"},
  {19, "MED BAG"},
  {20, "MED JAR"},
  {21, "MED PACK"},
  {22, "MED PKG"},
  {23, "MED CAN"},
  {24, "MED DRUM"},
  {25, "JUMBO CASE"},
  {26, "JUMBO BOX"},
  {27, "JUMBO BAG"},
  {28, "JUMBO JAR"},
  {29, "JUMBO PACK"},
  {30, "JUMBO PKG"},
  {31, "JUMBO CAN"},
  {32, "JUMBO DRUM"},
  {33, "WRAP CASE"},
  {34, "WRAP BOX"},
  {35, "WRAP BAG"},
  {36, "WRAP JAR"},
  {37, "WRAP PACK"},
  {38, "WRAP PKG"},
  {39, "WRAP CAN"},
  {40, "WRAP DRUM"},
};
distribution jcch_p_cntr_set = {
  40,
  40,
  jcch_p_cntr_set_list,
  NULL
};
set_member jcch_colors_list[] = {
  {1, "almond"},
  {2, "antique"},
  {3, "aquamarine"},
  {4, "azure"},
  {5, "beige"},
  {6, "bisque"},
  {7, "black"},
  {8, "blanched"},
  {9, "blue"},
  {10, "blush"},
  {11, "brown"},
  {12, "burlywood"},
  {13, "burnished"},
  {14, "chartreuse"},
  {15, "chiffon"},
  {16, "chocolate"},
  {17, "coral"},
  {18, "cornflower"},
  {19, "cornsilk"},
  {20, "cream"},
  {21, "cyan"},
  {22, "dark"},
  {23, "deep"},
  {24, "dim"},
  {25, "dodger"},
  {26, "drab"},
  {27, "firebrick"},
  {28, "floral"},
  {29, "forest"},
  {30, "frosted"},
  {31, "gainsboro"},
  {32, "ghost"},
  {33, "goldenrod"},
  {34, "green"},
  {35, "grey"},
  {36, "honeydew"},
  {37, "hot"},
  {38, "indian"},
  {39, "ivory"},
  {40, "khaki"},
  {41, "lace"},
  {42, "lavender"},
  {43, "lawn"},
  {44, "lemon"},
  {45, "light"},
  {46, "lime"},
  {47, "linen"},
  {48, "magenta"},
  {49, "maroon"},
  {50, "medium"},
  {51, "metallic"},
  {52, "midnight"},
  {53, "mint"},
  {54, "misty"},
  {55, "moccasin"},
  {56, "navajo"},
  {57, "navy"},
  {58, "olive"},
  {59, "orange"},
  {60, "orchid"},
  {61, "pale"},
  {62, "papaya"},
  {63, "peach"},
  {64, "peru"},
  {65, "pink"},
  {66, "plum"},
  {67, "powder"},
  {68, "puff"},
  {69, "purple"},
  {70, "red"},
  {71, "rose"},
  {72, "rosy"},
  {73, "royal"},
  {74, "saddle"},
  {75, "salmon"},
  {76, "sandy"},
  {77, "seashell"},
  {78, "sienna"},
  {79, "sky"},
  {80, "slate"},
  {81, "smoke"},
  {82, "snow"},
  {83, "spring"},
  {84, "steel"},
  {85, "tan"},
  {86, "thistle"},
  {87, "tomato"},
  {88, "turquoise"},
  {89, "violet"},
  {90, "wheat"},
  {91, "white"},
  {92, "yellow"},
};
distribution jcch_colors = {
  92,
  92,
  jcch_colors_list,
  NULL
};
set_member jcch_p_types_set_list[] = {
  {1, "STANDARD ANODIZED TIN"},
  {2, "STANDARD ANODIZED NICKEL"},
  {3, "STANDARD ANODIZED BRASS"},
  {4, "STANDARD ANODIZED STEEL"},
  {5, "STANDARD ANODIZED COPPER"},
  {6, "STANDARD BURNISHED TIN"},
  {7, "STANDARD BURNISHED NICKEL"},
  {8, "STANDARD BURNISHED BRASS"},
  {9, "STANDARD BURNISHED STEEL"},
  {10, "STANDARD BURNISHED COPPER"},
  {11, "STANDARD PLATED TIN"},
  {12, "STANDARD PLATED NICKEL"},
  {13, "STANDARD PLATED BRASS"},
  {14, "STANDARD PLATED STEEL"},
  {15, "STANDARD PLATED COPPER"},
  {16, "STANDARD POLISHED TIN"},
  {17, "STANDARD POLISHED NICKEL"},
  {18, "STANDARD POLISHED BRASS"},
  {19, "STANDARD POLISHED STEEL"},
  {20, "STANDARD POLISHED COPPER"},
  {21, "STANDARD BRUSHED TIN"},
  {22, "STANDARD BRUSHED NICKEL"},
  {23, "STANDARD BRUSHED BRASS"},
  {24, "STANDARD BRUSHED STEEL"},
  {25, "STANDARD BRUSHED COPPER"},
  {26, "SMALL ANODIZED TIN"},
  {27, "SMALL ANODIZED NICKEL"},
  {28, "SMALL ANODIZED BRASS"},
  {29, "SMALL ANODIZED STEEL"},
  {30, "SMALL ANODIZED COPPER"},
  {31, "SMALL BURNISHED TIN"},
  {32, "SMALL BURNISHED NICKEL"},
  {33, "SMALL BURNISHED BRASS"},
  {34, "SMALL BURNISHED STEEL"},
  {35, "SMALL BURNISHED COPPER"},
  {36, "SMALL PLATED TIN"},
  {37, "SMALL PLATED NICKEL"},
  {38, "SMALL PLATED BRASS"},
  {39, "SMALL PLATED STEEL"},
  {40, "SMALL PLATED COPPER"},
  {41, "SMALL POLISHED TIN"},
  {42, "SMALL POLISHED NICKEL"},
  {43, "SMALL POLISHED BRASS"},
  {44, "SMALL POLISHED STEEL"},
  {45, "SMALL POLISHED COPPER"},
  {46, "SMALL BRUSHED TIN"},
  {47, "SMALL BRUSHED NICKEL"},
  {48, "SMALL BRUSHED BRASS"},
  {49, "SMALL BRUSHED STEEL"},
  {50, "SMALL BRUSHED COPPER"},
  {51, "MEDIUM ANODIZED TIN"},
  {52, "MEDIUM ANODIZED NICKEL"},
  {53, "MEDIUM ANODIZED BRASS"},
  {54, "MEDIUM ANODIZED STEEL"},
  {55, "MEDIUM ANODIZED COPPER"},
  {56, "MEDIUM BURNISHED TIN"},
  {57, "MEDIUM BURNISHED NICKEL"},
  {58, "MEDIUM BURNISHED BRASS"},
  {59, "MEDIUM BURNISHED STEEL"},
  {60, "MEDIUM BURNISHED COPPER"},
  {61, "MEDIUM PLATED TIN"},
  {62, "MEDIUM PLATED NICKEL"},
  {63, "MEDIUM PLATED BRASS"},
  {64, "MEDIUM PLATED STEEL"},
  {65, "MEDIUM PLATED COPPER"},
  {66, "MEDIUM POLISHED TIN"},
  {67, "MEDIUM POLISHED NICKEL"},
  {68, "MEDIUM POLISHED BRASS"},
  {69, "MEDIUM POLISHED STEEL"},
  {70, "MEDIUM POLISHED COPPER"},
  {71, "MEDIUM BRUSHED TIN"},
  {72, "MEDIUM BRUSHED NICKEL"},
  {73, "MEDIUM BRUSHED BRASS"},
  {74, "MEDIUM BRUSHED STEEL"},
  {75, "MEDIUM BRUSHED COPPER"},
  {76, "LARGE ANODIZED TIN"},
  {77, "LARGE ANODIZED NICKEL"},
  {78, "LARGE ANODIZED BRASS"},
  {79, "LARGE ANODIZED STEEL"},
  {80, "LARGE ANODIZED COPPER"},
  {81, "LARGE BURNISHED TIN"},
  {82, "LARGE BURNISHED NICKEL"},
  {83, "LARGE BURNISHED BRASS"},
  {84, "LARGE BURNISHED STEEL"},
  {85, "LARGE BURNISHED COPPER"},
  {86, "LARGE PLATED TIN"},
  {87, "LARGE PLATED NICKEL"},
  {88, "LARGE PLATED BRASS"},
  {89, "LARGE PLATED STEEL"},
  {90, "LARGE PLATED COPPER"},
  {91, "LARGE POLISHED TIN"},
  {92, "LARGE POLISHED NICKEL"},
  {93, "LARGE POLISHED BRASS"},
  {94, "LARGE POLISHED STEEL"},
  {95, "LARGE POLISHED COPPER"},
  {96, "LARGE BRUSHED TIN"},
  {97, "LARGE BRUSHED NICKEL"},
  {98, "LARGE BRUSHED BRASS"},
  {99, "LARGE BRUSHED STEEL"},
  {100, "LARGE BRUSHED COPPER"},
  {101, "ECONOMY ANODIZED TIN"},
  {102, "ECONOMY ANODIZED NICKEL"},
  {103, "ECONOMY ANODIZED BRASS"},
  {104, "ECONOMY ANODIZED STEEL"},
  {105, "ECONOMY ANODIZED COPPER"},
  {106, "ECONOMY BURNISHED TIN"},
  {107, "ECONOMY BURNISHED NICKEL"},
  {108, "ECONOMY BURNISHED BRASS"},
  {109, "ECONOMY BURNISHED STEEL"},
  {110, "ECONOMY BURNISHED COPPER"},
  {111, "ECONOMY PLATED TIN"},
  {112, "ECONOMY PLATED NICKEL"},
  {113, "ECONOMY PLATED BRASS"},
  {114, "ECONOMY PLATED STEEL"},
  {115, "ECONOMY PLATED COPPER"},
  {116, "ECONOMY POLISHED TIN"},
  {117, "ECONOMY POLISHED NICKEL"},
  {118, "ECONOMY POLISHED BRASS"},
  {119, "ECONOMY POLISHED STEEL"},
  {120, "ECONOMY POLISHED COPPER"},
  {121, "ECONOMY BRUSHED TIN"},
  {122, "ECONOMY BRUSHED NICKEL"},
  {123, "ECONOMY BRUSHED BRASS"},
  {124, "ECONOMY BRUSHED STEEL"},
  {125, "ECONOMY BRUSHED COPPER"},
  {126, "PROMO ANODIZED TIN"},
  {127, "PROMO ANODIZED NICKEL"},
  {128, "PROMO ANODIZED BRASS"},
  {129, "PROMO ANODIZED STEEL"},
  {130, "PROMO ANODIZED COPPER"},
  {131, "PROMO BURNISHED TIN"},
  {132, "PROMO BURNISHED NICKEL"},
  {133, "PROMO BURNISHED BRASS"},
  {134, "PROMO BURNISHED STEEL"},
  {135, "PROMO BURNISHED COPPER"},
  {136, "PROMO PLATED TIN"},
  {137, "PROMO PLATED NICKEL"},
  {138, "PROMO PLATED BRASS"},
  {139, "PROMO PLATED STEEL"},
  {140, "PROMO PLATED COPPER"},
  {141, "PROMO POLISHED TIN"},
  {142, "PROMO POLISHED NICKEL"},
  {143, "PROMO POLISHED BRASS"},
  {144, "PROMO POLISHED STEEL"},
  {145, "PROMO POLISHED COPPER"},
  {146, "PROMO BRUSHED TIN"},
  {147, "PROMO BRUSHED NICKEL"},
  {148, "PROMO BRUSHED BRASS"},
  {149, "PROMO BRUSHED STEEL"},
  {150, "PROMO BRUSHED COPPER"},
};
distribution jcch_p_types_set = {
  150,
  150,
  jcch_p_types_set_list,
  NULL
};
set_member jcch_nations_list[NATIONS_COUNT] = {
  {0, "ALGERIA"},
  {1, "ARGENTINA"},
  {1, "BRAZIL"},
  {1, "CANADA"},
  {4, "EGYPT"},
  {0, "ETHIOPIA"},
  {3, "FRANCE"},
  {3, "GERMANY"},
  {2, "INDIA"},
  {2, "INDONESIA"},
  {4, "IRAN"},
  {4, "IRAQ"},
  {2, "JAPAN"},
  {4, "JORDAN"},
  {0, "KENYA"},
  {0, "MOROCCO"},
  {0, "MOZAMBIQUE"},
  {1, "PERU"},
  {2, "CHINA"},
  {3, "ROMANIA"},
  {4, "SAUDI ARABIA"},
  {2, "VIETNAM"},
  {3, "RUSSIA"},
  {3, "UNITED KINGDOM"},
  {1, "UNITED STATES"},
};
distribution jcch_nations = {
  NATIONS_COUNT,
  1,
  jcch_nations_list,
  NULL
};
set_member jcch_regions_list[REGIONS_COUNT] = {
  {1, "AFRICA"},
  {2, "AMERICA"},
  {3, "ASIA"},
  {4, "EUROPE"},
  {5, "MIDDLE EAST"},
};
distribution jcch_regions = {
  REGIONS_COUNT,
  5,
  jcch_regions_list,
  NULL
};
set_member jcch_o_priority_set_list[] = {
  {1, "1-URGENT"},
  {2, "2-HIGH"},
  {3, "3-MEDIUM"},
  {4, "4-NOT SPECIFIED"},
  {5, "5-LOW"},
};
distribution jcch_o_priority_set = {
  5,
  5,
  jcch_o_priority_set_list,
  NULL
};
set_member jcch_l_instruct_set_list[] = {
  {1, "DELIVER IN PERSON"},
  {2, "COLLECT COD"},
  {3, "TAKE BACK RETURN"},
  {4, "NONE"},
};
distribution jcch_l_instruct_set = {
  4,
  4,
  jcch_l_instruct_set_list,
  NULL
};
set_member jcch_l_smode_set_list[] = {
  {1, "REG AIR"},
  {2, "AIR"},
  {3, "RAIL"},
  {4, "TRUCK"},
  {5, "MAIL"},
  {6, "FOB"},
  {7, "SHIP"},
};
distribution jcch_l_smode_set = {
  7,
  7,
  jcch_l_smode_set_list,
  NULL
};
set_member jcch_l_category_set_list[] = {
  {1, "FURNITURE"},
  {2, "STORAGE EQUIP"},
  {3, "TOOLS"},
  {4, "MACHINE TOOLS"},
  {5, "OTHER"},
};
distribution jcch_l_category_set = {
  5,
  5,
  jcch_l_category_set_list,
  NULL
};
set_member jcch_l_rflag_set_list[] = {
  {1, "R"},
  {2, "A"},
};
distribution jcch_l_rflag_set = {
  2,
  2,
  jcch_l_rflag_set_list,
  NULL
};
set_member jcch_c_mseg_set_list[] = {
  {1, "AUTOMOBILE"},
  {2, "BUILDING"},
  {3, "FURNITURE"},
  {4, "HOUSEHOLD"},
  {5, "MACHINERY"},
};
distribution jcch_c_mseg_set = {
  5,
  5,
  jcch_c_mseg_set_list,
  NULL
};
set_member jcch_nouns_list[] = {
  {40, "packages"},
  {80, "requests"},
  {120, "accounts"},
  {160, "deposits"},
  {180, "foxes"},
  {200, "ideas"},
  {220, "theodolites"},
  {240, "pinto beans"},
  {260, "instructions"},
  {270, "dependencies"},
  {280, "excuses"},
  {290, "platelets"},
  {300, "asymptotes"},
  {305, "courts"},
  {310, "dolphins"},
  {311, "multipliers"},
  {312, "sauternes"},
  {313, "warthogs"},
  {314, "frets"},
  {315, "dinos"},
  {316, "attainments"},
  {317, "somas"},
  {318, "Tiresias"},
  {319, "patterns"},
  {320, "forges"},
  {321, "braids"},
  {322, "frays"},
  {323, "warhorses"},
  {324, "dugouts"},
  {325, "notornis"},
  {326, "epitaphs"},
  {327, "pearls"},
  {328, "tithes"},
  {329, "waters"},
  {330, "orbits"},
  {331, "gifts"},
  {332, "sheaves"},
  {333, "depths"},
  {334, "sentiments"},
  {335, "decoys"},
  {336, "realms"},
  {337, "pains"},
  {338, "grouches"},
  {339, "escapades"},
  {340, "hockey players"},
};
distribution jcch_nouns = {
  45,
  340,
  jcch_nouns_list,
  NULL
};
set_member jcch_verbs_list[] = {
  {20, "sleep"},
  {40, "wake"},
  {60, "are"},
  {80, "cajole"},
  {100, "haggle"},
  {110, "nag"},
  {120, "use"},
  {130, "boost"},
  {135, "affix"},
  {140, "detect"},
  {145, "integrate"},
  {146, "maintain"},
  {147, "nod"},
  {148, "was"},
  {149, "lose"},
  {150, "sublate"},
  {151, "solve"},
  {152, "thrash"},
  {153, "promise"},
  {154, "engage"},
  {155, "hinder"},
  {156, "print"},
  {157, "x-ray"},
  {158, "breach"},
  {159, "eat"},
  {160, "grow"},
  {161, "impress"},
  {162, "mold"},
  {163, "poach"},
  {164, "serve"},
  {165, "run"},
  {166, "dazzle"},
  {167, "snooze"},
  {168, "doze"},
  {169, "unwind"},
  {170, "kindle"},
  {171, "play"},
  {172, "hang"},
  {173, "believe"},
  {174, "doubt"},
};
distribution jcch_verbs = {
  40,
  174,
  jcch_verbs_list,
  NULL
};
set_member jcch_adjectives_list[] = {
  {20, "special"},
  {40, "pending"},
  {60, "unusual"},
  {80, "express"},
  {81, "furious"},
  {82, "sly"},
  {83, "careful"},
  {84, "blithe"},
  {85, "quick"},
  {86, "fluffy"},
  {87, "slow"},
  {88, "quiet"},
  {89, "ruthless"},
  {90, "thin"},
  {91, "close"},
  {92, "dogged"},
  {93, "daring"},
  {94, "brave"},
  {95, "stealthy"},
  {96, "permanent"},
  {97, "enticing"},
  {98, "idle"},
  {99, "busy"},
  {149, "regular"},
  {189, "final"},
  {229, "ironic"},
  {259, "even"},
  {279, "bold"},
  {289, "silent"},
};
distribution jcch_adjectives = {
  29,
  289,
  jcch_adjectives_list,
  NULL
};
set_member jcch_adverbs_list[] = {
  {1, "sometimes"},
  {2, "always"},
  {3, "never"},
  {53, "furiously"},
  {103, "slyly"},
  {153, "carefully"},
  {193, "blithely"},
  {223, "quickly"},
  {243, "fluffily"},
  {244, "slowly"},
  {245, "quietly"},
  {246, "ruthlessly"},
  {247, "thinly"},
  {248, "closely"},
  {249, "doggedly"},
  {250, "daringly"},
  {251, "bravely"},
  {252, "stealthily"},
  {253, "permanently"},
  {254, "enticingly"},
  {255, "idly"},
  {256, "busily"},
  {257, "regularly"},
  {258, "finally"},
  {259, "ironically"},
  {260, "evenly"},
  {261, "boldly"},
  {262, "silently"},
};
distribution jcch_adverbs = {
  28,
  262,
  jcch_adverbs_list,
  NULL
};
set_member jcch_auxillaries_list[] = {
  {1, "do"},
  {2, "may"},
  {3, "might"},
  {4, "shall"},
  {5, "will"},
  {6, "would"},
  {7, "can"},
  {8, "could"},
  {9, "should"},
  {10, "ought to"},
  {11, "must"},
  {12, "will have to"},
  {13, "shall have to"},
  {14, "could have to"},
  {15, "should have to"},
  {16, "must have to"},
  {17, "need to"},
  {18, "try to"},
};
distribution jcch_auxillaries = {
  18,
  18,
  jcch_auxillaries_list,
  NULL
};
set_member jcch_terminators_list[] = {
  {50, "."},
  {51, ";"},
  {52, ":"},
  {53, "?"},
  {54, "!"},
  {55, "--"},
};
distribution jcch_terminators = {
  6,
  55,
  jcch_terminators_list,
  NULL
};
set_member jcch_articles_list[] = {
  {50, "the"},
  {70, "a"},
  {75, "an"},
};
distribution jcch_articles = {
  3,
  75,
  jcch_articles_list,
  NULL
};
set_member jcch_prepositions_list[] = {
  {50, "about"},
  {100, "above"},
  {150, "according to"},
  {200, "across"},
  {250, "after"},
  {290, "against"},
  {330, "along"},
  {360, "alongside of"},
  {390, "among"},
  {410, "around"},
  {420, "at"},
  {421, "atop"},
  {422, "before"},
  {423, "behind"},
  {424, "beneath"},
  {425, "beside"},
  {426, "besides"},
  {427, "between"},
  {428, "beyond"},
  {429, "by"},
  {430, "despite"},
  {431, "during"},
  {432, "except"},
  {433, "for"},
  {434, "from"},
  {435, "in place of"},
  {436, "inside"},
  {437, "instead of"},
  {438, "into"},
  {439, "near"},
  {440, "of"},
  {441, "on"},
  {442, "outside"},
  {443, "over"},
  {444, "past"},
  {445, "since"},
  {446, "through"},
  {447, "throughout"},
  {448, "to"},
  {449, "toward"},
  {450, "under"},
  {451, "until"},
  {452, "up"},
  {453, "upon"},
  {454, "whithout"},
  {455, "with"},
  {456, "within"},
};
distribution jcch_prepositions = {
  47,
  456,
  jcch_prepositions_list,
  NULL
};
set_member jcch_grammar_list[] = {
  {3, "N V T"},
  {6, "N V P T"},
  {9, "N V N T"},
  {10, "N P V N T"},
  {11, "N P V P T"},
};
distribution jcch_grammar = {
  5,
  11,
  jcch_grammar_list,
  NULL
};
set_member jcch_np_list[] = {
  {10, "N"},
  {30, "J N"},
  {40, "J, J N"},
  {90, "D J N"},
};
distribution jcch_np = {
  4,
  90,
  jcch_np_list,
  NULL
};
set_member jcch_vp_list[] = {
  {30, "V"},
  {31, "X V"},
  {71, "V D"},
  {72, "X V D"},
};
distribution jcch_vp = {
  4,
  72,
  jcch_vp_list,
  NULL
};

order_t order;
part_t part;

double   jcch_dM = 2147483647.0;

seed_t     jcch_Seed[MAX_STREAM + 1] =
{
    {JCCH_PART,   1,          0,	1},					/* P_MFG_SD     0 */
    {JCCH_PART,   46831694,   0, 1},					/* P_BRND_SD    1 */
    {JCCH_PART,   1841581359, 0, 1},					/* P_TYPE_SD    2 */
    {JCCH_PART,   1193163244, 0, 1},					/* P_SIZE_SD    3 */
    {JCCH_PART,   727633698,  0, 1},					/* P_CNTR_SD    4 */
    {NONE,   933588178,  0, 1},					/* text pregeneration  5 */
    {JCCH_PART,   804159733,  0, 2},	/* P_CMNT_SD    6 */
    {JCCH_PSUPP,  1671059989, 0, SUPP_PER_PART},     /* PS_QTY_SD    7 */
    {JCCH_PSUPP,  1051288424, 0, SUPP_PER_PART},     /* PS_SCST_SD   8 */
    {JCCH_PSUPP,  1961692154, 0, SUPP_PER_PART * 2},     /* PS_CMNT_SD   9 */
    {JCCH_ORDER,  1227283347, 0, 1},				    /* O_SUPP_SD    10 */
    {JCCH_ORDER,  1171034773, 0, 1},					/* O_CLRK_SD    11 */
    {JCCH_ORDER,  276090261,  0, 2},  /* O_CMNT_SD    12 */
	{JCCH_ORDER,  1066728069, 0, 1},					/* O_ODATE_SD   13 */
    {JCCH_LINE,   209208115,  0, O_LCNT_MAX},        /* L_QTY_SD     14 */
    {JCCH_LINE,   554590007,  0, O_LCNT_MAX},        /* L_DCNT_SD    15 */
    {JCCH_LINE,   721958466,  0, O_LCNT_MAX},        /* L_TAX_SD     16 */
    {JCCH_LINE,   1371272478, 0, O_LCNT_MAX},        /* L_SHIP_SD    17 */
    {JCCH_LINE,   675466456,  0, O_LCNT_MAX},        /* L_SMODE_SD   18 */
    {JCCH_LINE,   1808217256, 0, O_LCNT_MAX},      /* L_PKEY_SD    19 */
    {JCCH_LINE,   2095021727, 0, O_LCNT_MAX},      /* L_SKEY_SD    20 */
    {JCCH_LINE,   1769349045, 0, O_LCNT_MAX},      /* L_SDTE_SD    21 */
    {JCCH_LINE,   904914315,  0, O_LCNT_MAX},      /* L_CDTE_SD    22 */
    {JCCH_LINE,   373135028,  0, O_LCNT_MAX},      /* L_RDTE_SD    23 */
    {JCCH_LINE,   717419739,  0, O_LCNT_MAX},      /* L_RFLG_SD    24 */
    {JCCH_LINE,   1095462486, 0, O_LCNT_MAX * 2},   /* L_CMNT_SD    25 */
    {JCCH_CUST,   881155353,  0, 9},      /* C_ADDR_SD    26 */
    {JCCH_CUST,   1489529863, 0, 1},      /* C_NTRG_SD    27 */
    {JCCH_CUST,   1521138112, 0, 3},      /* C_PHNE_SD    28 */
    {JCCH_CUST,   298370230,  0, 1},      /* C_ABAL_SD    29 */
    {JCCH_CUST,   1140279430, 0, 1},      /* C_MSEG_SD    30 */
    {JCCH_CUST,   1335826707, 0, 2},     /* C_CMNT_SD    31 */
    {JCCH_SUPP,   706178559,  0, 9},      /* S_ADDR_SD    32 */
    {JCCH_SUPP,   110356601,  0, 1},      /* S_NTRG_SD    33 */
    {JCCH_SUPP,   884434366,  0, 3},      /* S_PHNE_SD    34 */
    {JCCH_SUPP,   962338209,  0, 1},      /* S_ABAL_SD    35 */
    {JCCH_SUPP,   1341315363, 0, 2},     /* S_CMNT_SD    36 */
    {JCCH_PART,   709314158,  0, 92},      /* P_NAME_SD    37 */
    {JCCH_ORDER,  591449447,  0, 1},      /* O_PRIO_SD    38 */
    {JCCH_LINE,   431918286,  0, 1},      /* HVAR_SD      39 */
    {JCCH_ORDER,  851767375,  0, 1},      /* O_CKEY_SD    40 */
    {JCCH_NATION, 606179079,  0, 2},      /* N_CMNT_SD    41 */
    {JCCH_REGION, 1500869201, 0, 2},      /* R_CMNT_SD    42 */
    {JCCH_ORDER,  1434868289, 0, 1},      /* O_LCNT_SD    43 */
    {JCCH_SUPP,   263032577,  0, 1},      /* BBB offset   44 */
    {JCCH_SUPP,   753643799,  0, 1},      /* BBB type     45 */
    {JCCH_SUPP,   202794285,  0, 1},      /* BBB comment  46 */
    {JCCH_SUPP,   715851524,  0, 1}       /* BBB junk     47 */
};

static seed_t mk_seed(long jcch_table, JCCH_DSS_HUGE value, JCCH_DSS_HUGE jcch_usage, JCCH_DSS_HUGE boundary) {
  seed_t seed = {.jcch_table=jcch_table, .value=value, .jcch_usage=jcch_usage, .boundary=boundary};
  return seed;
}

void jcch_dbgen_reset_seeds() {
  jcch_Seed[0] = mk_seed(JCCH_PART,   1,          0,	1);					/* P_MFG_SD     0 */
  jcch_Seed[1] = mk_seed(JCCH_PART,   46831694,   0, 1);					/* P_BRND_SD    1 */
  jcch_Seed[2] = mk_seed(JCCH_PART,   1841581359, 0, 1);					/* P_TYPE_SD    2 */
  jcch_Seed[3] = mk_seed(JCCH_PART,   1193163244, 0, 1);					/* P_SIZE_SD    3 */
  jcch_Seed[4] = mk_seed(JCCH_PART,   727633698,  0, 1);					/* P_CNTR_SD    4 */
  jcch_Seed[5] = mk_seed(NONE,   933588178,  0, 1);					/* text pregeneration  5 */
  jcch_Seed[6] = mk_seed(JCCH_PART,   804159733,  0, 2);	/* P_CMNT_SD    6 */
  jcch_Seed[7] = mk_seed(JCCH_PSUPP,  1671059989, 0, SUPP_PER_PART);     /* PS_QTY_SD    7 */
  jcch_Seed[8] = mk_seed(JCCH_PSUPP,  1051288424, 0, SUPP_PER_PART);     /* PS_SCST_SD   8 */
  jcch_Seed[9] = mk_seed(JCCH_PSUPP,  1961692154, 0, SUPP_PER_PART * 2);     /* PS_CMNT_SD   9 */
  jcch_Seed[10] = mk_seed(JCCH_ORDER,  1227283347, 0, 1);				    /* O_SUPP_SD    10 */
  jcch_Seed[11] = mk_seed(JCCH_ORDER,  1171034773, 0, 1);					/* O_CLRK_SD    11 */
  jcch_Seed[12] = mk_seed(JCCH_ORDER,  276090261,  0, 2);  /* O_CMNT_SD    12 */
  jcch_Seed[13] = mk_seed(JCCH_ORDER,  1066728069, 0, 1);					/* O_ODATE_SD   13 */
  jcch_Seed[14] = mk_seed(JCCH_LINE,   209208115,  0, O_LCNT_MAX);        /* L_QTY_SD     14 */
  jcch_Seed[15] = mk_seed(JCCH_LINE,   554590007,  0, O_LCNT_MAX);        /* L_DCNT_SD    15 */
  jcch_Seed[16] = mk_seed(JCCH_LINE,   721958466,  0, O_LCNT_MAX);        /* L_TAX_SD     16 */
  jcch_Seed[17] = mk_seed(JCCH_LINE,   1371272478, 0, O_LCNT_MAX);        /* L_SHIP_SD    17 */
  jcch_Seed[18] = mk_seed(JCCH_LINE,   675466456,  0, O_LCNT_MAX);        /* L_SMODE_SD   18 */
  jcch_Seed[19] = mk_seed(JCCH_LINE,   1808217256, 0, O_LCNT_MAX);      /* L_PKEY_SD    19 */
  jcch_Seed[20] = mk_seed(JCCH_LINE,   2095021727, 0, O_LCNT_MAX);      /* L_SKEY_SD    20 */
  jcch_Seed[21] = mk_seed(JCCH_LINE,   1769349045, 0, O_LCNT_MAX);      /* L_SDTE_SD    21 */
  jcch_Seed[22] = mk_seed(JCCH_LINE,   904914315,  0, O_LCNT_MAX);      /* L_CDTE_SD    22 */
  jcch_Seed[23] = mk_seed(JCCH_LINE,   373135028,  0, O_LCNT_MAX);      /* L_RDTE_SD    23 */
  jcch_Seed[24] = mk_seed(JCCH_LINE,   717419739,  0, O_LCNT_MAX);      /* L_RFLG_SD    24 */
  jcch_Seed[25] = mk_seed(JCCH_LINE,   1095462486, 0, O_LCNT_MAX * 2);   /* L_CMNT_SD    25 */
  jcch_Seed[26] = mk_seed(JCCH_CUST,   881155353,  0, 9);      /* C_ADDR_SD    26 */
  jcch_Seed[27] = mk_seed(JCCH_CUST,   1489529863, 0, 1);      /* C_NTRG_SD    27 */
  jcch_Seed[28] = mk_seed(JCCH_CUST,   1521138112, 0, 3);      /* C_PHNE_SD    28 */
  jcch_Seed[29] = mk_seed(JCCH_CUST,   298370230,  0, 1);      /* C_ABAL_SD    29 */
  jcch_Seed[30] = mk_seed(JCCH_CUST,   1140279430, 0, 1);      /* C_MSEG_SD    30 */
  jcch_Seed[31] = mk_seed(JCCH_CUST,   1335826707, 0, 2);     /* C_CMNT_SD    31 */
  jcch_Seed[32] = mk_seed(JCCH_SUPP,   706178559,  0, 9);      /* S_ADDR_SD    32 */
  jcch_Seed[33] = mk_seed(JCCH_SUPP,   110356601,  0, 1);      /* S_NTRG_SD    33 */
  jcch_Seed[34] = mk_seed(JCCH_SUPP,   884434366,  0, 3);      /* S_PHNE_SD    34 */
  jcch_Seed[35] = mk_seed(JCCH_SUPP,   962338209,  0, 1);      /* S_ABAL_SD    35 */
  jcch_Seed[36] = mk_seed(JCCH_SUPP,   1341315363, 0, 2);     /* S_CMNT_SD    36 */
  jcch_Seed[37] = mk_seed(JCCH_PART,   709314158,  0, 92);      /* P_NAME_SD    37 */
  jcch_Seed[38] = mk_seed(JCCH_ORDER,  591449447,  0, 1);      /* O_PRIO_SD    38 */
  jcch_Seed[39] = mk_seed(JCCH_LINE,   431918286,  0, 1);      /* HVAR_SD      39 */
  jcch_Seed[40] = mk_seed(JCCH_ORDER,  851767375,  0, 1);      /* O_CKEY_SD    40 */
  jcch_Seed[41] = mk_seed(JCCH_NATION, 606179079,  0, 2);      /* N_CMNT_SD    41 */
  jcch_Seed[42] = mk_seed(JCCH_REGION, 1500869201, 0, 2);      /* R_CMNT_SD    42 */
  jcch_Seed[43] = mk_seed(JCCH_ORDER,  1434868289, 0, 1);      /* O_LCNT_SD    43 */
  jcch_Seed[44] = mk_seed(JCCH_SUPP,   263032577,  0, 1);      /* BBB offset   44 */
  jcch_Seed[45] = mk_seed(JCCH_SUPP,   753643799,  0, 1);      /* BBB type     45 */
  jcch_Seed[46] = mk_seed(JCCH_SUPP,   202794285,  0, 1);      /* BBB comment  46 */
  jcch_Seed[47] = mk_seed(JCCH_SUPP,   715851524,  0, 1);       /* BBB junk     47 */
}

// Copied from driver.c:process_options()
void jcch_dbgen_init_scale_factor(float scale_factor) {
  jcch_tdefs[JCCH_PART].base = 200000;
  jcch_tdefs[JCCH_PSUPP].base = 200000;
  jcch_tdefs[JCCH_SUPP].base = 10000;
  jcch_tdefs[JCCH_CUST].base = 150000;
  jcch_tdefs[JCCH_ORDER].base = 150000;
  jcch_tdefs[JCCH_LINE].base = 150000;
  jcch_tdefs[ORDER_LINE].base = 150000;
  jcch_tdefs[PART_PSUPP].base = 200000;
  jcch_tdefs[JCCH_NATION].base = NATIONS_COUNT;
  jcch_tdefs[JCCH_REGION].base = REGIONS_COUNT;

  jcch_flt_scale = scale_factor;
  if (jcch_flt_scale < MIN_SCALE)
  {
    int i;
    int int_scale;

    jcch_scale = 1;
    int_scale = (int)(round(10000 * jcch_flt_scale));
    for (i = JCCH_PART; i < JCCH_REGION; i++)
    {
      jcch_tdefs[i].base = (JCCH_DSS_HUGE)(int_scale * jcch_tdefs[i].base)/10000;
      if (jcch_tdefs[i].base < 1)
        jcch_tdefs[i].base = 1;
    }
  }
  else
    jcch_scale = (long) jcch_flt_scale;
  if (jcch_scale > MAX_SCALE)
  {
    fprintf (stderr, "%s %5.0f %s\n\t%s\n\n",
             "NOTE: Data generation for jcch_scale factors >",
             MAX_SCALE,
             "GB is still in development,",
             "and is not yet supported.\n");
    fprintf (stderr,
             "Your resulting data set MAY NOT BE COMPLIANT!\n");
  }

  // tpch_dbgen does the same in main() - weird thing to do, should be sufficient to omit scaling down
  // `jcch_tdefs[JCCH_NATION].base` above... Anyway, let's do what they do.
  jcch_tdefs[JCCH_NATION].base = jcch_nations.count;
  jcch_tdefs[JCCH_REGION].base = jcch_regions.count;

  // Copied from tpch_dbgen as well.
  jcch_tdefs[JCCH_ORDER].base *= ORDERS_PER_CUST;
  jcch_tdefs[JCCH_LINE].base *= ORDERS_PER_CUST;
  jcch_tdefs[ORDER_LINE].base *= ORDERS_PER_CUST;
}

/**
 * Call this after using dbgen to avoid memory leaks
 */
void jcch_dbgen_cleanup() {
  free(jcch_nations.permute);
  jcch_nations.permute = NULL;

  free(jcch_regions.permute);
  jcch_regions.permute = NULL;

  free(jcch_o_priority_set.permute);
  jcch_o_priority_set.permute = NULL;

  free(jcch_l_instruct_set.permute);
  jcch_l_instruct_set.permute = NULL;

  free(jcch_l_smode_set.permute);
  jcch_l_smode_set.permute = NULL;

  free(jcch_l_category_set.permute);
  jcch_l_category_set.permute = NULL;

  free(jcch_l_rflag_set.permute);
  jcch_l_rflag_set.permute = NULL;

  free(jcch_c_mseg_set.permute);
  jcch_c_mseg_set.permute = NULL;

  free(jcch_colors.permute);
  jcch_colors.permute = NULL;

  free(jcch_p_types_set.permute);
  jcch_p_types_set.permute = NULL;

  free(jcch_p_cntr_set.permute);
  jcch_p_cntr_set.permute = NULL;

  free(jcch_articles.permute);
  jcch_articles.permute = NULL;

  free(jcch_nouns.permute);
  jcch_nouns.permute = NULL;

  free(jcch_adjectives.permute);
  jcch_adjectives.permute = NULL;

  free(jcch_adverbs.permute);
  jcch_adverbs.permute = NULL;

  free(jcch_prepositions.permute);
  jcch_prepositions.permute = NULL;

  free(jcch_verbs.permute);
  jcch_verbs.permute = NULL;

  free(jcch_terminators.permute);
  jcch_terminators.permute = NULL;

  free(jcch_auxillaries.permute);
  jcch_auxillaries.permute = NULL;

  free(jcch_np.permute);
  jcch_np.permute = NULL;

  free(jcch_vp.permute);
  jcch_vp.permute = NULL;

  free(jcch_grammar.permute);
  jcch_grammar.permute = NULL;

  if (jcch_asc_date) {
    for (long long idx = 0; idx < TOTDATE; ++idx) {
      free(jcch_asc_date[idx]);
    }
    free(jcch_asc_date);
  }
  jcch_asc_date = NULL;
}
