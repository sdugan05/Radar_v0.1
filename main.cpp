#include <iostream>
#include <thread>

#include "utils/process/process.hpp"
#include "utils/Vector2.hpp"

#include "Weapons.hpp"

#include "Offsets.hpp"

#include "Player.hpp"


int main() {
    std::cout << "LC's Radar" << std::endl;

    std::cout << "Waiting for Counter-Strike: Global Offensive..." << std::endl;

    while (!process->window)
    {
        process->window = FindWindowA(nullptr, "Counter-Strike: Global Offensive");
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    std::cout << "Counter-Strike: Global Offensive found!" << std::endl;

    while (!process->process_id)
    {
        GetWindowThreadProcessId(process->window, reinterpret_cast<LPDWORD>(&process->process_id));
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    std::cout << "ProcessID = " << process->process_id << std::endl;

    if (!process->attach()) {
        std::cout << "Attach failed! -> Error: " << GetLastError() << std::endl;
        std::cin.get();
        ExitProcess(0);
    }

    std::cout << "Waiting for serverbrowser.dll..." << std::endl;

    while (!process->GetModuleBaseAddress("serverbrowser.dll")[0])
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    std::cout << "serverbrowser.dll found!" << std::endl;

    std::cout << "Trying to get necessary modules..." << std::endl;

    while (!process->csgo[0] || !process->client[0] || !process->engine[0])
    {
        if (!process->csgo[0])
            process->csgo = process->GetModuleBaseAddress("csgo.exe");

        if (!process->client[0])
            process->client = process->GetModuleBaseAddress("client.dll");

        if (!process->engine[0])
            process->engine = process->GetModuleBaseAddress("engine.dll");

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    std::cout << "All necessary modules found!" << std::endl;

    std::cout << "csgo.exe = " << std::hex << process->csgo[0] << std::endl;
    std::cout << "client.dll = " << std::hex << process->client[0] << std::endl;
    std::cout << "engine.dll = " << std::hex << process->engine[0] << std::endl;

    Offsets::generate_offsets();


    // Main loop
    int _local_player_team;

    while (true)
    {
        // Exit key
        if (GetAsyncKeyState(VK_END)) {
            break;
        }

        const char* map[64];
        ReadProcessMemory(process->process, reinterpret_cast<LPCVOID>(process->read<DWORD>(Offsets::client_state) + Offsets::client_state_map), &map, sizeof(map), nullptr);

        const auto max_clients = process->read<int>(Offsets::global_pointer + 0x18);

        Player *curr_player;
        Player _curr_local;

        int vis_teammate_count = 0;
        int vis_enemy_count = 0;

        for (auto i = 0; i <= 64; i++)
        {
            const auto _local_player = process->read<DWORD>(Offsets::local_player);
            const auto entity = process->read<DWORD>(Offsets::entity_list + 0x10 * i);

            bool is_local_player = false;

            if (!entity)
                continue;

            const auto team = process->read<int>(entity + 0xF4);

            const auto health = process->read<int>(entity + 0x100);

            if (!health)
                continue;

            const auto lifestate = process->read<BYTE>(entity + 0x25F);

            if (lifestate)
                continue;

            const auto bdormant = process->read<BYTE>(entity + Offsets::dormant);

            if (bdormant)
                continue;

            const auto active_weapon = process->read<ULONG64>(entity + 0x2EF8);
            const auto weapon_entity = process->read<ULONG64>(Offsets::entity_list + ((active_weapon & 0xFFF) - 1) * 0x10);
            auto weapon_index = process->read<short>(weapon_entity + 0x2FAA);

            if (weapon_index > 64 || weapon_index < 1)
                weapon_index = 42;

            const auto weapon_name = strcmp(Weapons::weapon_names[weapon_index], "none") ? Weapons::weapon_names[weapon_index] : "KNIFE";

            auto radar = process->read<DWORD_PTR>(Offsets::radar_base);
            radar = process->read<DWORD_PTR>(radar + 0x78);
            const char* name[128];
            ReadProcessMemory(process->process, reinterpret_cast<LPCVOID>(radar + 0x174 * (i + 2) + 0x18), &name, sizeof(name), nullptr);

            const auto x = process->read<float>(entity + 0x138);
            const auto y = process->read<float>(entity + 0x13C);

            Player curr;
            curr.is_local = is_local_player;
            curr.team = team;
            curr.health = health;
            curr.name = reinterpret_cast<const char *>(name);
            curr.weapon_name = weapon_name;
            curr.x = x;
            curr.y = y;

            if (_local_player == entity) {
                _local_player_team = team;
                is_local_player = true;
                _curr_local = curr;
            }


            curr_player = &curr;

            
            if (team != _local_player_team) {
                vis_enemy_count++;
            } else if (team == _local_player_team) {
                vis_teammate_count++;
            }

            const auto _i = std::to_string(i);
            float dist = Vector2::distance(Vector2(_curr_local.x, _curr_local.y), Vector2(curr_player->x, curr_player->y));

//            if (curr_player->team != _local_player_team && dist < 1000) {
//                std::cout << "opponent :" << curr_player->name << std::dec << " health: " << curr_player->health;
//
//                std::cout << " distance :" << dist << std::endl;
//
//                std::this_thread::sleep_for(std::chrono::milliseconds(500));
//            }

        }

        std::cout << "visible enemys :" << vis_enemy_count << "\n";
    }



    return 0;
}
