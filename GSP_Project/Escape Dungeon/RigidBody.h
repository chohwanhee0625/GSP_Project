#pragma once

class Object;

class RigidBody
{
private:
	Object*		j_pOwner;
	Vec2		j_vForce;	// ũ�� ����
	
	Vec2		j_vAccel;	// ���ӵ�
	Vec2		j_vAccelA;	// �߰� ���ӵ� 
	float		j_fMass; 	// ����
	Vec2		j_vVelocity; //�ӵ�(ũ�� : �ӷ�, ����)

	Vec2		j_vMaxVelocity; // �ִ�ӵ�

	float		j_fFric;		// ���� ���

	// F = M*A;
	// V += A*timer


public:
	RigidBody();
	~RigidBody();

public:
	void finalupdate(int back_wid);

	void Move(int back_wid);


public:
	void AddForce(Vec2 _vF){j_vForce += _vF;}
	void SetMass(float fmass){j_fMass = fmass;}
	float GetMass(){return j_fMass;}
	float GetSpeed() { return j_vVelocity.Length(); }
	Vec2 GetVelocity() { return j_vVelocity; }


	void SetVelocity(Vec2 _v) { j_vVelocity = _v; }
	void AddVelocity(Vec2 _v) { j_vVelocity += _v; }
	void SetMaxVelocity(Vec2 _v) { j_vMaxVelocity = _v; }
	void SetAccelAlpha(Vec2 _vAccel) { j_vAccelA = _vAccel; }
	void Setfirc(float _f) { j_fFric = _f; }
	float Getfirc() { return j_fFric; }
	void Hitted();
	void Hitted_1();

	friend class Object;
};

