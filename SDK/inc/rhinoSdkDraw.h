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
#if 1

// Location for new V6 public SDK display

// When things are ready, include this file, rhinoSdkDraw.h, in rhinoSdk.h.
// Until then, include this file directly in the few cpp files that use it.

// Have Dale Lear put #include <memory> in a core SDK header if we 
// end up using shared_ptr in this code or someplace else.
#include <memory>

class RHINO_SDK_CLASS CRhinoDrawCommonAttributes
{
public:
  // If the data members are all "simple", like ON_Color,
  // then let C++ handle everything for maximum efficiency.
  // If a "managed" data member is added, like a pointer to
  // something on the heap, then see CRhinoDrawFancyAttributes
  // for an example of the best way to manage that member.
  CRhinoDrawCommonAttributes() RHINO_NOEXCEPT;
  ////~CRhinoDrawCommonAttributes() RHINO_NOEXCEPT;
  ////CRhinoDrawCommonAttributes(const CRhinoDrawCommonAttributes& ) RHINO_NOEXCEPT;
  ////CRhinoDrawCommonAttributes(CRhinoDrawCommonAttributes&& ) RHINO_NOEXCEPT;
  ////CRhinoDrawCommonAttributes& operator=(const CRhinoDrawCommonAttributes&) RHINO_NOEXCEPT;
  ////CRhinoDrawCommonAttributes& operator=(CRhinoDrawCommonAttributes&&) RHINO_NOEXCEPT;

private:
  // no friends
  ////void ClearHelper() RHINO_NOEXCEPT;
  ////void DestroyHelper() RHINO_NOEXCEPT;
  ////void CopyHelper(const CRhinoDrawCommonAttributes&) RHINO_NOEXCEPT;
  ////void CloneHelper(CRhinoDrawCommonAttributes&) RHINO_NOEXCEPT;

public:
  // Simple data member
  ON_Color m_color;
};


class RHINO_SDK_CLASS CRhinoDrawFancyAttributes
{
public:
  CRhinoDrawFancyAttributes() RHINO_NOEXCEPT;

  // The following are here to demonstrate
  // how to handle parameters that are pointers
  // to classes that need to be managed by this
  // class.
  ~CRhinoDrawFancyAttributes() RHINO_NOEXCEPT;
  CRhinoDrawFancyAttributes(const CRhinoDrawFancyAttributes& ) RHINO_NOEXCEPT;
  CRhinoDrawFancyAttributes(CRhinoDrawFancyAttributes&& ) RHINO_NOEXCEPT;
  CRhinoDrawFancyAttributes& operator=(const CRhinoDrawFancyAttributes&) RHINO_NOEXCEPT;
  CRhinoDrawFancyAttributes& operator=(CRhinoDrawFancyAttributes&&) RHINO_NOEXCEPT;

private:
  // no friends
  void ClearHelper() RHINO_NOEXCEPT;
  void DestroyHelper() RHINO_NOEXCEPT;
  void CopyHelper(const CRhinoDrawFancyAttributes&) RHINO_NOEXCEPT;
  void CloneHelper(CRhinoDrawFancyAttributes&) RHINO_NOEXCEPT;

public:
  // The mesh that is pointed to will be deleted
  // by ~CRhinoDrawFancyAttributes() and any future calls
  // to SetFancyMeshManaged().
  const ON_Mesh* FancyMesh() const  RHINO_NOEXCEPT;

  // fancy_mesh will be managed by this class.
  // It will be deleted by ~CRhinoDrawFancyAttributes() or
  // by any future calls to SetFancyMeshManaged().
  void SetFancyMesh(ON_Mesh* fancy_mesh)  RHINO_NOEXCEPT;

private:
  // m_fancy_mesh is managed by this class.
  ON_Mesh* m_fancy_mesh;
};


class RHINO_SDK_CLASS CRhinoDrawAttributes
{
public:
  CRhinoDrawAttributes() RHINO_NOEXCEPT;
  ~CRhinoDrawAttributes() RHINO_NOEXCEPT;
  CRhinoDrawAttributes(const CRhinoDrawAttributes& ) RHINO_NOEXCEPT;
  CRhinoDrawAttributes(CRhinoDrawAttributes&& ) RHINO_NOEXCEPT;
  CRhinoDrawAttributes& operator=(const CRhinoDrawAttributes&) RHINO_NOEXCEPT;
  CRhinoDrawAttributes& operator=(CRhinoDrawAttributes&&) RHINO_NOEXCEPT;

private:
  // no friends
  void ClearHelper() RHINO_NOEXCEPT;
  void DestroyHelper() RHINO_NOEXCEPT;
  void CopyHelper(const CRhinoDrawAttributes&) RHINO_NOEXCEPT;
  void CloneHelper(CRhinoDrawAttributes&) RHINO_NOEXCEPT;

public:
  // Drawing attributes are broken into chunks so we do not use
  // lots of time and memory to manage information that is 
  // rarely used.
  
  // Common attributes that are almost always needed.
  // Fancy attributes are rarely needed.
  // There will probably be other "chunks" of attributes.

  // One per object might be implemented this way?
  class CRhinoDrawCommonAttributes* m_common_attributes;
  class CRhinoDrawFancyAttributes* m_fancy_attributes;

  // Shared among multiple objects could use reference counting?
  std::shared_ptr<class CRhinoDrawCommonAttributes> Common() RHINO_NOEXCEPT;
  std::shared_ptr<class CRhinoDrawFancyAttributes> Fancy() RHINO_NOEXCEPT;

private:
  std::shared_ptr<class CRhinoDrawCommonAttributes> m_sp_common_attributes;
  std::shared_ptr<class CRhinoDrawFancyAttributes> m_sp_fancy_attributes;
};


class RHINO_SDK_CLASS CRhinoObjectDrawContext
{
public:
  CRhinoObjectDrawContext() RHINO_NOEXCEPT;

  // The current implementation of CRhinoObjectDrawContext
  // works perfectly with the C++ defaults for the following.
  // That may change in the future.
  ////~CRhinoObjectDrawContext() RHINO_NOEXCEPT;
  ////CRhinoObjectDrawContext(const CRhinoObjectDrawContext& ) RHINO_NOEXCEPT;
  ////CRhinoObjectDrawContext(const CRhinoObjectDrawContext&& ) RHINO_NOEXCEPT;
  ////CRhinoObjectDrawContext& operator=(const CRhinoObjectDrawContext&) RHINO_NOEXCEPT;
  ////CRhinoObjectDrawContext& operator=(const CRhinoObjectDrawContext&&) RHINO_NOEXCEPT;

private:
  // no friends
  ////void ClearHelper() RHINO_NOEXCEPT;
  ////void DestroyHelper() RHINO_NOEXCEPT;
  ////void CopyHelper(const CRhinoObjectDrawContext&) RHINO_NOEXCEPT;
  ////void CloneHelper(CRhinoObjectDrawContext&) RHINO_NOEXCEPT;

public:
  // prototype example.
  // Change early and often!

  // perhaps a dp pointer is needed to get up and running ?
  class CRhinoDisplayPipeline* m_dp;

  // These attributes will depend on both the object and
  // the context (type of view (page, workd), projection, ...)
  // Ideally most of the information can be reused for
  // many "frames" or quickly updated each frame.  The 
  // CRhinoDrawAttributes class should be on the heap
  // and efficiently managed by a view and object.
  const class CRhinoDrawAttributes* m_da;
};

/*
Description:
  The CRhinoObjectDrawIterator is a customized iterator
  used to iterate through the list of objects being
  drawn in a viewport.
*/
class RHINO_SDK_CLASS CRhinoObjectDrawIterator
{
  CRhinoObjectDrawIterator() RHINO_NOEXCEPT;


};



#endif
