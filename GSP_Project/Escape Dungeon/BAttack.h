#pragma once
#include "Object.h"

class Player;

class BAttack :
    public Object
{
private:
    int             j_count; // 남아있을시간
    CImage          j_Limg;   // 이미지
    CImage          j_Rimg;
    bool            j_LR;

public:
    BAttack();
    BAttack(Vec2 _v);
    ~BAttack();

public:
    void SetLimg(const CImage& img) { j_Limg = img; }
    void SetRimg(const CImage& img) { j_Rimg = img; }


public:
    void update(Player& oplayer);
    void render(HDC _dc)override;
    void Setting()override;
};

