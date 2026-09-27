/* Copyright Institute of Sound and Vibration Research - All rights reserved */

#ifndef VISR_LIBRBBL_ACCELERATE_FFT_WRAPPER_HPP_INCLUDED
#define VISR_LIBRBBL_ACCELERATE_FFT_WRAPPER_HPP_INCLUDED

#include "export_symbols.hpp"
#include "fft_wrapper_base.hpp"

#include <memory>

namespace visr
{
namespace rbbl
{

/**
 * FFT wrapper class encapsulating Apple's Accelerate/vDSP framework for
 * real-to-complex transforms. Native on Apple Silicon and Intel Macs, and
 * typically faster than the KissFFT fallback used by default.
 * @tparam DataType The floating-point element type for the transform. The
 * class is specialized for type \p float (vDSP's single-precision real FFT).
 */
template< typename DataType >
class VISR_RBBL_LIBRARY_SYMBOL AccelerateFftWrapper: public FftWrapperBase<DataType>
{
public:
  /**
   * Typedef for the frequency-domain samples.
   * Needs to be redeclared and marked as 'typename' by GCC.
   */
  using FrequencyDomainType = typename FftWrapperBase<DataType>::FrequencyDomainType;

  /**
   * Create a wrapper object for executing FFTs.
   * @param fftSize Input FFT size, number of points in the real-valued FFT
   * input or IFFT output. Must be a power of two (a hard requirement of
   * vDSP's radix-2 real FFT).
   * @param alignment Minimum alignment of input and output data, in number
   * of elements. Unused by this backend (vDSP does not require a specific
   * external alignment), kept for interface compatibility with the other
   * wrappers.
   */
  AccelerateFftWrapper( std::size_t fftSize, std::size_t alignment );

  /**
   * Destructor.
   */
  ~AccelerateFftWrapper();

  /*virtual*/ efl::ErrorCode forwardTransform( DataType const * const in, FrequencyDomainType * out ) const override;

  /*virtual*/ efl::ErrorCode inverseTransform( FrequencyDomainType const * const in, DataType * out ) const override;

  /**
   * vDSP's real forward FFT (vDSP_fft_zrip) returns coefficients scaled by 2
   * relative to a textbook unnormalized DFT. Reporting this here (rather
   * than correcting for it on every block) lets callers such as
   * CoreConvolverUniform fold the correction once into the filter's
   * frequency-domain representation, at no extra runtime cost.
   */
  /*virtual*/ DataType forwardScalingFactor( ) const override { return static_cast<DataType>(2.0); };

  /*virtual*/ DataType inverseScalingFactor() const override { return static_cast<DataType>(1.0); }

private:
  /**
   * Internal implementation object to avoid Accelerate/vDSP dependencies in
   * the header. Holds the FFT setup and scratch split-complex buffers.
   */
  class Impl;
  /**
   * Pointer to the implementation object (pimpl idiom).
   */
  std::unique_ptr<Impl> mImpl;
};

} // namespace rbbl
} // namespace visr

#endif // #ifndef VISR_LIBRBBL_ACCELERATE_FFT_WRAPPER_HPP_INCLUDED
