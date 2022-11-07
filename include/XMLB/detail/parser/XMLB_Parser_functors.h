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



#ifndef XMLB_PARSER_FUNCTORS_H
#define XMLB_PARSER_FUNCTORS_H

#include <algorithm>

#include "XMLB/detail/parser/XMLB_Parser_states.h"
#include "XMLB/detail/utilities/XMLB_sup_functions.h"



namespace XMLB { namespace detail {

	/**************************************************************************
	* @brief Функтор для символа открытия XML тега
	* 
	* @ingroup secondary
	**************************************************************************/
	struct Open_tag_functor final
	{
		/**********************************************************************
		* @tparam CharT - тип входящих символов символов
		* @tparam DecorT - тип декоратора
		* @tparam DataT - тип контроллера данных
		*
		* @param token - символ
		* @param data - контроллер данных
		* @param decorator - декоратор
		* 
		* @return состояние парсера в зависимости от текущего символа
		*
		* @todo Нужно подумать над оптимизацией
		**********************************************************************/
		template<typename CharT, typename DecorT, typename DataT>
		Base_state operator()(CharT&& token, DataT& data,
			const DecorT& decorator) const
		{
			if (token == decorator.last_tag_symbol)
			{
				return Last_tag_state{};
			}
			else if (token == decorator.comment_symbol)
			{
				return Comment_state{};
			}
			else if (token == decorator.doc_info_symbol)
			{
				return Document_info_state{};
			}
			else
			{
				if (data.is_correct_tag_name() &&
					data.is_correct_tag_attributes())
				{
					data.add_tag_as_parent();
				}

				data.clear_all_buffers();

				data.push_to_tag_name(std::forward<CharT>(token));

				return Name_tag_state{};
			}
		}
	};

	//*************************************************************************



	/**************************************************************************
	* @brief Функтор для символа закрытия XML тега
	* 
	* @ingroup secondary
	**************************************************************************/
	struct Close_tag_functor final
	{
		/**********************************************************************
		* @tparam CharT - тип входящих символов символов
		* @tparam DecorT - тип декоратора
		* @tparam DataT - тип контроллера данных
		*
		* @param token - символ
		* @param data - контроллер данных
		* @param decorator - декоратор
		*
		* @return состояние парсера в зависимости от текущего символа
		*
		* @todo Нужно подумать над оптимизацией
		**********************************************************************/
		template<typename CharT, typename DecorT, typename DataT>
		Base_state operator()(CharT&& token, DataT& data,
			const DecorT& decorator) const
		{
			if (token == decorator.open_tag_symbol)
			{
				return Open_tag_state{};
			}
			else
			{
				if (token != decorator.line_break_symbol &&
					token != decorator.white_space_symbol &&
					token != decorator.fill_symbol &&
					token != decorator.tab_symbol &&
					token != decorator.carriage_symbol)
				{
					data.push_to_tag_value(std::forward<CharT>(token));
				}

				return Value_tag_state{};
			}
		}
	};

	//*************************************************************************



	/**************************************************************************
	* @brief Функтор для символа обозначающего последний из пары XML тег
	* 
	* @ingroup secondary
	**************************************************************************/
	struct Last_tag_functor final
	{
		/**********************************************************************
		* @tparam CharT - тип входящих символов символов
		* @tparam DecorT - тип декоратора
		* @tparam DataT - тип контроллера данных
		*
		* @param token - символ
		* @param data - контроллер данных
		* @param decorator - декоратор
		*
		* @return состояние парсера в зависимости от текущего символа
		*
		* @todo Нужно подумать над оптимизацией
		**********************************************************************/
		template<typename CharT, typename DecorT, typename DataT>
		Base_state operator()(CharT&& token, DataT& data,
			const DecorT& decorator) const
		{
			if (token != decorator.white_space_symbol &&
				token != decorator.line_break_symbol &&
				token != decorator.carriage_symbol &&
				token != decorator.tab_symbol &&
				token != decorator.close_tag_symbol)
			{
				data.push_to_tag_last_name(std::forward<CharT>(token));
			}
			else if (token == decorator.close_tag_symbol)
			{
				if (data.is_correct_tag_name() &&
					data.is_correct_tag_attributes())
				{
					data.add_tag_as_child();
				}
				else if (data.is_tag_name_equal_parent_name() &&
					data.is_correct_tag_attributes())
				{
					data.pop_to_parent();
				}
				else
				{
					return Error_state{};
				}

				data.clear_all_buffers();

				return Close_tag_state{};
			}

			return Last_tag_state{};
		}
	};

	//*************************************************************************



	/**************************************************************************
	* @brief Функтор для символа одиночного XML тега
	* 
	* @ingroup secondary
	**************************************************************************/
	struct Single_tag_functor final
	{
		/**********************************************************************
		* @tparam CharT - тип входящих символов символов
		* @tparam DecorT - тип декоратора
		* @tparam DataT - тип контроллера данных
		*
		* @param token - символ
		* @param data - контроллер данных
		* @param decorator - декоратор
		*
		* @return состояние парсера в зависимости от текущего символа
		*
		* @todo Нужно подумать над оптимизацией
		**********************************************************************/
		template<typename CharT, typename DecorT, typename DataT>
		Base_state operator()(CharT&& token, DataT& data,
			const DecorT& decorator) const
		{
			if (token == decorator.close_tag_symbol &&
				data.is_correct_tag_name() &&
				data.is_correct_tag_attributes())
			{
				data.add_tag_as_child();

				data.clear_all_buffers();

				return Start_state{};
			}
			else
			{
				return Error_state{};
			}
		}
	};

	//*************************************************************************



	/**************************************************************************
	* @brief Функтор для обработки имени XML тега
	* 
	* @ingroup secondary
	**************************************************************************/
	struct Name_tag_functor final
	{
		/**********************************************************************
		* @tparam CharT - тип входящих символов символов
		* @tparam DecorT - тип декоратора
		* @tparam DataT - тип контроллера данных
		*
		* @param token - символ
		* @param data - контроллер данных
		* @param decorator - декоратор
		*
		* @return состояние парсера в зависимости от текущего символа
		*
		* @todo Нужно подумать над оптимизацией
		**********************************************************************/
		template<typename CharT, typename DecorT, typename DataT>
		Base_state operator()(CharT&& token, DataT& data,
			const DecorT& decorator) const
		{
			if (token == decorator.white_space_symbol)
			{
				return Attribute_name_state{};
			}
			else if (token == decorator.close_tag_symbol)
			{
				return Close_tag_state{};
			}
			else if (token == decorator.single_tag_symbol)
			{
				return Single_tag_state{};
			}
			else if (token == decorator.line_break_symbol ||
				token == decorator.fill_symbol ||
				token == decorator.tab_symbol ||
				token == decorator.open_tag_symbol ||
				token == decorator.carriage_symbol)
			{
				return Error_state{};
			}
			else
			{
				data.push_to_tag_name(std::forward<CharT>(token));

				return Name_tag_state{};
			}
		}
	};

	//*************************************************************************



	/**************************************************************************
	* @brief Функтор для обработки значения XML тега
	* 
	* @ingroup secondary
	**************************************************************************/
	struct Value_tag_functor final
	{
		/**********************************************************************
		* @tparam CharT - тип входящих символов символов
		* @tparam DecorT - тип декоратора
		* @tparam DataT - тип контроллера данных
		*
		* @param token - символ
		* @param data - контроллер данных
		* @param decorator - декоратор
		*
		* @return состояние парсера в зависимости от текущего символа
		*
		* @todo Нужно подумать над оптимизацией
		**********************************************************************/
		template<typename CharT, typename DecorT, typename DataT>
		Base_state operator()(CharT&& token, DataT& data,
			const DecorT& decorator) const
		{
			if (token != decorator.white_space_symbol &&
				token != decorator.line_break_symbol &&
				token != decorator.tab_symbol &&
				token != decorator.fill_symbol &&
				token != decorator.open_tag_symbol &&
				token != decorator.carriage_symbol)
			{
				data.push_to_tag_value(std::forward<CharT>(token));
			}
			else if (token == decorator.white_space_symbol)
			{
				auto&& tag_value = data.get_tag_value();

				if (tag_value.size() &&
					tag_value[0] != decorator.white_space_symbol)
				{
					data.push_to_tag_value(std::forward<CharT>(token));
				}
			}
			else if (token == decorator.open_tag_symbol)
			{
				auto&& tag_value = data.get_tag_value();

				if (tag_value.size())
				{
					auto found_last_symbol = tag_value.find_last_not_of(
						decorator.white_space_symbol);

					if (found_last_symbol != tag_value.npos)
					{
						auto size = tag_value.size();

						while (found_last_symbol < size - 1)
						{
							data.pop_from_tag_value();

							++found_last_symbol;
						}
					}
				}

				return Open_tag_state{};
			}

			return Value_tag_state{};
		}
	};

	//*************************************************************************



	/**************************************************************************
	* @brief Функтор для обработки имени атрибута XML тега
	* 
	* @ingroup secondary
	**************************************************************************/
	struct Attribute_name_functor final
	{
		/**********************************************************************
		* @tparam CharT - тип входящих символов символов
		* @tparam DecorT - тип декоратора
		* @tparam DataT - тип контроллера данных
		*
		* @param token - символ
		* @param data - контроллер данных
		* @param decorator - декоратор
		*
		* @return состояние парсера в зависимости от текущего символа
		*
		* @todo Нужно подумать над оптимизацией
		**********************************************************************/
		template<typename CharT, typename DecorT, typename DataT>
		Base_state operator()(CharT&& token, DataT& data,
			const DecorT& decorator) const
		{
			if (token == decorator.open_attribute_symbol)
			{
				return Attribute_value_state{};
			}
			else if (token == decorator.single_tag_symbol)
			{
				return Single_tag_state{};
			}
			else if (token == decorator.close_tag_symbol)
			{
				return Close_tag_state{};
			}
			else if (token == decorator.doc_info_last_symbol)
			{
				return Document_info_state{};
			}
			else if (token != decorator.white_space_symbol &&
				token != decorator.equal_attribute_symbol)
			{
				data.push_to_attribute_name(std::forward<CharT>(token));
			}

			return Attribute_name_state{};
		}
	};

	//*************************************************************************



	/**************************************************************************
	* @brief Функтор для обработки значения атрибута XML тега
	* 
	* @ingroup secondary
	**************************************************************************/
	struct Attribute_value_functor final
	{
		/**********************************************************************
		* @tparam CharT - тип входящих символов символов
		* @tparam DecorT - тип декоратора
		* @tparam DataT - тип контроллера данных
		*
		* @param token - символ
		* @param data - контроллер данных
		* @param decorator - декоратор
		*
		* @return состояние парсера в зависимости от текущего символа
		*
		* @todo Нужно подумать над оптимизацией
		**********************************************************************/
		template<typename CharT, typename DecorT, typename DataT>
		Base_state operator()(CharT&& token, DataT& data,
			const DecorT& decorator) const
		{
			if (token == decorator.close_attribute_symbol)
			{
				data.add_attribute();

				data.clear_attribute_name();
				data.clear_attribute_value();

				return Attribute_name_state{};
			}
			else if (token == decorator.close_tag_symbol)
			{
				data.add_attribute();

				data.clear_attribute_name();
				data.clear_attribute_value();

				return Close_tag_state{};
			}
			else if (token == decorator.line_break_symbol ||
				token == decorator.tab_symbol ||
				token == decorator.fill_symbol ||
				token == decorator.carriage_symbol)
			{
				return Error_state{};
			}
			else
			{
				data.push_to_attribute_value(std::forward<CharT>(token));

				return Attribute_value_state{};
			}
		}
	};

	//*************************************************************************



	/**************************************************************************
	* @brief Функтор для символа начала XML комментария
	* 
	* @ingroup secondary
	**************************************************************************/
	struct Comment_functor final
	{
		/**********************************************************************
		* @tparam CharT - тип входящих символов символов
		* @tparam DecorT - тип декоратора
		* @tparam DataT - тип контроллера данных
		*
		* @param token - символ
		* @param data - контроллер данных
		* @param decorator - декоратор
		*
		* @return состояние парсера в зависимости от текущего символа
		*
		* @todo Нужно подумать над оптимизацией
		**********************************************************************/
		template<typename CharT, typename DecorT, typename DataT>
		Base_state operator()(CharT&& token, DataT&& data,
			const DecorT& decorator) const
		{
			using std::begin;
			using std::end;

			if (token == decorator.close_tag_symbol)
			{
				unsigned int comment_signature_size = 4;

				auto&& comment = data.get_comment();

				bool is_all_equal = std::all_of(begin(comment), end(comment),
					[&decorator](const auto& symbol)
					{
						return symbol == decorator.comment_last_symbol;
					});

				if (comment.size() >= comment_signature_size &&
					is_all_equal)
				{
					data.clear_comment();

					return Close_tag_state{};
				}
			}
			else if (token == decorator.comment_last_symbol)
			{
				data.push_to_comment(std::forward<CharT>(token));
			}

			return Comment_state{};
		}
	};

	//*************************************************************************



	/**************************************************************************
	* @brief Функтор отвечающий за начало синтаксического анализа XML документа
	* 
	* @ingroup secondary
	**************************************************************************/
	struct Start_functor final
	{
		/**********************************************************************
		* @tparam CharT - тип входящих символов символов
		* @tparam DecorT - тип декоратора
		* @tparam DataT - тип контроллера данных
		*
		* @param token - символ
		* @param data - контроллер данных
		* @param decorator - декоратор
		*
		* @return состояние парсера в зависимости от текущего символа
		*
		* @todo Нужно подумать над оптимизацией
		**********************************************************************/
		template<typename CharT, typename DecorT, typename DataT>
		Base_state operator()(CharT&& token, DataT&& data,
			const DecorT& decorator) const
		{
			if (token == decorator.open_tag_symbol)
			{
				return Open_tag_state{};
			}

			return Start_state{};
		}
	};

	//*************************************************************************



	/**************************************************************************
	* @brief Функтор для символа начала информации о XML документе
	* 
	* @ingroup secondary
	**************************************************************************/
	struct Document_info_functor final
	{
		/**********************************************************************
		* @tparam CharT - тип входящих символов символов
		* @tparam DecorT - тип декоратора
		* @tparam DataT - тип контроллера данных
		*
		* @param token - символ
		* @param data - контроллер данных
		* @param decorator - декоратор
		*
		* @return состояние парсера в зависимости от текущего символа
		*
		* @todo Нужно подумать над оптимизацией и убрать constexpr
		**********************************************************************/
		template<typename CharT, typename DecorT, typename DataT>
		Base_state operator()(CharT&& token, DataT&& data,
			const DecorT& decorator) const
		{
			using symbol_type = std::decay_t<CharT>;
			using string_type = typename std::decay_t<DataT>::string_type;
			using string_wrapper =
				typename std::decay_t<DataT>::string_wrapper;
			using size_type = typename std::decay_t<DataT>::size_type;

			if (token == decorator.close_tag_symbol)
			{
				if (data.is_correct_tag_name() &&
					data.is_correct_tag_attributes())
				{
					string_wrapper kDoc_info{};
					string_wrapper kDoc_version{};
					string_wrapper kDoc_encode{};

					if constexpr (std::is_same_v<symbol_type, char16_t>)
					{
						kDoc_info = string_wrapper{ u"xml" };
						kDoc_version = string_wrapper{ u"version" };
						kDoc_encode = string_wrapper{ u"encoding" };
					}
					else if constexpr (std::is_same_v<symbol_type, char32_t>)
					{
						kDoc_info = string_wrapper{ U"xml" };
						kDoc_version = string_wrapper{ U"version" };
						kDoc_encode = string_wrapper{ U"encoding" };
					}
					else if constexpr (std::is_same_v<symbol_type, wchar_t>)
					{
						kDoc_info = string_wrapper{ L"xml" };
						kDoc_version = string_wrapper{ L"version" };
						kDoc_encode = string_wrapper{ L"encoding" };
					}
					else
					{
						kDoc_info = string_wrapper{ "xml" };
						kDoc_version = string_wrapper{ "version" };
						kDoc_encode = string_wrapper{ "encoding" };
					}

					auto&& tag_name = data.get_tag_name();
					auto&& tag_value = data.get_tag_value();

					size_type only_doc_version = 1;
					size_type doc_version_and_endoding = 2;

					if (tag_name == kDoc_info && !tag_value.size())
					{
						if (data.get_attribute_names_size() ==
							only_doc_version)
						{
							auto&& first_attribute =
								data.get_attribute_name(0);

							if (first_attribute == kDoc_version)
							{
								data.add_tag_as_doc_info();
							}
						}
						else if (data.get_attribute_names_size() ==
							doc_version_and_endoding)
						{
							auto&& first_attribute =
								data.get_attribute_name(0);

							auto&& second_attribute =
								data.get_attribute_name(1);

							if (first_attribute == kDoc_version &&
								second_attribute == kDoc_encode)
							{
								data.add_tag_as_doc_info();
							}
						}
						else
						{
							data.add_tag_as_doc_info();
						}
					}
				}

				data.clear_all_buffers();

				return Close_tag_state{};
			}
			else if (token == decorator.white_space_symbol ||
				token == decorator.line_break_symbol ||
				token == decorator.tab_symbol ||
				token == decorator.carriage_symbol ||
				token == decorator.comment_symbol ||
				token == decorator.fill_symbol ||
				token == decorator.open_tag_symbol ||
				token == decorator.single_tag_symbol ||
				token == decorator.last_tag_symbol ||
				token == decorator.open_attribute_symbol ||
				token == decorator.close_attribute_symbol ||
				token == decorator.equal_attribute_symbol ||
				token == decorator.comment_last_symbol ||
				token == decorator.doc_info_symbol ||
				token == decorator.doc_info_last_symbol)
			{
				return Error_state{};
			}
			else
			{
				data.push_to_tag_name(std::forward<CharT>(token));
			}

			return Name_tag_state{};
		}
	};

	//*************************************************************************

}} // namespace XMLB::detail

#endif // !XMLB_PARSER_FUNCTORS_H