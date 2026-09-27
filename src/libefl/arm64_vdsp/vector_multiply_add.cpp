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
ErrorCode vectorMultiplyAdd( float const * const factor1,
                            float const * const factor2,
                            float const * const addend,
                            float * const result,
                            std::size_t numElements,
                            std::size_t alignment /*= 0*/ )
{
#ifndef NDEBUG
  if( not checkAlignment( factor1, alignment ) ) return alignmentError;
  if( not checkAlignment( factor2, alignment ) ) return alignmentError;
  if( not checkAlignment( addend, alignment ) ) return alignmentError;
  if( not checkAlignment( result, alignment ) ) return alignmentError;
#endif
  vDSP_vma( factor1, 1, factor2, 1, addend, 1, result, 1, static_cast<vDSP_Length>( numElements ) );
  return noError;
}

template<>
ErrorCode vectorMultiplyAddInplace( float const * const factor1,
                                   float const * const factor2,
                                   float * const accumulator,
                                   std::size_t numElements,
                                   std::size_t alignment /*= 0*/ )
{
#ifndef NDEBUG
  if( not checkAlignment( factor1, alignment ) ) return alignmentError;
  if( not checkAlignment( factor2, alignment ) ) return alignmentError;
  if( not checkAlignment( accumulator, alignment ) ) return alignmentError;
#endif
  vDSP_vma( factor1, 1, factor2, 1, accumulator, 1, accumulator, 1,
           static_cast<vDSP_Length>( numElements ) );
  return noError;
}

template<>
ErrorCode vectorMultiplyConstantAdd( float constFactor,
                                    float const * const factor,
                                    float const * const addend,
                                    float * const result,
                                    std::size_t numElements,
                                    std::size_t alignment /*= 0*/ )
{
#ifndef NDEBUG
  if( not checkAlignment( factor, alignment ) ) return alignmentError;
  if( not checkAlignment( addend, alignment ) ) return alignmentError;
  if( not checkAlignment( result, alignment ) ) return alignmentError;
#endif
  vDSP_vsma( factor, 1, &constFactor, addend, 1, result, 1,
            static_cast<vDSP_Length>( numElements ) );
  return noError;
}

template<>
ErrorCode vectorMultiplyConstantAddInplace( float constFactor,
                                           float const * const factor,
                                           float * const accumulator,
                                           std::size_t numElements,
                                           std::size_t alignment /*= 0*/ )
{
#ifndef NDEBUG
  if( not checkAlignment( factor, alignment ) ) return alignmentError;
  if( not checkAlignment( accumulator, alignment ) ) return alignmentError;
#endif
  vDSP_vsma( factor, 1, &constFactor, accumulator, 1, accumulator, 1,
            static_cast<vDSP_Length>( numElements ) );
  return noError;
}

} // namespace arm64_vdsp
} // namespace efl
} // namespace visr
