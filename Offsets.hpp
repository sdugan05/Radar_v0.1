//
// Created by Saul on 3/24/2021.
//

#ifndef RADAR_V0_1_OFFSETS_HPP
#define RADAR_V0_1_OFFSETS_HPP

#include <iostream>
#include "utils/process/process.hpp"


class Offsets {
    public: static void generate_offsets();

public:
    static DWORD global_pointer;
    static DWORD local_player;
    static DWORD client_state;
    static DWORD client_state_local_player;
    static DWORD client_state_player_info;
    static DWORD client_state_map;
    static DWORD client_state_map_directory;
    static DWORD game_dir;
    static DWORD entity_list;
    static DWORD radar_base;
    static DWORD dormant;
    static DWORD force_jump;

    static const char* game_path;

    static constexpr ptrdiff_t m_fFlags = 0x104;



};


#endif //RADAR_V0_1_OFFSETS_HPP
