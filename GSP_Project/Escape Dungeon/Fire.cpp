#include "pch.h"
#include "Fire.h"
#include "Collider.h"
#include "RigidBody.h"
#include "Gravity.h"
#include "Monster.h"
#include "Ground.h"

random_device rd_1;
default_random_engine dre_1(rd_1());


uniform_int_distribution<int> uid_pos(50, 5000);
uniform_int_distribution<int> uid_scale(100, 200);
uniform_int_distribution<int> uid_LR(0, 1);

Fire::Fire()
	: j_img{}
	, j_aniidx{}
	, j_Landed{3}
	, j_LR{(bool)uid_LR(dre_1)}
	, j_colcheck{}
{
	CreateCollider();
	CreateRidBody();
	CreateGravity();

	SetPsyPos(Vec2(-500.f, -500.f));
	SetLogPos(Vec2((float)uid_pos(dre_1), 0.f));
	float sc{ (float)uid_scale(dre_1) };
	
	SetScale(Vec2(sc, sc));
	
	GetCollider()->SetScale(Vec2(sc / 2 , sc / 2));
	GetRigidbody()->SetMaxVelocity(Vec2(400.f, 1000.f));


}


Fire::~Fire()
{
}

void Fire::render(HDC _dc)
{
	if (j_aniidx < 8)
	{
		j_img->Draw(_dc, GetPsyLeft(), GetPsyTop(), j_vScale.x, j_vScale.y
			, 109 * j_aniidx
			, 0
			, 109
			, 185);
	}
	else
	{
		j_img->Draw(_dc, GetPsyLeft(), GetPsyTop(), j_vScale.x, j_vScale.y
			, 109 * (j_aniidx%8)
			, 190
			, 109
			, 185);
	}

	GetCollider()->render(_dc);
}



void Fire::update(int back_wid)
{
	finalupdate(back_wid);
}

void Fire::Upidx()
{

	++j_aniidx;

}

void Fire::Collider_ground(vector<Object*> _pground)
{
	Collider* pCol = GetCollider();

	for (int i{}; i < _pground.size(); ++i)
	{
		Collider* pOthercol = _pground.at(i)->GetCollider();
		
		if (IsCollision(pCol, pOthercol)&& false == j_colcheck)
		{
			AddyPos(-9.f);

			j_colcheck = true;
			if (0 == j_Landed)
			{
				SetAlive(false);
			}
			else
			{
				if (j_LR)
				{
					GetRigidbody()->SetVelocity(Vec2(-300.f / (4-j_Landed), -800.f/(4 - j_Landed)));
				}
				else
				{
					GetRigidbody()->SetVelocity(Vec2(300.f/ (4-j_Landed), -800.f/ (4 - j_Landed)));
				}
				--j_Landed;
			}

		}
		else
		{
			j_colcheck = false;
		}

	}

}

