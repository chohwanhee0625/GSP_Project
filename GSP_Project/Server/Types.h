#pragma once
#include <memory>

using BYTE = unsigned char;
using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;
using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;


// shared_ptr
#define USING_SHARED_PTR(name)	using name##Ref = std::shared_ptr<class name>;
USING_SHARED_PTR(Session);
USING_SHARED_PTR(NPC);
USING_SHARED_PTR(IocpCore);
