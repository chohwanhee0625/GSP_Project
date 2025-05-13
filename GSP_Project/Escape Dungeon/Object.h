#pragma once

class RigidBody;
class Collider;
class Gravity;
class Player;
class BAttack;
class Object
{
protected:
	Vec2			j_vLogPos;  // 중심 (논리좌표) 충돌검사등에서 쓰는 좌표
	Vec2			j_vScale;  // 크기 
	Vec2			j_vPhypo;   // 중심 (물리좌표) 그릴때만 쓰는 좌표 매프레임마다 주인공오브젝트의 이동값을 받아서 그값만큼 논리좌표에서 뺴서 구함
	RigidBody*		j_pRigidBody; // 물리강체포인터 없을거면 NULL
	Collider*		j_pCollider; // 출동체 포인터 없을거면 NULL
	Gravity*		j_pGravity; // 중력 포인터 없을거면 NULL
	
	OBJECTTYPE		j_Objtype; // 오브젝트 타입 ,ex) 플레이어, 몬스터, 땅 등등
	bool			j_bAlive; // 생사



public:
	Object();
	virtual ~Object();

public:
	virtual void Setting() {};
	virtual void update();
	virtual void finalupdate(int back_wid);
	virtual void render(HDC _dc);
	virtual void SetMoved(Vec2 _v);
	virtual Vec2 GetMoved();
	virtual PLAYERSTATE GetPState() {return PLAYERSTATE::STAND; }
	virtual RECT getRect() {return RECT();}
	virtual void SetPState(PLAYERSTATE pS) {}
	virtual void Setimg(const CImage& img) {}
	virtual void Setimg(CImage* img){}
	virtual void Upidx() {}
	virtual bool GetLR() { return false; }
	virtual void OnCollision_Ground(vector<Object*> _pOther) {}
	virtual void update_state(Player& p) {}
	virtual bool GetIsice() { return true; }
	virtual void SetIce(bool _b) {}
	
	void SetPos(Vec2 _vPos) { j_vPhypo = _vPos; j_vLogPos = _vPos; }
	void SetPsyPos(Vec2 _vPos){ j_vPhypo = _vPos; }
	void SetLogPos(Vec2 _vPos) { j_vLogPos = _vPos; }
	void SetScale(Vec2 _vScale) { j_vScale = _vScale; }
	void SetObject(Vec2 pos, Vec2 scale);
	void SetObject(RECT rect);
	void SetyPos(float _y) { j_vLogPos.y = _y; j_vPhypo.y = _y; }
	void SetAlive(bool _b) { j_bAlive = _b; }

	void AddyPos(float _y){ j_vLogPos.y += _y; j_vPhypo.y += _y; } 
	void AddxPos(float _f) { j_vLogPos.x += _f;}
	void AddPhyPos(float _f) { j_vPhypo.x += _f; }

	Vec2 GetPos() { return j_vLogPos; }
	Vec2 GetScale() { return j_vScale; }
	Vec2 GetPhyPos() { return j_vPhypo; }

	int GetLeft() { return (int)j_vLogPos.x - (int)j_vScale.x / 2.f; } // 충돌검사등에 사용
	int GetRight() { return (int)j_vLogPos.x + (int)j_vScale.x / 2.f; } // 충돌검사등에 사용
	int GetTop() { return (int)j_vLogPos.y - (int)j_vScale.y / 2.f; } // 충돌검사등에 사용
	int GetBottom() { return (int)j_vLogPos.y + (int)j_vScale.y / 2.f; } // 충돌검사등에 사용

	int GetPsyLeft() { return (int)j_vPhypo.x - (int)j_vScale.x / 2.f; } // 출력시 사용
	int GetPsyRight() { return (int)j_vPhypo.x + (int)j_vScale.x / 2.f; } // 출력시 사용
	int GetPsyTop() { return (int)j_vPhypo.y - (int)j_vScale.y / 2.f; } // 출력시 사용
	int GetPsyBottom() { return (int)j_vPhypo.y + (int)j_vScale.y / 2.f; } // 출력시 사용

	bool GetAlive() { return j_bAlive; }

	RigidBody* GetRigidbody() { return j_pRigidBody; }
	Collider* GetCollider() { return j_pCollider; }
	Gravity* GetGravity() { return j_pGravity; }
	OBJECTTYPE GetObjtype() { return j_Objtype; }

public:
	Object& GetObj() { return *this; }
	void CreateRidBody();
	void CreateCollider();
	void CreateGravity();
	
	void updatePhypo(Vec2 moved);
	
	


	virtual void OnCollision(Collider* _pOther) {}
	virtual void OnCollisionExit(Collider* _pOther) {}
};




bool RectsCollision(Object& rect1, Object& rect2);


