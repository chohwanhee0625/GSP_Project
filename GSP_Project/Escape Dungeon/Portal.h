#pragma once
#include "Object.h"
class Portal :
    public Object
{
    CImage*      j_img;

public:

    Portal();
    ~Portal();

public:

    void Setimg(CImage* img)override { j_img = img; }
    void render(HDC _dc)override;
    void Setting()override;

};

