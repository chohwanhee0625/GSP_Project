#include "pch.h"
#include "KeyMng.h"

KeyMng::KeyMng()
	: left{false}
	, up{ false }
	, down{ false }
	, right{ false }
	, space{ false }
	, shift{ false }
	, control{ false }
{	  
}	  

KeyMng::~KeyMng()
{
	
}

void KeyMng::init()
{
	left = false;
	up = false;
	down = false;
	right = false;
	space = false;
	shift = false;
	control = false;
}
