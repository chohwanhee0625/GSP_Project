#pragma once
#include "Object.h"
class Monster :
    public Object
{
    MONSTERSTATE    j_act;
    bool            j_LR;
    int             j_HP;
    int             j_Aniidx[(int)MONSTERSTATE::DEAD+1];
    int             j_Hitcount;
    bool            j_boss;
    CImage*         j_pimg;

public:
    Monster();
    ~Monster();

public:
    void Setimg(CImage* img)override { j_pimg = img; }
    void Upidx()override;
    void Setting()override;
    void render(HDC _dc)override;
    void update(int width);
    void update_state(Player& p)override;
    bool OnCollider_BAttack(BAttack& _Ba);
    void OnCollision_Ground(vector<Object*> _pOther)override;
    bool GetLR() override{ return j_LR; }
    void SetHp(int _ihp) { j_HP = _ihp; }
    void SetLR(bool _b) { j_LR = _b; }
    void Setboss(bool _b) { j_boss = _b; }
    MONSTERSTATE GetMstate() { return j_act; }
};

