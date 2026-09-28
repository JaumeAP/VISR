/* Copyright Institute of Sound and Vibration Research - All rights reserved */

#include "vector_functions.hpp"

#include "../alignment.hpp"

#include <Accelerate/Accelerate.h>

namespace visr
{
namespace efl
{
namespace arm64_vdsp
{

// vDSP_vsub(B, IB, A, IA, C, IC, N) computes C = A - B (the arguments are
// swapped relative to the mathematical order), so the "minuend" (the value
// subtracted from) must be passed as B and the "subtrahend" as A.

template<>
ErrorCode vectorSubtract( float const * const subtrahend,
                         float const * const minuend,
                         float * const result,
                         std::size_t numElements,
                         std::size_t alignment /*= 0*/ )
{
#ifndef NDEBUG
  if( not checkAlignment( subtrahend, alignment ) ) return alignmentError;
  if( not checkAlignment( minuend, alignment ) ) return alignmentError;
  if( not checkAlignment( result, alignment ) ) return alignmentError;
#endif
  vDSP_vsub( minuend, 1, subtrahend, 1, result, 1, static_cast<vDSP_Length>( numElements ) );
  return noError;
}

template<>
ErrorCode vectorSubtractInplace( float const * const minuend,
                                float * const subtrahendResult,
                                std::size_t numElements,
                                std::size_t alignment /*= 0*/ )
{
#ifndef NDEBUG
  if( not checkAlignment( minuend, alignment ) ) return alignmentError;
  if( not checkAlignment( subtrahendResult, alignment ) ) return alignmentError;
#endif
  vDSP_vsub( subtrahendResult, 1, minuend, 1, subtrahendResult, 1, static_cast<vDSP_Length>( numElements ) );
  return noError;
}

template<>
ErrorCode vectorSubtractConstant( float constantMinuend,
                                 float const * const subtrahend,
                                 float * const result,
                                 std::size_t numElements,
                                 std::size_t alignment /*= 0*/ )
{
#ifndef NDEBUG
  if( not checkAlignment( subtrahend, alignment ) ) return alignmentError;
  if( not checkAlignment( result, alignment ) ) return alignmentError;
#endif
  float const negConstant = -constantMinuend;
  vDSP_vsadd( subtrahend, 1, &negConstant, result, 1, static_cast<vDSP_Length>( numElements ) );
  return noError;
}

template<>
ErrorCode vectorSubtractConstantInplace( float constantMinuend,
                                        float * const subtrahendResult,
                                        std::size_t numElements,
                                        std::size_t alignment /*= 0*/ )
{
#ifndef NDEBUG
  if( not checkAlignment( subtrahendResult, alignment ) ) return alignmentError;
#endif
  float const negConstant = -constantMinuend;
  vDSP_vsadd( subtrahendResult, 1, &negConstant, subtrahendResult, 1, static_cast<vDSP_Length>( numElements ) );
  return noError;
}

} // namespace arm64_vdsp
} // namespace efl
} // namespace visr
