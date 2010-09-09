/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_CONCURRENCY_WIN32_DETAIL_PRIMITIVES_HPP
#define ZI_CONCURRENCY_WIN32_DETAIL_PRIMITIVES_HPP 1

#include <zi/concurrency/config.hpp>

namespace zi {
namespace concurrency_ {
namespace win32 {


typedef CRITICAL_SECTION critical_section;

using ::InitializeCriticalSection ;
using ::EnterCriticalSection      ;
using ::TryEnterCriticalSection   ;
using ::LeaveCriticalSection      ;
using ::DeleteCriticalSection     ;

typedef HANDLE   handle  ;
typedef DWORD    dword   ;
typedef LONGLONG longlong;

using ::CreateEvent;
using ::CreateMutex;
using ::CreateSemaphore;
using ::CreateThread;
using ::ReleaseMutex;
using ::ReleaseSemaphore;
using ::WaitForSingleObject;
using ::CloseHandle;
using ::GetCurrentThreadId;
using ::GetCurrentThread;
using ::SignalObjectAndWait;
using ::ResumeThread;

const unsigned forever = INFINITE;


} // namespace win32
} // namespace concurrency_
} // namespace zi

#endif
