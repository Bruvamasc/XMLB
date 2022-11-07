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



#ifndef XMLB_TYPE_CONVERTING_TRAITS_H
#define XMLB_TYPE_CONVERTING_TRAITS_H

#include <string>

#include "XMLB/detail/traits/XMLB_Type_traits.h"



namespace XMLB { namespace detail {

	//#########################################################################

	template<typename, typename = void>
	struct is_std_string : std::false_type {};

	//-------------------------------------------------------------------------

	template<typename T>
	struct is_std_string<T, 
		std::enable_if_t<std::is_same_v<std::decay_t<T>,
		std::basic_string<value_type_t<std::decay_t<T>>>>, void>
	> : std::true_type{};

	//-------------------------------------------------------------------------

	template<typename T>
	inline constexpr bool is_std_string_v = is_std_string<T>::value;

	//#########################################################################

	template<typename, typename = void>
	struct is_constuctible_from_std_string : std::false_type {};

	//-------------------------------------------------------------------------

	template<typename T>
	struct is_constuctible_from_std_string<T, 
		std::enable_if_t<std::is_assignable_v<T, 
		std::basic_string<value_type_t<std::decay_t<T>>>>, void>
	> : std::true_type {};

	//-------------------------------------------------------------------------

	template<typename T>
	inline constexpr bool is_constuctible_from_std_string_v =
		is_constuctible_from_std_string<T>::value;

	//#########################################################################

}} // namespace XMLB::detail

#endif // !XMLB_TYPE_CONVERTING_TRAITS_H