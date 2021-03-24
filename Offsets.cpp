//
// Created by Saul on 3/24/2021.
//

#include "Offsets.hpp"

DWORD Offsets::global_pointer;
DWORD Offsets::local_player;
DWORD Offsets::client_state;
DWORD Offsets::client_state_local_player;
DWORD Offsets::client_state_player_info;
DWORD Offsets::client_state_map;
DWORD Offsets::client_state_map_directory;
DWORD Offsets::game_dir;
DWORD Offsets::entity_list;
DWORD Offsets::radar_base;
DWORD Offsets::dormant;
DWORD Offsets::force_jump;


const char* Offsets::game_path;


void Offsets::generate_offsets() {
    // Generate Offsets

    global_pointer = process->scan(process->client[0], process->client[1], "\xA1\x00\x00\x00\x00\xF3\x0F\x10\x40\x10", "x????xxxxx") + 1;
    global_pointer = process->read<DWORD>(global_pointer);

    local_player = process->scan(process->client[0], process->client[1], "\x8D\x34\x85\x00\x00\x00\x00\x89\x15\x00\x00\x00\x00\x8B\x41\x08\x8B\x48\x04\x83\xF9\xFF", "xxx????xx????xxxxxxxxx") + 3;
    local_player = process->read<DWORD>(local_player) + 4;

    client_state = process->scan(process->engine[0], process->engine[1], "\xA1\x00\x00\x00\x00\x33\xD2\x6A\x00\x6A\x00\x33\xC9\x89\xB0", "x????xxxxxxxxxx") + 1;
    client_state = process->read<DWORD>(client_state);

    client_state_local_player = process->scan(process->engine[0], process->engine[1], "\x8B\x80\x00\x00\x00\x00\x40\xC3", "xx????xx") + 2;
    client_state_local_player = process->read<DWORD>(client_state_local_player);

    client_state_player_info = process->scan(process->engine[0], process->engine[1], "\x8B\x89\x00\x00\x00\x00\x85\xC9\x0F\x84\x00\x00\x00\x00\x8B\x01", "xx????xxxx????xx") + 2;
    client_state_player_info = process->read<DWORD>(client_state_player_info);

    client_state_map = process->scan(process->engine[0], process->engine[1], "\x05\x00\x00\x00\x00\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xA1", "x????xxxxxxxxx") + 1;
    client_state_map = process->read<DWORD>(client_state_map);

    client_state_map_directory = process->scan(process->engine[0], process->engine[1], "\xB8\x00\x00\x00\x00\xC3\x05\x00\x00\x00\x00\xC3", "x????xx????x") + 7;
    client_state_map_directory = process->read<DWORD>(client_state_map_directory);

    game_dir = process->scan(process->engine[0], process->engine[1], "\x68\x00\x00\x00\x00\x8D\x85\x00\x00\x00\x00\x50\x68\x00\x00\x00\x00\x68", "x????xx????xx????x") + 1;
    game_dir = process->read<DWORD>(game_dir);
    const char* path[MAX_PATH];
    Offsets::game_path = *path;
    ReadProcessMemory(process->process, reinterpret_cast<LPCVOID>(game_dir), &path, sizeof(path), nullptr);

    entity_list = process->scan(process->client[0], process->client[1], "\xBB\x00\x00\x00\x00\x83\xFF\x01\x0F\x8C\x00\x00\x00\x00\x3B\xF8", "x????xxxxx????xx") + 1;
    entity_list = process->read<DWORD>(entity_list);

    radar_base = process->scan(process->client[0], process->client[1], "\xA1\x00\x00\x00\x00\x8B\x0C\xB0\x8B\x01\xFF\x50\x00\x46\x3B\x35\x00\x00\x00\x00\x7C\xEA\x8B\x0D", "x????xxxxxxx?xxx????xxxx") + 1;
    radar_base = process->read<DWORD>(radar_base);

    dormant = process->scan(process->client[0], process->client[1], "\x8A\x81\x00\x00\x00\x00\xC3\x32\xC0", "xx????xxx") + 2;
    dormant = process->read<DWORD>(dormant) + 8;

    force_jump = process->scan(process->client[0], process->client[1], "\x8B\x0D\x00\x00\x00\x00\x8B\xD6\x8B\xC1\x83\xCA\x02", "xx????xxxxxxx") + 2;
    force_jump = process->read<DWORD>(force_jump);



    std::cout << "global_pointer = " << std::hex << global_pointer << std::endl;
    std::cout << "local_player = " << std::hex << local_player << "(" << local_player - process->client[0] << ")" << std::endl;
    std::cout << "client_state = " << std::hex << client_state << "(" << client_state - process->engine[0] << ")" << std::endl;
    std::cout << "client_state_local_player = " << std::hex << client_state_local_player << std::endl;
    std::cout << "client_state_player_info = " << std::hex << client_state_player_info << std::endl;
    std::cout << "client_state_map = " << std::hex << client_state_map << std::endl;
    std::cout << "client_state_map_directory = " << std::hex << client_state_map_directory << std::endl;
    std::cout << "game_dir = " << std::hex << game_dir << "(" << game_dir - process->engine[0] << ")" << " " << reinterpret_cast<const char*>(path) << std::endl;
    std::cout << "entity_list = " << std::hex << entity_list << "(" << entity_list - process->client[0] << ")" << std::endl;
    std::cout << "radar_base = " << std::hex << radar_base << "(" << radar_base - process->client[0] << ")" << std::endl;
    std::cout << "dormant = " << std::hex << dormant << std::endl;
    std::cout << "force_jump = " << std::hex << force_jump << "(" << force_jump - process->client[0] << ")"<< std::endl;


    std::cout << entity_list << std::endl;
}
