#include "pch.h"
#include "Object.h"
#include "Gravity.h"
#include "RigidBody.h"

Gravity::Gravity()
	: j_pOwner{nullptr}
	, j_bGround{false}

{
}

Gravity::~Gravity()
{
}

void Gravity::finalupdate()
{
	if (false == j_bGround) {
		j_pOwner->GetRigidbody()->SetAccelAlpha(Vec2(0.f, 1000.f));
		
		
	}

}

void Gravity::SetGround(bool _b)
{
	j_bGround = _b;
	if (j_bGround)
	{
		Vec2 vV = j_pOwner->GetRigidbody()->GetVelocity();
		j_pOwner->GetRigidbody()->SetVelocity(Vec2(vV.x, 0.f));
	}

}
