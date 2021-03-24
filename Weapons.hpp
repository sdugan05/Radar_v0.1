//
// Created by Saul on 3/24/2021.
//

#ifndef RADAR_V0_1_WEAPONS_HPP
#define RADAR_V0_1_WEAPONS_HPP

#include <iostream>
#include <vector>

class Weapons {
    enum weapons : int {
        DEAGLE = 1,
        ELITE = 2,
        FIVESEVEN = 3,
        GLOCK = 4,
        AK47 = 7,
        AUG = 8,
        AWP = 9,
        FAMAS = 10,
        G3SG1 = 11,
        GALIL = 13,
        M249 = 14,
        M4A4 = 16,
        MAC10 = 17,
        P90 = 19,
        UMP45 = 24,
        XM1014 = 25,
        BIZON = 26,
        MAG7 = 27,
        NEGEV = 28,
        SAWEDOFF = 29,
        TEC9 = 30,
        ZEUS = 31,
        P2000 = 32,
        MP7 = 33,
        MP9 = 34,
        NOVA = 35,
        P250 = 36,
        SCAR20 = 38,
        SG553 = 39,
        SSG08 = 40,
        KNIFE_T = 42,
        FLASHBANG = 43,
        HEGRENADE = 44,
        SMOKE = 45,
        MOLOTOV = 46,
        DECOY = 47,
        FIREBOMB = 48,
        C4 = 49,
        MUSICKIT = 58,
        KNIFE_CT = 59,
        M4A1S = 60,
        USPS = 61,
        TRADEUPCONTRACT = 62,
        CZ75A = 63,
        REVOLVER = 64,
        KNIFE_BAYONET = 500,
        KNIFE_FLIP = 505,
        KNIFE_GUT = 506,
        KNIFE_KARAMBIT = 507,
        KNIFE_M9_BAYONET = 508,
        KNIFE_HUNTSMAN = 509,
        KNIFE_FALCHION = 512,
        KNIFE_BOWIE = 514,
        KNIFE_BUTTERFLY = 515,
        KNIFE_SHADOW_DAGGERS = 516,
        LAST
    };

public:
    static const std::vector<const char*> weapon_names;
};


#endif //RADAR_V0_1_WEAPONS_HPP
