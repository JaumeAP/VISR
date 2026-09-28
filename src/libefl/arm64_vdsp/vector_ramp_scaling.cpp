/* Copyright Institute of Sound and Vibration Research - All rights reserved */

#include "vector_functions.hpp"

#include "../alignment.hpp"

#include <Accelerate/Accelerate.h>

#include <algorithm>
#include <cstddef>

namespace visr
{
namespace efl
{
namespace arm64_vdsp
{

// The per-sample gain (baseGain + rampGain * ramp[i]) has no single vDSP
// call that fuses it with the following multiply, so it is computed into a
// small fixed-size stack buffer, processed in bounded chunks to avoid a
// numberOfElements-sized allocation on what may be an audio thread.

template<>
ErrorCode vectorRampScaling( float const * input,
                            float const * ramp,
                            float * output,
                            float baseGain,
                            float rampGain,
                            std::size_t numberOfElements,
                            bool accumulate /*= false*/,
                            std::size_t alignmentElements /*= 0*/ )
{
#ifndef NDEBUG
  if( not checkAlignment( input, alignmentElements ) ) return alignmentError;
  if( not checkAlignment( ramp, alignmentElements ) ) return alignmentError;
  if( not checkAlignment( output, alignmentElements ) ) return alignmentError;
#endif
  constexpr std::size_t cChunkSize = 64;
  float scale[cChunkSize];
  std::size_t remaining = numberOfElements;
  while( remaining > 0 )
  {
    std::size_t const chunk = std::min( remaining, cChunkSize );
    vDSP_Length const vChunk = static_cast<vDSP_Length>( chunk );
    // scale[i] = ramp[i] * rampGain + baseGain
    vDSP_vsmsa( ramp, 1, &rampGain, &baseGain, scale, 1, vChunk );
    if( accumulate )
    {
      // output[i] += scale[i] * input[i]
      vDSP_vma( scale, 1, input, 1, output, 1, output, 1, vChunk );
    }
    else
    {
      // output[i] = scale[i] * input[i]
      vDSP_vmul( scale, 1, input, 1, output, 1, vChunk );
    }
    input += chunk;
    ramp += chunk;
    output += chunk;
    remaining -= chunk;
  }
  return noError;
}

} // namespace arm64_vdsp
} // namespace efl
} // namespace visr
