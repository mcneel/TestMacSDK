
#pragma once // SDK Header

#if defined (RHINO_SDK_MFC)

#define RD_MOVE						0xffffffff
#define RD_MOVE_HALF				0xfffffffd
#define RD_MOVE_QUARTER				0xfffffffc
#define RD_MOVE_3Q					0xfffffffe
#define RD_MOVE_THIRD				0xfffffffb
#define RD_MOVE_2T					0xfffffffa

#define DECLARE_RESIZING_MAP						void RepositionControls(BOOL32 bInit);
#define RESIZING_MINSIZE(mx,my,s,r)					if(r->right-r->left < mx) { if((s == WMSZ_BOTTOMLEFT)||(s == WMSZ_LEFT)||(s == WMSZ_TOPLEFT)) r->left = r->right-mx; else r->right = r->left+mx; } if(r->bottom-r->top < my) { if((s == WMSZ_TOP)||(s == WMSZ_TOPLEFT)||(s == WMSZ_TOPRIGHT)) r->top = r->bottom-my; else r->bottom = r->top+my; }
#define RESIZING_MAXSIZE(mx,my,s,r)					if(r->right-r->left > mx) { if((s == WMSZ_BOTTOMLEFT)||(s == WMSZ_LEFT)||(s == WMSZ_TOPLEFT)) r->left = r->right-mx; else r->right = r->left+mx; } if(r->bottom-r->top > my) { if((s == WMSZ_TOP)||(s == WMSZ_TOPLEFT)||(s == WMSZ_TOPRIGHT)) r->top = r->bottom-my; else r->bottom = r->top+my; }
#define BEGIN_RESIZING_MAP(class)					void class::RepositionControls(BOOL32 bInit) { int i=0;
#define BEGIN_RESIZING_MAP_SUBCLASS(class, base)	void class::RepositionControls(BOOL32 bInit) { base::RepositionControls(bInit); int i=0;
#define END_RESIZING_MAP							}

#define HIDE_CONTROL(id, width)			HideControl(id, width);

#define CONTROL(id,l,t,r,b)				bInit ?	InitControl(id,l,t,r,b,0,i) : SizeControl(id,l,t,r,b,0,i);i++;

#define FIXED_TOPRIGHT(id)				CONTROL(id, 0,				RD_MOVE,	RD_MOVE,			0)
#define FIXED_TOPLEFT(id)				CONTROL(id, RD_MOVE,		RD_MOVE,	0,					0)
#define FIXED_BOTTOMRIGHT(id)			CONTROL(id, 0,				0,			RD_MOVE,			RD_MOVE)
#define FIXED_BOTTOMLEFT(id)			CONTROL(id, RD_MOVE,		0,			0,					RD_MOVE)

#define STRETCH_HORZ_TOP(id)			CONTROL(id, RD_MOVE,		RD_MOVE,	RD_MOVE,			0)
#define STRETCH_HORZ_BOTTOM(id)			CONTROL(id, RD_MOVE,		0,			RD_MOVE,			RD_MOVE)
#define STRETCH_VERT_LEFT(id)			CONTROL(id, RD_MOVE,		RD_MOVE,	0,					RD_MOVE)
#define STRETCH_VERT_RIGHT(id)			CONTROL(id, 0,				RD_MOVE,	RD_MOVE,			RD_MOVE)
#define STRETCH_ALL(id)					CONTROL(id, RD_MOVE,		RD_MOVE,	RD_MOVE,			RD_MOVE)
#define STRETCH_HALF_LEFT(id)			CONTROL(id, RD_MOVE,		RD_MOVE,	RD_MOVE_HALF,		0)
#define STRETCH_HALF_RIGHT(id)			CONTROL(id, RD_MOVE_HALF,	RD_MOVE,	RD_MOVE,			0)
#define MOVE_HALF_HORZ(id)				CONTROL(id, RD_MOVE_HALF,	RD_MOVE,	RD_MOVE_HALF,		0)
#define STRETCH_HALF_TOP(id)			CONTROL(id, RD_MOVE,		RD_MOVE,		RD_MOVE,		RD_MOVE_HALF)
#define STRETCH_HALF_BOTTOM(id)			CONTROL(id, RD_MOVE,		RD_MOVE_HALF,	RD_MOVE,		RD_MOVE)
#define MOVE_HALF_VERT(id)				CONTROL(id, RD_MOVE,		RD_MOVE_HALF,	RD_MOVE,		RD_MOVE_HALF)

//Horizontal sets of four controls
#define HORZ_4SET_1_FIXED_BOTTOM(id)	CONTROL(id, RD_MOVE,		0,			RD_MOVE_QUARTER,	RD_MOVE)
#define HORZ_4SET_2_FIXED_BOTTOM(id)	CONTROL(id, RD_MOVE_QUARTER,0,			RD_MOVE_HALF,		RD_MOVE)
#define HORZ_4SET_3_FIXED_BOTTOM(id)	CONTROL(id, RD_MOVE_HALF,	0,			RD_MOVE_3Q,			RD_MOVE)
#define HORZ_4SET_4_FIXED_BOTTOM(id)	CONTROL(id, RD_MOVE_3Q,		0,			RD_MOVE,			RD_MOVE)

#define HORZ_4SET_1_FIXED_TOP(id)		CONTROL(id, RD_MOVE,		RD_MOVE,	RD_MOVE_QUARTER,	0)
#define HORZ_4SET_2_FIXED_TOP(id)		CONTROL(id, RD_MOVE_QUARTER,RD_MOVE,	RD_MOVE_HALF,		0)
#define HORZ_4SET_3_FIXED_TOP(id)		CONTROL(id, RD_MOVE_HALF,	RD_MOVE,	RD_MOVE_3Q,			0)
#define HORZ_4SET_4_FIXED_TOP(id)		CONTROL(id, RD_MOVE_3Q,		RD_MOVE,	RD_MOVE,			0)


//Horizontal sets of three controls
#define HORZ_3SET_1_FIXED_BOTTOM(id)	CONTROL(id, RD_MOVE,		0,			RD_MOVE_THIRD,		RD_MOVE)
#define HORZ_3SET_2_FIXED_BOTTOM(id)	CONTROL(id, RD_MOVE_THIRD,	0,			RD_MOVE_2T,			RD_MOVE)
#define HORZ_3SET_3_FIXED_BOTTOM(id)	CONTROL(id, RD_MOVE_2T,		0,			RD_MOVE,			RD_MOVE)

#define HORZ_3SET_1_FIXED_TOP(id)		CONTROL(id, RD_MOVE,		RD_MOVE,	RD_MOVE_THIRD,		0)
#define HORZ_3SET_2_FIXED_TOP(id)		CONTROL(id, RD_MOVE_THIRD,	RD_MOVE,	RD_MOVE_2T,			0)
#define HORZ_3SET_3_FIXED_TOP(id)		CONTROL(id, RD_MOVE_2T,		RD_MOVE,	RD_MOVE,			0)

class RHRDK_SDK CRhRdkResizingDialog : public CRhinoDialog
{
public:
	explicit CRhRdkResizingDialog(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
	explicit CRhRdkResizingDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	virtual ~CRhRdkResizingDialog();

	/** Override this method to transfer data into the UI. Typically used to read data
		from storage and display it in edit boxes, check boxes, combo boxes and so on.
		The default implementation does nothing. */
	virtual void DisplayData(void);

	/** Override this method to enable or disable the controls on the dialog.
		This is typically used for 'graying in and out' controls using EnableWindow().
		The default implementation does nothing. */
	virtual void EnableDisableControls(void);

	/** Override this method to stop certain controls from being automatically shortened.
		Call the base class if you don't know. */
	virtual BOOL32 IsControlClipped(CWnd* pWnd);

	/** Called from OnInitDialog() just after calling the base class but before anything else. */
	virtual void OnPreInitResizing(void);

public:
	// Access functions
	virtual void SetFillBackground(BOOL32 bFill);
	virtual void SetReleaseClipping(BOOL32 bRelease);
	virtual void SetAutoSubclassGroupBoxes(BOOL32 bSubclass);
	virtual BOOL32 AddControlString(HWND hwnd, const TCHAR* szText);
	virtual void SetResizingConstraints(BOOL32 bAllowResizeWidth, BOOL32 bAllowResizeHeight);

	// Emergency virtual function for future expansion.
	virtual void* EVF(const wchar_t* wszFunc, void* pvData);

protected:
	// CRhRdkWnd/CDialog overrides
	virtual BOOL32 OnInitDialog() override;
	virtual BOOL32 OnEraseBkgnd(CDC* pDC);

protected:
	// Support for resizing macros.
	virtual void RepositionControls(BOOL32 bInit);
	virtual void InitControl(UINT id, int l, int t, int r, int b, int o, int iPos);
	virtual void SizeControl(UINT id, int l, int t, int r, int b, int o, int iPos);
	virtual void HideControl(UINT id, int width);

protected:
	afx_msg void OnSize(UINT, int, int); 
	afx_msg void OnSysCommand(UINT,LPARAM);
	afx_msg BOOL32 OnToolTipNotify(UINT, NMHDR*,LRESULT*);
	afx_msg LRESULT OnNcHitTest(CPoint);
	afx_msg void OnDestroy(void);
	afx_msg LRESULT OnCtlColorBtn(WPARAM, LPARAM);
	afx_msg LRESULT OnCtlColorEdit(WPARAM, LPARAM);
	afx_msg LRESULT OnCtlColorListBox(WPARAM, LPARAM);
	afx_msg LRESULT OnCtlColorStatic(WPARAM, LPARAM);
	afx_msg LRESULT OnRdkPaintColors(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CRhRdkResizingDialog)

protected:
	// Internal utility functions.
	virtual void SubclassGroupBoxes();
	virtual void UnsubclassGroupBoxes();

private:
	/** \internal For RDK internal use only. */
	class CImpl;
	CImpl* m_pImpl;

	void CommonCtor();
};

#endif
