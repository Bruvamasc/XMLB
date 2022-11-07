//*****************************************************************************
// MIT License
//
// Copyright(c) 2022 Vladislav Kurmanenko (Bruvamasc)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this softwareand associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright noticeand this permission notice shall be included in 
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//*****************************************************************************



#ifndef XMLB_TYPE_CALLABLE_TRAITS_H
#define XMLB_TYPE_CALLABLE_TRAITS_H

#include <tuple>

#include "XMLB/detail/traits/XMLB_Type_traits.h"



namespace XMLB { namespace detail {

	//#########################################################################

	template<typename>
	struct function_signature;

	//-------------------------------------------------------------------------

	template<typename T, typename ... Args>
	struct function_signature<T(Args...)>
	{
		using return_type = T;
		using arguments = std::tuple<Args...>;
		using signature = T(Args...);
	};

	//-------------------------------------------------------------------------

	template<typename T, typename U, typename ... Args>
	struct function_signature<T(U::*)(Args...)>
	{
		using return_type = T;
		using arguments = std::tuple<Args...>;
		using signature = T(U::*)(Args...);
	};

	//-------------------------------------------------------------------------

	template<typename T, typename ... Args>
	struct function_signature<T(*)(Args...)>
	{
		using return_type = T;
		using arguments = std::tuple<Args...>;
		using signature = T(*)(Args...);
	};

	//#########################################################################

	template<typename T>
	using function_return_type_t = 
		typename function_signature<T>::return_type;

	//*************************************************************************

	template<typename T>
	using function_arguments_t = 
		typename function_signature<T>::arguments;

	//*************************************************************************

	template<typename T>
	using function_signature_t = typename function_signature<T>::signature;

	//*************************************************************************

	template<std::size_t index, typename T>
	using function_argument_t = std::tuple_element_t<index, T>;

	//*************************************************************************

	template<typename T>
	inline constexpr std::size_t function_arguments_size_v = 
		std::tuple_size_v<T>;

	//*************************************************************************

}} // namespace XMLB::detail

#endif // !XMLB_TYPE_CALLABLE_TRAITS_H