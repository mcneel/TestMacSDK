/////////////////////////////////////////////////////////////////////////////
// RhinoSdkUiCheckListBox.h
//

#pragma once

#if defined (ON_OS_WINDOWS)

/////////////////////////////////////////////////////////////////////////////
// CRhinoUiListBox.h
//

class RHINO_SDK_CLASS CRhinoUiListBox : public CListBox
{
  DECLARE_DYNAMIC(CRhinoUiListBox)

public:
  // Constructors
  CRhinoUiListBox();
  virtual BOOL32 Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

  virtual ~CRhinoUiListBox();

protected:
  virtual void PreSubclassWindow();
  virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
};


/////////////////////////////////////////////////////////////////////////////
// CRhinoUiCheckListBox.h
//

class RHINO_SDK_CLASS CRhinoUiCheckListBox : public CCheckListBox
{
	DECLARE_DYNAMIC(CRhinoUiCheckListBox)

public:
  // Constructors
	CRhinoUiCheckListBox();
  virtual BOOL32 Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

  virtual ~CRhinoUiCheckListBox();

  /*
  Description:
    Enables or disables right button click to uncheck all checked items and
    only check the item that was clicked on.
  */
  void EnableRightButtonClickCheckOne(bool bEnable);

  /*
  Description:
    Enables or disabled left mouse button dragging to check items.
  */
  void EnableLeftButtonDragCheck(bool bEnable);

protected:
  virtual void PreSubclassWindow();
  virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
  afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnMouseMove(UINT nFlags, CPoint point);
  DECLARE_MESSAGE_MAP()

private:
  bool m_bRightButtonClickCheckOne = false;
  bool m_bLeftButtonDragCheck = false;
};

#else

#define CRhinoUiListBox CListBox
#define CRhinoUiCheckListBox CCheckListBox

#endif // if defined (ON_OS_WINDOWS)
