#pragma once

#define MAIN_POSX 50.f
#define MAIN_POSY 450.f
#define IMG_L 80.f
#define IMG_M 120
#define TD 0.01f

// 520.f
enum class SOUNDS
{
	WALK,
	JUMP,
	COIN,
	ATTACK,
	HITTED,
	CLEAR,
	END,
};

enum class MONSTERSTATE
{
	MOVE,
	AIM,
	HITTED,
	DEAD,
};

enum class PLAYERSTATE
{
	STAND,
	RUN,
	JUMP,
	BATTACK,
	SATTACK,
	HITTED,
	DEAD,

};

enum class OBJECTTYPE
{
	PLAYER,
	GROUND,
	MONSTER,
	BOSS,
	THORN,
	BATTACK,
	COIN,
	PORTAL,
	NONE

};

enum class BRUSH_TYPE
{
	HOLLOW,
	END

};

enum class PEN_TYPE
{
	RED,
	GREEN,
	BLUE,
	END,

};






inline void CreateBrushPen(HBRUSH arrB[(int)BRUSH_TYPE::END], HPEN arrP[(int)PEN_TYPE::END])
{
	arrB[(int)BRUSH_TYPE::HOLLOW] = (HBRUSH)GetStockObject(HOLLOW_BRUSH);

	arrP[(int)PEN_TYPE::RED] = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	arrP[(int)PEN_TYPE::GREEN] = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	arrP[(int)PEN_TYPE::BLUE] = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
}

inline HBRUSH GetBrush(HBRUSH arrBrushs[(int)BRUSH_TYPE::END], BRUSH_TYPE _type)
{
	return arrBrushs[(int)_type];
}

inline HPEN GetPen(HPEN arrPens[(int)PEN_TYPE::END], PEN_TYPE _type)
{
	return arrPens[(int)_type];
}


