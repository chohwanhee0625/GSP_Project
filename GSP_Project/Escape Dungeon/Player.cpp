#include "pch.h"
#include "Player.h"
#include "RigidBody.h"
#include "Collider.h"
#include "Gravity.h"
#include "BAttack.h"
#include "Monster.h"
#include "Portal.h"
#include "Fire.h"

Player::Player()
	: j_act{ PLAYERSTATE::STAND }
	, j_prevact{ PLAYERSTATE::STAND }
	, j_Hp{5}
	, j_Colltime{}
	, j_ClientRect{}
	, j_moved{}
	, j_Aniidx{}
	, j_LR{}
	, j_cimage{}
	, j_BodyCollider{}
	, j_backwidth{}
	, j_coincount{}
	, j_onportal{}
	, j_jumpcount{}
	, j_isice{}
{
	
	j_BodyCollider = new Collider;
	j_BodyCollider->j_pOwner = this;
	j_BodyCollider->SetScale(Vec2(40.f, 60.f));
	j_BodyCollider->SetOffsetPos(Vec2(0.f, 0.f));

	CreateRidBody();
	CreateCollider();
	CreateGravity();
	GetCollider()->SetOffsetPos(Vec2(0.f, 32.f));
	GetCollider()->SetScale(Vec2(32.f, 6.f));


	j_Objtype = OBJECTTYPE::PLAYER;

	for (int i{}; i < (int)PLAYERSTATE::DEAD; ++i)
	{
		j_Aniidx[i] = 0;
	}
}



Player::~Player()
{
	j_cimage.Destroy();
}

Vec2 Player::GetMoved()
{
	return j_moved;
}


void Player::Goleft(bool onleft)
{
	

	if (PLAYERSTATE::HITTED == j_act)
	{
		return;
	}

	RigidBody* pRigid = GetRigidbody();	
	Gravity* pGravity = GetGravity();

	if (pGravity->GetGround() && PLAYERSTATE::BATTACK != j_act) {
		j_act = PLAYERSTATE::RUN;
	}

	j_LR = true;
	
	if (j_vLogPos.x < 0.f)
	{
		pRigid->SetVelocity(Vec2(0.f, 0.f));
		return;
	}


	if (onleft) {
		if (j_vLogPos.x - j_vScale.x / 2 <= j_ClientRect.left)
		{
			return;
		}

			if (j_isice)
			{
				pRigid->AddForce(Vec2(-800.f, 0.f));
			}
			else {
				pRigid->AddForce(Vec2(-500.f, 0.f));
			}
		
	}
	else
	{
	
		
		if (j_vLogPos.x - j_vScale.x / 2 <= j_ClientRect.left)
		{
			return;
		}

		if (j_isice)
		{
			pRigid->SetVelocity(Vec2(-200.f, pRigid->GetVelocity().y));
		}
		else {
			pRigid->SetVelocity(Vec2(-100.f, pRigid->GetVelocity().y));
		}

	}
}

void Player::Goright(int width, bool onright)
{
	
	if (PLAYERSTATE::HITTED == j_act)
	{
		return;
	}

	RigidBody* pRigid = GetRigidbody();
	Gravity* pGravity = GetGravity();
	
	if (pGravity->GetGround()&& PLAYERSTATE::BATTACK != j_act) {
		j_act = PLAYERSTATE::RUN;
	}

	j_LR = false;
	
	if (GetRight()+ IMG_L / 2.f > (float)width)
	{
		pRigid->SetVelocity(Vec2(0.f, 0.f));
		return;
	}


	if (onright) {
		
		if (j_isice)
		{
			pRigid->AddForce(Vec2(800.f, 0.f));
		}
		else {
			pRigid->AddForce(Vec2(500.f, 0.f));
		}


	}
	else
	{
	
		if (j_isice)
		{
			pRigid->SetVelocity(Vec2(200.f, pRigid->GetVelocity().y));
		}
		else {
			pRigid->SetVelocity(Vec2(100.f, pRigid->GetVelocity().y));
		}

	}

}

void Player::updatestate(bool Isattack)
{
	Gravity* pGravity = GetGravity();

	

	if (false == Isattack && PLAYERSTATE::BATTACK == j_act)
	{
		j_act = PLAYERSTATE::STAND;

	}

	if (false == pGravity->GetGround() && PLAYERSTATE::BATTACK != j_act && PLAYERSTATE::HITTED != j_act)
	{
		if (PLAYERSTATE::RUN == j_act)
		{
			/*j_pSoundChannel->stop();*/

		}

		j_act = PLAYERSTATE::JUMP;
	}

	if (pGravity->GetGround() && PLAYERSTATE::BATTACK != j_act)
	{

		if (0.3f >= GetRigidbody()->GetSpeed())
		{

			if (0 == j_Hp)
			{
				SetAlive(false);
				return;
			}

			j_act = PLAYERSTATE::STAND;

				
		}
		else
		{
			if (j_act != PLAYERSTATE::HITTED)
				j_act = PLAYERSTATE::RUN;
			else
			{
				
				GetRigidbody()->SetMaxVelocity(Vec2(250.f, 800.f));
			}
		}

	}
}

void Player::update_move(int width)
{
	RigidBody* pRigid = GetRigidbody();
	
	if (GetLeft() < 0.f)
	{
		pRigid->SetVelocity(Vec2(0.f, 0.f));
		AddPhyPos(2.f);
	}

	if (GetRight()+ IMG_L/2.f > (float)width)
	{
		pRigid->SetVelocity(Vec2(0.f, 0.f));
		AddPhyPos(-2.f);
		return;
	}


}

void Player::updateLogPos()
{

	j_vLogPos.x = j_vPhypo.x + j_moved.x;
  
	//j_vLogPos.y = j_vPhypo.y + j_moved.y;
}

void Player::update(int width, bool Isattack)
{
	updatestate(Isattack);
	update_move(width);
	updateLogPos();

}


void Player::onjump(bool space)
{

	if (j_jumpcount < 2) {
	

		if (PLAYERSTATE::HITTED == j_act)
		{
			return;
		}
		if (PLAYERSTATE::BATTACK != j_act) {
			j_act = PLAYERSTATE::JUMP;
		}
		if (false == space) {

			if (GetRigidbody())
			{
				++j_jumpcount;
				j_pSystem->playSound(j_pSounds[(int)SOUNDS::JUMP], 0, false, &j_pSoundChannel);
				AddyPos(-2.f);
				GetRigidbody()->SetVelocity(Vec2(GetRigidbody()->GetVelocity().x, -530.f));

			}
		}
	}


}

void Player::hit()
{
	j_pSystem->playSound(j_pSounds[(int)SOUNDS::HITTED], 0, false, &j_pSoundChannel);
	j_act = PLAYERSTATE::HITTED;
	GetRigidbody()->Hitted();

}

void Player::hit_1()
{
	j_pSystem->playSound(j_pSounds[(int)SOUNDS::HITTED], 0, false, &j_pSoundChannel);
	j_act = PLAYERSTATE::HITTED;
	GetRigidbody()->Hitted_1();
}

void Player::OnCollision_update(vector<Object*> grounds, vector<Object*> coins
	, vector<Monster*> monsters,vector<Object*> thorns, Portal& _Po, vector<Fire*> _fire)
{
	OnCollision_Thorn(thorns);
	OnCollision_Coin(coins);
	OnCollision_Monster(monsters);
	OnCollision_Ground(grounds);
	OnCollision_Portal(_Po);
	OnCollision_Fire(_fire);
}

void Player::OnCollision_Ground(vector<Object*> _pOther)
{
	bool check{ false };
	bool icecheck{ false };
	RigidBody* prigid = GetRigidbody();
	for (int i{}; i < _pOther.size(); ++i)
	{
		Collider* pColl = _pOther.at(i)->GetCollider();

		if (true == IsCollision(j_BodyCollider, pColl))
		{
			Vec2 vObjPos = pColl->GetFinalPos();
			Vec2 vObjScale = pColl->GetScale();

			Vec2 vPos =j_BodyCollider->GetFinalPos();
			Vec2 vScale = j_BodyCollider->GetScale();
		
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
				if (GetPos().x <= 500.f || GetPos().x + MAIN_POSX + getRect().right / 2.f >= j_backwidth) {
					float psypos = GetPhyPos().x;

					if (check_x) {
						psypos += fValue_x + 1.f;
					}
					else
					{
						psypos -= fValue_x + 1.f;
					}
					SetPsyPos(Vec2(psypos, GetPhyPos().y));

					
				}
				else
				{
					float moved = GetMoved().x;
					if (check_x) {
						moved += fValue_x + 1.f;
					}
					else
					{
						moved -= fValue_x + 1.f;
					}
					SetMoved(Vec2(moved, GetMoved().y));
				
				}

				Vec2 vV = GetRigidbody()->GetVelocity();
				GetRigidbody()->SetVelocity(Vec2(0.f, vV.y));
			}

			else {
				vObjPos = GetPos();

				if (check_y) {
					vObjPos.y += fValue +1.f;
				}
				else
				{
					vObjPos.y -= fValue + 1.f;
				}


				SetyPos(vObjPos.y);
				Vec2 vV = GetRigidbody()->GetVelocity();
				GetRigidbody()->SetVelocity(Vec2(vV.x,0.f));
			}
		}




		if (true == IsCollision(GetCollider(), pColl)) //&& true == _pOther->GetPrevCol(OBJECTTYPE::GROUND)
		{
			check = true;
			GetCollider()->SetPrevCol(OBJECTTYPE::GROUND, true);
			
			if (false == pColl->GetPrevCol(OBJECTTYPE::PLAYER) && PLAYERSTATE::BATTACK != j_act && PLAYERSTATE::HITTED != j_act)
			{
				j_act = PLAYERSTATE::STAND;
			}
			pColl->SetPrevCol(OBJECTTYPE::PLAYER, true);
			j_jumpcount = 0;
			GetGravity()->SetGround(true);
			

			if (_pOther.at(i)->GetIsice())
			{
				j_isice = true;
				prigid->Setfirc(100.f);
			
			}
			else {
				j_isice = false;
				prigid->Setfirc(300.f);
				
			}


			if(PLAYERSTATE::HITTED != j_act && false == j_isice)
				prigid->SetMaxVelocity(Vec2(250.f, 1000.f));

			else if (PLAYERSTATE::HITTED != j_act && true == j_isice)
				prigid->SetMaxVelocity(Vec2(400.f, 1000.f));


			Vec2 vObjPos = pColl->GetFinalPos();
			Vec2 vObjScale = pColl->GetScale();

			Vec2 vPos = GetCollider()->GetFinalPos();
			Vec2 vScale = GetCollider()->GetScale();

			float fLen = abs(vObjPos.y - vPos.y);
			float fValue = (vObjScale.y / 2.f + vScale.y / 2.f) - fLen;


			vObjPos = GetPos();
			vObjPos.y -= (fValue - 1.f);

			SetyPos(vObjPos.y);

		}
		else
		{
			pColl->SetPrevCol(OBJECTTYPE::PLAYER, false);
		}
	}

		

		if (false == check)
		{
			GetCollider()->SetPrevCol(OBJECTTYPE::GROUND, false);
			GetGravity()->SetGround(false);
		}
}

void Player::OnCollision_Coin(vector<Object*> _pOther)
{
	for (int i{}; i < _pOther.size(); ++i)
	{
		Collider* pColl = _pOther.at(i)->GetCollider();

		if (true == IsCollision(j_BodyCollider, pColl))
		{
			_pOther.at(i)->SetAlive(false);
			j_pSystem->playSound(j_pSounds[(int)SOUNDS::COIN], 0, false, & j_pSoundChannel);
			upcoin();

		}
	
	}
}

void Player::OnCollision_Thorn(vector<Object*> _pOther)
{
	for (int i{}; i < _pOther.size(); ++i)
	{
		Collider* pColl = _pOther.at(i)->GetCollider();

		if (true == IsCollision(j_BodyCollider, pColl))
		{
			if (PLAYERSTATE::HITTED != j_act) {
				--j_Hp;
				hit();
			}
		}

	}
}

void Player::OnCollision_Fire(vector<Fire*> _fire)
{
	for (int i{}; i < _fire.size(); ++i)
	{
		Collider* pColl = _fire.at(i)->GetCollider();

		if (true == IsCollision(j_BodyCollider, pColl))
		{
			if (PLAYERSTATE::HITTED != j_act) {
				_fire.at(i)->SetPos(Vec2(-1000.f, -1000.f));
				_fire.at(i)->SetScale(Vec2(0.f, 0.f));
				_fire.at(i)->SetAlive(false);
				--j_Hp;
				hit_1();
			}
		}

	}

}

void Player::OnCollision_Monster(vector<Monster*> _pOther)
{
	for (int i{}; i < _pOther.size(); ++i)
	{
		Collider* pColl = _pOther.at(i)->GetCollider();

		if (true == IsCollision(j_BodyCollider, pColl) && MONSTERSTATE::AIM  == _pOther.at(i)->GetMstate())
		{
			if (PLAYERSTATE::HITTED != j_act) {
				hit();
				--j_Hp;
			}

		}

	}
}

void Player::OnCollision_Portal(Portal& _po)
{

	Collider* pColl = _po.GetCollider();

	if (true == IsCollision(GetCollider(), pColl))
	{
		j_onportal = true;
	}
	else {
		j_onportal = false;
	}
}


void Player::MakeBAttack(BAttack& attack)
{
	if (PLAYERSTATE::HITTED == j_act)
	{
		return;
	}
	j_pSystem->playSound(j_pSounds[(int)SOUNDS::ATTACK], 0, false, &j_pSoundChannel);
	j_act = PLAYERSTATE::BATTACK;

	attack.SetAlive(true);

}



void Player::finalupdate(int back_wid)

{


	if (j_pGravity)
		j_pGravity->finalupdate();

	if (j_pRigidBody)
		j_pRigidBody->finalupdate(back_wid);

	if (j_pCollider)
		j_pCollider->finalupdate();

	j_BodyCollider->finalupdate();
	
}



void Player::render(HDC mdc)
{


	if (PLAYERSTATE::STAND == j_act)
	{

		if (j_LR) {

			j_cimage.Draw(mdc, GetPsyLeft(), GetPsyTop(), j_vScale.x, j_vScale.y
				, IMG_L * 9
				, IMG_L 
				, IMG_L
				, IMG_L);

		}
		else
		{
			j_cimage.Draw(mdc, GetPsyLeft(), GetPsyTop(), j_vScale.x, j_vScale.y
				, 0
				, 0
				, IMG_L
				, IMG_L);
		}

	}

	else if (PLAYERSTATE::RUN == j_act)
	{
		if (j_LR) {

			j_cimage.Draw(mdc, GetPsyLeft(), GetPsyTop(), j_vScale.x, j_vScale.y
				, IMG_L * (1+j_Aniidx[(int)PLAYERSTATE::RUN]%6)
				, IMG_L
				, IMG_L
				, IMG_L);

		}
		else
		{
			j_cimage.Draw(mdc, GetPsyLeft(), GetPsyTop(), j_vScale.x, j_vScale.y
				, IMG_L * (1+j_Aniidx[(int)PLAYERSTATE::RUN] % 6)
				, 0
				, IMG_L
				, IMG_L);
		}
	}

	else if (PLAYERSTATE::JUMP == j_act)
	{

		if (j_LR) { 

			j_cimage.Draw(mdc, GetPsyLeft(), GetPsyTop(), j_vScale.x, j_vScale.y
				, 0
				, IMG_L * 10.f
				, IMG_L
				, IMG_L);

		}
		else
		{
			j_cimage.Draw(mdc, GetPsyLeft(), GetPsyTop(), j_vScale.x, j_vScale.y
				, IMG_L * 6.f
				, IMG_L * 9.f
				, IMG_L
				, IMG_L);
		}

	}
	
	else if (PLAYERSTATE::BATTACK == j_act)
	{
		if (j_LR) {
			//По
			
			j_cimage.Draw(mdc, GetPsyLeft(), GetPsyTop(), j_vScale.x, j_vScale.y
				, IMG_L * (5-(j_Aniidx[(int)PLAYERSTATE::BATTACK] % 3)) //IMG_L * 3.f
				, IMG_L * 5.f
				, IMG_L
				, IMG_L);
		}
		else
		{
			j_cimage.Draw(mdc, GetPsyLeft(), GetPsyTop(), j_vScale.x, j_vScale.y
				, IMG_L * (j_Aniidx[(int)PLAYERSTATE::BATTACK] % 3)
				, IMG_L * 4.f
				, IMG_L
				, IMG_L);
		}
	}

	else if (PLAYERSTATE::HITTED == j_act)
	{
		if (j_LR) {
			//По
			if (4 <= j_Aniidx[(int)PLAYERSTATE::HITTED])
			{
				j_cimage.Draw(mdc, GetPsyLeft(), GetPsyTop()+4, j_vScale.x, j_vScale.y
					, IMG_L * 9
					, IMG_L * 0
					, IMG_L
					, IMG_L);
			}
			else
			{
				j_cimage.Draw(mdc, GetPsyLeft(), GetPsyTop(), j_vScale.x, j_vScale.y
					, IMG_L * ((j_Aniidx[(int)PLAYERSTATE::HITTED] % 4)) //IMG_L * 3.f
					, IMG_L * 3.f
					, IMG_L
					, IMG_L);
			}
		}
		else
		{
			if (4 <= j_Aniidx[(int)PLAYERSTATE::HITTED])
			{
				j_cimage.Draw(mdc, GetPsyLeft(), GetPsyTop() + 4, j_vScale.x, j_vScale.y
					, IMG_L * 0
					, IMG_L * 1
					, IMG_L
					, IMG_L);
			}
			else {
				j_cimage.Draw(mdc, GetPsyLeft(), GetPsyTop(), j_vScale.x, j_vScale.y
					, IMG_L * (4 - j_Aniidx[(int)PLAYERSTATE::HITTED] % 4)
					, IMG_L * 2.f
					, IMG_L
					, IMG_L);
			}
		}
	}


	j_pCollider->render(mdc);
	j_BodyCollider->render(mdc);
}
  
 
void Player::UpAniidx()
{
	if (PLAYERSTATE::RUN == j_act )
	{
		if (j_Aniidx[(int)PLAYERSTATE::RUN] > 10000)
		{
			j_Aniidx[(int)PLAYERSTATE::RUN] = 0;
		}
		++j_Aniidx[(int)PLAYERSTATE::RUN];
		
	}

	if (PLAYERSTATE::HITTED == j_act)
	{
		++j_Aniidx[(int)PLAYERSTATE::HITTED];
	}
	else
	{
		j_Aniidx[(int)PLAYERSTATE::HITTED] = 0;
	}

	if (PLAYERSTATE::BATTACK == j_act)
	{
		if(j_Aniidx[(int)PLAYERSTATE::BATTACK] < 3)
		++j_Aniidx[(int)PLAYERSTATE::BATTACK];
	}
	else
	{
		j_Aniidx[(int)PLAYERSTATE::BATTACK] = 0;
	}
}





