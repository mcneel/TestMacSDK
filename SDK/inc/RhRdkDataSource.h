
#pragma once // Public SDK header

#include "RhRdkUuids.h"
#include "RhRdkEventWatcher.h"

/** \class IRhinoUiEventInfo

	Used by certain data source events. Depending on the event UUID
	this interface is cast to a more specific interface to allow
	information about the event to be retrieved.

*/
class RHRDK_SDK IRhinoUiEventInfo
{
public:
	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t*, void*) = 0;

protected:
	virtual ~IRhinoUiEventInfo() { }
};

/** \class IRhinoUiDataSourceEventWatcher

	Any object that has access to a data source can register itself
	to receive events from that data source by implementing this
	interface and calling IRhinoUiDataSource::RegisterEventWatcher()

*/
class RHRDK_SDK IRhinoUiDataSourceEventWatcher
{
public:
	/** Implement this method to unregister and delete the event watcher. */
	virtual void Delete(void) = 0;

	/** Currently implemented by EVF. It should supplement Delete() which should only delete it.
		Sometimes an event watcher needs to be unregistered without being deleted. */
		//[SDK_UNFREEZE] virtual void OnDeleteDataSource(IRhinoUiDataSource* pDS) = 0;

	/** Implement this method to handle an event when the data of a certain type changes.
		Your handler will typically transfer data of that type to elements of your user interface.
		\param uuidData is the identifier of the data.
		\param pInfo is a pointer to an object that contains more information about the event.
		\see IRhinoUiDataSource::RegisterEventWatcher() */
	virtual void OnEvent(const UUID& uuidData, const IRhinoUiEventInfo* pInfo) = 0;

	/** Emergency virtual function for future expansion.
		Vanilla (non-host) data source event watchers \e must unregister by handling EVF(\"OnDeleteDataSource\")
		wszFunc = "OnDeleteDataSource", pvData = IRhinoUiDataSource* */
	virtual void* EVF(const wchar_t* wszFunc, void* pvData) = 0;

protected:
	virtual ~IRhinoUiDataSourceEventWatcher() { }
};

/** \class IRhinoUiDataSourceHost

	A data source host is an object, usually a controller, that hosts one or more data sources.
	These data sources allow the host to get or set data that is stored somewhere, often in a document.
	All data source hosts are automatically also data source event watchers which means they can
	be notified when the data changes. A data source host can host any number of data sources,
	each one capable of providing different data types. Note that the host does not own the
	data sources, it simply stores references to them.

*/
class RHRDK_SDK IRhinoUiDataSourceHost : public IRhinoUiDataSourceEventWatcher
{
public:
	/** Add a data source to the host. The implementation of this method
		typically also registers the host as a data source event watcher. */
	virtual void AddDataSource(class IRhinoUiDataSource& ds) = 0;

	/** Removes a data source from the host. This is called when a data source
		is being deleted. The implementation of this method typically also
		unregisters the host as a data source event watcher. */
	virtual void RemoveDataSource(IRhinoUiDataSource& ds) = 0;

	/** Get the data sources that are being hosted. */
	virtual void GetDataSources(ON_SimpleArray<IRhinoUiDataSource*>& aOut) const = 0;

	/** Copy the data sources from another host. The implementation of this method
		typically also registers this host as a data source event watcher. */
	virtual void CopyDataSourcesFrom(IRhinoUiDataSourceHost& host) = 0;

	/** Get data, specified by a unique id, from one of the host's data source(s).
		Each data source will be asked to provide the data until either one provides
		it or there are no more data sources to ask.
		\param uuidData is the identifier of the data.
		\param bForWrite is \e true if the data source is going to be written to.
		 Every call to GetData() with bForWrite=true \e must be matched by a call to either Commit() or Discard().
		\param bAutoChangeBracket is \e true if a content change bracket should be applied.
		\return The data from a data source or null if no such data could be provided. */
	virtual void* GetData(const UUID& uuidData, bool bForWrite, bool bAutoChangeBracket=true) const = 0;

	/** Commit changes made using GetData() with bForWrite=true.
		\param uuidData is the identifier of the data.
		\param pInfo is a pointer to optional info to be sent with any event that is raised.
		Events will be sent for any data objects that have had data committed.
		Every call to GetData() with bForWrite=true \e must be matched by a call to either Commit() or Discard().
		This method should not be called if the operation is cancelled. Instead you should call Discard().
		\see GetData() \see Discard() */
	virtual void Commit(const UUID& uuidData, IRhinoUiEventInfo* pInfo=nullptr) = 0;

	/** Discard changes made using GetData() with bForWrite=true.
		\param uuidData is the identifier of the data.
		Every call to GetData() with bForWrite=true \e must be matched by a call to either Commit() or Discard().
		This method should only be called if the operation is cancelled. Otherwise you should call Commit().
		\see GetData() \see Commit() */
	virtual void Discard(const UUID& uuidData) = 0;
};

class RHRDK_SDK IRhinoUiController : public IRhinoUiDataSourceHost
{
public:
	/** Call this method when UI construction is complete and the values should be passed
		for the first time from the model to the view. */
	virtual void Activate(void) = 0;

	/** Register an event watcher to be notified when the model changes. */
	virtual void RegisterEventWatcher(class IRhinoUiControllerEventWatcher& ew) = 0;

	/** Unregister an event watcher which has been added by RegisterEventWatcher().
		This must be done when the event watcher is deleted. */
	virtual void UnregisterEventWatcher(class IRhinoUiControllerEventWatcher& ew) = 0;

#pragma region RH_C_SHARED_ENUM [IRhinoUiController::EventPriority] [Rhino.UI.Controls.RdkViewModel.EventPriority] [nested]
	/// <summary>Event priority</summary>
	enum class EventPriority : unsigned int
	{
		/// <summary>Low priority</summary>
		Low,
		/// <summary>Normal priority</summary>
		Normal,
		/// <summary>High priority</summary>
		High,
		/// <summary>Real-time priority</summary>
		RealTime,
	};
#pragma endregion

	/** Set the event priority. You should only change the event priority around an
		event send and set it back to the old value afterwards.
		\return the old event priority. */
	virtual EventPriority SetEventPriority(EventPriority ep) = 0;

	/** \return The unique id of this controller. */
	virtual UUID Id(void) const = 0;

	/** Retrieve an array of data source ids required by this controller. */
	virtual void RequiredDataSources(OUT ON_SimpleArray<ON_UUID>& aId) const = 0;

	virtual bool IsActivated(void) const = 0;

	static void CustomDeleter(IRhinoUiController*);

	static std::shared_ptr<IRhinoUiController> make_shared(IRhinoUiController*);

protected:
	virtual ~IRhinoUiController() { }
};

/** \class IRhinoUiControllerEventWatcher

	Any object that has access to a IRhinoUiController can register itself
	to receive events from that controller by implementing this
	interface and calling IRhinoUiController::RegisterEventWatcher()

*/
class RHRDK_SDK IRhinoUiControllerEventWatcher
{
public: // Events.

	/** Implement this method to handle an event when the data of a certain type changes.
		Your handler will typically transfer data of that type to elements of your user interface.
		\param con is the controller sending the event.
		\param uuidData is the identifier of the data.
		\param ep is the event priority.
		\param pInfo is an optional pointer to extra event-specific information. This is often null.
		\see IRhinoUiDataSource::RegisterEventWatcher(). */
	virtual void OnEvent(IRhinoUiController& con, const UUID& uuidData, IRhinoUiController::EventPriority ep, const IRhinoUiEventInfo* pInfo) = 0;

	/** Implement this method to handle an event when the controller is first activated. */
	virtual void OnControllerActivatedEvent(IRhinoUiController& con) = 0;

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t* wszFunc, void* pvData) = 0;

protected:
	virtual ~IRhinoUiControllerEventWatcher() { }
};

/** \class IRhinoUiDataSource

	Data source for user interfaces. A UI can have any number of data sources.

*/
class RHRDK_SDK IRhinoUiDataSource
{
public:
	/** You must implement this method as \code delete this; \endcode */
	virtual void DeleteThis(void) = 0;

	/** For use with smart pointers. */
	static void Deleter(IRhinoUiDataSource*);

	/** Get data specified by a unique id.
		\param uuidData is the identifier of the data. UUIDs defined by Rhino are specified above.
		\param bForWrite specifies if you intend to write to the data. \see Commit() \see Discard.
		\param bAutoChangeBracket is \e true if you want BeginChange() and EndChange() to be called
		 automatically when the data is contents.
		\return a pointer which can be cast to the type for the identifier as documented above.
		 Note that this method will return null if the requested data type cannot be provided. */
	virtual void* GetData(const UUID& uuidData, bool bForWrite, bool bAutoChangeBracket=true) const = 0;

	/** Set the data source as changed. This must be called in between GetData() with bForWrite == true
		and Commit() in order for the commit to succeed and an event to be sent. If it is called when
		the data source is not in write mode, the call will fail.
		\return \e true if successful, else \e false. */
	virtual bool SetChanged(void) = 0;

	/** Commit changes made using GetData() with bForWrite=true.
		\param uuidData is the identifier of the data.
		\param pInfo is a pointer to optional info to be sent with any event that is raised.
		Events will be sent for any data objects that have had data committed.
		Every call to GetData() with bForWrite=true \e must be matched by a call to either Commit() or Discard().
		This method should not be called if the operation is cancelled. Instead you should call Discard().
		\see GetData() \see Discard() */
	virtual void Commit(const UUID& uuidData, IRhinoUiEventInfo* pInfo=nullptr) = 0;

	/** Discard changes made using GetData() with bForWrite=true.
		\param uuidData is the identifier of the data.
		Every call to GetData() with bForWrite=true \e must be matched by a call to either Commit() or Discard().
		This method should only be called if the operation is cancelled. Otherwise you should call Commit().
		\see GetData() \see Commit() */
	virtual void Discard(const UUID& uuidData) = 0;

	/** Add a data source host which would like to be notified of events
		that affect its data. When such an event arrives, the data source
		checks if the host is interested in the event and if so, it calls
		the host's OnEvent() method. */
	virtual void RegisterEventWatcher(IRhinoUiDataSourceEventWatcher& ew) = 0;

	/** Remove a data source host which has been added by RegisterEventWatcher().
		This must be done when such a host is deleted. */
	virtual void UnregisterEventWatcher(IRhinoUiDataSourceEventWatcher& ew) = 0;

	RDK_DEPRECATED virtual bool ResetToDefaults(void) = 0; // Not used.

	/** Called when the data source is added to a data source host. */
	virtual void AddHost(IRhinoUiDataSourceHost& h) = 0;

	/** Called by a data source host when it is being deleted. */
	virtual void RemoveHost(IRhinoUiDataSourceHost& h) = 0;

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t* wszFunc, void* pvData) = 0;

protected:
	virtual ~IRhinoUiDataSource() { }
};

/** \class IRhRdkUndoRecord

	This interface is returned by IRhRdkUndo::NewUndoRecord().

	\see IRhRdkUndo
	\see uuidData_RdkUndo

*/
class RHRDK_SDK IRhRdkUndoRecord
{
public:
	/** This method is implemented as \code delete this; \endcode */
	virtual void DeleteThis(void) = 0;

	/** Set the localized description of the undo record. */
	virtual void SetDescription(const wchar_t* wsz) = 0;

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t* wszFunc, void* pvData) = 0;

protected:
	virtual ~IRhRdkUndoRecord() { }
};
