#include "pch.h"

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> distX(0, MAP_WIDTH - 1);
std::uniform_int_distribution<> distY(0, MAP_HEIGHT - 1);

std::atomic_int GClientIDGen = 0;

bool ConvertCharToWide(const char* src, wchar_t* dst, int dstMaxCount)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, src, -1, dst, dstMaxCount);
	return (len > 0 && len <= dstMaxCount);
}
