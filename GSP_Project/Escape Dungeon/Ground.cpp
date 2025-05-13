#include "pch.h"
#include "Object.h"
#include "Ground.h"
#include "Collider.h"
#include "Gravity.h"

// 위치 조정하고 그래피티 그라운드 false로 바꿈


void Ground::OnCollision(Collider* _pOther)
{
	Object* pOtherobj = _pOther->GetObj();

	if (OBJECTTYPE::PLAYER == pOtherobj->GetObjtype()) {

		if (true == IsCollision(GetCollider(), _pOther)) //&& true == _pOther->GetPrevCol(OBJECTTYPE::GROUND)
		{
			pOtherobj->GetGravity()->SetGround(true);

			Vec2 vObjPos = _pOther->GetFinalPos();
			Vec2 vObjScale = _pOther->GetScale();

			Vec2 vPos = GetCollider()->GetFinalPos();
			Vec2 vScale = GetCollider()->GetScale();

			float fLen = abs(vObjPos.y - vPos.y);
			float fValue = (vObjScale.y / 2.f + vScale.y / 2.f) - fLen;


			vObjPos = pOtherobj->GetPos();
			vObjPos.y -= (fValue - 1.f);

			//pOtherobj->SetMoved(vPmoved);


			pOtherobj->SetyPos(vObjPos.y);

			_pOther->finalupdate();
			//_pOther->SetPrevCol(OBJECTTYPE::GROUND, true);
		}

	}
}

// 그래비티 그라운드 true로 바꿈
void Ground::OnCollisionExit(Collider* _pOther)
{
	Object* pOtherobj = _pOther->GetObj();

	if (OBJECTTYPE::PLAYER == pOtherobj->GetObjtype()) {
	
	if (false == IsCollision(GetCollider(), _pOther) && _pOther->GetPrevCol(OBJECTTYPE::GROUND))
		{
		pOtherobj->GetGravity()->SetGround(false);

		}
	}
}




Ground::Ground()
	: 
	j_img{nullptr}
	, j_Isice{}
{
	CreateCollider();
	j_Objtype = OBJECTTYPE::GROUND;
}

Ground::~Ground()
{
}

void Ground::Setting()
{
	GetCollider()->SetScale(Vec2(GetScale()));
	GetCollider()->finalupdate();
}

void Ground::render(HDC _dc)
{
	if (nullptr != j_img)
	{
		j_img->Draw(_dc, GetPsyLeft(), GetPsyTop(), j_vScale.x, j_vScale.y
			, 0
			, 0
			, j_img->GetWidth()
			, j_img->GetHeight());
	}
	
	j_pCollider->render(_dc);
}


