
#pragma once // Public SDK header

/** \class IRhRdkSafeFrame

	This interface is deprecated and only kept for backward compatibility.

*/
/*RDK_DEPRECATED*/ class RHRDK_SDK IRhRdkSafeFrame
{
public:
	/** Call this method to get a non-const object upon which you can call non-const methods.
		Calls to this method are counted; you must call EndChange() once for every call to
		BeginChange(). Please do not use const_cast or any other means of circumventing this requirement. */
	virtual IRhRdkSafeFrame& BeginChange(RhRdkChangeContext cc) const = 0;
	virtual bool EndChange(void) = 0;

	/** Returns true if the safe frame is on. */
	virtual bool On(void) const = 0;

	/** Returns true if the safe frame is only displayed in perspective views. */
	virtual bool PerspectiveOnly(void) const = 0;

	/** Returns true if the 4x3 field grid is on. */
	virtual bool FieldsOn(void) const = 0;

	/** Returns true if the live frame is on. */
	virtual bool LiveFrameOn(void) const = 0;

	/** Returns true if the action frame is on. */
	virtual bool ActionFrameOn(void) const = 0;

	/** Returns true if the action frame X and Y scales are linked. */
	virtual bool ActionFrameLinked(void) const = 0;

	/** Returns the action frame X scale. */
	virtual double ActionFrameXScale(void) const = 0;

	/** Returns the action frame Y scale. */
	virtual double ActionFrameYScale(void) const = 0;

	/** Returns true if the title frame is on. */
	virtual bool TitleFrameOn(void) const = 0;

	/** Returns true if the title frame X and Y scales are linked. */
	virtual bool TitleFrameLinked(void) const = 0;

	/** Returns the title frame X scale. */
	virtual double TitleFrameXScale(void) const = 0;

	/** Returns the title frame Y scale. */
	virtual double TitleFrameYScale(void) const = 0;

	/** Sets the safe frame on or off. */
	virtual void SetOn(bool b) = 0;

	/** Sets whether or not the safe frame is only displayed in perspective views. */
	virtual void SetPerspectiveOnly(bool b) = 0;

	/** Sets whether or not the 4x3 field grid is on. */
	virtual void SetFieldsOn(bool b) = 0;

	/** Sets whether or not the live frame is on. */
	virtual void SetLiveFrameOn(bool b) = 0;

	/** Sets whether or not the action frame is on. */
	virtual void SetActionFrameOn(bool b) = 0;

	/** Sets whether or not the action frame X and Y scales are linked. */
	virtual void SetActionFrameLinked(bool b) = 0;

	/** Sets the action frame X scale. */
	virtual void SetActionFrameXScale(double d) = 0;

	/** Sets the action frame Y scale. */
	virtual void SetActionFrameYScale(double d) = 0;

	/** Sets whether or not the title frame is on. */
	virtual void SetTitleFrameOn(bool b) = 0;

	/** Sets whether or not the title frame X and Y scales are linked. */
	virtual void SetTitleFrameLinked(bool b) = 0;

	/** Sets the title frame X scale. */
	virtual void SetTitleFrameXScale(double d) = 0;

	/** Sets the title frame Y scale. */
	virtual void SetTitleFrameYScale(double d) = 0;

	/** Begins a notify bracket. Only implemented in the RDK. */
	virtual void BeginNotifyBracket(void) = 0;

	/** Ends a notify bracket. Only implemented in the RDK. */
	virtual void EndNotifyBracket(void) = 0;

	/** Returns true if this safe frame is equal to 'sf'. */
	virtual bool IsEqual(const IRhRdkSafeFrame& sf) const = 0;

	/** Sets the contents of this safe frame to be the same as 'sf'. */
	virtual void CopyFrom(const IRhRdkSafeFrame& sf) = 0;

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t*, void*) = 0;

	// These are only here to allow duck-typing to work with ON_SafeFrame.
	bool Enabled(void) const { return On(); }
	void SetEnabled(bool b) { SetOn(b); }
	bool FieldGridOn(void) const { return FieldsOn(); }
	void SetFieldGridOn(bool b) { SetFieldsOn(b); }

protected:
	virtual ~IRhRdkSafeFrame() { }
};
