/* Copyright Institute of Sound and Vibration Research - All rights reserved */

#ifndef VISR_LIBEFL_ARM64_VDSP_VECTOR_FUNCTIONS_HPP_INCLUDED
#define VISR_LIBEFL_ARM64_VDSP_VECTOR_FUNCTIONS_HPP_INCLUDED

#include "../vector_functions.hpp"

namespace visr
{
namespace efl
{
namespace arm64_vdsp
{

// Only the real (float) specialisations are provided; complex<float> falls
// back to the generic scalar reference implementation, same as today.

template<typename T>
VISR_EFL_LIBRARY_SYMBOL
ErrorCode vectorAdd( T const * const op1,
                     T const * const op2,
                     T * const result,
                     std::size_t numElements,
                     std::size_t alignment = 0 );

template<typename T>
VISR_EFL_LIBRARY_SYMBOL
ErrorCode vectorAddInplace( T const * const op1,
                            T * const op2Result,
                            std::size_t numElements,
                            std::size_t alignment = 0 );

template<typename T>
VISR_EFL_LIBRARY_SYMBOL
ErrorCode vectorAddConstant( T constantValue,
                             T const * const op,
                             T * const result,
                             std::size_t numElements,
                             std::size_t alignment = 0 );

template<typename T>
VISR_EFL_LIBRARY_SYMBOL
ErrorCode vectorAddConstantInplace( T constantValue,
                                    T * const opResult,
                                    std::size_t numElements,
                                    std::size_t alignment = 0 );

template<typename T>
VISR_EFL_LIBRARY_SYMBOL
ErrorCode vectorSubtract( T const * const subtrahend,
                         T const * const minuend,
                         T * const result,
                         std::size_t numElements,
                         std::size_t alignment = 0 );

template<typename T>
VISR_EFL_LIBRARY_SYMBOL
ErrorCode vectorSubtractInplace( T const * const minuend,
                                T * const subtrahendResult,
                                std::size_t numElements,
                                std::size_t alignment = 0 );

template<typename T>
VISR_EFL_LIBRARY_SYMBOL
ErrorCode vectorSubtractConstant( T constantMinuend,
                                 T const * const subtrahend,
                                 T * const result,
                                 std::size_t numElements,
                                 std::size_t alignment = 0 );

template<typename T>
VISR_EFL_LIBRARY_SYMBOL
ErrorCode vectorSubtractConstantInplace( T constantMinuend,
                                        T * const subtrahendResult,
                                        std::size_t numElements,
                                        std::size_t alignment = 0 );

template<typename T>
VISR_EFL_LIBRARY_SYMBOL
ErrorCode vectorMultiply( T const * const factor1,
                         T const * const factor2,
                         T * const result,
                         std::size_t numElements,
                         std::size_t alignment = 0 );

template<typename T>
VISR_EFL_LIBRARY_SYMBOL
ErrorCode vectorMultiplyInplace( T const * const factor1,
                                T * const factor2Result,
                                std::size_t numElements,
                                std::size_t alignment = 0 );

template<typename T>
VISR_EFL_LIBRARY_SYMBOL
ErrorCode vectorMultiplyConstant( T constantValue,
                                 T const * const factor,
                                 T * const result,
                                 std::size_t numElements,
                                 std::size_t alignment = 0 );

template<typename T>
VISR_EFL_LIBRARY_SYMBOL
ErrorCode vectorMultiplyConstantInplace( T constantValue,
                                        T * const factorResult,
                                        std::size_t numElements,
                                        std::size_t alignment = 0 );

template<typename T>
VISR_EFL_LIBRARY_SYMBOL
ErrorCode vectorMultiplyAdd( T const * const factor1,
                            T const * const factor2,
                            T const * const addend,
                            T * const result,
                            std::size_t numElements,
                            std::size_t alignment = 0 );

template<typename T>
VISR_EFL_LIBRARY_SYMBOL
ErrorCode vectorMultiplyAddInplace( T const * const factor1,
                                   T const * const factor2,
                                   T * const accumulator,
                                   std::size_t numElements,
                                   std::size_t alignment = 0 );

template<typename T>
VISR_EFL_LIBRARY_SYMBOL
ErrorCode vectorMultiplyConstantAdd( T constFactor,
                                    T const * const factor,
                                    T const * const addend,
                                    T * const result,
                                    std::size_t numElements,
                                    std::size_t alignment = 0 );

template<typename T>
VISR_EFL_LIBRARY_SYMBOL
ErrorCode vectorMultiplyConstantAddInplace( T constFactor,
                                           T const * const factor,
                                           T * const accumulator,
                                           std::size_t numElements,
                                           std::size_t alignment = 0 );

template<typename T>
VISR_EFL_LIBRARY_SYMBOL
ErrorCode vectorRampScaling( T const * input,
                            T const * ramp,
                            T * output,
                            T baseGain,
                            T rampGain,
                            std::size_t numberOfElements,
                            bool accumulate = false,
                            std::size_t alignmentElements = 0 );

} // namespace arm64_vdsp
} // namespace efl
} // namespace visr

#endif // #ifndef VISR_LIBEFL_ARM64_VDSP_VECTOR_FUNCTIONS_HPP_INCLUDED
