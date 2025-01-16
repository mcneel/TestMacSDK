
#pragma once // Public SDK header

/** \class IRhRdkRenderChannels

	This interface is deprecated and only kept for backward compatibility.

*/
/*RDK_DEPRECATED*/ class RHRDK_SDK IRhRdkRenderChannels
{
public:
	/** Call this method to get a non-const object upon which you can call non-const methods.
		Calls to this method are counted; you must call EndChange() once for every call to
		BeginChange(). Please do not use const_cast or any other means of circumventing this requirement. */
	virtual IRhRdkRenderChannels& BeginChange(RhRdkChangeContext cc) const = 0;
	virtual bool EndChange(void) = 0;

#pragma region RH_C_SHARED_ENUM [IRhRdkRenderChannels::Modes] [Rhino.Render.RenderChannels.Modes] [nested]
	/// <summary>
	/// Mode.
	/// </summary>
	enum class Modes : unsigned int
	{
		/// <summary>Render-channels are managed automatically</summary>
		Automatic,
		/// <summary>Render-channels are specified by the user</summary>
		Custom,
	};
#pragma endregion

	/** Get the mode. */
	virtual Modes Mode(void) const = 0;

	/** Set the mode. */
	virtual void SetMode(Modes m) = 0;

	/** Get the list of channels to render when in 'custom' mode.
		\param aChan accepts the channel ids. */
	virtual void GetCustomList(OUT ON_SimpleArray<UUID>& aChan) const = 0;

	/** Set the list of channels to render when in 'custom' mode.
		\param aChan contains the channel ids. */
	virtual void SetCustomList(const ON_SimpleArray<UUID>& aChan) = 0;

	/** Copy this object from another one. */
	virtual void CopyFrom(const IRhRdkRenderChannels& rch) = 0;

	/** Call this before calling multiple 'Set' methods. */
	virtual void BeginNotifyBracket(void) = 0;

	/** Call this after calling multiple 'Set' methods. */
	virtual void EndNotifyBracket(void) = 0;

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t*, void*) = 0;

	static IRhRdkRenderChannels* New(void);

	virtual ~IRhRdkRenderChannels() { }
};
