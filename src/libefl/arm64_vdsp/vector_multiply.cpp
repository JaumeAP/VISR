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
ErrorCode vectorMultiply( float const * const factor1,
                         float const * const factor2,
                         float * const result,
                         std::size_t numElements,
                         std::size_t alignment /*= 0*/ )
{
#ifndef NDEBUG
  if( not checkAlignment( factor1, alignment ) ) return alignmentError;
  if( not checkAlignment( factor2, alignment ) ) return alignmentError;
  if( not checkAlignment( result, alignment ) ) return alignmentError;
#endif
  vDSP_vmul( factor1, 1, factor2, 1, result, 1, static_cast<vDSP_Length>( numElements ) );
  return noError;
}

template<>
ErrorCode vectorMultiplyInplace( float const * const factor1,
                                float * const factor2Result,
                                std::size_t numElements,
                                std::size_t alignment /*= 0*/ )
{
#ifndef NDEBUG
  if( not checkAlignment( factor1, alignment ) ) return alignmentError;
  if( not checkAlignment( factor2Result, alignment ) ) return alignmentError;
#endif
  vDSP_vmul( factor1, 1, factor2Result, 1, factor2Result, 1, static_cast<vDSP_Length>( numElements ) );
  return noError;
}

template<>
ErrorCode vectorMultiplyConstant( float constantValue,
                                 float const * const factor,
                                 float * const result,
                                 std::size_t numElements,
                                 std::size_t alignment /*= 0*/ )
{
#ifndef NDEBUG
  if( not checkAlignment( factor, alignment ) ) return alignmentError;
  if( not checkAlignment( result, alignment ) ) return alignmentError;
#endif
  vDSP_vsmul( factor, 1, &constantValue, result, 1, static_cast<vDSP_Length>( numElements ) );
  return noError;
}

template<>
ErrorCode vectorMultiplyConstantInplace( float constantValue,
                                        float * const factorResult,
                                        std::size_t numElements,
                                        std::size_t alignment /*= 0*/ )
{
#ifndef NDEBUG
  if( not checkAlignment( factorResult, alignment ) ) return alignmentError;
#endif
  vDSP_vsmul( factorResult, 1, &constantValue, factorResult, 1, static_cast<vDSP_Length>( numElements ) );
  return noError;
}

} // namespace arm64_vdsp
} // namespace efl
} // namespace visr
