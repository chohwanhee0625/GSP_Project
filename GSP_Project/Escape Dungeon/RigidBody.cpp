#include "pch.h"
#include "RigidBody.h"
#include "Object.h"
#include "Collider.h"
#include "Gravity.h"
RigidBody::RigidBody()
	: j_pOwner{nullptr}
	, j_vForce{}
	, j_vAccel{}
	, j_vVelocity{}
	, j_fMass{1.f}
	, j_vMaxVelocity{Vec2(250.f,1000.f)}
	, j_fFric{300.f}
{
}

RigidBody::~RigidBody()
{
}

void RigidBody::finalupdate(int back_wid)
{
	
	// 힘의크기 
	float fForce = j_vForce.Length();
	
	if (0.f != fForce) {

		// 가속도
		j_vAccel = j_vForce / j_fMass;
		
	
	}

	// 추가가속도
	j_vAccel += j_vAccelA;
	
	// 속도
	j_vVelocity += j_vAccel * TD;
	
	// 마찰력 적용
	if (0.f !=j_vVelocity.Length()) 
	{
		// 방향
		Vec2 vFricDIr = -j_vVelocity;
		vFricDIr.Nomalize();
		
		// 속도
		Vec2 vFfiction = vFricDIr * j_fFric * TD;
		
		//속력 비교
		if (j_vVelocity.Length() <= vFfiction.Length())
		{
			//마찰가속도가 본래 속도보다 큰 경우
			j_vVelocity = Vec2(0.f, 0.f);

		}
		else
		{
			j_vVelocity += vFfiction;
		}
	}

	// 속력 제한 검사
	if (abs(j_vMaxVelocity.x) < abs(j_vVelocity.x))
	{
		j_vVelocity.x = (j_vVelocity.x / abs(j_vVelocity.x)) * abs(j_vMaxVelocity.x);
	}

	if (abs(j_vMaxVelocity.y) < abs(j_vVelocity.y))
	{
		j_vVelocity.y = (j_vVelocity.y / abs(j_vVelocity.y)) * abs(j_vMaxVelocity.y);
	}

	// 속도에 따른 이동
	Move(back_wid);
	
	// 힘초기화
	j_vForce = Vec2(0.f, 0.f);
	j_vAccelA = Vec2(0.f, 0.f);
	j_vAccel = Vec2(0.f, 0.f);
}

void RigidBody::Move(int back_wid)
{
	float fSpeed = j_vVelocity.Length();
	
	if (0.f != fSpeed)
	{
		if (OBJECTTYPE::PLAYER == j_pOwner->GetObjtype())
		{

			// x 좌표는 이동량

			// y 좌표는 물리좌표, 논리좌표

			// 처음에 맵 가운데까지 갈떄는 플레이어객체가 움직임
			if (j_pOwner->GetPos().x <= 500.f|| j_pOwner->GetPos().x + MAIN_POSX + j_pOwner->getRect().right/2.f >= back_wid)
			{
				float vMoved = j_pOwner->GetPhyPos().x;
				vMoved += j_vVelocity.x * TD;
				j_pOwner->SetPsyPos(Vec2(vMoved, j_pOwner->GetMoved().y));
			
				float vPosy = j_pOwner->GetPos().y;
				vPosy += j_vVelocity.y * TD;

				j_pOwner->SetyPos(vPosy);
			}
			
			else {
				float vMoved = j_pOwner->GetMoved().x;
				vMoved += j_vVelocity.x * TD;
				j_pOwner->SetMoved(Vec2(vMoved, j_pOwner->GetMoved().y));

				float vPosy = j_pOwner->GetPos().y;
				vPosy += j_vVelocity.y * TD;

				j_pOwner->SetyPos(vPosy);
			}

		}

		else {
			Vec2 vPos = j_pOwner->GetPos();
			vPos += j_vVelocity * TD;
			j_pOwner->SetPos(vPos);

		}
	}
}

void RigidBody::Hitted()
{
	if (OBJECTTYPE::PLAYER == j_pOwner->GetObjtype()) {
		SetMaxVelocity(Vec2(500.f, 800.f));

		if (j_pOwner->GetLR())
		{
			j_pOwner->AddyPos(-2.f);
			SetVelocity(Vec2(500.f, -300.f));
		}
		else
		{
			j_pOwner->AddyPos(-2.f);
			SetVelocity(Vec2(-500.f, -300.f));
		}

		j_pOwner->GetGravity()->SetGround(false);
		j_pOwner->GetCollider()->finalupdate();
	}
	else
	{
		if (j_pOwner->GetLR())
		{

			SetVelocity(Vec2(300.f, -150.f));
		}
		else
		{
			SetVelocity(Vec2(-300.f, -150.f));
		}
	}

}

void RigidBody::Hitted_1()
{
	if (OBJECTTYPE::PLAYER == j_pOwner->GetObjtype()) {
		SetMaxVelocity(Vec2(250.f, 1000.f));

		if (j_pOwner->GetLR())
		{
			j_pOwner->AddyPos(-2.f);
			SetVelocity(Vec2(50.f, -400.f));
		}
		else
		{
			j_pOwner->AddyPos(-2.f);
			SetVelocity(Vec2(-50.f, -400.f));
		}

		j_pOwner->GetGravity()->SetGround(false);
		j_pOwner->GetCollider()->finalupdate();
	}
	else
	{
		if (j_pOwner->GetLR())
		{

			SetVelocity(Vec2(300.f, -150.f));
		}
		else
		{
			SetVelocity(Vec2(-300.f, -150.f));
		}
	}

}


