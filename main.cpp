#include <iostream>
#include <thread>

#include "utils/process/process.hpp"
#include "utils/Vector2.hpp"

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

std::vector<const char*> weapon_names = {
        "none", // 0
        "DEAGLE", // 1
        "DUALS", // 2
        "FIVESEVEN", // 3
        "GLOCK", // 4
        "none",
        "none",
        "AK47", // 7
        "AUG", // 8
        "AWP", // 9
        "FAMAS", // 10
        "G3SG1", // 11
        "none",
        "GALIL", // 13
        "M249", // 14
        "none",
        "M4A4", // 16
        "MAC10", // 17
        "none",
        "P90", // 19
        "none",
        "none",
        "none",
        "none",
        "UMP45", // 24
        "XM1014", // 25
        "BIZON", // 26
        "MAG7", // 27
        "NEGEV", // 28
        "SAWEDOFF", // 29
        "TEC9", // 30
        "ZEUS", // 31
        "P2000", // 32
        "MP7", // 33
        "MP9", // 34
        "NOVA", // 35
        "P250", // 36
        "none",
        "SCAR20", // 38
        "SG553", // 39
        "SSG08", // 40
        "none", // 41
        "KNIFE", // 42 KNIFE_T
        "FLASHBANG", // 43
        "HEGRANADE", // 44
        "SMOKE", // 45
        "MOLOTOV", // 46
        "DECOY", // 47
        "FIREBOMB", // 48
        "C4", // 49
        "none",
        "none",
        "none",
        "none",
        "none",
        "none",
        "none",
        "none",
        "none", // 58 MUSICKIT
        "KNIFE", // 59 KNIFE_CT
        "M4A1S", // 60
        "USPS", // 61
        "none", // 62 TRADEUPCONTRACT
        "CZ75A", // 63
        "REVOLVER" // 64
};

struct player {
    bool is_local;

    int health;

    float x,y;

    const char *name;

    int team;

    const char *weapon_name;

};

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

    // Generate Offsets

    auto global_pointer = process->scan(process->client[0], process->client[1], "\xA1\x00\x00\x00\x00\xF3\x0F\x10\x40\x10", "x????xxxxx") + 1;
    global_pointer = process->read<DWORD>(global_pointer);

    auto local_player = process->scan(process->client[0], process->client[1], "\x8D\x34\x85\x00\x00\x00\x00\x89\x15\x00\x00\x00\x00\x8B\x41\x08\x8B\x48\x04\x83\xF9\xFF", "xxx????xx????xxxxxxxxx") + 3;
    local_player = process->read<DWORD>(local_player) + 4;

    auto client_state = process->scan(process->engine[0], process->engine[1], "\xA1\x00\x00\x00\x00\x33\xD2\x6A\x00\x6A\x00\x33\xC9\x89\xB0", "x????xxxxxxxxxx") + 1;
    client_state = process->read<DWORD>(client_state);

    auto client_state_local_player = process->scan(process->engine[0], process->engine[1], "\x8B\x80\x00\x00\x00\x00\x40\xC3", "xx????xx") + 2;
    client_state_local_player = process->read<DWORD>(client_state_local_player);

    auto client_state_player_info = process->scan(process->engine[0], process->engine[1], "\x8B\x89\x00\x00\x00\x00\x85\xC9\x0F\x84\x00\x00\x00\x00\x8B\x01", "xx????xxxx????xx") + 2;
    client_state_player_info = process->read<DWORD>(client_state_player_info);

    auto client_state_map = process->scan(process->engine[0], process->engine[1], "\x05\x00\x00\x00\x00\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xA1", "x????xxxxxxxxx") + 1;
    client_state_map = process->read<DWORD>(client_state_map);

    auto client_state_map_directory = process->scan(process->engine[0], process->engine[1], "\xB8\x00\x00\x00\x00\xC3\x05\x00\x00\x00\x00\xC3", "x????xx????x") + 7;
    client_state_map_directory = process->read<DWORD>(client_state_map_directory);

    auto game_dir = process->scan(process->engine[0], process->engine[1], "\x68\x00\x00\x00\x00\x8D\x85\x00\x00\x00\x00\x50\x68\x00\x00\x00\x00\x68", "x????xx????xx????x") + 1;
    game_dir = process->read<DWORD>(game_dir);
    const char* path[MAX_PATH];
    ReadProcessMemory(process->process, reinterpret_cast<LPCVOID>(game_dir), &path, sizeof(path), nullptr);

    auto entity_list = process->scan(process->client[0], process->client[1], "\xBB\x00\x00\x00\x00\x83\xFF\x01\x0F\x8C\x00\x00\x00\x00\x3B\xF8", "x????xxxxx????xx") + 1;
    entity_list = process->read<DWORD>(entity_list);

    auto radar_base = process->scan(process->client[0], process->client[1], "\xA1\x00\x00\x00\x00\x8B\x0C\xB0\x8B\x01\xFF\x50\x00\x46\x3B\x35\x00\x00\x00\x00\x7C\xEA\x8B\x0D", "x????xxxxxxx?xxx????xxxx") + 1;
    radar_base = process->read<DWORD>(radar_base);

    auto dormant = process->scan(process->client[0], process->client[1], "\x8A\x81\x00\x00\x00\x00\xC3\x32\xC0", "xx????xxx") + 2;
    dormant = process->read<DWORD>(dormant) + 8;

    auto force_jump = process->scan(process->client[0], process->client[1], "\x8B\x0D\x00\x00\x00\x00\x8B\xD6\x8B\xC1\x83\xCA\x02", "xx????xxxxxxx") + 2;
    force_jump = process->read<DWORD>(force_jump);

    constexpr ptrdiff_t m_fFlags = 0x104;

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
    // Main loop

    float _x, _y;
    int _local_player_team;

    while (true) {
        // Exit key
        if (GetAsyncKeyState(VK_END)) {
            break;
        }


        const char* map[64];
        ReadProcessMemory(process->process, reinterpret_cast<LPCVOID>(process->read<DWORD>(client_state) + client_state_map), &map, sizeof(map), nullptr);

        const auto max_clients = process->read<int>(global_pointer + 0x18);

        player *curr_player;
        player _curr_local;

        int vis_teammate_count = 0;
        int vis_enemy_count = 0;

        player vis_enemies[10];

        for (auto i = 0; i <= 64; i++)
        {
            const auto _local_player = process->read<DWORD>(local_player);
            const auto entity = process->read<DWORD>(entity_list + 0x10 * i);

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

            const auto bdormant = process->read<BYTE>(entity + dormant);

            if (bdormant)
                continue;

            const auto active_weapon = process->read<ULONG64>(entity + 0x2EF8);
            const auto weapon_entity = process->read<ULONG64>(entity_list + ((active_weapon & 0xFFF) - 1) * 0x10);
            auto weapon_index = process->read<short>(weapon_entity + 0x2FAA);

            if (weapon_index > 64 || weapon_index < 1)
                weapon_index = 42;

            const auto weapon_name = strcmp(weapon_names[weapon_index], "none") ? weapon_names[weapon_index] : "KNIFE";

            auto radar = process->read<DWORD_PTR>(radar_base);
            radar = process->read<DWORD_PTR>(radar + 0x78);
            const char* name[128];
            ReadProcessMemory(process->process, reinterpret_cast<LPCVOID>(radar + 0x174 * (i + 2) + 0x18), &name, sizeof(name), nullptr);

            const auto x = process->read<float>(entity + 0x138);
            const auto y = process->read<float>(entity + 0x13C);

            player curr = player();
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




            _x = x;
            _y = y;
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


        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }



    return 0;
}
