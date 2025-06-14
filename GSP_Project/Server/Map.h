#pragma once


class Map
{
public:
	static Map& GetInstance() {
		static Map instance;
		return instance;
	}

	static bool IsObstacle(short x, short y)
	{
		if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) return false;
		return GetInstance().obstacles[x][y];
	}

private:
	Map()
	{
		for (unsigned short x = 0; x < MAP_WIDTH; ++x) {
			for (unsigned short y = 0; y < MAP_HEIGHT; ++y) {
				obstacles[x][y] = false;
			}
		}
	}
	~Map() = default;

	Map(const Map&) = delete;
	Map& operator=(const Map&) = delete;

private:
	bool obstacles[MAP_WIDTH][MAP_HEIGHT] = {};  // false: ºó °ø°£, true: º®
};

