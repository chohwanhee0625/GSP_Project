#include "pch.h"
#include "Object.h"
#include "RigidBody.h"
#include "Collider.h"
#include "Gravity.h"
#include "Ground.h"

Object::Object()
	: j_vLogPos{}
	, j_vScale{}
	, j_vPhypo{}
	, j_pRigidBody{nullptr}
	, j_pCollider{nullptr}
	, j_pGravity{nullptr}
	, j_Objtype{OBJECTTYPE::NONE}
	, j_bAlive{true}

{
}

Object::~Object()
{
	if (nullptr != j_pRigidBody)
	{
		delete j_pRigidBody;
	}

	if (nullptr != j_pCollider)
	{
		delete j_pCollider;
	}

	if (nullptr != j_pGravity)
	{
		delete j_pGravity;
	}

}

void Object::update()
{
}

// 여기서 컴포넌트
void Object::finalupdate(int back_wid)
{
	if (j_pCollider)
		j_pCollider->finalupdate();
	
	if (j_pGravity)
		j_pGravity->finalupdate();

	if (j_pRigidBody)
		j_pRigidBody->finalupdate(back_wid);

}

void Object::render(HDC _dc)
{
	Rectangle(_dc, GetPsyLeft(), GetPsyTop(), GetPsyRight(), GetPsyBottom());
}

void Object::SetMoved(Vec2 _v)
{
}

Vec2 Object::GetMoved()
{
	return Vec2();
}

void Object::SetObject(Vec2 pos, Vec2 scale)
{
	
	j_vLogPos.x = pos.x;
	j_vLogPos.y = pos.y;
	j_vPhypo.x = pos.x;
	j_vPhypo.y = pos.y;

	j_vScale.x = scale.x;
	j_vScale.y = scale.y;
}

void Object::SetObject(RECT rect)
{
	int w = rect.right - rect.left;
	int h = rect.bottom - rect.top;

	int p_x = rect.right - w / 2;
	int p_y = rect.bottom - h / 2;

	j_vLogPos.x = (float)p_x;
	j_vLogPos.y = (float)p_y;
	j_vPhypo.x = (float)p_x;
	j_vPhypo.y = (float)p_y;

	j_vScale.x = (float)w;
	j_vScale.y = (float)h;

}

void Object::CreateRidBody()
{
	j_pRigidBody = new RigidBody;
	j_pRigidBody->j_pOwner = this;
}

void Object::CreateCollider()
{
	j_pCollider = new Collider;
	j_pCollider->j_pOwner = this;
}

void Object::CreateGravity()
{
	j_pGravity = new Gravity;
	j_pGravity->j_pOwner = this;
}

void Object::updatePhypo(Vec2 moved)
{

	j_vPhypo.x = j_vLogPos.x - moved.x;

}

bool RectsCollision(Object& rect1, Object& rect2)
{
	if ((rect1.GetLeft() < rect2.GetRight())
		&& (rect1.GetTop() < rect2.GetBottom())
		&& (rect1.GetRight() > rect2.GetLeft())
		&& (rect1.GetBottom() > rect2.GetTop()))
	{
		return true;
	}
	else return false;
}




