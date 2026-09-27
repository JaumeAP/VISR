/* Copyright Institute of Sound and Vibration Research - All rights reserved. */

#include <librbbl/accelerate_fft_wrapper.hpp>

#include <libefl/basic_vector.hpp>
#include <libvisr/constants.hpp>

#include <boost/test/unit_test.hpp>

#include <cmath>
#include <complex>
#include <random>

namespace visr
{
namespace rbbl
{
namespace test
{

namespace
{
  /**
   * Round-trips a signal through forward+inverse and checks that the result
   * matches the input up to the scale factor the wrapper itself reports
   * (forwardScalingFactor() * inverseScalingFactor() * dftSize), which is
   * exactly the contract CoreConvolverUniform::calculateFilterScalingFactor()
   * relies on. A wrong scale here would make BRIR-convolved audio silently
   * too loud or too quiet by a constant factor.
   */
  void checkRoundTrip( std::size_t dftSize, std::vector<float> const & input )
  {
    AccelerateFftWrapper<float> fft( dftSize, cVectorAlignmentSamples );

    std::size_t const outputSize = dftSize / 2 + 1;
    efl::BasicVector<std::complex<float> > spectrum( outputSize, cVectorAlignmentSamples );
    efl::BasicVector<float> result( dftSize, cVectorAlignmentSamples );

    fft.forwardTransform( input.data(), spectrum.data() );
    fft.inverseTransform( spectrum.data(), result.data() );

    float const scale = fft.forwardScalingFactor() * fft.inverseScalingFactor()
                       * static_cast<float>( dftSize );

    // A relative check alone is meaningless where the expected value is (near)
    // zero, so combine it with a small absolute tolerance for floating-point
    // round-off noise (several orders of magnitude below the audio noise floor).
    for( std::size_t i = 0; i < dftSize; ++i )
    {
      float const expected = input[ i ] * scale;
      float const absTolerance = std::max( 1e-3f, std::abs( expected ) * 1e-4f );
      BOOST_CHECK_SMALL( result[ i ] - expected, absTolerance );
    }
  }
}

BOOST_AUTO_TEST_CASE( AccelerateFftWrapperRoundTripImpulse )
{
  std::size_t const dftSize = 64;
  std::vector<float> input( dftSize, 0.0f );
  input[ 3 ] = 1.0f;
  checkRoundTrip( dftSize, input );
}

BOOST_AUTO_TEST_CASE( AccelerateFftWrapperRoundTripNoise )
{
  std::size_t const dftSize = 1024;
  std::mt19937 rng( 1234 );
  std::uniform_real_distribution<float> dist( -1.0f, 1.0f );
  std::vector<float> input( dftSize );
  for( auto & v : input )
  {
    v = dist( rng );
  }
  checkRoundTrip( dftSize, input );
}

BOOST_AUTO_TEST_CASE( AccelerateFftWrapperRejectsNonPowerOfTwo )
{
  BOOST_CHECK_THROW( AccelerateFftWrapper<float>( 100, cVectorAlignmentSamples ), std::invalid_argument );
}

} // namespace test
} // namespace rbbl
} // namespace visr
