#pragma once
#include "Object.h"
class Thorn :
    public Object
{
    CImage*      j_img;

public:

    Thorn();
    ~Thorn();

public:


    void Setimg(CImage* img)override { j_img = img; }
    void render(HDC _dc)override;
    void Setting()override;

};

