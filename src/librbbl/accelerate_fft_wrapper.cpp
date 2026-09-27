/* Copyright Institute of Sound and Vibration Research - All rights reserved */

#include "accelerate_fft_wrapper.hpp"

#include <Accelerate/Accelerate.h>

#include <cmath>
#include <stdexcept>

namespace visr
{
namespace rbbl
{

namespace
{
  /**
   * vDSP's radix-2 real FFT requires a power-of-two size. Returns log2(size),
   * or throws if size is not a power of two.
   */
  vDSP_Length log2OfPowerOfTwo( std::size_t size )
  {
    if( size == 0 or ( size bitand ( size - 1 ) ) != 0 )
    {
      throw std::invalid_argument( "AccelerateFftWrapper: fftSize must be a power of two." );
    }
    return static_cast<vDSP_Length>( std::log2( static_cast<double>(size) ) );
  }
}

template<>
class AccelerateFftWrapper<float>::Impl
{
public:
  Impl( std::size_t fftSize )
   : mFftSize( fftSize )
   , mLog2Size( log2OfPowerOfTwo( fftSize ) )
   , mHalfSize( fftSize / 2 )
  {
    mSetup = vDSP_create_fftsetup( mLog2Size, kFFTRadix2 );
    if( not mSetup )
    {
      throw std::invalid_argument( "AccelerateFftWrapper: vDSP_create_fftsetup() failed." );
    }
    mSplit.realp = new float[ mHalfSize ];
    mSplit.imagp = new float[ mHalfSize ];
  }

  ~Impl()
  {
    vDSP_destroy_fftsetup( mSetup );
    delete [] mSplit.realp;
    delete [] mSplit.imagp;
  }

  std::size_t const mFftSize;
  vDSP_Length const mLog2Size;
  std::size_t const mHalfSize;
  FFTSetup mSetup;
  /**
   * Scratch split-complex buffer of mHalfSize elements, reused for both
   * forward and inverse transforms. Not thread-safe, matching the
   * const-but-mutates-scratch-space contract of the other wrappers, which
   * are used from a single rendering thread per instance.
   */
  DSPSplitComplex mSplit;
};

template<>
AccelerateFftWrapper<float>::AccelerateFftWrapper( std::size_t fftSize, std::size_t /*alignmentElements*/ )
 : mImpl( new AccelerateFftWrapper<float>::Impl( fftSize ) )
{
}

template<>
AccelerateFftWrapper<float>::~AccelerateFftWrapper()
{
}

template<>
efl::ErrorCode AccelerateFftWrapper<float>::forwardTransform( float const * const in, std::complex<float> * out ) const
{
  DSPSplitComplex & split = mImpl->mSplit;
  // Pack the N real input samples into vDSP's split-complex representation
  // (N/2 complex pairs), as vDSP_fft_zrip expects.
  vDSP_ctoz( reinterpret_cast<DSPComplex const *>( in ), 2, &split, 1, mImpl->mHalfSize );
  vDSP_fft_zrip( mImpl->mSetup, &split, 1, mImpl->mLog2Size, kFFTDirection_Forward );

  // Unpack vDSP's DC/Nyquist-in-bin-0 layout into the N/2+1 complex bins
  // expected by the caller (the same layout KissFFT/FFTS produce): bin 0 is
  // DC, bin N/2 is Nyquist, both purely real.
  out[ 0 ] = std::complex<float>( split.realp[ 0 ], 0.0f );
  out[ mImpl->mHalfSize ] = std::complex<float>( split.imagp[ 0 ], 0.0f );
  for( std::size_t k = 1; k < mImpl->mHalfSize; ++k )
  {
    out[ k ] = std::complex<float>( split.realp[ k ], split.imagp[ k ] );
  }
  return efl::noError;
}

template<>
efl::ErrorCode AccelerateFftWrapper<float>::inverseTransform( std::complex<float> const * const in, float * out ) const
{
  DSPSplitComplex & split = mImpl->mSplit;
  // Re-pack the N/2+1 complex bins into vDSP's DC/Nyquist-in-bin-0 layout.
  split.realp[ 0 ] = in[ 0 ].real();
  split.imagp[ 0 ] = in[ mImpl->mHalfSize ].real();
  for( std::size_t k = 1; k < mImpl->mHalfSize; ++k )
  {
    split.realp[ k ] = in[ k ].real();
    split.imagp[ k ] = in[ k ].imag();
  }

  vDSP_fft_zrip( mImpl->mSetup, &split, 1, mImpl->mLog2Size, kFFTDirection_Inverse );
  vDSP_ztoc( &split, 1, reinterpret_cast<DSPComplex *>( out ), 2, mImpl->mHalfSize );
  return efl::noError;
}

} // namespace rbbl
} // namespace visr
