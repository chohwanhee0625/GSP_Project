#pragma once
#include "Object.h"

class Object;
class Fire :
    public Object
{
private:
    
    CImage*      j_img;
    int          j_aniidx;
    int          j_Landed;
    bool         j_LR;
    bool         j_colcheck;

public:
    Fire();
    ~Fire();

    void render(HDC _dc);
    void update(int back_wid);
    void Upidx();
    void Setimg(CImage* img)override { j_img = img; }
    void Collider_ground(vector<Object*> _pground);
};

