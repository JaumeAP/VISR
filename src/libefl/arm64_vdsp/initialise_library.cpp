/* Copyright Institute of Sound and Vibration Research - All rights reserved */

#include "initialise_library.hpp"

#include "vector_functions.hpp"

#include "../reference/vector_functions.hpp"

namespace visr
{
namespace efl
{
namespace arm64_vdsp
{

bool initialiseLibrary( char const * /*processor = ""*/ )
{
  VectorAddWrapper<float>::set( &arm64_vdsp::vectorAdd<float> );
  VectorAddInplaceWrapper<float>::set( &arm64_vdsp::vectorAddInplace<float> );
  VectorAddConstantWrapper<float>::set( &arm64_vdsp::vectorAddConstant<float> );
  VectorAddConstantInplaceWrapper<float>::set( &arm64_vdsp::vectorAddConstantInplace<float> );

  VectorSubtractWrapper<float>::set( &arm64_vdsp::vectorSubtract<float> );
  VectorSubtractInplaceWrapper<float>::set( &arm64_vdsp::vectorSubtractInplace<float> );
  VectorSubtractConstantWrapper<float>::set( &arm64_vdsp::vectorSubtractConstant<float> );
  VectorSubtractConstantInplaceWrapper<float>::set( &arm64_vdsp::vectorSubtractConstantInplace<float> );

  VectorMultiplyWrapper<float>::set( &arm64_vdsp::vectorMultiply<float> );
  VectorMultiplyInplaceWrapper<float>::set( &arm64_vdsp::vectorMultiplyInplace<float> );
  VectorMultiplyConstantWrapper<float>::set( &arm64_vdsp::vectorMultiplyConstant<float> );
  VectorMultiplyConstantInplaceWrapper<float>::set( &arm64_vdsp::vectorMultiplyConstantInplace<float> );

  VectorMultiplyAddWrapper<float>::set( &arm64_vdsp::vectorMultiplyAdd<float> );
  VectorMultiplyAddInplaceWrapper<float>::set( &arm64_vdsp::vectorMultiplyAddInplace<float> );
  VectorMultiplyConstantAddWrapper<float>::set( &arm64_vdsp::vectorMultiplyConstantAdd<float> );
  VectorMultiplyConstantAddInplaceWrapper<float>::set( &arm64_vdsp::vectorMultiplyConstantAddInplace<float> );

  VectorRampScalingWrapper<float>::set( &arm64_vdsp::vectorRampScaling<float> );

  return true;
}

bool uninitialiseLibrary()
{
  VectorAddWrapper<float>::set( &reference::vectorAdd<float> );
  VectorAddInplaceWrapper<float>::set( &reference::vectorAddInplace<float> );
  VectorAddConstantWrapper<float>::set( &reference::vectorAddConstant<float> );
  VectorAddConstantInplaceWrapper<float>::set( &reference::vectorAddConstantInplace<float> );

  VectorSubtractWrapper<float>::set( &reference::vectorSubtract<float> );
  VectorSubtractInplaceWrapper<float>::set( &reference::vectorSubtractInplace<float> );
  VectorSubtractConstantWrapper<float>::set( &reference::vectorSubtractConstant<float> );
  VectorSubtractConstantInplaceWrapper<float>::set( &reference::vectorSubtractConstantInplace<float> );

  VectorMultiplyWrapper<float>::set( &reference::vectorMultiply<float> );
  VectorMultiplyInplaceWrapper<float>::set( &reference::vectorMultiplyInplace<float> );
  VectorMultiplyConstantWrapper<float>::set( &reference::vectorMultiplyConstant<float> );
  VectorMultiplyConstantInplaceWrapper<float>::set( &reference::vectorMultiplyConstantInplace<float> );

  VectorMultiplyAddWrapper<float>::set( &reference::vectorMultiplyAdd<float> );
  VectorMultiplyAddInplaceWrapper<float>::set( &reference::vectorMultiplyAddInplace<float> );
  VectorMultiplyConstantAddWrapper<float>::set( &reference::vectorMultiplyConstantAdd<float> );
  VectorMultiplyConstantAddInplaceWrapper<float>::set( &reference::vectorMultiplyConstantAddInplace<float> );

  VectorRampScalingWrapper<float>::set( &reference::vectorRampScaling<float> );

  return true;
}

} // namespace arm64_vdsp
} // namespace efl
} // namespace visr
