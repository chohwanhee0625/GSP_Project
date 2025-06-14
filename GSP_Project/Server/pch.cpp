#include "pch.h"

std::atomic_int GClientIDGen = 0;

bool ConvertCharToWide(const char* src, wchar_t* dst, int dstMaxCount)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, src, -1, dst, dstMaxCount);
	return (len > 0 && len <= dstMaxCount);
}
