#include "pch.h"
#include "Player.h"
#include "Monster.h"
#include "RigidBody.h"
#include "Collider.h"
#include "Gravity.h"
#include "BAttack.h"


random_device rd;
default_random_engine dre(rd());
uniform_int_distribution <int>uid_bool(0, 1);

Monster::Monster()
	: j_act{MONSTERSTATE::MOVE}
	, j_LR{ static_cast<bool>(uid_bool(dre))}
	, j_HP{ 3 }
	, j_Aniidx{}
	, j_Hitcount{}
	, j_boss{}
	, j_pimg{}
{
	CreateRidBody();
	CreateCollider();
	CreateGravity();
	
	SetScale(Vec2(130.f, 100.f));
	GetCollider()->SetScale(Vec2(120.f, 80.f));
	GetCollider()->SetOffsetPos(Vec2(0.f, -10.f));
	GetRigidbody()->SetMass(1.5f);
	j_Objtype = OBJECTTYPE::MONSTER;

}

Monster::~Monster()
{
}

void Monster::Upidx()
{
	if (MONSTERSTATE::MOVE == j_act|| MONSTERSTATE::AIM == j_act)
	{
		if (j_Aniidx[(int)MONSTERSTATE::MOVE] > 10000)
		{
			j_Aniidx[(int)MONSTERSTATE::MOVE] = 0;
		}
		++j_Aniidx[(int)MONSTERSTATE::MOVE];
	}
	
	if (MONSTERSTATE::HITTED == j_act)
	{
		if(j_Aniidx[(int)MONSTERSTATE::HITTED]<30)
	
		++j_Aniidx[(int)MONSTERSTATE::HITTED];
	}
	else
	{
		j_Aniidx[(int)MONSTERSTATE::HITTED] = 0;
	}

	if (MONSTERSTATE::DEAD == j_act)
	{
		++j_Aniidx[(int)MONSTERSTATE::DEAD];
	}
}

void Monster::Setting()
{
	
	GetCollider()->finalupdate();
}

void Monster::render(HDC _dc)
{
	if (GetAlive()) {

		if (MONSTERSTATE::MOVE == j_act || MONSTERSTATE::AIM == j_act) {
			if (j_LR) {
				// ¿ÞÂÊ
				j_pimg->Draw(_dc, GetPsyLeft(), GetPsyTop(), j_vScale.x, j_vScale.y
					, IMG_M * (j_Aniidx[(int)MONSTERSTATE::MOVE] % 3)
					, 10
					, IMG_M
					, IMG_M);
			}
			else
			{
				j_pimg->Draw(_dc, GetPsyLeft(), GetPsyTop(), j_vScale.x, j_vScale.y
					, IMG_M * (j_Aniidx[(int)MONSTERSTATE::MOVE] % 3)
					, IMG_M
					, IMG_M
					, IMG_M);
			}
		}
		else if (MONSTERSTATE::HITTED == j_act)
		{
			if (j_LR) {
				if (4 <= j_Aniidx[(int)MONSTERSTATE::HITTED])
				{
					j_pimg->Draw(_dc, GetPsyLeft(), GetPsyTop(), j_vScale.x, j_vScale.y
						, 0
						, IMG_M * 4
						, IMG_M
						, IMG_M);
				}
				else {
					j_pimg->Draw(_dc, GetPsyLeft(), GetPsyTop(), j_vScale.x, j_vScale.y
						, IMG_M * (j_Aniidx[(int)MONSTERSTATE::HITTED] % 3)
						, IMG_M * 2
						, IMG_M
						, IMG_M);
				}
			}
			else
			{
				if (4 <= j_Aniidx[(int)MONSTERSTATE::HITTED])
				{
					j_pimg->Draw(_dc, GetPsyLeft(), GetPsyTop(), j_vScale.x, j_vScale.y
						, 0
						, IMG_M * 5 - 17
						, IMG_M + 20
						, IMG_M);
				}
				else {
					j_pimg->Draw(_dc, GetPsyLeft(), GetPsyTop(), j_vScale.x, j_vScale.y
						, IMG_M * (2 - j_Aniidx[(int)MONSTERSTATE::HITTED] % 3)
						, IMG_M * 3
						, IMG_M
						, IMG_M);
				}
			}
		}
		else if (MONSTERSTATE::DEAD == j_act)
		{
			if (j_LR)
			{
				j_pimg->Draw(_dc, GetPsyLeft(), GetPsyTop(), j_vScale.x, j_vScale.y
					, 0
					, IMG_M * 4
					, IMG_M
					, IMG_M);
			}
			else
			{
				j_pimg->Draw(_dc, GetPsyLeft(), GetPsyTop(), j_vScale.x, j_vScale.y
					, 0
					, IMG_M * 5 - 17
					, IMG_M + 20
					, IMG_M);
			}
		}

		GetCollider()->render(_dc);
	}
}

void Monster::update(int width)
{

	if (false == GetAlive())
	{
		SetScale(Vec2(0.f, 0.f));
		SetLogPos(Vec2(0.f, 0.f));
	}
	GetCollider()->finalupdate();


	RigidBody* pregid = GetRigidbody();

	if (0 >= j_HP)
	{
		j_act = MONSTERSTATE::DEAD;
	} 


	/*if (GetLeft() < 0.f)
	{
		pregid->SetVelocity(Vec2(0.f, 0.f));
		AddxPos(2.f);
		return;
	}*/

	if (GetRight() > width)
	{
		pregid->SetVelocity(Vec2(0.f, 0.f));
		AddxPos(-2.f);
		return;
	}



	if (MONSTERSTATE::MOVE == j_act) {
		
		if (GetGravity()->GetGround()) {
			
			if (j_boss)
			{
				if (j_LR)
				{
					pregid->SetVelocity(Vec2(-200.f, 0.f));
				}
				else
				{
					pregid->SetVelocity(Vec2(200.f, 0.f));
				}

			
			}
			else {
				if (j_LR)
				{
					pregid->SetVelocity(Vec2(-100.f, 0.f));
				}
				else
				{
					pregid->SetVelocity(Vec2(100.f, 0.f));
				}
			}
		}
	}

	else if (MONSTERSTATE::AIM == j_act)
	{
		if (GetGravity()->GetGround()) {
			
			if (j_boss)
			{
				if (j_LR)
				{
					pregid->SetVelocity(Vec2(-200.f, 0.f));
				}

				else
				{
					pregid->SetVelocity(Vec2(200.f, 0.f));
				}

			}
			else
			{
				if (j_LR)
				{
					pregid->SetVelocity(Vec2(-150.f, 0.f));
				}

				else
				{
					pregid->SetVelocity(Vec2(150.f, 0.f));
				}
			}
		}
	}
	
}

void Monster::update_state(Player& p)
{
	

	Vec2 vlogpos = p.GetPos();
	Vec2 mypos = GetPos();
	
	
	if(GetGravity()->GetGround())
	{
		if (MONSTERSTATE::DEAD == j_act)
		{
			if (GetRigidbody()->GetSpeed() < 0.2f)
				SetAlive(false);
			else
				return;
		}

		else if ( MONSTERSTATE::HITTED == j_act) 
		{
			if(GetRigidbody()->GetSpeed() < 0.2f)
			j_act = MONSTERSTATE::MOVE;
		}

		else {
			if (300.f > mypos.Length(vlogpos))
			{
				
					j_act = MONSTERSTATE::AIM;

					if (mypos.x > vlogpos.x)
					{
						j_LR = true;
					}
					else
					{
						j_LR = false;
					}
				
			}
			else
			{
				j_act = MONSTERSTATE::MOVE;
			}
		}


		if (p.GetPState() == PLAYERSTATE::HITTED)
		{
			j_act = MONSTERSTATE::MOVE;
		}

	}

	
}

bool Monster::OnCollider_BAttack(BAttack& _Ba)
{
	Collider* pothercol = _Ba.GetCollider();
	RigidBody* prigid = GetRigidbody();


	if (IsCollision(GetCollider(), pothercol) && MONSTERSTATE::DEAD != j_act &&MONSTERSTATE::HITTED != j_act && true == GetAlive())
	{

		j_act = MONSTERSTATE::HITTED;

		--j_HP;
		prigid->Hitted();
		return true;
	}
	else return false;
}

void Monster::OnCollision_Ground(vector<Object*> _pOther)
{
	bool check{ false };

	for (int i{}; i < _pOther.size(); ++i)
	{
		Collider* pColl = _pOther.at(i)->GetCollider();

		if (true == IsCollision(GetCollider(), pColl))
		{
			

			Vec2 vObjPos = pColl->GetFinalPos();
			Vec2 vObjScale = pColl->GetScale();

			Vec2 vPos = GetCollider()->GetFinalPos();
			Vec2 vScale = GetCollider()->GetScale();

			bool check_x{}, check_y{};
			
			if (vObjPos.x > vPos.x) check_x = false;
			else check_x = true;

			if (vObjPos.y > vPos.y) check_y = false;
			else check_y = true;


			float fLen_x = abs(vObjPos.x - vPos.x);
			float fValue_x = (vObjScale.x / 2.f + vScale.x / 2.f) - fLen_x;

			float fLen = abs(vObjPos.y - vPos.y);
			float fValue = (vObjScale.y / 2.f + vScale.y / 2.f) - fLen;


			if (fValue_x < fValue) {
				
					float pos = GetPos().x;

					if (check_x) {
						pos += fValue_x + 1.f;
					}
					else
					{
						pos -= fValue_x + 1.f;
					}
					
					if (j_act == MONSTERSTATE::MOVE)
					{
						if (j_LR)
						{
							j_LR = false;
						}
						else

						{
							j_LR = true;
						}
					}
					
					SetPos(Vec2(pos, GetPos().y));
					Vec2 vV = GetRigidbody()->GetVelocity();
					GetRigidbody()->SetVelocity(Vec2(0.f, vV.y));
				}

			else {
				vObjPos = GetPos();
				
				// ´ê´Â°Ô ´õ ³·°ÔÀÖÀ¸¸é ¶¥¹âÀº »óÅÂ
				if (GetCollider()->GetFinalPos().y < pColl->GetFinalPos().y)
				{
					GetGravity()->SetGround(true);
					check = true;
				}

				
				if (check_y) {
					vObjPos.y += fValue + 1.f;
				}
				else
				{
					vObjPos.y -= fValue + 1.f;
				}


				SetyPos(vObjPos.y);
				
				Vec2 vV = GetRigidbody()->GetVelocity();
				GetRigidbody()->SetVelocity(Vec2(vV.x, 0.f));
			}
		
		}
		
	}

	if (false == check)
	{
		GetCollider()->SetPrevCol(OBJECTTYPE::GROUND, false);
		GetGravity()->SetGround(false);
	}



}

