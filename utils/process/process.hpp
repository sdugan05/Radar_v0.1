#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <memory>
#include <string>
#include <array>
#include <vector>

class Process
{
public:
	auto attach() -> bool;
	auto detach() -> void;
	auto write(DWORD dw_address, LPCVOID lpc_buffer, DWORD dw_size) -> bool;
	auto read(DWORD dw_address, LPVOID lp_buffer, DWORD dw_size) -> bool;
	auto scan(DWORD start, DWORD size, const char* signature, const char* mask) -> DWORD;
	auto find_netvar(DWORD dwClasses, const char* table, const char* var) -> DWORD;

	template<typename T>
	auto read(const DWORD& dw_address, const T& t_default = T()) -> T
	{
		T t_ret;
		if (!read(dw_address, &t_ret, sizeof(T)))
			return t_default;

		return t_ret;
	}

	template<typename T>
	auto read_multi(DWORD dw_address, std::vector<DWORD> v_offset, const T& tDefault = T()) -> T
	{
		T t_ret = dw_address;
		for (auto i = 0; i < v_offset.size(); i++)
		{
			read(t_ret + v_offset[i], &t_ret, sizeof(T));
		}
		return t_ret;
	}
	
	template<typename T>
	auto write(const DWORD& dw_address, const T& t_value) -> bool
	{
		return write(dw_address, &t_value, sizeof(T));
	}

	auto get_module_base_address(const char* str_module_name) -> std::array<ULONG64, 2>;


    std::array<ULONG64, 2> GetModuleBaseAddress(const char* modName);

public:
	HWND window = nullptr;
	HANDLE process = nullptr;
	ULONG64 process_id = NULL;

	std::array<ULONG64, 2> csgo = { NULL, NULL };
	std::array<ULONG64, 2> engine = { NULL, NULL };
	std::array<ULONG64, 2> client = { NULL, NULL };
};

inline std::unique_ptr<Process> process = std::make_unique<Process>();