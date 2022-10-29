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

#ifndef XMLB_TYPE_TRAITS_H
#define XMLB_TYPE_TRAITS_H

#include <utility>
#include <iterator>

namespace XMLB { namespace detail {

	/**************************************************************************
	*							LIB TYPE TRAITS
	**************************************************************************/

	template<typename NodeT>
	struct node_traits
	{
		using symbol_type = typename NodeT::symbol_type;
		using string_type = typename NodeT::string_type;
		using string_wrapper = typename NodeT::string_wrapper;
		using attribute_type = typename NodeT::attribute_type;
		using node_type = typename NodeT::node_type;
		using size_type = typename NodeT::size_type;
		using Ptr = typename NodeT::Ptr;
		using attr_iterator = typename NodeT::attr_iterator;
		using attr_const_iterator = typename NodeT::attr_const_iterator;
		using iterator = typename NodeT::iterator;
		using const_iterator = typename NodeT::const_iterator;
	};

	//*************************************************************************

	template<typename DocT>
	struct document_traits
	{
		using symbol_type = typename DocT::symbol_type;
		using string_type = typename DocT::string_type;
		using string_wrapper = typename DocT::string_wrapper;
		using node_type = typename DocT::node_type;
		using node_pointer = typename DocT::node_pointer;
		using document_type = typename DocT::document_type;
		using size_type = typename DocT::size_type;
		using Ptr = typename DocT::Ptr;
		using iterator = typename DocT::iterator;
		using const_iterator = typename DocT::const_iterator;
	};

	//*************************************************************************



	/**************************************************************************
	*						LIB TYPE SUPPORT TRAITS
	**************************************************************************/

	template<typename T>
	using symbol_type_t = typename T::symbol_type;

	template<typename T>
	using string_type_t = typename T::string_type;

	template<typename T>
	using string_wrapper_t = typename T::string_wrapper;

	template<typename T>
	using attribute_type_t = typename T::attribute_type;

	template<typename T>
	using node_type_t = typename T::node_type;

	template<typename T>
	using tree_node_t = typename T::tree_node_t;

	template<typename T>
	using size_type_t = typename T::size_type;

	template<typename T>
	using Ptr_t = typename T::Ptr;

	template<typename T>
	using attr_iterator_t = typename T::attr_iterator;

	template<typename T>
	using attr_const_iterator_t = typename T::attr_const_iterator;

	template<typename T>
	using iterator_t = typename T::iterator;

	template<typename T>
	using const_iterator_t = typename T::const_iterator;

	template<typename T>
	using node_pointer_t = typename T::node_pointer;

	template<typename T>
	using document_type_t = typename T::document_type;

	//#########################################################################

	template<typename, typename = void>
	struct is_has_symbol_type : std::false_type {};

	//-------------------------------------------------------------------------

	template<typename T>
	struct is_has_symbol_type<T, std::void_t<symbol_type_t<T>>
	> : std::true_type {};

	//-------------------------------------------------------------------------

	template<typename T>
	inline constexpr bool is_has_symbol_type_v = is_has_symbol_type<T>::value;

	//#########################################################################

	template<typename, typename = void>
	struct is_has_string_type : std::false_type {};

	//-------------------------------------------------------------------------

	template<typename T>
	struct is_has_string_type<T, std::void_t<string_type_t<T>>
	> : std::true_type {};

	//-------------------------------------------------------------------------

	template<typename T>
	inline constexpr bool is_has_string_type_v = is_has_string_type<T>::value;

	//#########################################################################

	template<typename, typename = void>
	struct is_has_string_wrapper : std::false_type {};

	//-------------------------------------------------------------------------

	template<typename T>
	struct is_has_string_wrapper<T, std::void_t<string_wrapper_t<T>>
	> : std::true_type {};

	//-------------------------------------------------------------------------

	template<typename T>
	inline constexpr bool is_has_string_wrapper_v = 
		is_has_string_wrapper<T>::value;

	//#########################################################################

	template<typename, typename = void>
	struct is_has_attribute_type : std::false_type {};

	//-------------------------------------------------------------------------

	template<typename T>
	struct is_has_attribute_type<T, std::void_t<attribute_type_t<T>>
	> : std::true_type {};

	//-------------------------------------------------------------------------

	template<typename T>
	inline constexpr bool is_has_attribute_type_v = 
		is_has_attribute_type<T>::value;

	//#########################################################################

	template<typename, typename = void>
	struct is_has_node_type : std::false_type {};

	//-------------------------------------------------------------------------

	template<typename T>
	struct is_has_node_type<T, std::void_t<node_type_t<T>>
	> : std::true_type {};

	//-------------------------------------------------------------------------

	template<typename T>
	inline constexpr bool is_has_node_type_v = is_has_node_type<T>::value;

	//#########################################################################

	template<typename, typename = void>
	struct is_has_tree_node_type : std::false_type {};

	//-------------------------------------------------------------------------

	template<typename T>
	struct is_has_tree_node_type<T, std::void_t<tree_node_t<T>>
	> : std::true_type {};

	//-------------------------------------------------------------------------

	template<typename T>
	inline constexpr bool is_has_tree_node_type_v = 
		is_has_tree_node_type<T>::value;

	//#########################################################################

	template<typename, typename = void>
	struct is_has_size_type : std::false_type {};

	//-------------------------------------------------------------------------

	template<typename T>
	struct is_has_size_type<T, std::void_t<size_type_t<T>>
	> : std::true_type {};

	//-------------------------------------------------------------------------

	template<typename T>
	inline constexpr bool is_has_size_type_v = is_has_size_type<T>::value;

	//#########################################################################

	template<typename, typename = void>
	struct is_has_Ptr : std::false_type {};

	//-------------------------------------------------------------------------

	template<typename T>
	struct is_has_Ptr<T, std::void_t<Ptr_t<T>>> : std::true_type {};

	//-------------------------------------------------------------------------

	template<typename T>
	inline constexpr bool is_has_Ptr_v = is_has_Ptr<T>::value;

	//#########################################################################

	template<typename, typename = void>
	struct is_has_attr_iterator : std::false_type {};

	//-------------------------------------------------------------------------

	template<typename T>
	struct is_has_attr_iterator<T, std::void_t<attr_iterator_t<T>>
	> : std::true_type {};

	//-------------------------------------------------------------------------

	template<typename T>
	inline constexpr bool is_has_attr_iterator_v = 
		is_has_attr_iterator<T>::value;

	//#########################################################################

	template<typename, typename = void>
	struct is_has_attr_const_iterator : std::false_type {};

	//-------------------------------------------------------------------------

	template<typename T>
	struct is_has_attr_const_iterator<T, std::void_t<attr_const_iterator_t<T>>
	> : std::true_type {};

	//-------------------------------------------------------------------------

	template<typename T>
	inline constexpr bool is_has_attr_const_iterator_v = 
		is_has_attr_const_iterator<T>::value;

	//#########################################################################

	template<typename, typename = void>
	struct is_has_iterator : std::false_type {};

	//-------------------------------------------------------------------------

	template<typename T>
	struct is_has_iterator<T, std::void_t<iterator_t<T>>> : std::true_type {};

	//-------------------------------------------------------------------------

	template<typename T>
	inline constexpr bool is_has_iterator_v = is_has_iterator<T>::value;

	//#########################################################################

	template<typename, typename = void>
	struct is_has_const_iterator : std::false_type {};

	//-------------------------------------------------------------------------

	template<typename T>
	struct is_has_const_iterator<T, std::void_t<const_iterator_t<T>>
	> : std::true_type {};

	//-------------------------------------------------------------------------

	template<typename T>
	inline constexpr bool is_has_const_iterator_v = 
		is_has_const_iterator<T>::value;

	//#########################################################################

	template<typename, typename = void>
	struct is_has_node_pointer : std::false_type {};

	//-------------------------------------------------------------------------

	template<typename T>
	struct is_has_node_pointer<T, std::void_t<node_pointer_t<T>>
	> : std::true_type {};

	//-------------------------------------------------------------------------

	template<typename T>
	inline constexpr bool is_has_node_pointer_v = 
		is_has_node_pointer<T>::value;

	//#########################################################################

	template<typename, typename = void>
	struct is_has_document_type : std::false_type {};

	//-------------------------------------------------------------------------

	template<typename T>
	struct is_has_document_type<T, std::void_t<document_type_t<T>>
	> : std::true_type {};

	//-------------------------------------------------------------------------

	template<typename T>
	inline constexpr bool is_has_document_type_v = 
		is_has_document_type<T>::value;

	//#########################################################################



	/**************************************************************************
	*						ITERATOR SUPPORT TRAITS
	**************************************************************************/

	template<typename T>
	using iterator_category_t =
		typename std::iterator_traits<T>::iterator_category;

	template<typename T>
	using iterator_value_t = typename std::iterator_traits<T>::value_type;

	template<typename T>
	using iterator_reference_t = typename std::iterator_traits<T>::reference;

	template<typename T>
	using iterator_pointer_t = typename std::iterator_traits<T>::pointer;

	template<typename T>
	using iterator_difference_t =
		typename std::iterator_traits<T>::difference_type;

	template<typename T>
	using iterator_char_t = typename T::char_type;

	template<typename T>
	using iterator_container_t = typename T::container_type;

	//#########################################################################

	template<typename, typename = void>
	struct is_has_iterator_category : std::false_type {};

	//-------------------------------------------------------------------------

	template<typename T>
	struct is_has_iterator_category<T,
		std::void_t<iterator_category_t<T>>> : std::true_type {};

	//-------------------------------------------------------------------------

	template<typename T>
	inline constexpr bool is_has_iterator_category_v = 
		is_has_iterator_category<T>::value;

	//#########################################################################

	template<typename, typename = void>
	struct is_has_value_type : std::false_type {};

	//-------------------------------------------------------------------------

	template<typename T>
	struct is_has_value_type<T,
		std::void_t<typename T::value_type>> : std::true_type {};

	//-------------------------------------------------------------------------

	template<typename T>
	inline constexpr bool is_has_value_type_v = is_has_value_type<T>::value;

	//#########################################################################

	template<typename, typename = void>
	struct is_has_char_type : std::false_type {};

	//-------------------------------------------------------------------------

	template<typename T>
	struct is_has_char_type<T,
		std::void_t<typename T::char_type>> : std::true_type {};

	//-------------------------------------------------------------------------

	template<typename T>
	inline constexpr bool is_has_char_type_v = is_has_char_type<T>::value;

	//#########################################################################

	template<typename, typename = void>
	struct is_has_container_type : std::false_type {};

	//-------------------------------------------------------------------------

	template<typename T>
	struct is_has_container_type<T,
		std::void_t<typename T::container_type>> : std::true_type {};

	//-------------------------------------------------------------------------

	template<typename T>
	inline constexpr bool is_has_container_type_v = 
		is_has_container_type<T>::value;

	//#########################################################################



	/**************************************************************************
	*							COMMON SUPPORT TRAITS
	**************************************************************************/

	template<typename T>
	using value_type_t = typename T::value_type;

	//*************************************************************************

	template<typename T>
	using dereferenced_t = decltype(*std::declval<T>());

	//*************************************************************************

	template<typename T>
	using pre_increment_t = decltype(++std::declval<T>());

	//*************************************************************************

	template<typename T>
	using pre_decrement_t = decltype(--std::declval<T>());

	/**************************************************************************
	* @brief Шаблон, который "зарывается" внутрь объектов, контейнеров и их 
	* value_type. Конеыный тип будет простым арифметическим типом.
	**************************************************************************/
	template<typename, typename = void>
	struct universal_value_type;

	//-------------------------------------------------------------------------

	template<typename T>
	struct universal_value_type<T, 
		std::enable_if_t<std::is_arithmetic_v<T>, void>>
	{
		using type = T;
	};

	//-------------------------------------------------------------------------

	template<typename T>
	struct universal_value_type<T,
		std::enable_if_t<is_has_container_type_v<T>, void>>
	{
		using type = 
			typename universal_value_type<iterator_container_t<T>>::type;
	};

	//-------------------------------------------------------------------------

	template<typename T>
	struct universal_value_type<T, 
		std::enable_if_t<!is_has_container_type_v<T> &&

		is_has_value_type_v<T>, void>>
	{
		using type = typename T::value_type;
	};

	//-------------------------------------------------------------------------

	template<typename T>
	struct universal_value_type<T*>
	{
		using type = typename universal_value_type<T>::type;
	};

	//-------------------------------------------------------------------------

	template<typename T>
	struct universal_value_type<T&>
	{
		using type = typename universal_value_type<T>::type;
	};

	//-------------------------------------------------------------------------

	template<typename T>
	struct universal_value_type<T&&>
	{
		using type = typename universal_value_type<T>::type;
	};

	//-------------------------------------------------------------------------

	template<typename T>
	using universal_value_type_t = typename universal_value_type<T>::type;

	//#########################################################################

}} // namespace XMLB::detail

#endif // !XMLB_TYPE_TRAITS_H