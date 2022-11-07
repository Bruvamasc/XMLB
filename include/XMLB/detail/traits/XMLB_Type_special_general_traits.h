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



#ifndef XMLB_TYPE_SPECIAL_GENERAL_TRAITS_H
#define XMLB_TYPE_SPECIAL_GENERAL_TRAITS_H

#include "XMLB/detail/traits/XMLB_Type_traits.h"
#include "XMLB/detail/traits/XMLB_Type_methods_traits.h"
#include "XMLB/detail/traits/XMLB_Type_converting_traits.h"
#include "XMLB/detail/traits/XMLB_Type_callable_traits.h"



namespace XMLB { namespace detail {

	//*************************************************************************
	//						COMMON SUPPORT TRAITS
	//*************************************************************************

	template<typename ClassT, typename ParamT>
	using self_return_assign_signature = 
		decltype(static_cast<
			ClassT& (ClassT::*)(const ParamT&)>(&ClassT::operator=));

	//*************************************************************************

	template<typename IterT, typename ParamT>
	using first_argument_in_iterator_assign_type_function_t = 
		function_argument_t<0, 
		function_arguments_t<self_return_assign_signature<IterT, ParamT>>>;

	//#########################################################################

	template<typename, typename = void>
	struct is_back_inserter_iterator : std::false_type {};

	//-------------------------------------------------------------------------

	template<typename IterT>
	struct is_back_inserter_iterator<IterT, 
		std::void_t<iterator_category_t<IterT>, iterator_container_t<IterT>>
	> : std::true_type {};

	//-------------------------------------------------------------------------

	template<typename IterT>
	inline constexpr bool is_back_inserter_iterator_v = 
		is_back_inserter_iterator<IterT>::value;

	//#########################################################################

	template<typename, typename = void>
	struct is_output_iterator : std::false_type {};

	//-------------------------------------------------------------------------

	template<typename IterT>
	struct is_output_iterator<IterT, 
		std::enable_if_t<!is_has_container_type_v<IterT> &&

		std::is_same_v<iterator_category_t<IterT>, std::output_iterator_tag>, 
		void>> : std::true_type {};

	//-------------------------------------------------------------------------

	template<typename IterT>
	inline constexpr bool is_output_iterator_v = 
		is_output_iterator<IterT>::value;

	//#########################################################################

	template<typename, typename = void>
	struct is_iterator_or_pointer : std::false_type {};

	//-------------------------------------------------------------------------

	template<typename IterT>
	struct is_iterator_or_pointer<IterT, 
		std::enable_if_t<!is_has_container_type_v<IterT> && 

		!is_has_char_type_v<IterT> &&

		(is_has_iterator_category_v<IterT> || std::is_pointer_v<IterT>), void>
	> : std::true_type {};

	//-------------------------------------------------------------------------

	template<typename IterT>
	inline constexpr bool is_iterator_or_pointer_v = 
		is_iterator_or_pointer<IterT>::value;

	//#########################################################################



	//*************************************************************************
	//						TYPE TRAITS FOR DECORATOR
	//*************************************************************************

	template<typename, typename = void>
	struct type_for_decorator;

	//-------------------------------------------------------------------------

	template<typename T>
	struct type_for_decorator<T, 
		std::enable_if_t<std::is_class_v<T> && 

		is_has_begin_and_end_v<T>, void>>
	{
		using type = typename type_for_decorator<
			universal_value_type_t<const_iterator_t<T>>>::type;
	};

	//-------------------------------------------------------------------------

	template<typename T>
	struct type_for_decorator<T, 
		std::enable_if_t<is_has_iterator_category_v<T> &&

		is_has_value_type_v<T>, void>>
	{
		using type = 
			typename type_for_decorator<universal_value_type_t<T>>::type;
	};

	//-------------------------------------------------------------------------

	template<typename T>
	struct type_for_decorator<T, 
		std::enable_if_t<std::is_pointer_v<T> &&

		std::is_class_v<universal_value_type_t<T>>, void>>
	{
		using type = typename type_for_decorator<
			std::decay_t<universal_value_type_t<T>>>::type;
	};

	//-------------------------------------------------------------------------

	template<typename T>
	struct type_for_decorator<T, 
		std::enable_if_t<std::is_pointer_v<T> &&

		std::is_arithmetic_v<universal_value_type_t<T>>, void>>
	{
		using type = std::decay_t<universal_value_type_t<T>>;
	};

	//-------------------------------------------------------------------------

	template<typename T>
	struct type_for_decorator<T,
		std::enable_if_t<std::is_arithmetic_v<T>, void>>
	{
		using type = T;
	};

	//-------------------------------------------------------------------------

	template<typename T>
	using type_for_decorator_t = typename type_for_decorator<T>::type;

	//#########################################################################



	//*************************************************************************
	//						TYPE TRAITS FOR SAVE FUNCTIONS
	//*************************************************************************

	template<typename, typename, typename = void>
	struct is_back_inserter_iterator_to_symbol : std::false_type {};

	//-------------------------------------------------------------------------

	template<typename IterT, typename SymbolT>
	struct is_back_inserter_iterator_to_symbol<IterT, SymbolT,
		std::enable_if_t<is_back_inserter_iterator_v<IterT> &&

		std::is_same_v<value_type_t<iterator_container_t<IterT>>, SymbolT> &&

		std::is_assignable_v<dereferenced_t<IterT>, SymbolT>, void>
	
	> : std::true_type{};

	//-------------------------------------------------------------------------

	template<typename IterT, typename SymbolT>
	inline constexpr bool is_back_inserter_iterator_to_symbol_v = 
		is_back_inserter_iterator_to_symbol<IterT, SymbolT>::value;

	//#########################################################################

	template<typename, typename, typename = void>
	struct is_back_inserter_iterator_to_container : std::false_type {};

	//-------------------------------------------------------------------------

	template<typename IterT, typename ContainerT>
	struct is_back_inserter_iterator_to_container<IterT, ContainerT,
		std::enable_if_t<is_back_inserter_iterator_v<IterT> &&

		std::is_same_v<
		value_type_t<iterator_container_t<IterT>>, ContainerT> &&

		std::is_assignable_v<dereferenced_t<IterT>, ContainerT>, void>

	> : std::true_type{};

	//-------------------------------------------------------------------------

	template<typename IterT, typename ContainerT>
	inline constexpr bool is_back_inserter_iterator_to_container_v = 
		is_back_inserter_iterator_to_container<IterT, ContainerT>::value;

	//#########################################################################

	template<typename, typename, typename = void>
	struct is_output_iterator_to_symbol : std::false_type {};

	//-------------------------------------------------------------------------

	template<typename IterT, typename SymbolT>
	struct is_output_iterator_to_symbol<IterT, SymbolT,
		std::enable_if_t<is_output_iterator_v<IterT> &&

		std::is_same_v<std::decay_t<
		first_argument_in_iterator_assign_type_function_t<
		IterT, SymbolT>>, SymbolT> &&

		std::is_convertible_v<SymbolT, 
		first_argument_in_iterator_assign_type_function_t<IterT, SymbolT>>,
		void>> : std::true_type{};

	//-------------------------------------------------------------------------

	template<typename IterT, typename SymbolT>
	inline constexpr bool is_output_iterator_to_symbol_v = 
		is_output_iterator_to_symbol<IterT, SymbolT>::value;

	//#########################################################################

	template<typename, typename, typename = void>
	struct is_output_iterator_to_container : std::false_type {};

	//-------------------------------------------------------------------------

	template<typename IterT, typename ContainerT>
	struct is_output_iterator_to_container<IterT, ContainerT,
		std::enable_if_t<is_output_iterator_v<IterT> &&

		std::is_same_v<std::decay_t<
		first_argument_in_iterator_assign_type_function_t<
		IterT, ContainerT>>, ContainerT> &&

		std::is_convertible_v<ContainerT, 
		first_argument_in_iterator_assign_type_function_t<IterT, ContainerT>>,
		void>> : std::true_type{};

	//-------------------------------------------------------------------------

	template<typename IterT, typename ContainerT>
	inline constexpr bool is_output_iterator_to_container_v = 
		is_output_iterator_to_container<IterT, ContainerT>::value;

	//#########################################################################

	template<typename, typename, typename = void>
	struct is_input_derived_iterator_or_pointer_to_symbol : std::false_type {};

	//-------------------------------------------------------------------------

	template<typename IterT, typename SymbolT>
	struct is_input_derived_iterator_or_pointer_to_symbol<IterT, SymbolT,
		std::enable_if_t<is_iterator_or_pointer_v<IterT> &&

		std::is_same_v<
		std::remove_reference_t<dereferenced_t<IterT>>, SymbolT> &&

		std::is_assignable_v<dereferenced_t<IterT>, SymbolT>, void>

	> : std::true_type {};

	//-------------------------------------------------------------------------

	template<typename IterT, typename SymbolT>
	inline constexpr bool is_input_derived_iterator_or_pointer_to_symbol_v =
		is_input_derived_iterator_or_pointer_to_symbol<IterT, SymbolT>::value;

	//#########################################################################

	template<typename, typename, typename = void>
	struct is_input_derived_iterator_or_pointer_to_container : 
		std::false_type {};

	//-------------------------------------------------------------------------

	template<typename IterT, typename ContainerT>
	struct is_input_derived_iterator_or_pointer_to_container<IterT, ContainerT,
		std::enable_if_t<is_iterator_or_pointer_v<IterT> &&

		std::is_same_v<
		std::remove_reference_t<dereferenced_t<IterT>>, ContainerT> &&

		std::is_assignable_v<dereferenced_t<IterT>, ContainerT>, void>

	> : std::true_type {};

	//-------------------------------------------------------------------------

	template<typename IterT, typename ContainerT>
	inline constexpr bool is_input_derived_iterator_or_pointer_to_container_v =
		is_input_derived_iterator_or_pointer_to_container<
		IterT, ContainerT>::value;

	//#########################################################################



	//*************************************************************************
	//						TYPE TRAITS FOR LOAD FUNCTIONS
	//*************************************************************************

	template<typename, typename = void>
	struct is_iterator_and_char : std::false_type {};

	//-------------------------------------------------------------------------

	template<typename IterT>
	struct is_iterator_and_char<IterT,
		std::enable_if_t<is_has_dereferencing_v<IterT> &&

		is_has_pre_increment_v<IterT> &&

		std::is_assignable_v<dereferenced_t<IterT>,
		std::decay_t<universal_value_type_t<IterT>>> &&

		is_has_value_type_v<IterT>&&

		std::is_arithmetic_v<
		std::decay_t<universal_value_type_t<IterT>>>, void>

	> : std::true_type{};

	//-------------------------------------------------------------------------

	template<typename IterT>
	struct is_iterator_and_char<IterT,
		std::enable_if_t<is_has_dereferencing_v<IterT> &&

		is_has_pre_increment_v<IterT> &&

		std::is_assignable_v<IterT, iterator_char_t<IterT>> &&

		is_has_char_type_v<IterT> &&

		std::is_same_v<iterator_category_t<IterT>,
		std::output_iterator_tag> &&

		std::is_arithmetic_v<iterator_char_t<IterT>>, void>

	> : std::true_type{};

	//-------------------------------------------------------------------------

	template<typename IterT>
	struct is_iterator_and_char<IterT,
		std::enable_if_t<std::is_pointer_v<IterT> &&

		std::is_arithmetic_v<std::decay_t<universal_value_type_t<IterT>>>,
		void>> : std::true_type {};

	//-------------------------------------------------------------------------

	template<typename IterT>
	inline constexpr bool is_iterator_and_char_v = 
		is_iterator_and_char<IterT>::value;

	//#########################################################################

	template<typename, typename = void>
	struct is_iterator_and_container : std::false_type {};

	//-------------------------------------------------------------------------

	template<typename IterT>
	struct is_iterator_and_container<IterT,
		std::enable_if_t<is_has_dereferencing_v<IterT> &&

		is_has_pre_increment_v<IterT> &&

		std::is_assignable_v<dereferenced_t<IterT>,
		std::decay_t<universal_value_type_t<IterT>>> &&

		is_has_value_type_v<IterT> &&

		(is_std_string_v<universal_value_type_t<IterT>> ||
			is_constuctible_from_std_string_v<universal_value_type_t<IterT>>), 
		void>> : std::true_type{};

	//-------------------------------------------------------------------------

	/*template<typename IterT>
	struct is_iterator_and_container<IterT,
		std::enable_if_t<is_has_dereferencing_v<IterT> &&

		is_has_pre_increment_v<IterT> &&

		std::is_same_v<iterator_category_t<IterT>,
		std::output_iterator_tag>, void>

	> : std::true_type{};*/

	//-------------------------------------------------------------------------

	template<typename IterT>
	struct is_iterator_and_container<IterT,
		std::enable_if_t<std::is_pointer_v<IterT> &&

		(is_std_string_v<universal_value_type_t<IterT>> ||
			is_constuctible_from_std_string_v<universal_value_type_t<IterT>>), 
		void>> : std::true_type {};

	//-------------------------------------------------------------------------

	template<typename IterT>
	inline constexpr bool is_iterator_and_container_v = 
		is_iterator_and_container<IterT>::value;

	//#########################################################################

	template<typename, typename, typename, typename = void>
	struct is_input_iterator_and_container : std::false_type {};

	//-------------------------------------------------------------------------

	template<typename IterT, typename DecorT, typename ParserT>
	struct is_input_iterator_and_container<IterT, DecorT, ParserT,
		std::enable_if_t<
		
		std::is_same_v<std::decay_t<type_for_decorator_t<IterT>>,
		symbol_type_t<DecorT>> &&

		std::is_same_v<std::decay_t<
		type_for_decorator_t<IterT>>, symbol_type_t<ParserT>> &&

		std::is_same_v<std::input_iterator_tag,
		iterator_category_t<IterT>>&&

		is_has_begin_and_end_v<std::decay_t<universal_value_type_t<IterT>>> &&

		std::is_same_v<universal_value_type_t<
		const_iterator_t<universal_value_type_t<IterT>>>,
		symbol_type_t<DecorT>>, 
		
		void>> : std::true_type{};

	//-------------------------------------------------------------------------

	template<typename IterT, typename DecorT, typename ParserT>
	inline constexpr bool is_input_iterator_and_container_v =
		is_input_iterator_and_container<IterT, DecorT, ParserT>::value;

	//#########################################################################

	template<typename, typename, typename, typename = void>
	struct is_input_iterator_and_char : std::false_type {};

	//-------------------------------------------------------------------------

	template<typename IterT, typename DecorT, typename ParserT>
	struct is_input_iterator_and_char<IterT, DecorT, ParserT,
		std::enable_if_t<
		
		std::is_same_v<std::decay_t<type_for_decorator_t<IterT>>,
		symbol_type_t<DecorT>> &&

		std::is_same_v<std::decay_t<
		type_for_decorator_t<IterT>>, symbol_type_t<ParserT>> &&

		std::is_same_v<std::input_iterator_tag,
		iterator_category_t<IterT>> &&

		std::is_arithmetic_v<std::decay_t<universal_value_type_t<IterT>>>, 
		
		void>> : std::true_type{};

	//-------------------------------------------------------------------------

	template<typename IterT, typename DecorT, typename ParserT>
	inline constexpr bool is_input_iterator_and_char_v =
		is_input_iterator_and_char<IterT, DecorT, ParserT>::value;

	//#########################################################################

	template<typename, typename, typename, typename = void>
	struct is_forward_iterator_or_pointer_and_container :
		std::false_type {};

	//-------------------------------------------------------------------------

	template<typename IterT, typename DecorT, typename ParserT>
	struct is_forward_iterator_or_pointer_and_container<IterT,
		DecorT,
		ParserT,

		std::enable_if_t<
		
		std::is_same_v<std::decay_t<type_for_decorator_t<IterT>>,
		symbol_type_t<DecorT>> &&

		std::is_same_v<std::decay_t<type_for_decorator_t<IterT>>,
		symbol_type_t<ParserT>> &&

		(std::is_base_of_v<std::forward_iterator_tag,
			iterator_category_t<IterT>> || std::is_pointer_v<IterT>) &&

		is_has_begin_and_end_v<std::decay_t<universal_value_type_t<IterT>>> &&

		std::is_same_v<universal_value_type_t<
		const_iterator_t<universal_value_type_t<IterT>>>,
		symbol_type_t<DecorT>>, 
		
		void>> : std::true_type{};

	//-------------------------------------------------------------------------

	template<typename IterT, typename DecorT, typename ParserT>
	inline constexpr bool is_forward_iterator_or_pointer_and_container_v =
		is_forward_iterator_or_pointer_and_container<
		IterT, DecorT, ParserT>::value;

	//#########################################################################

	template<typename, typename, typename, typename = void>
	struct is_forward_iterator_or_pointer_and_char : std::false_type {};

	//-------------------------------------------------------------------------

	template<typename IterT, typename DecorT, typename ParserT>
	struct is_forward_iterator_or_pointer_and_char<IterT, DecorT, ParserT,
		std::enable_if_t<

		std::is_same_v<std::decay_t<type_for_decorator_t<IterT>>,
		symbol_type_t<DecorT>> &&

		std::is_same_v<std::decay_t<type_for_decorator_t<IterT>>,
		symbol_type_t<ParserT>> &&

		(std::is_base_of_v<std::forward_iterator_tag,
			iterator_category_t<IterT>> || std::is_pointer_v<IterT>) &&

		std::is_arithmetic_v<std::decay_t<universal_value_type_t<IterT>>>, 
		
		void>> : std::true_type{};

	//-------------------------------------------------------------------------

	template<typename IterT, typename DecorT, typename ParserT>
	inline constexpr bool is_forward_iterator_or_pointer_and_char_v = 
		is_forward_iterator_or_pointer_and_char<IterT, DecorT, ParserT>::value;

	//#########################################################################

}} // namespace XMLB::detail

#endif // !XMLB_TYPE_SPECIAL_GENERAL_TRAITS_H