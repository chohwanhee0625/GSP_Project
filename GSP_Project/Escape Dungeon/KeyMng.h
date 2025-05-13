#pragma once

class KeyMng
{

	bool	left;
	bool	up;
	bool	down;
	bool	right;
	bool	space;
	bool	shift;
	bool	control;
	bool    enter;

public:

	KeyMng();
	~KeyMng();
	void init();

	void onenter() { enter = true; }
	void offenter() { enter = false; }
	bool checkenter() { return enter; }

	void onleft() { left = true; }
	void offleft() { left = false; }
	bool checkleft() { return left; }

	void onup() { up = true; }
	void offup() { up = false; }
	bool checkup() { return up; }

	void ondown() { down = true; }
	void offdown() { down = false; }
	bool checkdown() { return down; }

	void onright() { right = true; }
	void offright() { right = false; }
	bool checkright() { return right; }

	void onspace() { space = true; }
	void offspace() { space = false; }
	bool checkspace() { return space; }

	void oncontrol() { control = true; }
	void offcontrol() { control = false; }
	bool checkcontrol() { return control; }

	void onshift() { shift = true; }
	void offshift() { shift = false; }
	bool checkshift() { return shift; }

};

