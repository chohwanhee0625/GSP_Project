#include "pch.h"
#include "BAttack.h"
#include "Player.h"
#include "Collider.h"


BAttack::BAttack()
	: j_count{}
	, j_Limg{}
	, j_Rimg{}
	, j_LR{}
{
	j_Objtype = OBJECTTYPE::BATTACK;
	CreateCollider();

	j_bAlive = false;
}

BAttack::BAttack(Vec2 _v)
	: j_count{}
	, j_Limg{}
	, j_Rimg{}
	, j_LR{}
{
	j_Objtype = OBJECTTYPE::BATTACK;
	SetPos(_v);
	SetScale(Vec2(1.f,1.f));
	CreateCollider();
	GetCollider();
	j_bAlive = false;
}

BAttack::~BAttack()
{
}

void BAttack::update(Player& oplayer)
{
	

	if (j_count == 20)
	{
		j_bAlive = false;
		j_count = 0;
	}

	if (j_bAlive)
	{
		++j_count;

		SetScale(Vec2(100.f, 100.f));
		if (oplayer.GetLR()) //true¸é ¿ÞÂÊ 
		{
			j_LR = true;
			SetPos(Vec2(oplayer.GetPos().x - 50.f , oplayer.GetPos().y+10.f));
		
		} 

		else
		{
			j_LR = false;
			SetPos(Vec2(oplayer.GetPos().x + 50.f, oplayer.GetPos().y + 10.f));
		}
	}
	else
	{
		SetScale(Vec2(1.f, 1.f));
		SetPos(Vec2(0.f, 0.f));
	}

	updatePhypo(oplayer.GetMoved());

	GetCollider()->SetScale(Vec2(GetScale()));
	GetCollider()->finalupdate();

}

void BAttack::render(HDC _dc)
{
	if (j_bAlive) {
		if (j_LR)
		{
			j_Limg.TransparentBlt(_dc, GetPsyLeft(), GetPsyTop(), j_vScale.x, j_vScale.y
				, 0
				, 0
				, 100
				, 100
				, RGB(0, 211, 234));
		}
		else {
			j_Rimg.TransparentBlt(_dc, GetPsyLeft(), GetPsyTop(), j_vScale.x, j_vScale.y
				, 0
				, 0
				, 100
				, 100
				, RGB(0, 211, 234));
		}
	}
	GetCollider()->render(_dc);
}

void BAttack::Setting()
{
	GetCollider()->SetScale(Vec2(GetScale()));
	GetCollider()->finalupdate();
}
