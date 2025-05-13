#pragma once
#include "Object.h"
class Coin :
    public Object
{
private:

    int     j_aniidx;
    CImage*  j_img;

public:
    Coin();
    Coin(Vec2 _v);
    ~Coin();

public:

    void Setimg(CImage* img)override { j_img = img; }
    

    void update()override;
    void render(HDC _dc) override;
    void Setting()override;
    void Upidx()override;

};

