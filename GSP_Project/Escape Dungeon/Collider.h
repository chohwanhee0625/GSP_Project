#pragma once

class Object;

class Collider
{
private:
	Object*			j_pOwner;			// 가르키는 오브젝트
	Vec2			j_vOffsetPos;		// 오브젝트로 부터 상대적인 위치
	Vec2			j_vFinalPos;		// 매 프레임마다 계산 리얼 위치 논리좌표
	Vec2			j_vFinalPhyPos;		// 매 프레임마다 계산 물리좌표 찍힐좌표
	Vec2			j_vScale;			// 충돌체 크기
	bool			j_bPrevCol[(int)OBJECTTYPE::NONE];
	bool			j_ISdraw;

public:
	void finalupdate();
	void render(HDC _dc);
	void SetOffsetPos(Vec2 _vPos) { j_vOffsetPos = _vPos;}
	void SetScale(Vec2 _vScale) { j_vScale = _vScale; }
	void SetPrevCol(OBJECTTYPE _Otype, bool _b) { j_bPrevCol[(int)_Otype] = _b; }
	bool GetPrevCol(OBJECTTYPE _Otype) { return j_bPrevCol[(int)_Otype]; }
	void Changedraw() {
		if (j_ISdraw) j_ISdraw = false;
		else j_ISdraw = true;
	}


	Vec2 GetOffsetPos() { return j_vOffsetPos; }
	Vec2 GetScale() { return j_vScale; }
	Vec2 GetFinalPos() { return j_vFinalPos; }
	Object* GetObj() { return j_pOwner; }
	


public:
	Collider();
	~Collider();

	friend class  Object;
	friend class Player;
};

bool IsCollision(Collider* _leftcol, Collider* _rightcol);
