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
	
	// ����ũ�� 
	float fForce = j_vForce.Length();
	
	if (0.f != fForce) {

		// ���ӵ�
		j_vAccel = j_vForce / j_fMass;
		
	
	}

	// �߰����ӵ�
	j_vAccel += j_vAccelA;
	
	// �ӵ�
	j_vVelocity += j_vAccel * TD;
	
	// ������ ����
	if (0.f !=j_vVelocity.Length()) 
	{
		// ����
		Vec2 vFricDIr = -j_vVelocity;
		vFricDIr.Nomalize();
		
		// �ӵ�
		Vec2 vFfiction = vFricDIr * j_fFric * TD;
		
		//�ӷ� ��
		if (j_vVelocity.Length() <= vFfiction.Length())
		{
			//�������ӵ��� ���� �ӵ����� ū ���
			j_vVelocity = Vec2(0.f, 0.f);

		}
		else
		{
			j_vVelocity += vFfiction;
		}
	}

	// �ӷ� ���� �˻�
	if (abs(j_vMaxVelocity.x) < abs(j_vVelocity.x))
	{
		j_vVelocity.x = (j_vVelocity.x / abs(j_vVelocity.x)) * abs(j_vMaxVelocity.x);
	}

	if (abs(j_vMaxVelocity.y) < abs(j_vVelocity.y))
	{
		j_vVelocity.y = (j_vVelocity.y / abs(j_vVelocity.y)) * abs(j_vMaxVelocity.y);
	}

	// �ӵ��� ���� �̵�
	Move(back_wid);
	
	// ���ʱ�ȭ
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

			// x ��ǥ�� �̵���

			// y ��ǥ�� ������ǥ, ����ǥ

			// ó���� �� ������� ������ �÷��̾ü�� ������
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


