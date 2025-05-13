#pragma once

struct Vec2
{
	float x;
	float y;

public:
	Vec2()
		: x{ 0.f }
		, y{ 0.f }
	{
	}

	Vec2(float _x, float _y)
		: x{ _x }
		, y{ _y }
	{}

	Vec2(int _x, int _y)
		: x{ static_cast<float>(_x) }
		, y{ static_cast<float>(_y) }
	{}

	Vec2(long _x, long _y)
		: x{ static_cast<float>(_x) }
		, y{ static_cast<float>(_y) }
	{}

public:
	
	Vec2 operator -()
	{
		return Vec2(-x, -y);
	}

	


	void operator +=(Vec2 other)
	{
		x += other.x;
		y += other.y;

	}

	void operator -=(Vec2 other)
	{
		x -= other.x;
		y -= other.y;

	}

	void operator *=(float _f)
	{
		x *= _f;
		y *= _f;
	}


	Vec2 operator *(float f)
	{
		return Vec2(x * f, y * f);
	}

	Vec2 operator /(float f)
	{
		if (f != 0.f)
			return Vec2(x / f, y / f);
	}

	Vec2 operator -(float _f)
	{
		return Vec2(x - _f, y - _f);
	}

	Vec2 operator *(int i)
	{
		return Vec2(x * i, y * i);
	}
	
	Vec2 operator *(Vec2 _v)
	{
		return Vec2(x * _v.x ,  y * _v.y);
	}
	
	Vec2 operator +(Vec2 _v)
	{
		return Vec2(x + _v.x, y + _v.y);
	}

	Vec2 operator -(Vec2 _v)
	{
		return Vec2(x - _v.x, y - _v.y);
	}

	Vec2 operator /(Vec2 _v)
	{
		if(_v.x != 0 && _v.y != 0)
		return Vec2(x / _v.x, y / _v.y);
	}


public:

	float Length()
	{
		return sqrt(x * x + y * y);
	}
	float Length(Vec2 other)
	{
		return sqrt((x - other.x)* (x - other.x) + (y - other.y) * (y - other.y));
	}

	Vec2 Nomalize()
	{
		float Len = Length();

		assert(Len != 0.f);

		x /= Len;
		y /= Len;

		return *this;
	}

	bool Iszero()
	{
		if (0.f == x && 0.f == y)return true;
		else return false;
	}
};