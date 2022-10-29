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