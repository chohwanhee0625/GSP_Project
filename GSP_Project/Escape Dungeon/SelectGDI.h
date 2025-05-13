#pragma once

class SelectGDI
{
private:
	HDC j_mdc;
	HPEN j_hDefaultPen;
	HBRUSH j_hDefaultBrush;

public:
	SelectGDI(HDC _dc, PEN_TYPE pt);
	SelectGDI(HDC _dc, BRUSH_TYPE bt);
	~SelectGDI();


};

