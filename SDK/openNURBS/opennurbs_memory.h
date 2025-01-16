//
// Copyright (c) 1993-2022 Robert McNeel & Associates. All rights reserved.
// OpenNURBS, Rhinoceros, and Rhino3D are registered trademarks of Robert
// McNeel & Associates.
//
// THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY.
// ALL IMPLIED WARRANTIES OF FITNESS FOR ANY PARTICULAR PURPOSE AND OF
// MERCHANTABILITY ARE HEREBY DISCLAIMED.
//				
// For complete openNURBS copyright information see <http://www.opennurbs.org>.
//
////////////////////////////////////////////////////////////////

#if !defined(OPENNURBS_MEMORY_INC_)
#define OPENNURBS_MEMORY_INC_

#if defined (cplusplus) || defined(_cplusplus) || defined(__cplusplus)
extern "C" {
#endif

ON_DECL
size_t ON_MemoryPageSize();

#if defined(OPENNURBS_PLUS)

#if defined(ON_DEBUG) && defined(ON_COMPILER_MSC) && defined(ON_RUNTIME_WIN)
#define ON_DEBUG_MEMORY_MSC_WIN

// copied from C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\crt\src\dbgint.h
// We cannot include dbgint.h because it uses #define for malloc and new which makes
// it impossible to use some other libraries and has unknowable consequenses for
// Rhino plug-in developers.
#define ON_DEBUG_MEMORY_MSC_WIN_nNoMansLandSize 4
struct ON_DEBUG_MEMORY_MSC_WIN_CrtMemBlockHeader
{
  struct ON_DEBUG_MEMORY_MSC_WIN_CrtMemBlockHeader * pBlockHeaderNext;
  struct ON_DEBUG_MEMORY_MSC_WIN_CrtMemBlockHeader * pBlockHeaderPrev;
  const char*                 szFileName;
  int                         nLine;
#ifdef _WIN64
  /* These items are reversed on Win64 to eliminate gaps in the struct
  * and ensure that sizeof(struct)%16 == 0, so 16-byte alignment is
  * maintained in the debug heap.
  */
  int                         nBlockUse;
  size_t                      nDataSize;
#else  /* _WIN64 */
  size_t                      nDataSize;
  int                         nBlockUse;
#endif  /* _WIN64 */
  long                        lRequest;
  unsigned char               gap[ON_DEBUG_MEMORY_MSC_WIN_nNoMansLandSize];
  /* followed by:
  *  unsigned char           data[nDataSize];
  *  unsigned char           anotherGap[nNoMansLandSize];
  */
};

/*
Returns:
  True if nBLockUse is marked a client block or normal block
  False otherwise.
Remarks:
  Return value is an int so this function can be used in .C files.
*/
ON_DECL
int ON_DEBUG_MEMORY_MSC_WIN_IsTrackedBlockUse(int nBlockUse);

/*
Returns:
  True if block_head->nBLockUse is marked a client block or normal block
  False otherwise.
Remarks:
  Return value is an int so this function can be used in .C files.
*/
ON_DECL
int ON_DEBUG_MEMORY_MSC_WIN_IsTrackedBlock(const struct ON_DEBUG_MEMORY_MSC_WIN_CrtMemBlockHeader* block_head);

/*
Returns:
  The block header for the pointer
*/
ON_DECL
const struct ON_DEBUG_MEMORY_MSC_WIN_CrtMemBlockHeader* ON_DEBUG_MEMORY_MSC_WIN_CrtMemBlockHeader(const void* ptr);

/*
Returns:
  True if the block was marked as not a leak.
  False otherwise.
Remarks:
  Return value is an int so this function can be used in .C files.
*/
ON_DECL
int ON_DEBUG_MEMORY_MSC_WIN_IsNotALeak(const void* ptr);

/*
Returns:
  True if the block was marked as not a leak.
  False otherwise.
Remarks:
  Return value is an int so this function can be used in .C files.
*/
ON_DECL
int ON_DEBUG_MEMORY_MSC_WIN_IsNotALeakBlock(const struct ON_DEBUG_MEMORY_MSC_WIN_CrtMemBlockHeader* block_head);

#endif


ON_DECL
void ON_MemoryManagerBegin(void);

ON_DECL
void ON_MemoryManagerEnd(void);

/*
/////////////////////////////////////////////////////////////////////////////
//
// ALL memory management in the openNURBS toolkit is done through calls to
//    onmalloc(), oncalloc(), onrealloc(), onfree(), 
//    onmemdup(), onstrdup(), ..., and the
//    new and delete operators
// The on*() functions are all declared in opennurbs_memory.h and defined
// in opennurbs_memory.c.  The new and delete operators that use onmalloc()
// and onfree() are defined in opennurbs_plus_memory_new.cpp.
//
// You may OPTIONALLY provide your own memory management functions.  See
// opennurbs_memory.c for details.
//
/////////////////////////////////////////////////////////////////////////////
//
// Details:
//
// void* onmalloc( site_t sz );
//
//   If sz is zero, nullptr is returned.
//   If sz is positive and there is not enough memory to satisfy
//   the allocation request, the ON_memory_error_handler(0) is called
//   and nullptr is returned.  
//
//   If you have implemented multiple memory pools in a custom manager,
//   the request is sent to the current pool.
//        
//
// void* oncalloc( size_t num, size_t sz );
//
//   If sz or num is zero, nullptr is returned.
//   If sz and num are positive and there is not enough memory to satisfy
//   the allocation request, the ON_memory_error_handler(0) is called
//   and nullptr is returned.
//
//   If you have implemented multiple memory pools in a custom manager,
//   the request is sent to the current pool.
//               
//               
// void* onrealloc( void* p, size_t sz );
//
//   If p is nullptr, then onmalloc(sz) is called.
//   If sz is zero, then onfree(p) is called and
//   nullptr is returned.
//   If p is not nullptr, sz is positive, and there is not
//   enough memory to to satisfy the allocation request,
//   the ON_memory_error_handler(0) is called and nullptr is returned.
//   If p is not nullptr and is known to be invalid, then
//   ON_memory_error_handler(1) is called.
//
//   If you have implemented multiple memory pools in a custom manager,
//   the request is sent to the current pool.
//
//   NOTE WELL: Microsoft's VC 6.0 realloc() contains a bug that can cause
//              crashes and should be avoided.  See MSDN Knowledge Base
//              article ID Q225099 for more information.
//
//
// void onfree( void* p );
//
//   nullptr p is tolerated but considered poor style.
//   If p is not nullptr and is known to be invalid, then
//   ON_memory_error_handler(2) is called.
//
// void* onmemdup( const void* src, size_t sz );
//
//   If src is not nullptr and sz is positive, then onmalloc( sz )
//   is called to get memory, sz bytes of src are copied into this
//   memory, and the pointer to this memory is returned.
//   If onmalloc() returns nullptr, then nullptr is returned.
//   If src is nullptr or sz is zero, then nullptr is returned.
//               
//
// char* onstrdup( const char* src );
//
//   If src is not nullptr, then onmemdup( sc, (strlen(src)+1)*sizeof(src[0]) )
//   is called.  If onmemdup() returns nullptr, then nullptr is returned.
//   If src is nullptr, then nullptr is returned.
//               
//
// ON_register_memory_error_handler( ON_memory_error_handler my_handler );
//
//    Use this function to specify the error handler function that will be called
//    if an error occurs.  See the description of ON_memory_error_handler() for the
//    error handler function specification.
//
// int ON_memory_error_handler( int error_number );
//
//   This function is called when an error occurs.  The values of error_number
//   are:
//
//     0: There is not enough memory to satisfy an onmalloc() or onrealloc()
//        request.
//
//        If ON_memory_error_handler() returns 0, then onmalloc() or onrealloc()
//        returns nullptr.  If ON_memory_error_handler() returns 1, the allocation
//        is attempted again.
//
//     1: An invalid pointer was passed to onrealloc().
//
//        If ON_memory_error_handler() returns 0, then onrealloc() returns nullptr.
//        If ON_memory_error_handler() returns 1, the reallocation is attempted
//        again;
//
//     2: An invalid pointer was passed to onfree().
//
//        If ON_memory_error_handler() returns 0, then free() returns.
//        If ON_memory_error_handler() returns 1, the deallocation is
//        attempted again;
//
*/

/* ^^^ see comments above for details ^^^ */

/*
/////////////////////////////////////////////////////////////
//
// Memory error handler - the default handler does nothing.
//
// 
// See opennurbs_memory.c for instructions on using
// custom memory managers.
//
// ON_memory_error_register_handler() returns a pointer to
// the handler you are replacing.  If you pass a nullptr,
// the default memory error handler is used.  The default 
// memory error handler returns 0 for all errors.
//
// The input to ON_memory_error_handler(int) is:
//
//   0: malloc/calloc/realloc request returned nullptr
//   1: invalid pointer passed to realloc()
//   2: invalid pointer passed to free()
//   3: msize query returned zero or 0xFFFFFFFF
//
// If ON_memory_error_handler(int) returns
//
//   0: The malloc()/calloc()/realloc()/msize() request returns 0.
//   1: The call to malloc()/calloc()/realloc()/msize() is repeated.
//  
*/
/* error handling */
typedef int (*ON_memory_error_handler)(int);
ON_DECL
ON_memory_error_handler ON_memory_error_register_handler( ON_memory_error_handler );

/*
Description:
  Your app can call ON_memory_error_text_out_of_memory_handler() to
  trigger a call to the handler you registered with ON_memory_error_register_handler().
  The parameter error_code is passed to your handler.
Parameters:
  error_code - [in]
    Passed as the parameter to the registered ON_memory_error_handler function.
     0: malloc/calloc/realloc request returned nullptr
     1: invalid pointer passed to realloc()
     2: invalid pointer passed to free()
     3: msize query returned zero or 0xFFFFFFFF
Returns:
  The value returned by the registered ON_memory_error_handler function.
*/
ON_DECL int ON_memory_error_test_handler(int error_code);

/*
Description:
  Set the worker thread context. ON_SetWorkerThreadContext() is used to register
  the callback function that will be called during every memory allocation event.
  In order for this function to be called, you must call 
  ON_WorkerThreadEnableTerminateRequest() after calling ON_SetWorkerThreadContext().
Parameters:
  m_platform_thread_id - [in]
    Cannot be zero.
  fTerminateThreadFunction - [in]
    Function to call to terminate thread. Cannot be null
  function_parameter - [in]
    fTerminateThreadFunction() parameter value
Returns:
  0: A context is already set, or the input parameter is null.
  >0: Serial number of the new context.
*/
ON_DECL
ON__UINT64 ON_SetWorkerThreadContext(
  void(*fTerminateThreadFunction)(ON__UINT_PTR),
  ON__UINT_PTR function_parameter
);

/*
Description:
  If a worker thread context is active, then every subsequent
  memory allocation will call the fTerminateThreadFunction function
  passed to ON_SetWorkerThreadContext().
Returns:
  0: No worker thread context is set.
  0>: Serial number of the worker thread context with a pending terminate request.
*/
ON_DECL
ON__UINT64 ON_WorkerThreadEnableTerminateRequest();

/*
Description:
  Clear all worker thread context information.
Returns:
  0:  No worker thread context was set.
  >0: Serial number of the worker thread context that was cleared.
*/
ON_DECL
ON__UINT64 ON_ClearWorkerThreadContext();

/*
Returns:
  0:  No worker thread context is set.
  >0: Serial number of the worker thread context.
*/
ON_DECL
ON__UINT64 ON_WorkerThreadContextSerialNumber();

/*
Returns:
  0:  No pending terminate request.
  >0: Serial number of the worker thread context with a pending terminate request.
*/
ON_DECL
ON__UINT64 ON_WorkerThreadTerminateRequestStatus();

ON_DECL
void ON_Disableonmalloc();

#if defined (ON_RUNTIME_APPLE)
ON_DECL
void ON_SetWorkerThreadTestCancel (void(*fTestCancelThreadFunction)(void));
#endif

#endif

/*
Allocate memory that is intentionally never returned
should not be considered a memory leak. Typically this is
for an application workspace.
*/
ON_DECL
void* onmalloc_forever( size_t );

ON_DECL
void* onmalloc( size_t );

ON_DECL
void* oncalloc( size_t, size_t );

ON_DECL
void   onfree( void* );

ON_DECL
void* onrealloc( void*, size_t );

ON_DECL
void* onmemdup( const void*, size_t );

ON_DECL
char* onstrdup( const char* );

ON_DECL
wchar_t* onwcsdup( const wchar_t* );

ON_DECL
unsigned char* onmbsdup( const unsigned char* );

#if defined (cplusplus) || defined(_cplusplus) || defined(__cplusplus)
}

class ON_CLASS ON_MemoryAllocationTracking
{
public:
  /*
  Description:
    Windows Debug Builds:
      The constructor saves the current state of memory allocation tracking
      and then enables/disables memory allocation tracking.
    Otherwise:
      Does nothing.
  */
  ON_MemoryAllocationTracking(
    bool bEnableAllocationTracking
  );

  /*
  Description:
    Windows Debug Builds:
      The destructor restores the saved state of memory allocation tracking.
    Otherwise:
      Does nothing.
  */
  ~ON_MemoryAllocationTracking();

private:
  static unsigned int m_g_stack_depth;
  static int m_g_crt_dbg_flag0;
  const unsigned int m_this_statck_depth;
  const int m_this_crt_dbg_flag0;

private:
  ON_MemoryAllocationTracking() = delete;
  ON_MemoryAllocationTracking(const ON_MemoryAllocationTracking&) = delete;
  ON_MemoryAllocationTracking& operator=(const ON_MemoryAllocationTracking&) = delete;
};

#endif

#endif
