#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <stdio.h>  
#include <chrono>
#include <functional>
#include <random>

#include <vector>
#include <array>
#include <set>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <concurrent_unordered_map.h>
#include <concurrent_unordered_set.h>
#include <concurrent_queue.h>

#include <thread>
#include <mutex>
#include <shared_mutex>
#include <atomic>

#include <WS2tcpip.h>
#include <MSWSock.h>

#include "include/lua.hpp"

#include "game_header.h"
#include "Types.h"

#pragma comment (lib, "WS2_32.lib")
#pragma comment (lib, "MSWSock.lib")
#pragma comment	(lib, "lua54.lib")

constexpr int VIEW_RANGE = 7;
constexpr int SECTOR_SIZE = 15;
constexpr int SECTOR_COUNT_X = (MAP_WIDTH + SECTOR_SIZE + 1) / SECTOR_SIZE;
constexpr int SECTOR_COUNT_Y = (MAP_HEIGHT + SECTOR_SIZE + 1) / SECTOR_SIZE;

namespace std {
	template<> struct hash<std::pair<int, int>> {
		size_t operator()(const std::pair<int, int>& p) const noexcept {
			return (static_cast<size_t>(p.first) << 32)
				^ static_cast<unsigned>(p.second);
		}
	};
}

extern std::random_device rd;
extern std::mt19937 gen;
extern std::uniform_int_distribution<> distX;
extern std::uniform_int_distribution<> distY;

extern std::atomic_int GClientIDGen;

bool ConvertCharToWide(const char* src, wchar_t* dst, int dstMaxCount);
