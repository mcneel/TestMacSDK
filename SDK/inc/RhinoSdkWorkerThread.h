/* $NoKeywords: $ */
/*
//
// Copyright (c) 1993-2007 Robert McNeel & Associates. All rights reserved.
// Rhinoceros is a registered trademark of Robert McNeel & Associates.
//
// THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY.
// ALL IMPLIED WARRANTIES OF FITNESS FOR ANY PARTICULAR PURPOSE AND OF
// MERCHANTABILITY ARE HEREBY DISCLAIMED.
//				
// For complete Rhino SDK copyright information see <http://www.rhino3d.com/developer>.
//
////////////////////////////////////////////////////////////////
*/

#pragma once

/*
Description:
  In any function that allocates memory use by the core
  Rhino application, create a CRhCoreAppActivity class
  on the first line and all memory will come from the
  correct pool and the function will work in both
  ordinary running and while a worker thread is running.

  It is critical to make sure memory comes from the main
  pool in things that handle Windows events.
*/
class RHINO_SDK_CLASS CRhinoCoreAppActivity
{
public:
  CRhinoCoreAppActivity();
  ~CRhinoCoreAppActivity();

private:
  friend class CRhinoWorkerThread;

  ON__UINT_PTR m_worker_thread_id = 0;
  unsigned int m_worker_pool_disabled = 0;
  unsigned int m_worker_pool_index = 0;

  bool m_bInWorkerThread = false;
  bool m_bReserved1 = false;
  bool m_bReserved2 = false;
  bool m_bReserved3 = false;
};




class RHINO_SDK_CLASS CRhinoWorkerThread
{
public:
  CRhinoWorkerThread();
  virtual ~CRhinoWorkerThread();

  /*
  Description:
    Override Worker to perform time consuming calculations that
    the user can immediately cancel by pressing the "Esc" key.
    Use member variables in your derived class to pass information
    in and out of your Worker function.
  Example:

          class CMyWorker : public CRhinoWorkerThread
          {
          public:
            CMyWorker();
            ~CMyWorker();
            void Worker();
            // override of pure virtual runtime cache cleaner
            void CleanUpRuntimeCaches( bool bDelete );

            // use member variables to pass information to/from worker
          };

          void CMyWorker::Worker()
          {
            // NEVER EVER Call ExitThread() in your worker function!!!

            // See the CRhinoWorkerThread::KeepRunning() example if
            // if you have a time consuming loop with no memory
            // allocation.

            ...
          }

          void CMyWorker::CleanUpRuntimeCaches( bool bDelete )
          {
            // Call the virtual ON_Object::DestroyRuntimeCache(bDelete)
            // on object that are in the main thread.

            // Be sure to get this right, or we get hard to trace
            // crashes that happen in other places after a cancel.
          }
          ...

          {
            ...
            CMyWorker worker;
            ...
            BOOL32 bCompletedTask = worker.Run( RhLocalizeString( L"Calculating widget hyperextension coefficient", 43572) );
            if ( !bCompletedTask )
            {
              // User canceled task - carefully destroy worker
            }
            else
            {
              // harvest results from worker membr variables
            }
          }

  Remarks:
    Do NOT create dialogs and do NOT call Rhino SDK functions in a Worker()
    function except for those listed below.

    You may call the following Rhino SDK functions in a Worker function:
      CRhinoApp.Print
      CRhinoDoc::AddPointObject
      CRhinoDoc::AddPointCloudObject
      CRhinoDoc::AddCurveObject
      CRhinoDoc::AddSurfaceObject
      CRhinoDoc::AddMeshObject
      CRhinoDoc::AddBrepObject

    Other than adding objects, don't mess with the document,
    application, or views in a Worker() function.
  */
  virtual 
  void Worker() = 0;

  /*
  Description:
    Cleans up runtime caches that exist on any objects
    whose memory belongs on the main thread stack or heap
    but whose runtime cache may be created in worker
    thread's stack or worker heap.  Basically, this function
    needs to call the ON_Object::DestroyRuntimeCache() member
    functions of breps, curves and surfaces that exist in
    the main thread's stack or heap but are used in closest
    point or intersection calculations performed in the
    Worker() function.

  Parameters:
    bDelete - [in] pass this parameter on to the
      ON_Object::DestroyRuntimeCache(bDelete) function.
      Basically, if bDelete is true, then you should 
      free/delete runtime cache information that is on
      the heap.  If bDelete is false, then you should
      simply zero out pointers on main heap/stack classes
      that point to memory allocated from the worker
      pool.  If this is confusing, then you need to get
      help before you check in anything using cancel 
      threads.
      

  Remarks:
    It is critical to do this right.  If you don't we end
    up with hrd to repeat crashes that happen one to many
    commands after the actual cancel event.

    This function is pure virtual so that any developer
    using a cancel thread is forced to think about cleaning
    up runtime memory caches.  If you are at all confused,
    then please ask for help or do not use cancel threads.
    
    Test thoroughly before checking in.
  */
  virtual 
  void CleanUpRuntimeCaches( bool bDelete ) = 0;

  /*
  Description:
    Runs the Worker() task in a thread that the user can
    cancel by pressing the "Esc" key.
  Returns:
    TRUE if Worker() ask runs to completion.
    FALSE if Worker() task is canceled.
  See Also:
    CRhinoWorkerThread::Worker
  */
  bool Run(
          const wchar_t* sCommandLineMessage
          );

#if defined (ON_RUNTIME_APPLE)
  CRhinoDoc* GetDocument();
  void SetDocument(CRhinoDoc*);

protected:
  UINT_PTR m_doc_serial_number;
  //   CRhinoDoc* pDoc = RhinoApp().GetDocument((unsigned int)doc_id);
  //   return (int)(pDoc->m_runtime_doc_serial_number);
#endif

public:
  /*
    static function interface to the query and manage the
    thread.  There may be nested calls to CWorkerThread::Run 
    but there is only a single Windows worker thread at 
    any one time. 
  */

  /*
  Returns:   
    TRUE if a worker thread is running.
  */
  static bool IsRunning();

  /*
  Returns;
    Worker thread id or zero.
  */
  static ON__UINT64 WorkerContextSerialNumber();

  /*
  Returns;
    Worker thread id or zero.
  */
  static ON__UINT_PTR ThreadId();

  /*
  Description:
    Call CancelWorker() to cancel a worker thread.  If a dialog
    runs a time consuming task, then you may want to add a cancel
    button to the dialog and call CRhinoWorkerThread::CancelWorker
    if the user presses that button.
  Remarks:
    If Worker() is running in a thread, this sets the thread's
    memory pool flags so ExitThread() will be called on next 
    onmalloc(), oncalloc(), onrealloc(), onfree(), new, or delete.
  */
  static void CancelWorker();

  /*
  Description:
    Call this function in Worker() member functions that
    that do not regularly allocate memory.  
  Example:

          for(...)
          {
            // loop with LOTS of calculation and no memory allocation
            if ( !CRhinoWorkerThread::KeepRunning() )
            {
              // thread will die soon, just bail out
              return;
            }
          }

  Returns:
    TRUE if you should keep going.
    FALSE if you should bail out.
  */
  static bool KeepRunning();

  /*
  Returns:
    TRUE if the use of worker threads is enabled.
    FALSE if the use of worker threads has been disabled.
  Remarks:
    By default, the use of worker threads is enabled.
    When using Purify or doing delicate debugging of a
    Worker function, disabling the use of separate threads
    can be helpful.
  See Also:
    CRhinoWorkerThread::EnableWorkerThreads
  */
  static bool WorkerThreadsEnabled();

  /*
  Description:
    By default, worker threads are enabled.  If you need to user Purify or
    debug something that happens in a CRhinoWorkerThread::Worker override,
    then call CRhinoWorkerThread::EnableWorkerThreads(FALSE) and your task
    will be much easier.
  See Also:
    CRhinoWorkerThread::WorkerThreadsEnabled
  */
  static void EnableWorkerThreads( int bEnable = true );

  /*
   Description:
     These functions are used when a custom built worker-thread style mechanism
     is running, and the developer wants Rhino to act as though a real worker thread
     is running.  The UI will be disabled and views will not draw.  Of course, it is
     absolutely essential that these functions are called in pairs.
     */
  static void IncrementFakeWorkerThreads(void);
  static void DecrementFakeWorkerThreads(void);

  
private:
#if defined (ON_RUNTIME_APPLE)
  
  //
  // Class (static) private methods for managing all threads.
  // You must use a mutex when accessing these variables.
  //
  //
  //
  
  // This mutex ensures that only one CRhinoWorkerThread thread executes at a time
  static pthread_mutex_t m_execution_mutex;
  
  
  // This mutex protects access to all the following static variables
  static pthread_mutex_t m_manager_mutex;

#endif

  static volatile int m_s_FakeWorkerThreadCounter;
  
#if defined (ON_RUNTIME_WIN)  
  // Value returned by ON_SetWorkerThreadContext().
  static volatile ON__UINT64 m_s_WorkerThreadContextSerialNumber;
  
  static volatile ON__UINT_PTR m_s_WorkerThreadId;

  // The pointers in m_s_PoolList[] are valid at all
  // times because they point to memory that is in the
  // main app heap.  The pool list is used to destroy
  // pool(s) when worker(s) are canceled.
  static volatile unsigned int m_s_WorkerCount;

  /* 
  Description:
    When a worker thread is looking at or changing these static
    member variables or memory pool flags, it needs to do so
    in a critical section.  This critical section is initialized
    at the start of CRhinoWorkerThread::Run() and destroyed at
    the end of CRhinoWorkerThread::Run(). 
  */
  static CRITICAL_SECTION m_s_critical_section;

  /* 
  Description:
    Set to TRUE if worker thread needs to exit ASAP
  See Also:
    CRhinoWorkerThread::CancelWorker
  */
  static volatile bool m_s_bKillThreadASAP;

  /* 
  Description:
    Set to TRUE to disable creation of multiple pools/threads
    so that purify can be used to testn code in Worker() functions.
    By default, the use of worker threads is enabled.
  See Also:
    CRhinoWorkerThread::WorkerThreadsEnabled
    CRhinoWorkerThread::EnableWorkerThreads
  */
  static bool m_s_bDisableWorkerThreads;

  // kills the thread in an unsafe manner
#if defined (RHINO_SDK_MFC)
  int Internal_Stop( class CWinThread* );
#endif

  bool Internal_PushWorker();
  bool Internal_PopWorker();

  static void Internal_ZeroAllStatics();

  static UINT Internal_RunWorkerHelper(LPVOID);
  static void Internal_EndWorkerHelper(
    ON__UINT_PTR ignored_parameter
  );

  int             m__worker_stage = 0;
  int             m__reserved = 0;


#endif
};
