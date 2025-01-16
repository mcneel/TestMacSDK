
#pragma once // Public SDK header

#include "RhRdkObject.h"

class CRhRdkColor;

/** \class IRhRdkAngleDialDecorator

	STARTING WITH RHINO 8, THIS INTERFACE IS DEPRECATED AND DOES NOT WORK.

*/
RDK_DEPRECATED_CLASS class RHRDK_SDK IRhRdkAngleDialDecorator
{
public:
	virtual ~IRhRdkAngleDialDecorator() { }
	virtual void DeleteThis(void) = 0;
	virtual void DialColors(CRhRdkColor& colOut1, CRhRdkColor& colOut2) const = 0;
	virtual void Draw(HDC dc, const ON_4iRect& dialRect, double dRadius, double dAngle, bool bEnabled, bool bHot) const = 0;
	virtual IRhRdkAngleDialDecorator* NextDecorator(void) const = 0;
	virtual void* EVF(const wchar_t*, void*) = 0;
};

#if defined ON_RUNTIME_WIN

/** \class CRhRdkAngleCtrl

	STARTING WITH RHINO 8, THIS CONTROL IS DEPRECATED AND DOES NOT WORK. See [DEPCTL]

*/
RDK_DEPRECATED_CLASS class RHRDK_SDK CRhRdkAngleCtrl : public CRhRdkObject
{
public:
	enum RDK_DEPRECATED eStyle { unmarked, compass, azimuth, altitude, altitude0, force32_style = 0xFFFFFFFF };

	enum class Style : unsigned int { kUnmarked, kCompass, kAzimuth, kAltitude, kAltitude0 };

	RDK_DEPRECATED CRhRdkAngleCtrl(eStyle s); // Not implemented.
	CRhRdkAngleCtrl(Style s);
	virtual ~CRhRdkAngleCtrl();

	virtual bool Create(DWORD dwStyle, const ON_4iRect& rect, HWND pParent, UINT uID);
	virtual double Angle(void) const;
	virtual void SetAngle(double dAngle);
	virtual void SetBackgroundColor(COLORREF col);
	virtual void SetDialColors(COLORREF col1, COLORREF col2);
	virtual void SetDecoratorPre(IRhRdkAngleDialDecorator* pDecorator);
	virtual void SetDecoratorPost(IRhRdkAngleDialDecorator* pDecorator);
	virtual void SetHotTrackingOn(bool bOn);
	virtual void SetDebugColorsOn(bool bOn);
	operator HWND (void) const;
	HWND Wnd(void) const;
	virtual bool IsWindow(void) const;
	virtual void EnableWindow(bool bEnable);
	virtual void ShowWindow(bool bShow);
	virtual void MoveWindow(const ON_4iRect& rect);
	static UINT ChangedMessageNumber(void);
	static UINT EndDragMessageNumber(void);

public:
	class CImpl;
	CImpl* m_pImpl;
};

#endif
