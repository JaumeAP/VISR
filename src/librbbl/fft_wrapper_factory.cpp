/* Copyright Institute of Sound and Vibration Research - All rights reserved */

#include "fft_wrapper_factory.hpp"


#ifdef BUILD_USE_IPP
#include "ipp_fft_wrapper.hpp"
#endif
#include "kiss_fft_wrapper.hpp"

#ifdef BUILD_FFTS
#include "ffts_wrapper.hpp"
#endif

#ifdef BUILD_USE_ACCELERATE
#include "accelerate_fft_wrapper.hpp"
#endif

#include <stdexcept>

namespace visr
{
namespace rbbl
{

template<typename SampleType> 
FftWrapperFactory<SampleType>::Creator::Creator( CreateFunction fcn )
 : mCreateFunction( fcn )
{
}

template<typename SampleType> 
std::unique_ptr<FftWrapperBase< SampleType> >
FftWrapperFactory<SampleType>::Creator::create( std::size_t fftSize,
                                                std::size_t alignElements ) const
{
  return std::unique_ptr<FftWrapperBase< SampleType> >( mCreateFunction( fftSize, alignElements ) );
}

template<typename SampleType>
/*static*/ std::string FftWrapperFactory<SampleType>::listImplementations()
{
  std::string res;
  CreatorTable const & table = creatorTable();
  for( typename CreatorTable::const_iterator tblIt( table.begin() ); tblIt != table.end(); /*Increment is in loop */ )
  {
    res.append( tblIt->first );
    if( ++tblIt == table.end() )
    {
      break;
    }
    res.append( ", " );
  }
  return res;
}

template<typename SampleType> 
/*static*/ typename FftWrapperFactory<SampleType>::CreatorTable &
FftWrapperFactory<SampleType>::creatorTable()
{
  static typename FftWrapperFactory<SampleType>::CreatorTable sCreatorTable;
  return sCreatorTable;
}

template<typename SampleType> 
/*static*/ std::unique_ptr< FftWrapperBase<SampleType> > 
FftWrapperFactory<SampleType>::create( std::string const & wrapperName,
				       std::size_t fftSize,
				       std::size_t alignElements )
{
  std::string lowerName(wrapperName);
  boost::algorithm::to_lower(lowerName);

  typename CreatorTable::const_iterator findIt
    = creatorTable().find( lowerName );
  if( findIt == creatorTable().end() )
  {
    throw std::invalid_argument( "FftWrapperFactory: The specified FFT wrapper does not exist for this platform/data type combination." );
  }
  return std::unique_ptr<FftWrapperBase<SampleType> >( findIt->second.create( fftSize, alignElements ) );
}

template class FftWrapperFactory<float>;
template class FftWrapperFactory<double>;

/**
 * A helper class with whole purpose is to register the different object types in the factory.
 */
struct InstantiateFftWrapperFactory
{
  InstantiateFftWrapperFactory()
  {
    FftWrapperFactory<float>::registerWrapper<KissFftWrapper<float> >( "kissfft" );
    FftWrapperFactory<double>::registerWrapper<KissFftWrapper<double> >( "kissfft" );

#ifdef BUILD_FFTS
    FftWrapperFactory<float>::registerWrapper<FftsWrapper<float> >( "ffts" );
    // FFTS is not implemented for type double.
#endif

#ifdef BUILD_USE_IPP
    FftWrapperFactory<float>::registerWrapper<IppFftWrapper<float> >( "ipp" );
    FftWrapperFactory<double>::registerWrapper<IppFftWrapper<double> >( "ipp" );
#endif

#ifdef BUILD_USE_ACCELERATE
    FftWrapperFactory<float>::registerWrapper<AccelerateFftWrapper<float> >( "accelerate" );
    // vDSP's real FFT is only wrapped for single precision.
#endif
    // Create entries for the default FFT. These might also depend on the platform
    // and the chosen build options.
    //
    // "accelerate" is deliberately NOT used here even though it is faster:
    // callers that don't request an FFT implementation get "default", and
    // its numerical rounding differs slightly from KissFFT's (bit-exact
    // equality between implementations is not guaranteed by the FFTImpl
    // contract). Silently changing what unspecified callers get would risk
    // breaking anything relying on KissFFT's specific rounding, such as
    // bear's variable_block_size tests. Opt in explicitly with "accelerate".
    FftWrapperFactory<float>::registerWrapper<KissFftWrapper<float> >( "default" );
    FftWrapperFactory<double>::registerWrapper<KissFftWrapper<double> >( "default" );
  }
};

/**
 * Object which is used to initialise the object factory.
 */
InstantiateFftWrapperFactory const cInstantiationHelper;

} // namespace rbbl
} // namespace visr
