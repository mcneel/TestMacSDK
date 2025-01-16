//
// Copyright (c) 1993-2017 Robert McNeel & Associates. All rights reserved.
// Rhinoceros is a registered trademark of Robert McNeel & Associates.
//
// THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY.
// ALL IMPLIED WARRANTIES OF FITNESS FOR ANY PARTICULAR PURPOSE AND OF
// MERCHANTABILITY ARE HEREBY DISCLAIMED.
//				
// For complete Rhino SDK copyright information see <http://www.rhino3d.com/developer>.
//
////////////////////////////////////////////////////////////////

#pragma once

/** This is the abstract interface class for all Snapshot clients.
*/
class RHINO_SDK_CLASS IRhinoSnapshotsClient
{
public:
	virtual ~IRhinoSnapshotsClient(void) {};

public:
/** Call to register your client.
\param client client to register.
\return \e true if successful, otherwise \e false. */
static bool Register(IRhinoSnapshotsClient& client);

/** Call to unregister your client.
\param client client to unregister.
\return \e true if successful, otherwise \e false. */
static bool Unregister(IRhinoSnapshotsClient& client);

/** \return The view client ID. */
static UUID ViewClientId(void);

public:
	/** \return The plug-in id that registers this client. */
	virtual UUID PlugInId(void) const = 0;

	/** \return The unique id of this client. */
	virtual UUID ClientId(void) const = 0;

	/** \return Predefined categories. */
	static ON_wString ApplicationCategory(void);
	static ON_wString DocumentCategory(void);
	static ON_wString RenderingCategory(void);
	static ON_wString ViewsCategory(void);
	static ON_wString ObjectsCategory(void);
	static ON_wString LayersCategory(void);
	static ON_wString LightsCategory(void);
	static ON_wString MeshModifiersCategory(void);

	/** \return The category of this client. Usually one of the above.*/
	virtual ON_wString Category(void) const = 0;

	/** \return The client's name. */
	virtual ON_wString Name(void) const = 0;

	/** \return \e true if the client saves/restores document user data. */
	virtual bool SupportsDocument(void) const = 0;

	/** Called when the user saves a snapshot and SupportDocument() returns true.
	\param doc is the current document.
	\param buffer is the buffer to write the data to.
	\return \e true if successful, otherwise \e false. */
	virtual bool SaveDocument(CRhinoDoc& doc, ON_Buffer& buffer) = 0;

	/** Called when the user restores a snapshot and SupportDocument() returns true.
	\param doc is the current document.
	\param buffer is the buffer to read the data from.
	\return \e true if successful, otherwise \e false. */
	virtual bool RestoreDocument(CRhinoDoc& doc, const ON_Buffer& buffer) = 0;

	/** \return \e true if the client saves/restores object user data. */
	virtual bool SupportsObjects(void) const = 0;

	/** \return \e true if the client saves/restores object user data for the given object. */
	virtual bool SupportsObject(const CRhinoObject& obj) const = 0;

	/** Called when the user saves a snapshot and SupportsObjects() and SupportsObject(const CRhinoObject& obj) returns true.
	\param doc is the current document.
	\param obj is the current object.
	\param xformObject is a transformation matrix. The matrix is set to identity the first time an object is associated with a snapshot.
	After that the matrix is updated when the object is transformed (scale, rotate etc.).
	\param buffer is the buffer to write the data to.
	\return \e true if successful, otherwise \e false. */
	virtual bool SaveObject(CRhinoDoc& doc, const CRhinoObject& obj, const ON_Xform& xformObject, ON_Buffer& buffer) = 0;

	/** Called when the user restores a snapshot and SupportsObjects() and SupportsObject(const CRhinoObject& obj) returns true.
	\param doc is the current document.
	\param obj is the current object.
	\param xformObject is a transformation matrix. The matrix is set to identity the first time an object is associated with a snapshot.
	After that the matrix is updated when the object is transformed (scale, rotate etc.).
	\param buffer is the buffer to read the data from.
	\return \e true if successful, otherwise \e false. */
	virtual bool RestoreObject(CRhinoDoc& doc, const CRhinoObject& obj, const ON_Xform& xformObject, const ON_Buffer& buffer) = 0;

	/** Called after all clients restored their data. */
	virtual void SnapshotRestored(CRhinoDoc& doc) = 0;

	/** \return \e true if the client allows animation. */
	virtual bool SupportsAnimation(void) const = 0;

	/** Called once at the start of an animation.
	\param doc is the current document.
	\param iFrames is the number of frames to be animated. */
	virtual void AnimationStart(CRhinoDoc& doc, int iFrames) = 0;

	/** Called once at the start of an animation.
	\param doc is the current document.
	\param start is a buffer to the data of the starting position.
	\param stop is a buffer to the data of the ending position.
	\return \e true if successful, otherwise \e false. */
	virtual bool PrepareForDocumentAnimation(CRhinoDoc& doc, const ON_Buffer& start, const ON_Buffer& stop) = 0;

	/** Called once at the start of an animation. This can be used to extend the scene bounding box to avoid clipping.
	\param doc is the current document.
	\param start is a buffer to the data of the starting position.
	\param stop is a buffer to the data of the ending position.
	\param bb is the current scene bounding box. */
	virtual void ExtendBoundingBoxForDocumentAnimation(CRhinoDoc& doc, const ON_Buffer& start, const ON_Buffer& stop, ON_BoundingBox& bb) = 0;

	/** Called for each frame. Starting at 0.0.
	\param doc is the current document.
	\param dPos is is the current frame. Starting at 0.0.
	\param start is a buffer to the data of the starting position.
	\param stop is a buffer to the data of the ending position.
	\return \e true if successful, otherwise \e false. */
	virtual bool AnimateDocument(CRhinoDoc& doc, double dPos, const ON_Buffer& start, const ON_Buffer& stop) = 0;

	/** Called once at the start of an animation.
	\param doc is the current document.
	\param obj is the current object.
	\param xformObject is a transformation matrix. The matrix is set to identity the first time an object is associated with a snapshot.
	After that the matrix is updated when the object is transformed (scale, rotate etc.).
	\param start is a buffer to the data of the starting position.
	\param stop is a buffer to the data of the ending position.
	\return \e true if successful, otherwise \e false. */
	virtual bool PrepareForObjectAnimation(CRhinoDoc& doc, const CRhinoObject& obj, const ON_Xform& xformObject, const ON_Buffer& start, const ON_Buffer& stop) = 0;

	/** Called once at the start of an animation. This can be used to extend the scene bounding box to avoid clipping.
	\param doc is the current document.
	\param obj is the current object.
	\param xformObject is a transformation matrix. The matrix is set to identity the first time an object is associated with a snapshot.
	After that the matrix is updated when the object is transformed (scale, rotate etc.).
	\param start is a buffer to the data of the starting position.
	\param stop is a buffer to the data of the ending position.
	\param bb is the current scene bounding box. */
	virtual void ExtendBoundingBoxForObjectAnimation(CRhinoDoc& doc, const CRhinoObject& obj, const ON_Xform& xformObject, const ON_Buffer& start, const ON_Buffer& stop, ON_BoundingBox& bb) = 0;

	/** Called for each frame. Starting at 0.0.
	\param doc is the current document.
	\param obj is the current object.
	\param xformObject is a transformation matrix. The matrix is set to identity the first time an object is associated with a snapshot.
	After that the matrix is updated when the object is transformed (scale, rotate etc.).
	\param dPos is is the current frame. Starting at 0.0.
	\param start is a buffer to the data of the starting position.
	\param stop is a buffer to the data of the ending position.
	\return \e true if successful, otherwise \e false. */
	virtual bool AnimateObject(CRhinoDoc& doc, const CRhinoObject& obj, const ON_Xform& xformObject, double dPos, const ON_Buffer& start, const ON_Buffer& stop) = 0;

	/** Called once at the end of an animation. */
	virtual void AnimationStop(CRhinoDoc& doc) = 0;

	/** Called for every object that is associated with a snapshot and gets transformed in Rhino. This is getting called for each stored snapshot and gives the client the possibility to update the stored data.
	\param doc is the current document.
	\param obj is the current object.
	\param xformObject is a transformation matrix. The matrix is set to identity the first time an object is associated with a snapshot.
	After that the matrix is updated when the object is transformed (scale, rotate etc.).
	\param buffer is a buffer which can be used to update the stored data.
	\return \e true if successful, otherwise \e false. */
	virtual bool ObjectTransformNotification(CRhinoDoc& doc, const CRhinoObject& obj, const ON_Xform& xformObject, ON_Buffer& buffer) = 0;

	/** Called before restoring a snapshot. Warns the user if the current model state is not already saved.
	\param doc is the current document.
	\param bufferModel is the current state of the model.
	\param aSnapShots a list of client data.
	\param pReportMissing is used to list the missing items that cannot be found in the current model.
	\return \e true if successful, otherwise \e false. */
	virtual bool IsCurrentModelStateInAnySnapshot(CRhinoDoc& doc, const ON_Buffer& bufferModel, const ON_SimpleArray<const ON_Buffer*>& aSnapShots,
		ON_TextLog* pReportMissing = nullptr) const = 0;

	/** Called before restoring a snapshot. Warns the user if the current model state is not already saved.
	\param doc is the current document.
	\param obj is the current object.
	\param bufferModel is the current state of the model.
	\param aSnapShots a list of client data.
	\param pReportMissing is used to list the missing items that cannot be found in the current model.
	\return \e true if successful, otherwise \e false. */
	virtual bool IsCurrentModelStateInAnySnapshot(CRhinoDoc& doc, const CRhinoObject& obj, const ON_Buffer& bufferModel, const ON_SimpleArray<const ON_Buffer*>& aSnapShots,
		ON_TextLog* pReportMissing = nullptr) const = 0;

	/** Emergency virtual function for future expansion. */
	virtual void* EVF(const wchar_t*, void*) { return nullptr; };
};


class RHINO_SDK_CLASS IRhinoSnapshotsClientEx : public IRhinoSnapshotsClient
{
public:
  virtual ~IRhinoSnapshotsClientEx(void) {};

public:
  /** Called before saving too a file. For example when Rhino v7 saves as a v6 file.
  \param doc is the current document.
  \param iSaveAsVersion is the Rhino version to convert too. For Example 6,7,...
  \return \e true if the client wants to save its data into a different format, otherwise \e false. */
  virtual bool SupportsDataConversion(CRhinoDoc& doc, int iSaveAsVersion) const = 0;

  /** Called before saving too a file. For example when Rhino v7 saves as a v6 file.
  \param doc is the current document.
  \param bufferIn is the data to convert.
  \param bufferOut is the converted data.
  \param iSaveAsVersion is the Rhino version to convert too. For Example 6,7,...
  \return \e true if successful, otherwise \e false. */
  virtual bool ConvertData(CRhinoDoc& doc, const ON_Buffer& bufferIn, ON_Buffer& bufferOut, int iSaveAsVersion) = 0;

  /** Called before saving too a file. For example when Rhino v7 saves as a v6 file.
  \param doc is the current document.
  \param obj is the object the data is associated with.
  \param bufferIn is the data to convert.
  \param bufferOut is the converted data.
  \param iSaveAsVersion is the Rhino version to convert too. For Example 6,7,...
  \return \e true if successful, otherwise \e false. */
  virtual bool ConvertData(CRhinoDoc& doc, const CRhinoObject& obj, const ON_Buffer& bufferIn, ON_Buffer& bufferOut, int iSaveAsVersion) = 0;
};
