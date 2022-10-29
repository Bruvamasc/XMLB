/******************************************************************************
* @file
* Данный файл объявлет основной строительный элемент(узел) XML документа.
* На текущий момент, является завершенным без дебаг функций
*
* @author Bruvamasc
* @date   2022-09-17
*
* @todo Нужно подумать, как добавить режим дебага. Также нужно подумать, над
* кодировками - возможно, как-то их переделать
* ///< Указывает, что элемент недоступен для использования
*
******************************************************************************/

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

	template<typename, typename = void>
	struct is_can_equality : std::false_type {};

	//-------------------------------------------------------------------------

	template<typename T>
	struct is_can_equality<T,
		std::void_t<decltype(std::declval<T>() == (std::declval<T>()))>
	> : std::true_type {};

	//-------------------------------------------------------------------------

	template<typename T>
	inline constexpr bool is_can_equality_v = is_can_equality<T>::value;

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