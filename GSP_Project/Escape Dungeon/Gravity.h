#pragma once

class Object;
class Gravity
{
private:
	Object*			j_pOwner;
	bool			j_bGround;


public:
	Gravity();
	~Gravity();


	void finalupdate();
	void SetGround(bool _b);
	bool GetGround() { return j_bGround; }

	friend class Object;
};

