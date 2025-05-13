#pragma once
#include "Object.h"

// ���� ��ų �ǰ� 
class Collider;
class BAttack;
class Monster;
class Portal;
class Fire;

class Player :
    public Object
{
private:

    PLAYERSTATE         j_act; // ���� ����
    PLAYERSTATE         j_prevact; // ���� ����
    int                 j_Hp; // HP����
    int                 j_Colltime; // ��ų ��Ÿ��
    RECT                j_ClientRect; // ������ ũ��
    Vec2                j_moved; // �̵��� üũ

    int                 j_Aniidx[(int)PLAYERSTATE::DEAD]; // �ִϸ��̼� ���� �ε��� ����
    bool                j_LR; // �¿��Ǻ� false�� �� true�� ��

    CImage              j_cimage; // Animation �̹��� ����
    Collider*           j_BodyCollider;
    int                 j_backwidth; // ������
    int                 j_coincount;

    bool                j_onportal;
    FMOD::System*       j_pSystem;
    FMOD::Channel*      j_pSoundChannel;
    FMOD::Sound*        j_pSounds[(int)SOUNDS::END];

    int                 j_jumpcount;
    bool                j_isice;

public:
    Player();
    ~Player();

public:
    void SetSystem(FMOD::System* _pS) { j_pSystem = _pS; }
    void SetChannel(FMOD::Channel* _pC) { j_pSoundChannel = _pC; }
   
    void SetSound(FMOD::Sound* _pS[(int)SOUNDS::END]) { 
        for (int i{}; i < (int)SOUNDS::END; ++i)
        {
            j_pSounds[i] = _pS[i];

        }
    }

    int GetHp() { return j_Hp; }
    void SetHP(int _i) { j_Hp = _i; }

    void SetMoved(Vec2 _v) { j_moved = _v; }
    Vec2 GetMoved() override;
    
   void SetRECT(RECT rect) { j_ClientRect = rect; }
    RECT getRect() { return j_ClientRect; }
   
    void Setimg(const CImage& img) { j_cimage = img; }
    CImage& Getimg() { return j_cimage; }
   
    void SetState(PLAYERSTATE state) { j_act = state;}
    void UpAniidx();
    void down() { j_moved.y += 3.f; }
    PLAYERSTATE GetPState() override{ return j_act; }
  
    bool GetLR() override{ return j_LR; }
   
    void SetPState(PLAYERSTATE pS) override { j_act = pS; }
    void finalupdate(int back_wid)override;
    void Setbackwidth(int backwid) { j_backwidth = backwid; }
    void upcoin() { ++j_coincount; }

    Collider* GetBodyCol() { return j_BodyCollider; }

    bool GetPortal() { return j_onportal; }
    int Getcoinnum() { return j_coincount; }
public: // update ��ġ,���� ������Ʈ
   
    void update(int width, bool Isattack);
    void updatestate(bool Isattack);
    void update_move(int width);
    

    void Goleft(bool onleft);
    void Goright(int width, bool onright);
    void updateLogPos();
    void onjump(bool space);
    void hit();
    void hit_1();

public: // �浹 �� ������
    void OnCollision_update(vector<Object*> grounds, vector<Object*> coins,
        vector<Monster*> monsters,vector<Object*> thorns, Portal& _Po, vector<Fire*> _fire);
    void OnCollision_Ground(vector<Object*> _pOther);
    void OnCollision_Coin(vector<Object*> _pOther);
    void OnCollision_Thorn(vector<Object*> _pOther);
    void OnCollision_Fire(vector<Fire*> _fire);


    void MakeBAttack(BAttack& attack);
    void OnCollision_Monster(vector<Monster*> _pOther);

    void OnCollision_Portal(Portal& _po);
    public: // animation(render)�׸���

    void render(HDC mdc)override;


};

