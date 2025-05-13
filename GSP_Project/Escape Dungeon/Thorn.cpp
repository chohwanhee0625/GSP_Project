#include "pch.h"
#include "Thorn.h"
#include "Collider.h"

Thorn::Thorn()
	:j_img{}
{
	j_Objtype = OBJECTTYPE::THORN;
	CreateCollider();
	SetScale(Vec2(60.f, 60.f));
	GetCollider()->SetOffsetPos(Vec2(0.f, 10.f));
}

Thorn::~Thorn()
{
}

void Thorn::render(HDC _dc)
{
	j_img->TransparentBlt(_dc, GetPsyLeft(), GetPsyTop(), j_vScale.x, j_vScale.y
		, 0
		, 0
		, 99
		, 99
		, RGB(0, 211, 234));

	GetCollider()->render(_dc);
}



void Thorn::Setting()
{
	GetCollider()->SetScale(Vec2(GetScale()));
	GetCollider()->finalupdate();
}
