#include "pch.h"
#include "Collider.h"
#include "Object.h"
#include "SelectGDI.h"

// 충돌중인지 충돌 시점인지 충돌 벗어났는지의 정보를 오브젝트가 가지고있음

void Collider::finalupdate()
{
	Vec2 vObjectPos = j_pOwner->GetPos();
	j_vFinalPos = vObjectPos + j_vOffsetPos;
	
	Vec2 vObjectPhspos = j_pOwner->GetPhyPos();
	j_vFinalPhyPos = vObjectPhspos + j_vOffsetPos;

}



void Collider::render(HDC _dc)
{
	
	SelectGDI pen(_dc, PEN_TYPE::GREEN);
	SelectGDI brush(_dc, BRUSH_TYPE::HOLLOW);

	if(j_ISdraw)
	Rectangle(_dc, j_vFinalPhyPos.x - j_vScale.x / 2.f
		, j_vFinalPhyPos.y - j_vScale.y / 2.f
		, j_vFinalPhyPos.x + j_vScale.x / 2.f
		, j_vFinalPhyPos.y + j_vScale.y / 2.f);
	
	

}

Collider::Collider()
	: j_pOwner{nullptr}
	, j_bPrevCol{}
	, j_ISdraw{}
{
}

Collider::~Collider()
{
}

bool IsCollision(Collider* _leftcol, Collider* _rightcol)
{
	Vec2 vLPos = _leftcol->GetFinalPos();
	Vec2 vRPos = _rightcol->GetFinalPos();

	Vec2 vLScale = _leftcol->GetScale();
	Vec2 vRScale = _rightcol->GetScale();

	if (abs(vRPos.x - vLPos.x) <= (vLScale.x + vRScale.x) / 2.f
		&& abs(vRPos.y - vLPos.y) <= (vLScale.y + vRScale.y) / 2.f)
	{
		return true;
	}

	else {
		
		return false;
	}
}
