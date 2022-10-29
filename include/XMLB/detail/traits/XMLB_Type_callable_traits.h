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