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



#ifndef XMLB_TYPE_METHODS_TRAITS_H
#define XMLB_TYPE_METHODS_TRAITS_H

#include "XMLB/detail/traits/XMLB_Type_traits.h"



namespace XMLB { namespace detail {

	//#########################################################################

	template<typename, typename = void>
	struct is_has_pre_increment : std::false_type {};

	//-------------------------------------------------------------------------

	template<typename T>
	struct is_has_pre_increment<T, std::void_t<pre_increment_t<T>>
	> : std::true_type {};

	//-------------------------------------------------------------------------

	template<typename T>
	inline constexpr bool is_has_pre_increment_v = 
		is_has_pre_increment<T>::value;

	//#########################################################################

	template<typename, typename = void>
	struct is_has_pre_decrement : std::false_type {};

	//-------------------------------------------------------------------------

	template<typename T>
	struct is_has_pre_decrement<T, std::void_t<pre_decrement_t<T>>
	> : std::true_type {};

	//-------------------------------------------------------------------------

	template<typename T>
	inline constexpr bool is_has_pre_decrement_v = 
		is_has_pre_decrement<T>::value;

	//#########################################################################

	template<typename, typename = void>
	struct is_has_dereferencing : std::false_type {};

	//-------------------------------------------------------------------------

	template<typename T>
	struct is_has_dereferencing<T, std::void_t<dereferenced_t<T>>
	> : std::true_type {};

	//-------------------------------------------------------------------------

	template<typename T>
	inline constexpr bool is_has_dereferencing_v = 
		is_has_dereferencing<T>::value;

	//#########################################################################

	template<typename, typename = void>
	struct is_has_self_equality : std::false_type {};

	//-------------------------------------------------------------------------

	template<typename T>
	struct is_has_self_equality<T,
		std::void_t<decltype(std::declval<T>().operator==(std::declval<T>()))>
	> :std::true_type {};

	//-------------------------------------------------------------------------

	template<typename T>
	inline constexpr bool is_has_self_equality_v = 
		is_has_self_equality<T>::value;

	//#########################################################################

	template<typename, typename = void>
	struct is_has_self_not_equality : std::false_type {};

	//-------------------------------------------------------------------------

	template<typename T>
	struct is_has_self_not_equality<T,
		std::void_t<decltype(std::declval<T>().operator!=(std::declval<T>()))>
	> : std::true_type {};

	//-------------------------------------------------------------------------

	template<typename T>
	inline constexpr bool is_has_self_not_equality_v = 
		is_has_self_not_equality<T>::value;

	//#########################################################################

	template<typename, typename, typename = void>
	struct is_can_equality : std::false_type {};

	//-------------------------------------------------------------------------

	template<typename T, typename U>
	struct is_can_equality<T, U,
		std::void_t<decltype(std::declval<T>() == (std::declval<U>()))>
	> : std::true_type {};

	//-------------------------------------------------------------------------

	template<typename T, typename U = T>
	inline constexpr bool is_can_equality_v = is_can_equality<T, U>::value;

	//#########################################################################

	template<typename, typename = void>
	struct is_can_not_equality : std::false_type {};

	//-------------------------------------------------------------------------

	template<typename T>
	struct is_can_not_equality<T,
		std::void_t<decltype(std::declval<T>() != (std::declval<T>()))>
	> : std::true_type {};

	//-------------------------------------------------------------------------

	template<typename T>
	inline constexpr bool is_can_not_equality_v = 
		is_can_not_equality<T>::value;

	//#########################################################################

	template<typename, typename = void>
	struct is_has_begin_and_end : std::false_type {};

	//-------------------------------------------------------------------------

	template<typename T>
	struct is_has_begin_and_end<T,
		std::void_t<decltype(std::declval<T>().begin()),
		decltype(std::declval<T>().end())>> : std::true_type {};

	//-------------------------------------------------------------------------

	template<typename T>
	inline constexpr bool is_has_begin_and_end_v = 
		is_has_begin_and_end<T>::value;

	//#########################################################################

}} // namespace XMLB::detail

#endif // !XMLB_TYPE_METHODS_TRAITS_H