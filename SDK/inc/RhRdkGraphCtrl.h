
#pragma once // Public SDK header

#if defined ON_RUNTIME_WIN && defined RHINO_SDK_MFC

#include "RhRdkDefinitions.h"

RDK_DEPRECATED_CLASS class RHRDK_SDK CRhRdkGraphCtrl : public CWnd
{
public:
	CRhRdkGraphCtrl();
	virtual ~CRhRdkGraphCtrl();

	/** Create the control. */
	virtual bool CreateWnd(DWORD dwStyle, const CRect& rect, HWND pParent, UINT id);

	/** Query whether or not the area under the graph is filled. */
	/** \return \e true if filled, else \e false. */
	virtual bool IsFilled(void) const;

	/** Set whether or not the area under the graph is filled. */
	virtual void SetIsFilled(bool bFilled);

	/** \return The background color. */
	virtual COLORREF BackgroundColor(void) const;

	/** Set the background color. */
	virtual void SetBackgroundColor(COLORREF col);

	/** \return The grid color. */
	virtual COLORREF GridColor(void) const;

	/** Set the grid color. */
	virtual void SetGridColor(COLORREF col);

	/** \return The outline color. */
	virtual COLORREF OutlineColor(void) const;

	/** Set the outline color. */
	virtual void SetOutlineColor(COLORREF col);

	/** \return The interior color. */
	virtual COLORREF InteriorColor(void) const;

	/** Set the interior color. */
	virtual void SetInteriorColor(COLORREF col);

	/** \return The number of grid lines across and down the graph. */
	virtual int NumGridLines(void) const;

	/** Set the number of grid lines across and down the graph. */
	virtual void SetNumGridLines(int iNum);

	/** \return The level used to scale the V value returned by GraphFunction(). */
	virtual double Level(void) const;

	/** Set the level used to scale the V value returned by GraphFunction(). */
	virtual void SetLevel(double d);

	/** This method is called when the graph needs to be refreshed.
		Optionally override this method to update any variables prior to the graph being repainted.
		If you override this method, <b>please be sure to call the base class</b>. */
	virtual void Refresh(void);

	/** This method is called when the graph needs to be repainted.
		If you override this method, <b>please be sure to call the base class</b>. */
	virtual void SafeInvalidate(void);

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t* wszFunc, void* pvData);

public:
	/** Implement this method to return the V value (0 to 1) corresponding to the supplied U value (0 to 1). */
	virtual double GraphFunction(double u) const = 0;

protected:
	afx_msg int OnCreate(LPCREATESTRUCT);
	afx_msg void OnDestroy();
	afx_msg BOOL32 OnEraseBkgnd(CDC*);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnQueryFocus(WPARAM, LPARAM) { return 1; }
	DECLARE_MESSAGE_MAP()

	virtual void PreSubclassWindow() override;

	/** \internal For RDK internal use only. */
	void Initialize(void);

private:
	/** \internal For RDK internal use only. */
	class CImpl;
	CImpl* m_pImpl;
};

#endif
