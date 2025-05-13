#include "pch.h"
#include "Portal.h"
#include "Collider.h"
Portal::Portal()
	:j_img{}
{
	j_Objtype = OBJECTTYPE::PORTAL;
	CreateCollider();
	SetPos(Vec2(4075.f, 515.f));
	SetScale(Vec2(60.f, 60.f));
}

Portal::~Portal()
{
}

void Portal::render(HDC _dc)
{
	j_img->Draw(_dc, GetPsyLeft(), GetPsyTop(), j_vScale.x, j_vScale.y
		, 0
		, 0
		, j_img->GetWidth()
		, j_img->GetHeight());
	GetCollider()->render(_dc);
}

void Portal::Setting()
{
	GetCollider()->SetScale(Vec2(GetScale()));
	GetCollider()->finalupdate();
}

