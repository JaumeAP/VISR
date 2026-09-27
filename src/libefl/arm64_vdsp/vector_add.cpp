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

template<>
ErrorCode vectorAdd( float const * const op1,
                     float const * const op2,
                     float * const result,
                     std::size_t numElements,
                     std::size_t alignment /*= 0*/ )
{
#ifndef NDEBUG
  if( not checkAlignment( op1, alignment ) ) return alignmentError;
  if( not checkAlignment( op2, alignment ) ) return alignmentError;
  if( not checkAlignment( result, alignment ) ) return alignmentError;
#endif
  vDSP_vadd( op1, 1, op2, 1, result, 1, static_cast<vDSP_Length>( numElements ) );
  return noError;
}

template<>
ErrorCode vectorAddInplace( float const * const op1,
                            float * const op2Result,
                            std::size_t numElements,
                            std::size_t alignment /*= 0*/ )
{
#ifndef NDEBUG
  if( not checkAlignment( op1, alignment ) ) return alignmentError;
  if( not checkAlignment( op2Result, alignment ) ) return alignmentError;
#endif
  vDSP_vadd( op1, 1, op2Result, 1, op2Result, 1, static_cast<vDSP_Length>( numElements ) );
  return noError;
}

template<>
ErrorCode vectorAddConstant( float constantValue,
                             float const * const op,
                             float * const result,
                             std::size_t numElements,
                             std::size_t alignment /*= 0*/ )
{
#ifndef NDEBUG
  if( not checkAlignment( op, alignment ) ) return alignmentError;
  if( not checkAlignment( result, alignment ) ) return alignmentError;
#endif
  vDSP_vsadd( op, 1, &constantValue, result, 1, static_cast<vDSP_Length>( numElements ) );
  return noError;
}

template<>
ErrorCode vectorAddConstantInplace( float constantValue,
                                    float * const opResult,
                                    std::size_t numElements,
                                    std::size_t alignment /*= 0*/ )
{
#ifndef NDEBUG
  if( not checkAlignment( opResult, alignment ) ) return alignmentError;
#endif
  vDSP_vsadd( opResult, 1, &constantValue, opResult, 1, static_cast<vDSP_Length>( numElements ) );
  return noError;
}

} // namespace arm64_vdsp
} // namespace efl
} // namespace visr
