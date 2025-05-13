#include "pch.h"
#include "SelectGDI.h"

extern HBRUSH arrBrushs[(int)BRUSH_TYPE::END];
extern HPEN arrPens[(int)PEN_TYPE::END];

SelectGDI::SelectGDI(HDC _dc, PEN_TYPE pt)
	: j_mdc{_dc}
	, j_hDefaultBrush{nullptr}
	, j_hDefaultPen{ nullptr }
{
	HPEN hPen = GetPen(arrPens, pt);
	j_hDefaultPen = (HPEN)SelectObject(_dc, hPen);

}

SelectGDI::SelectGDI(HDC _dc, BRUSH_TYPE bt)
	: j_mdc{_dc}
	, j_hDefaultBrush{ nullptr }
	, j_hDefaultPen{ nullptr }
{
	HBRUSH hBrush = GetBrush(arrBrushs, bt);
	j_hDefaultBrush = (HBRUSH)SelectObject(_dc, hBrush);
}

SelectGDI::~SelectGDI()
{
	SelectObject(j_mdc, j_hDefaultBrush);
	SelectObject(j_mdc, j_hDefaultPen);
}
