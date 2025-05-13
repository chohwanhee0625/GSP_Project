#include "pch.h"
#include "Coin.h"
#include "Collider.h"


Coin::Coin()
	: j_img{}
	, j_aniidx{}

{
	j_Objtype = OBJECTTYPE::COIN;
	CreateCollider();
	SetScale(Vec2(60.f, 60.f));
}

Coin::Coin(Vec2 _v)
	: j_img{}

{
	j_Objtype = OBJECTTYPE::COIN;
	CreateCollider();
	SetPos(_v);
	SetScale(Vec2(60.f, 60.f));
}

Coin::~Coin()
{
	
}

void Coin::update()
{
	if (false == GetAlive())
	{
		SetScale(Vec2(0.f, 0.f));
		SetLogPos(Vec2(0.f, 0.f));
	}
	GetCollider()->finalupdate();
}

void Coin::render(HDC _dc)
{
	if (true == GetAlive()) {
		j_img->Draw(_dc, GetPsyLeft(), GetPsyTop(), j_vScale.x, j_vScale.y
			, 85 * (j_aniidx)
			, 0
			, 85
			, 99);
	GetCollider()->render(_dc);
	}

}

void Coin::Setting()
{
	Collider* pcol = GetCollider();
	pcol->SetScale(Vec2(GetScale()-20.f));
	pcol->SetOffsetPos(Vec2(0.f, 10.f));
	
	GetCollider()->finalupdate();

}

void Coin::Upidx()
{
	if (j_aniidx == 5)
	{
		j_aniidx = 0;
		return;
	}
	++j_aniidx;

}
