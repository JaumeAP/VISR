/* Copyright Institute of Sound and Vibration Research - All rights reserved */

#include "initialise_library.hpp"

#include <mutex>

#ifdef VISR_SYSTEM_PROCESSOR_x86_64
#include "intel_x86_64/initialise_library.hpp"
#endif

#ifdef VISR_SYSTEM_PROCESSOR_armv7l
#include "armv7l_neon_32bit/initialise_library.hpp"
#endif

#if defined( VISR_SYSTEM_PROCESSOR_arm64 ) && defined( __APPLE__ )
#include "arm64_vdsp/initialise_library.hpp"
#endif

namespace visr
{
namespace efl
{

bool initialiseLibrary( char const * processor /*= ""*/ )
{
  // The backend-specific initialisers register per-type function pointers
  // (Wrapper<T>::sPtr, a std::function) that every vector function call reads.
  // Constructing several renderers concurrently - plausible in a plugin host
  // with multiple simultaneous instances - previously called this
  // concurrently too, racing on those std::function assignments (observed
  // with ThreadSanitizer, including a race on the std::function's vtable
  // during construction, not just a stale-value race). Since re-registering
  // the same backend has no effect beyond the first call, run it at most
  // once, regardless of how many threads or how many times it is called.
  static std::once_flag flag;
  static bool result = true;
  std::call_once( flag, [processor]()
  {
#ifdef VISR_SYSTEM_PROCESSOR_x86_64
    result = intel_x86_64::initialiseLibrary( processor );
#endif
#ifdef VISR_SYSTEM_PROCESSOR_armv7l
    result = armv7l_neon_32bit::initialiseLibrary( processor );
#endif
#if defined( VISR_SYSTEM_PROCESSOR_arm64 ) && defined( __APPLE__ )
    result = arm64_vdsp::initialiseLibrary( processor );
#endif
  } );
  return result;
}

bool uninitialiseLibrary()
{
#ifdef VISR_SYSTEM_PROCESSOR_x86_64
  return intel_x86_64::uninitialiseLibrary();
#endif
#ifdef VISR_SYSTEM_PROCESSOR_armv7l
  return armv7l_neon_32bit::uninitialiseLibrary();
#endif
#if defined( VISR_SYSTEM_PROCESSOR_arm64 ) && defined( __APPLE__ )
  return arm64_vdsp::uninitialiseLibrary();
#endif
  return true;
}

} // namespace efl
} // namespace visr

