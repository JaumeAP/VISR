/* Copyright Institute of Sound and Vibration Research - All rights reserved */

// Correctness test for libefl's real (float) vector functions. Written
// BEFORE adding an arm64 SIMD backend, against an independent scalar
// reference (not libefl's own reference:: namespace), so it is a real
// check rather than a tautology, and to establish the baseline before and
// after the backend is registered.

#include <libefl/initialise_library.hpp>
#include <libefl/vector_functions.hpp>

#include <boost/test/unit_test.hpp>

#include <random>
#include <vector>

namespace visr
{
namespace efl
{
namespace test
{

namespace
{
  // Registers the platform-specific vector function backend (e.g. the arm64
  // vDSP one), matching what bear::Renderer's constructor does. Without this,
  // the dispatch wrappers stay at their default scalar reference
  // implementation and this test would silently exercise only that.
  bool const gLibraryInitialised = visr::efl::initialiseLibrary();

  std::vector<float> randomVector( std::size_t n, unsigned seed )
  {
    std::mt19937 rng( seed );
    std::uniform_real_distribution<float> dist( -1.0f, 1.0f );
    std::vector<float> v( n );
    for( auto & x : v ) x = dist( rng );
    return v;
  }

  // Sizes chosen to exercise SIMD width (4 lanes for NEON/vDSP float), its
  // remainder handling (1-3 leftover elements), and the empty case.
  std::vector<std::size_t> const cTestSizes{ 0, 1, 2, 3, 4, 5, 7, 8, 16, 17, 128, 257 };

  void checkEqual( std::vector<float> const & actual,
                   std::vector<float> const & expected )
  {
    BOOST_REQUIRE_EQUAL( actual.size(), expected.size() );
    for( std::size_t i = 0; i < actual.size(); ++i )
    {
      BOOST_CHECK_CLOSE( actual[i], expected[i], 1e-3f /* percent */ );
    }
  }
}

BOOST_AUTO_TEST_CASE( VectorAddFloat )
{
  for( std::size_t n : cTestSizes )
  {
    std::vector<float> a = randomVector( n, 1 ), b = randomVector( n, 2 );
    std::vector<float> expected( n );
    for( std::size_t i = 0; i < n; ++i ) expected[i] = a[i] + b[i];

    std::vector<float> result( n );
    BOOST_CHECK_EQUAL( vectorAdd( a.data(), b.data(), result.data(), n ), noError );
    checkEqual( result, expected );
  }
}

BOOST_AUTO_TEST_CASE( VectorAddInplaceFloat )
{
  for( std::size_t n : cTestSizes )
  {
    std::vector<float> a = randomVector( n, 3 ), bResult = randomVector( n, 4 );
    std::vector<float> expected( n );
    for( std::size_t i = 0; i < n; ++i ) expected[i] = a[i] + bResult[i];

    BOOST_CHECK_EQUAL( vectorAddInplace( a.data(), bResult.data(), n ), noError );
    checkEqual( bResult, expected );
  }
}

BOOST_AUTO_TEST_CASE( VectorAddConstantFloat )
{
  for( std::size_t n : cTestSizes )
  {
    float const c = 0.42f;
    std::vector<float> a = randomVector( n, 5 );
    std::vector<float> expected( n );
    for( std::size_t i = 0; i < n; ++i ) expected[i] = c + a[i];

    std::vector<float> result( n );
    BOOST_CHECK_EQUAL( vectorAddConstant( c, a.data(), result.data(), n ), noError );
    checkEqual( result, expected );
  }
}

BOOST_AUTO_TEST_CASE( VectorAddConstantInplaceFloat )
{
  for( std::size_t n : cTestSizes )
  {
    float const c = -0.17f;
    std::vector<float> aResult = randomVector( n, 6 );
    std::vector<float> expected( n );
    for( std::size_t i = 0; i < n; ++i ) expected[i] = c + aResult[i];

    BOOST_CHECK_EQUAL( vectorAddConstantInplace( c, aResult.data(), n ), noError );
    checkEqual( aResult, expected );
  }
}

BOOST_AUTO_TEST_CASE( VectorMultiplyFloat )
{
  for( std::size_t n : cTestSizes )
  {
    std::vector<float> a = randomVector( n, 7 ), b = randomVector( n, 8 );
    std::vector<float> expected( n );
    for( std::size_t i = 0; i < n; ++i ) expected[i] = a[i] * b[i];

    std::vector<float> result( n );
    BOOST_CHECK_EQUAL( vectorMultiply( a.data(), b.data(), result.data(), n ), noError );
    checkEqual( result, expected );
  }
}

BOOST_AUTO_TEST_CASE( VectorMultiplyInplaceFloat )
{
  for( std::size_t n : cTestSizes )
  {
    std::vector<float> a = randomVector( n, 9 ), bResult = randomVector( n, 10 );
    std::vector<float> expected( n );
    for( std::size_t i = 0; i < n; ++i ) expected[i] = a[i] * bResult[i];

    BOOST_CHECK_EQUAL( vectorMultiplyInplace( a.data(), bResult.data(), n ), noError );
    checkEqual( bResult, expected );
  }
}

BOOST_AUTO_TEST_CASE( VectorMultiplyConstantFloat )
{
  for( std::size_t n : cTestSizes )
  {
    float const c = 2.5f;
    std::vector<float> a = randomVector( n, 11 );
    std::vector<float> expected( n );
    for( std::size_t i = 0; i < n; ++i ) expected[i] = c * a[i];

    std::vector<float> result( n );
    BOOST_CHECK_EQUAL( vectorMultiplyConstant( c, a.data(), result.data(), n ), noError );
    checkEqual( result, expected );
  }
}

BOOST_AUTO_TEST_CASE( VectorMultiplyConstantInplaceFloat )
{
  for( std::size_t n : cTestSizes )
  {
    float const c = -3.0f;
    std::vector<float> aResult = randomVector( n, 12 );
    std::vector<float> expected( n );
    for( std::size_t i = 0; i < n; ++i ) expected[i] = c * aResult[i];

    BOOST_CHECK_EQUAL( vectorMultiplyConstantInplace( c, aResult.data(), n ), noError );
    checkEqual( aResult, expected );
  }
}

BOOST_AUTO_TEST_CASE( VectorMultiplyAddFloat )
{
  for( std::size_t n : cTestSizes )
  {
    std::vector<float> a = randomVector( n, 13 ), b = randomVector( n, 14 ),
                       c = randomVector( n, 15 );
    std::vector<float> expected( n );
    for( std::size_t i = 0; i < n; ++i ) expected[i] = a[i] * b[i] + c[i];

    std::vector<float> result( n );
    BOOST_CHECK_EQUAL(
        vectorMultiplyAdd( a.data(), b.data(), c.data(), result.data(), n ), noError );
    checkEqual( result, expected );
  }
}

BOOST_AUTO_TEST_CASE( VectorMultiplyAddInplaceFloat )
{
  for( std::size_t n : cTestSizes )
  {
    std::vector<float> a = randomVector( n, 16 ), b = randomVector( n, 17 ),
                       accumulator = randomVector( n, 18 );
    std::vector<float> expected( n );
    for( std::size_t i = 0; i < n; ++i ) expected[i] = a[i] * b[i] + accumulator[i];

    BOOST_CHECK_EQUAL(
        vectorMultiplyAddInplace( a.data(), b.data(), accumulator.data(), n ), noError );
    checkEqual( accumulator, expected );
  }
}

BOOST_AUTO_TEST_CASE( VectorMultiplyConstantAddFloat )
{
  for( std::size_t n : cTestSizes )
  {
    float const c = 1.7f;
    std::vector<float> a = randomVector( n, 19 ), addend = randomVector( n, 20 );
    std::vector<float> expected( n );
    for( std::size_t i = 0; i < n; ++i ) expected[i] = c * a[i] + addend[i];

    std::vector<float> result( n );
    BOOST_CHECK_EQUAL(
        vectorMultiplyConstantAdd( c, a.data(), addend.data(), result.data(), n ), noError );
    checkEqual( result, expected );
  }
}

BOOST_AUTO_TEST_CASE( VectorMultiplyConstantAddInplaceFloat )
{
  for( std::size_t n : cTestSizes )
  {
    float const c = -0.9f;
    std::vector<float> a = randomVector( n, 21 ), accumulator = randomVector( n, 22 );
    std::vector<float> expected( n );
    for( std::size_t i = 0; i < n; ++i ) expected[i] = c * a[i] + accumulator[i];

    BOOST_CHECK_EQUAL(
        vectorMultiplyConstantAddInplace( c, a.data(), accumulator.data(), n ), noError );
    checkEqual( accumulator, expected );
  }
}

} // namespace test
} // namespace efl
} // namespace visr
