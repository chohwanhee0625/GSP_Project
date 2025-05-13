#pragma once

class RigidBody;
class Collider;
class Gravity;
class Player;
class BAttack;
class Object
{
protected:
	Vec2			j_vLogPos;  // �߽� (����ǥ) �浹�˻��� ���� ��ǥ
	Vec2			j_vScale;  // ũ�� 
	Vec2			j_vPhypo;   // �߽� (������ǥ) �׸����� ���� ��ǥ �������Ӹ��� ���ΰ�������Ʈ�� �̵����� �޾Ƽ� �װ���ŭ ����ǥ���� ���� ����
	RigidBody*		j_pRigidBody; // ������ü������ �����Ÿ� NULL
	Collider*		j_pCollider; // �⵿ü ������ �����Ÿ� NULL
	Gravity*		j_pGravity; // �߷� ������ �����Ÿ� NULL
	
	OBJECTTYPE		j_Objtype; // ������Ʈ Ÿ�� ,ex) �÷��̾�, ����, �� ���
	bool			j_bAlive; // ����



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

	int GetLeft() { return (int)j_vLogPos.x - (int)j_vScale.x / 2.f; } // �浹�˻� ���
	int GetRight() { return (int)j_vLogPos.x + (int)j_vScale.x / 2.f; } // �浹�˻� ���
	int GetTop() { return (int)j_vLogPos.y - (int)j_vScale.y / 2.f; } // �浹�˻� ���
	int GetBottom() { return (int)j_vLogPos.y + (int)j_vScale.y / 2.f; } // �浹�˻� ���

	int GetPsyLeft() { return (int)j_vPhypo.x - (int)j_vScale.x / 2.f; } // ��½� ���
	int GetPsyRight() { return (int)j_vPhypo.x + (int)j_vScale.x / 2.f; } // ��½� ���
	int GetPsyTop() { return (int)j_vPhypo.y - (int)j_vScale.y / 2.f; } // ��½� ���
	int GetPsyBottom() { return (int)j_vPhypo.y + (int)j_vScale.y / 2.f; } // ��½� ���

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


