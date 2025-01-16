
#pragma once // Public SDK header

#include "RhRdkObject.h"

/** \class CRhRdkCurveGraphEvaluator

	Calculates the Y coordinate of a curve graph at any X coordinate.

	\see CRhRdkCurveGraph

*/
class RHRDK_SDK CRhRdkCurveGraphEvaluator
{
public:
	CRhRdkCurveGraphEvaluator(const wchar_t* wszXML);
	virtual ~CRhRdkCurveGraphEvaluator();

	/** \return The number of curves available to the evaluator. */
	virtual int CurveCount(void) const;

	/** Calculates the Y coordinate of a curve at the given X coordinate.
		\param iCurveIndex is the curve index of one of the curves on the graph.
		\param dX is the X value to get the Y value for.
		\return The Y coordinate at the given X coordinate or ON_UNSET_VALUE if the curve index is invalid. */
	virtual double GetCurveY(int iCurveIndex, double dX) const;

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t* wszFunc, void* pvData);

private:
	/** \internal For RDK internal use only. */
	class CImpl;
	CImpl* m_pImpl;
};

class CRhRdkInternalCurveGraphEvaluatorData
{
public:
	ON_ClassArray<ON_SimpleArray<ON_2fPoint>> m_points;
	ON_ClassArray<ON_SimpleArray<ON_4fPoint>> m_segments;
};

RDK_DEPRECATED_CLASS class RHRDK_SDK IRhRdkCustomCurveCtrl : public IRhinoUiWindow
{
public:
	class RHRDK_SDK IPoint
	{
	public:
		virtual ~IPoint() { }

		virtual double X(void) const = 0;

		virtual double Y(void) const = 0;

		/** \return the index of the curve the point is on.
			For RGB[A] curves, 0=red, 1=green, 2=blue [, 3=alpha] */
		virtual int CurveIndex(void) const = 0;

		/** Emergency virtual function for future expansion. */
		virtual void* EVF(const wchar_t* wszFunc, void* pvData) = 0;
	};

	/** Resets the control to factory defaults. */
	virtual void Reset(void) = 0;

	/** Adds a point to a curve. */
	virtual void AddPoint(int iCurve, double x, double y, double dTanX, double dTanY, bool bSelect) = 0;

	/** Adds a point to each curve in the control. */
	virtual void AddPoints(double x, double y, double dTanX, double dTanY, bool bSelect) = 0;

	/** Reset the scale and origin of the axes to their factory settings.
		Change scale values to < 1 to show axes even when no curves are present. */
	virtual void ResetAxes(double dScaleX=1.0, double dScaleY=1.0) = 0;

	/** \return \e true if the fixed grid frequency option is on. */
	virtual bool GridFrequencyOn(void) const = 0;

	/** By default, the grid looks nice and shows zooming but has no real meaning.
	    Use this method to switch the grid to show a fixed frequency instead.
		\see SetGridFrequencies() */
	virtual void SetGridFrequencyOn(bool bOn) = 0;

	/** Set the frequency of the major and minor grids in each axis. The defaults are major=1.0 and minor=0.1.
		\param dMajorFreqX sets the frequency of the x-axis major grid. Use zero to disable this grid.
		\param dMinorFreqX sets the frequency of the x-axis minor grid. Use zero to disable this grid.
		\param dMicroFreqX sets the frequency of the x-axis micro grid. Use zero to disable this grid.
		\param dMajorFreqY sets the frequency of the y-axis major grid. Use zero to disable this grid.
		\param dMinorFreqY sets the frequency of the y-axis minor grid. Use zero to disable this grid.
		\param dMicroFreqY sets the frequency of the y-axis micro grid. Use zero to disable this grid.
		\note These settings are only used after SetGridFrequencyOn(true) has been called. */
	virtual void SetGridFrequencies(double dMajorFreqX, double dMinorFreqX, double dMicroFreqX, double dMajorFreqY, double dMinorFreqY, double dMicroFreqY) = 0;

	/** Select all the points on all the curves.
		\param bIncludeTanPoints indicates if tangent points should also be selected. */
	virtual void SelectAllPoints(bool bIncludeTanPoints) = 0;

	/** Unselect all the points on all enabled curves.
		\param bIncludeTanPoints indicates if tangent points should also be unselected. */
	virtual void UnselectAllPoints(bool bIncludeTanPoints) = 0;

	/** Invert the selection on all enabled curves.
		\param bIncludeTanPoints indicates if tangent points should also be affected. */
	virtual void InvertSelection(bool bIncludeTanPoints) = 0;

	/** Count how many points are selected on all the curves.
		\param bIncludeTanPoints indicates if tangent points should also be counted. */
	virtual int CountSelection(bool bIncludeTanPoints) const = 0;

	/** Move all the selected points on all the curves by a pixel offset.
		If snap-to-grid is on, points will be snapped to the grid. \see PointSnapX().
		If the scale mode is 'constraints' for any axis, that axis' movement
		is limited by its constraint values. \see ScaleMode(). */
	virtual void MoveSelectedPoints(int dx, int dy) = 0;

	/** Delete all the selected points from all the curves. */
	virtual void DeleteSelectedPoints(void) = 0;

	/** \return The multi-select state of the curve control. */
	virtual bool MultiSelect(void) const = 0;

	/** Set the multi-select state of the curve. If \e true, multiple overlapping points on
		different curves will be selected with a single mouse click. */
	virtual void SetMultiSelect(bool bMultiSelect) = 0;

	/** Get the scaling for displaying tangents on all curves. */
	virtual double TangentScale(void) const = 0;

	/** Set the scaling for displaying tangents on all curves. */
	virtual void SetTangentScale(double d) = 0;

	/** \return The point size of a selectable point. This is measured in pixels
		and is about half the width or height of the point. Points are square. */
	virtual int PointSize(void) const = 0;

	/** Set the point size of a selectable point. */
	virtual void SetPointSize(int iSize) = 0;

	/** \return The resolution for point snap-to-grid in X. */
	virtual double PointSnapX(void) const = 0;

	/** Set the resolution for point snap-to-grid in X. Use zero to disable snap-to-grid in X. */
	virtual void SetPointSnapX(double dSnap) = 0;

	/** \return The resolution for point snap-to-grid in Y. */
	virtual double PointSnapY(void) const = 0;

	/** Set the resolution for point snap-to-grid in Y. Use zero to disable snap-to-grid in Y. */
	virtual void SetPointSnapY(double dSnap) = 0;

	/** \return The angle to snap tangent points to when the shift key is down. */
	virtual double TangentSnapToAngle(void) const = 0;

	/** Set the angle to snap tangent points to when the shift key is down. */
	virtual void SetTangentSnapToAngle(double dAngle) = 0;

	/** \return The pick margin of a selectable point.
		This is measured in pixels; it allows easier picking of small points. */
	virtual int PointPickMargin(void) const = 0;

	/** Set the pick margin of a selectable point. */
	virtual void SetPointPickMargin(int iMargin) = 0;

#pragma region RH_C_SHARED_ENUM [IRhRdkCustomCurveCtrl::ScaleModes] [ANDYENUM] [nested]
	/// <summary>Scale modes.</summary>
	enum class ScaleModes : unsigned int
	{
		/// <summary>Graph is automatically scaled to fit min/max bounds of all points.</summary>
		Bounds,
		/// <summary>Graph is automatically scaled to constraints.</summary>
		Constraints,
		/// <summary>Graph is automatically scaled to display min/max.</summary>
		Display,
	};
#pragma endregion

	/** Get the X scale mode. \see MoveSelectedPoints(). */
	virtual ScaleModes ScaleModeX(void) const = 0;

	/** Set the X scale mode. \see MoveSelectedPoints(). */
	virtual void SetScaleModeX(ScaleModes m) = 0;

	/** Get the Y scale mode. \see MoveSelectedPoints(). */
	virtual ScaleModes ScaleModeY(void) const = 0;

	/** Set the Y scale mode. \see MoveSelectedPoints(). */
	virtual void SetScaleModeY(ScaleModes m) = 0;

	/** Get the min X display value used when the X scale mode is 'display'. */
	virtual double DisplayMin(void) const = 0;

	/** Set the min X display value used when the X scale mode is 'display'. */
	virtual void SetDisplayMin(double d) = 0;

	/** Get the max X display value used when the X scale mode is 'display'. */
	virtual double DisplayMax(void) const = 0;

	/** Set the max X display value used when the X scale mode is 'display'. */
	virtual void SetDisplayMax(double d) = 0;

	/** Get the min/max X constraint values used when the X scale mode is 'constraints'.
		\note Points are always constrained to constraints, regardless of the scale mode.
		\see MoveSelectedPoints(). */
	virtual void ConstraintsX(double& dMinOut, double& dMaxOut) const = 0;

	/** Set the min/max X constraint values used when the X scale mode is 'constraints'.
		\note Points are always constrained to constraints, regardless of the scale mode.
		To disable a constraint, use the value cdNoConstraint.
		\see MoveSelectedPoints(). */
	virtual void SetConstraintsX(double dMin, double dMax) = 0;

	/** Get the min/max Y constraint values used when the Y scale mode is 'constraints'.
		\note Points are always constrained to constraints, regardless of the scale mode.
		\see MoveSelectedPoints(). */
	virtual void ConstraintsY(double& dMinOut, double& dMaxOut) const = 0;

	/** Set the min/max Y constraint values used when the Y scale mode is 'constraints'.
		\note Points are always constrained to constraints, regardless of the scale mode.
		To disable a constraint, use the value cdNoConstraint.
		\see MoveSelectedPoints(). */
	virtual void SetConstraintsY(double dMin, double dMax) = 0;

	/** Set the margins between the inner graph and the outer window frame. */
	virtual void SetMargins(int l, int t, int r, int b) = 0;

	/** Get the color of the graph's interior. */
	virtual ON_Color InnerColor(void) const = 0;

	/** OBSOLETE. */
	virtual void SetInnerColor(const ON_Color& col) = 0;

	/** Get the color of the graph's exterior border. */
	virtual ON_Color OuterColor(void) const = 0;

	/** OBSOLETE. */
	virtual void SetOuterColor(const ON_Color& col) = 0;

	/** Get the color of the graph's text. */
	virtual ON_Color TextColor(void) const = 0;

	/** OBSOLETE. */
	virtual void SetTextColor(const ON_Color& col) = 0;

#pragma region RH_C_SHARED_ENUM [IRhRdkCustomCurveCtrl::TextModes] [ANDYENUM] [nested]
	/// <summary>Text modes.</summary>
	enum class TextModes : unsigned int
	{
		/// <summary>Text is always displayed.</summary>
		Always,
		/// <summary>Text is only displayed when point is selected.</summary>
		WhenSelected,
		/// <summary>Text is only displayed when point is the only selected point.</summary>
		WhenOneSelected,
		/// <summary>Text is never displayed.</summary>
		Never,
	};
#pragma endregion

	/** \return The text mode. */
	virtual TextModes TextMode(void) const = 0;

	/** Set the text mode. */
	virtual void SetTextMode(TextModes m) = 0;

	/** Report if the cursor is on or off. */
	virtual bool CursorOn(void) const = 0;

	/** Set the cursor on or off. */
	virtual void SetCursorOn(bool bOn) = 0;

	/** Get the cursor position. */
	virtual double CursorPosition(void) const = 0;

	/** Set the cursor position. */
	virtual void SetCursorPosition(double d) = 0;

	/** Get the color of tangent lines and points for all curves. */
	virtual ON_Color TangentColor(void) const = 0;

	/** OBSOLETE. */
	virtual void SetTangentColor(const ON_Color& col) = 0;

	/** Get the color of an unselected curve point for all curves. */
	virtual ON_Color PointColor(void) const = 0;

	/** OBSOLETE. */
	virtual void SetPointColor(const ON_Color& col) = 0;

	/** Get the color of a selected curve point for all curves. */
	virtual ON_Color PointSelectedColor(void) const = 0;

	/** OBSOLETE. */
	virtual void SetPointSelectedColor(const ON_Color& col) = 0;

	/** Report if the ruler is on or off. */
	virtual bool RulerOn(void) const = 0;

	/** Set the ruler on or off. */
	virtual void SetRulerOn(bool bOn) = 0;

	/** OBSOLETE. */
	virtual void SetRulerColors(const ON_Color& colInt, const ON_Color& colFrac) = 0;

	/** Convert client area pixel coordinates to graph coordinates. */
	virtual ON_2dPoint PixelToGraph(int x, int y) const = 0;

	/** Convert graph coordinates to client area pixel coordinates. */
	virtual CPoint GraphToPixel(double x, double y) const = 0;

	/** \return Whether or not the area outside the contraints is shaded. */
	virtual bool ShadeConstraints(void) const = 0;

	/** Set whether or not the area outside the contraints is shaded. */
	virtual void SetShadeConstraints(bool bOn) = 0;

	/** \return Whether or not auto-scaling is enabled. */
	virtual bool UseAutoScaling(void) const = 0;

	/** Set whether or not auto-scaling is enabled. */
	virtual void SetUseAutoScaling(bool bOn) = 0;

	/** Set auto-scaling animation parameters. Only used if auto-scaling is enabled.
		\param iDelay is the animation delay in milliseconds. Use zero to disable animation.
		\param iSteps is the number of animation steps. */
	virtual void SetAutoScalingParams(int iDelay, int iSteps) = 0;

	/** Ask display to automatically scale.
		\param bAnimate if \e true, the scaling is animated.
		\param bForce if \e true, the method works even if auto-scaling is disabled.
		\param bScaleX is \e to include scaling in X.
		\param bScaleY is \e to include scaling in Y. */
	virtual void DoAutoScaling(bool bAnimate=true, bool bForce=false, bool bScaleX=true, bool bScaleY=true) = 0;

	/** Ask display to zoom such that all points on all curves fit within the window. */
	virtual void ZoomToExtents(void) = 0;

	/** Ask display to zoom to a rectangle, in pixels. */
	virtual void ZoomToPixelRect(const ON_4iRect& rect) = 0;

	/** Ask display to zoom to a rectangle, in graph coords. */
	virtual void ZoomToGraphRect(const ON_2dPoint& pt1, const ON_2dPoint& pt2) = 0;

	/** Get whether drag-zooming mode is on or off. */
	virtual bool DragZoomingOn(void) const = 0;

	/** Set drag-zooming mode on or off. */
	virtual void SetDragZoomingOn(bool bOn) = 0;

#pragma region RH_C_SHARED_ENUM [IRhRdkCustomCurveCtrl::NotificationContexts] [ANDYENUM] [nested]
	/// <summary>Notification contexts.</summary>
	enum class NotificationContexts : unsigned int
	{
		/// <summary>Notification is about point(s).</summary>
		Points,
		/// <summary>Notification is about cursor.</summary>
		Cursor,
		/// <summary>Notification is about context menu.</summary>
		Menu,
		/// <summary>Notification is about point(s) being double-clicked.</summary>
		PtDblClick,
	};
#pragma endregion

	/** Set a notification handler. RDK takes ownership of the object.
		When IRhRdkNotificationHandler::OnNotify(), OnBeginNotify() or OnEndNotify() are called,
		the type of the vOld and vNew variants will be as follows:
		- If 'context' is Points     : pointer; must be cast to ON_SimpleArray<IPoint*>*
		- If 'context' is Cursor     : double; old and new cursor positions.
		- If 'context' is Menu       : vOld is null, vNew is 2D vector (mouse position).
		- If 'context' is PtDblClick : vOld is null, vNew is pointer; must be cast to IPoint* */
	virtual void SetNotificationHandler(IRhRdkNotificationHandler* pNH) = 0;

#pragma region RH_C_SHARED_ENUM [IRhRdkCustomCurveCtrl::Styles] [ANDYENUM] [nested]
	/// <summary>Styles.</summary>
	enum class Styles : unsigned int
	{
		/// <summary>Single gray curve.</summary>
		Gray,
		/// <summary>Red, green and blue curve.</summary>
		RGB,
		/// <summary>Red, green, blue and alpha curves.</summary>
		RGBA,
	};
#pragma endregion

	/** Get the style that the control was created with. */
	virtual Styles Style(void) const = 0;

	/** STARTING WITH RHINO 8, THIS CONTROL IS DEPRECATED AND DOES NOT WORK. See [DEPCTL]
		\return null */
	static IRhRdkCustomCurveCtrl* New(Styles style, RhRdkUiFrameworks f);

protected:
	virtual ~IRhRdkCustomCurveCtrl() { }
};

#if defined ON_RUNTIME_WIN

/** \class CRhRdkCurveGraph

	Displays a modal curve-graph editor window containing a red, green and blue curve.
	Optionally also displays a gray alpha curve.

	\see CRhRdkCurveGraphEvaluator

*/
class RHRDK_SDK CRhRdkCurveGraph : public CRhRdkObject
{
public:
	virtual ~CRhRdkCurveGraph();

	/** \param style determines whether the graph has one, three or four curves.
		\param bCreateBlank determines if the graph should begin blank (faster) or if
		it should begin as a linear graph with 1, 3 or 4 curves (slower). Set this to \e true
		if you intend to call SetGraphXML() immediately after creating it.
		\see SetGraphXML() */
	CRhRdkCurveGraph(IRhRdkCustomCurveCtrl::Styles style, bool bCreateBlank=false);

	/** Show the modal curve editor dialog. The curve graph contains a red, green and blue curve,
		and optionally also an alpha curve.
		\return \e true if successful, \e false if the user canceled the dialog. */
	virtual bool ShowModalDialog(HWND pParent);

	/** Reset all curves on the graph to straight lines from (0,0) to (1,1). */
	virtual void Reset(void) const;

	/** Get an XML string representing the state of the graph. */
	virtual void GetGraphXML(ON_wString& sXMLOut) const;

	/** Set the state of the graph from an XML string.
		\note This can change every aspect of the graph including the number of curves.
		Therefore the use of eStyle as passed to the constructor could be overridden by this method. */
	virtual void SetGraphXML(const wchar_t* wszXML);

	/** \internal For RDK internal use only. */
	virtual void SetLiveUpdateOn(bool bOn);

private:
	/** \internal For RDK internal use only. */
	class CImpl;
	CImpl* m_pImpl;
};

#endif

/** IRhRdkCustomCurveGraphSectionCallback

	This interface is OBSOLETE and has been replaced by IRhRdkCustomCurveGraphSectionSharedCallback.

	DO NOT IMPLEMENT THIS INTERFACE. Implement IRhRdkCustomCurveGraphSectionSharedCallback instead.

	The return type of CRhRdkCoreContent::CustomCurveGraphSectionCallback() is wrong; it is this obsolete interface.
	Until we can fix this properly with [SDK_UNFREEZE] you must use reinterpret_cast to return a new instance of
	your implementation of IRhRdkCustomCurveGraphSectionSharedCallback as IRhRdkCustomCurveGraphSectionCallback.

	IRhRdkCustomCurveGraphSectionCallback* CMyMaterial::CustomCurveGraphSectionCallback(void)
	{
		return reinterpret_cast<IRhRdkCustomCurveGraphSectionCallback*>(new CMyCurveCallback);
	}

*/
class RHRDK_SDK IRhRdkCustomCurveGraphSectionCallback
{
public:
	virtual void DeleteThis(void) = 0;
	virtual IRhRdkCustomCurveCtrl::Styles Style(void) const = 0;
	virtual void GetGraphXML(ON_wString& sXML) const = 0;
	virtual void OnGraphChanged(const ON_wString& sXML) = 0;
	virtual void* EVF(const wchar_t* wszFunc, void* pvData) = 0;

protected:
	virtual ~IRhRdkCustomCurveGraphSectionCallback() { }
};

// [SDK_UNFREEZE] Delete IRhRdkCustomCurveGraphSectionCallback.

class RHRDK_SDK IRhRdkCustomCurveGraphSectionSharedCallback
{
public:
	/** Implement this method as \code delete this \endcode. */
	virtual void DeleteThis(void) = 0;

	/** Implement this method to return the style of the graph. */
	virtual IRhRdkCustomCurveCtrl::Styles Style(void) const = 0;

	/** Implement this method to return true if the custom curve section should be hidden. */
	virtual bool IsSectionHidden(const CRhRdkContentArray& aSelection) const = 0;

	/** Implement this method to load the graph XML into sXML. */
	virtual void GetGraphXML(const CRhRdkContentArray& aSelection, OUT ON_wString& sXML) const = 0;

	/** Implement this method to perform required actions when the user changes the graph.
		\param sXML contains the graph XML after the change. */
	virtual void OnGraphChanged(const CRhRdkContentArray& aSelection, const ON_wString& sXML) = 0;

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t* wszFunc, void* pvData) = 0;

protected:
	virtual ~IRhRdkCustomCurveGraphSectionSharedCallback() { }
};

#if defined ON_RUNTIME_WIN

/** \class CRhRdkRgbGradientSwatch

	Displays a gradient swatch suitable for showing the effect of an RGBA curve.

*/
class RHRDK_SDK CRhRdkRgbGradientSwatch
{
public:
	CRhRdkRgbGradientSwatch();
	virtual ~CRhRdkRgbGradientSwatch();

	/**	Create the control.
		\param uStyle specifies window styles for the control.
		\param rect specifies the size and position of the control in client coords of pParent.
		\param pParent is the parent window.
		\param uID is the ID of the control.
		\return \e true if created successfully, else \e false. */
	virtual bool Create(unsigned long uStyle, const ON_4iRect& rect, HWND pParent, unsigned int uID);

	/** Set the control up from graph XML.
		\param vXML should be a string containing XML from CRhRdkCurveGraph.
		This value can be set to 'varies' in which case the swatch displays "(varies)". */
	virtual void SetGraphXML(const CRhRdkVariant& vXML);

	/** Get the underlying window of the control. */
	operator HWND (void) const;

	/** Get the underlying window of the control. */
	virtual HWND Wnd(void) const;

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t* wszFunc, void* pvData);

public:
	/** \internal For RDK internal use only. */
	class CImpl;
	CImpl* m_pImpl;
};

#endif
