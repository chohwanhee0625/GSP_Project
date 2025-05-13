#pragma once
#include "Object.h"
class Collider;

class Ground :
    public Object
{
private:
    bool        j_Isice;
    CImage*     j_img;

    void OnCollision(Collider* _pOther)override;
    void OnCollisionExit(Collider* _pOther)override;

public:
    Ground();
    ~Ground();
    void Setting()override;
    void render(HDC _dc)override;
    bool GetIsice() override { return j_Isice; }
    void SetIce(bool _b) { j_Isice = _b; }
    void Setimg(CImage* img)override { j_img = img; }
};

