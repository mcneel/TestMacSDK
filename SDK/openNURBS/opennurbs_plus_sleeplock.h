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

#if !defined(OPENNURBS_PLUS) || defined(OPENNURBS_PUBLIC)
#error This file should not be distributed with the public opennurbs source code toolkit.
#endif

#if !defined(OPENNURBS_PLUS_SLEEPLOCK_INC_)
#define OPENNURBS_PLUS_SLEEPLOCK_INC_

#if defined(ON_COMPILER_MSC)
// Microsoft compiler

/*
Description:
  ON_SleepLock_SuspendThisThread suspends the thread for a specified
  number of milliseconds.  
Parameters:
  n - [in]
    number of milliseconds to sleep.
Remarks:
  It is critical that only the thread be suspended and not the 
  entire process.
*/
#define ON_PointerSleepLock_SuspendThisThread(n) Sleep(n) // Sleeps the THREAD not the PROCESS

/*
Description:
  ON_PointerSleepLock_Test is used as the test function in sleep locks that
  set pointer values.  If the input pointer value is 0, then it is set to 1.
  The operation is atomic and is used for thread safe sleep locks.
Parameters:
  ptr_type - [in]
    type that ptr points to
  ptr - [in/out]
    ptr to test. If ptr is null, it is set to 1. If ptr is not null, its
    value is not changed
Returns:
  The input value of the pointer.
*/


/*
Description:
  ON_PointerSleepLock_Set is used to set pointer as an atomic operation.
  It is intended to be used when a sleep lock is employed to make
  setting the pointer thread safe.
Parameters:
  ptr_type - [in]
    type that ptr points to
  ptr - [in/out]
    ptr is set to ptr_value
  ptr_value - [in]
Returns:
  The input value of the pointer.
*/
#if 8 == ON_SIZEOF_POINTER
#define ON_PointerSleepLock_Test(ptr_type,ptr) (ptr_type*)((INT_PTR)InterlockedCompareExchange64((LONGLONG*)(&(ptr)),1,0))
#define ON_PointerSleepLock_Set(ptr_type,ptr,ptr_value) (ptr_type*)((INT_PTR)InterlockedExchange64((LONGLONG*)(&(ptr)),(LONGLONG)((INT_PTR)ptr_value)))
#elif 4 == ON_SIZEOF_POINTER
#define ON_PointerSleepLock_Test(ptr_type,ptr) (ptr_type*)((INT_PTR)InterlockedCompareExchange((LONG*)(&(ptr)),1,0))
#define ON_PointerSleepLock_Set(ptr_type,ptr,ptr_value) (ptr_type*)((INT_PTR)InterlockedExchange((LONG*)(&(ptr)),(LONG)((INT_PTR)ptr_value)))
#else
#error invalid ON_SIZEOF_POINTER value
#endif

/*
Description:
  ON_IntSleepLock_Test is used as the test function in sleep locks that
  set 4 byte integer values.  If the input value of lock is test_value,
  then it is set to sleep_value.
Parameters:
  test_value - [in]
  sleep_value - [in]
  lock - [in/out]
    4 byte integer to test. 
    If lock == test_value, then lock is set to sleep_value. 
    If lock != test_value, then the value of lock is not changed
Returns:
  The input value of lock.
*/
#define ON_IntSleepLock_Test(lock,test_value,sleep_value) InterlockedCompareExchange(((LONG*)(&(lock))),sleep_value,test_value)

/*
Description:
  ON_PointerSleepLock_Set is used to unconditionally 
  lock = lock_value and return the original value of lock
  as an atomic operation.
  It is intended to be used when a sleep lock is employed to make
  setting the lock thread safe.
Parameters:
  lock - [in/out]
    4 byte integer to set
  lock_value - [in]
Returns:
  The input value of lock.
*/
#define ON_IntSleepLock_Set(lock,lock_value) InterlockedExchange(((LONG*)(&(lock))),lock_value)


#elif defined(ON_RUNTIME_APPLE) && defined(ON_COMPILER_CLANG) && defined(OPENNURBS_IN_RHINO) && !defined(ON_RUNTIME_APPLE_IOS)
// Apple compiler
#include <unistd.h>
#define ON_PointerSleepLock_SuspendThisThread(n) usleep(n) // Sleeps the THREAD not the PROCESS

void* ON_Internal_CompareExchangePointer (void* volatile* dest, void* exchange, void* comparand);
void* ON_Internal_ExchangePointer (void* volatile* dest, void* exchange);
int ON_Internal_CompareExchange (int volatile* dest, int exchange, int comparand);
int ON_Internal_Exchange (int volatile* dest, int exchange);

#define ON_PointerSleepLock_Test(ptr_type,ptr) (ptr_type*)(ON_Internal_CompareExchangePointer((void* volatile*)(&(ptr)),(void*)1,(void*)0))
#define ON_PointerSleepLock_Set(ptr_type,ptr,ptr_value) (ptr_type*)(ON_Internal_ExchangePointer((void* volatile*)(&(ptr)),(void*)ptr_value))
#define ON_IntSleepLock_Test(lock,test_value,sleep_value) ON_Internal_CompareExchange(((int volatile*)(&(lock))),sleep_value,test_value)
#define ON_IntSleepLock_Set(lock,lock_value) ON_Internal_Exchange((int volatile*)(&(lock)),lock_value)

#else

#include <unistd.h>
//#error Need to fill in the following defines or comment out this error for single threaded applications
#define ON_NO_SLEEPLOCK_SUPPORT
void* ON_Internal_CompareExchangePointer (void* volatile* dest, void* exchange, void* comparand);

#define ON_PointerSleepLock_SuspendThisThread(n) usleep(n) // Sleeps the THREAD not the PROCESS
#define ON_SleepLock_SuspendThisThread(n) usleep(n)// Sleeps the THREAD not the PROCESS
#define ON_PointerSleepLock_Test(ptr_type,ptr) (ptr_type*)(ON_Internal_CompareExchangePointer((void* volatile*)(&(ptr)),(void*)1,(void*)0))
#define ON_PointerSleepLock_Set(ptr_type,ptr,ptr_value) (ptr_type*)(ptr); ((ptr)=(ptr_value));
#define ON_IntSleepLock_Test(lock,test_value,sleep_value) lock;// if (test_value==lock) lock=sleep_value
#define ON_IntSleepLock_Set(lock,lock_value) lock=lock_value

#endif

class ON_CLASS ON_PointerSleepLock
{
private:
  // prohibit use of default constructor, copy constructor and operator=
  ON_PointerSleepLock(const ON_PointerSleepLock&) = delete;
  ON_PointerSleepLock& operator=(const ON_PointerSleepLock&) = delete;

public:
  // You should put your ON_PointerSleepLock on the stack.  
  // Using managed memory to construct / destruct these locks is a good way to 
  // lock something important and loose the key.
  ON_PointerSleepLock() = default;
  ~ON_PointerSleepLock() = default;

  /*
  Description:
    Gets the lock using a the specified waiting interval
    and maximum waiting time.
  Parameters:
    address_of_shared_resource_ptr - [in]
      The address of a shared resource pointer cast as (ON__UINT_PTR*).

    interval_wait_msecs - [in]
      number of milliseconds to wait between checking for the lock.
      If interval_wait_msecs is 0, then 50 is used.
      If interval_wait_msecs > max_wait_msecs > 0,
      then one attempt is made to get the lock.
    
    max_wait_msecs - [in]
      maximum number of milliseconds to wait for lock.
      If max_wait_msecs is 0, then no maximum waiting time is used.

    bStealLockAfterWaiting - [in]
      Steal the lock if the resource is not available after the
      maximum waiting time expires.

  Returns:
    0: The shared resource is locked by another process
       and the maximum waiting time expired.

    1: The shared resource pointer was nullptr and is now locked
       or the maximum waiting time expired and bStealLockAfterWaiting
       was true.
       You need to create the resource as quickly as possible and
       call SetPointerAndUnlock() with a pointer to the new resource.

    If GetLock() returns 1, then you must call SetPointerAndUnlock().
  Remarks:
     It is easy and clear to people reading your code if you use values
     like 
        ON_SleepLock::OneSecond, 
        ON_SleepLock::ThirtySeconds, 
        ON_SleepLock::OneMinute,
        47*ON_SleepLock::OneSecond or
        ON_SleepLock::OneMinute/3
     to specify the values for interval_wait_msecs and max_wait_msecs.
  
  Example:

    const ON_3dPoint* SharedResourcePointAt123()
    {
      static ON_3dPoint* shared_resource = nullptr;
      
      ON_PointerSleepLock lock;

      ON__UINT_PTR ptr = lock.GetLock( (ON__UINT_PTR*)&shared_resource, ... );

      if ( 1 == ptr )
      {
        // Create the shared resource
        ON_3dPoint* valid_point = new ON_3dPoint(1,2,3);
        lock.ReturnLock( (ON__UINT_PTR)valid_point );
      }

      return shared_resource;
    }
  */
  ON__UINT_PTR GetPointerOrLock( 
    ON__UINT_PTR* address_of_shared_resource_ptr,
    unsigned int interval_wait_msecs,
    unsigned int max_wait_msecs,
    bool bStealLockAfterWaiting
    );

  /*
  Description:
    Unconditionally returns the lock.
  Returns:
    True if a lock was released.
  */
  bool SetPointerAndUnlock(
    ON__UINT_PTR valid_shared_resource_ptr
    );

private:
  ON__UINT_PTR* m_address_of_shared_resource_ptr = nullptr;
};

#endif
