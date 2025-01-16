#pragma once

///////////////////////////////////////////////////////////////////////////////
//
// class IRhinoAddPropertiesPages
//
// Description:
//  Used to add custom object properties pages to the properties panel.
//
///////////////////////////////////////////////////////////////////////////////
class RHINO_SDK_CLASS IRhinoAddPropertiesPages
{
protected:
  //---------------------------------------------------------------------------
  // Description:
  //  Default constructor.
  IRhinoAddPropertiesPages();
public:
  //---------------------------------------------------------------------------
  // Description:
  //  Destructor.
  virtual ~IRhinoAddPropertiesPages();
  //---------------------------------------------------------------------------
  // Description:
  //  Must override and return the plug-in Id associated with the custom pages.
  // Returns:
  //  Return the plug-in Id associated with the custom pages.
  virtual ON_UUID PropertiesPlugInId() const = 0;
  //---------------------------------------------------------------------------
  // Description:
  //  The properties panel is a per document panel and a new instance gets
  //  generated for each new document.  This gets called each time the document
  //  instance of the panel is created.
  // Parameters:
  //   collection [out] - Add IRhinoOptionsPage objects to this collection,
  //     these IRhinoOptionsPage objects will get released when the document
  //     closes.
  // Remarks:
  //  Make sure you include the following macro prior to calling new:
  //    AFX_MANAGE_STATE(AfxGetStaticModuleState());
  //	Please see MFC Technical Notes 33 and 58 for additional details.
  //
  //  This method is called each time a new document is created.  The
  //  IRhinoPropertiesPanelPage objects are notified when CRhinoObjects 
  //  are added to a properties panel.  Pages added to the collection will
  //  get deleted when the document is closed.
  virtual void GetPropertiesPages(CRhinoPropertiesPanelPageCollection& collection) = 0;

private:
  IRhinoAddPropertiesPages(const IRhinoAddPropertiesPages&) = delete;
  IRhinoAddPropertiesPages& operator=(const IRhinoAddPropertiesPages&) = delete;
  // For internal use only
  class CRhPrivateAddPropertesPages* m_private = nullptr;
};
