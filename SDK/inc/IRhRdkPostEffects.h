
#pragma once // Public SDK header

#include "RhRdkNewPostEffectPlugIn.h"

/** \class IRhRdkPostEffects

	This is the interface to a collection of post effects.

*/
class RHRDK_SDK IRhRdkPostEffects
{
public:
	/** Call this method to get a non-const object upon which you can call non-const methods.
		Calls to this method are counted; you must call EndChange() once for every call to
		BeginChange(). Please do not use const_cast or any other means of circumventing this requirement. */
	virtual IRhRdkPostEffects& BeginChange(RhRdkChangeContext cc) const = 0;
	virtual bool EndChange(void) = 0;

	/** Find a post effect plug-in from its id. */
	virtual IRhRdkPostEffect* PostEffectFromId(const UUID& uuidPEP) const = 0;

	/** Get all the post effects.
		\param a accepts the post effects.
		\note This method is const but should not be. You can get non-const post effects without calling
		 Begin/EndChange(). If you do this, the system will not work properly, so please make sure you
		 call Begin/EndChange() if you want to modify the post effects. */
	virtual void GetPostEffects(IRhRdkPostEffect::Types type, OUT ON_SimpleArray<IRhRdkPostEffect*>& a) const = 0;

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t*, void*) = 0;

protected:
	virtual ~IRhRdkPostEffects() { }
};
