/******************************************************************************
* @file
* Данный файл объявляет и определяет стандартные классы-функторы для класса 
* Парсера XML документа
*
* @author Bruvamasc
* @date   2022-08-25
*
******************************************************************************/

#ifndef XMLB_PARSER_FUNCTORS_H
#define XMLB_PARSER_FUNCTORS_H

#include <algorithm>

#include "XMLB/detail/parser/XMLB_Parser_states.h"
#include "XMLB/detail/utilities/XMLB_sup_functions.h"

namespace XMLB { namespace detail {

	/**************************************************************************
	* @brief Функтор для символа открытия XML тега
	**************************************************************************/
	struct Open_tag_functor final
	{
		/**********************************************************************
		* @brief Функция функтора для парсинга
		*
		* @param[in] token - символ или ссылка(укзаатель) на символ
		* @param[in] data - контроллер данных
		* @param[in] decorator - объект декоратор с необходимыми символами
		*
		* @return новое состояние парсера в зависимости от текущего символа
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
	* @brief Функтор для закрытия закрытия XML тега
	**************************************************************************/
	struct Close_tag_functor final
	{
		/**********************************************************************
		* @brief Функция функтора для парсинга
		*
		* @param[in] token - символ или ссылка(укзаатель) на символ
		* @param[in] data - контроллер данных
		* @param[in] decorator - объект декоратор с необходимыми символами
		*
		* @return новое состояние парсера в зависимости от текущего символа
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
	* @brief Функтор для обработки закрывающего XML тега
	**************************************************************************/
	struct Last_tag_functor final
	{
		/**********************************************************************
		* @brief Функция функтора для парсинга
		*
		* @param[in] token - символ или ссылка(укзаатель) на символ
		* @param[in] data - контроллер данных
		* @param[in] decorator - объект декоратор с необходимыми символами
		*
		* @return новое состояние парсера в зависимости от текущего символа
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
				if (data.is_correct_tag_name())
				{
					if (data.is_correct_tag_attributes())
					{
						data.add_tag_as_child();
					}
					else
					{
						return Error_state{};
					}
				}
				else
				{
					if (data.is_tag_name_equal_parent_name())
					{
						data.pop_to_parent();
					}
					else
					{
						return Error_state{};
					}
				}

				data.clear_all_buffers();

				return Close_tag_state{};
			}

			return Last_tag_state{};
		}
	};

	//*************************************************************************



	/**************************************************************************
	* @brief Функтор для обработки одиночного XML тега
	**************************************************************************/
	struct Single_tag_functor final
	{
		/**********************************************************************
		* @brief Функция функтора для парсинга
		*
		* @param[in] token - символ или ссылка(укзаатель) на символ
		* @param[in] data - контроллер данных
		* @param[in] decorator - объект декоратор с необходимыми символами
		*
		* @return новое состояние парсера в зависимости от текущего символа
		**********************************************************************/
		template<typename CharT, typename DecorT, typename DataT>
		Base_state operator()(CharT&& token, DataT& data,
			const DecorT& decorator) const
		{
			if (token == decorator.close_tag_symbol)
			{
				if (data.is_correct_tag_name() &&
					data.is_correct_tag_attributes())
				{
					data.add_tag_as_child();
				}
				else
				{
					return Error_state{};
				}

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
	**************************************************************************/
	struct Name_tag_functor final
	{
		/**********************************************************************
		* @brief Функция функтора для парсинга
		*
		* @param[in] token - символ или ссылка(укзаатель) на символ
		* @param[in] data - контроллер данных
		* @param[in] decorator - объект декоратор с необходимыми символами
		*
		* @return новое состояние парсера в зависимости от текущего символа
		**********************************************************************/
		template<typename CharT, typename DecorT, typename DataT>
		Base_state operator()(CharT&& token, DataT& data,
			const DecorT& decorator) const
		{
			if (token != decorator.white_space_symbol &&
				token != decorator.line_break_symbol &&
				token != decorator.carriage_symbol &&
				token != decorator.tab_symbol &&
				token != decorator.close_tag_symbol &&
				token != decorator.single_tag_symbol)
			{
				data.push_to_tag_name(std::forward<CharT>(token));
			}
			else if (token == decorator.white_space_symbol)
			{
				return Attribute_name_state{};
			}
			else if (token == decorator.tab_symbol)
			{
				return Error_state{};
			}
			else if (token == decorator.close_tag_symbol)
			{
				return Close_tag_state{};
			}
			else if (token == decorator.single_tag_symbol)
			{
				return Single_tag_state{};
			}

			return Name_tag_state{};
		}
	};

	//*************************************************************************



	/**************************************************************************
	* @brief Функтор для обработки значения XML тега
	**************************************************************************/
	struct Value_tag_functor final
	{
		/**********************************************************************
		* @brief Функция функтора для парсинга
		*
		* @param[in] token - символ или ссылка(укзаатель) на символ
		* @param[in] data - контроллер данных
		* @param[in] decorator - объект декоратор с необходимыми символами
		*
		* @return новое состояние парсера в зависимости от текущего символа
		**********************************************************************/
		template<typename CharT, typename DecorT, typename DataT>
		Base_state operator()(CharT&& token, DataT& data,
			const DecorT& decorator) const
		{
			if (token == decorator.open_tag_symbol)
			{
				auto&& tag_value = data.get_tag_value();

				if (tag_value.size() &&
					tag_value.back() == decorator.white_space_symbol)
				{
					data.pop_from_tag_value();
				}

				return Open_tag_state{};
			}
			///@todo нужно подумать, как более лучше тут сделать контроль, чтобы
			///не добавлялись лишние символы
			else if (token == decorator.white_space_symbol)
			{
				auto&& tag_value = data.get_tag_value();

				if (tag_value.size() &&
					tag_value.back() != decorator.white_space_symbol)
				{
					data.push_to_tag_value(std::forward<CharT>(token));

					tag_value = data.get_tag_value();

					if (tag_value.back() == decorator.line_break_symbol ||
						tag_value.back() == decorator.tab_symbol ||
						tag_value.back() == decorator.carriage_symbol ||
						tag_value.back() == decorator.fill_symbol)
					{
						data.pop_from_tag_value();
					}
				}

				
			}
			else if (token != decorator.line_break_symbol &&
				token != decorator.tab_symbol &&
				token != decorator.carriage_symbol &&
				token != decorator.fill_symbol)
			{
				data.push_to_tag_value(std::forward<CharT>(token));
			}

			return Value_tag_state{};
		}
	};

	//*************************************************************************



	/**************************************************************************
	* @brief Функтор для обработки имени аттрибута XML тега
	**************************************************************************/
	struct Attribute_name_functor final
	{
		/**********************************************************************
		* @brief Функция функтора для парсинга
		*
		* @param[in] token - символ или ссылка(укзаатель) на символ
		* @param[in] data - контроллер данных
		* @param[in] decorator - объект декоратор с необходимыми символами
		*
		* @return новое состояние парсера в зависимости от текущего символа
		**********************************************************************/
		template<typename CharT, typename DecorT, typename DataT>
		Base_state operator()(CharT&& token, DataT& data,
			const DecorT& decorator) const
		{
			if (token != decorator.white_space_symbol &&
				token != decorator.open_attribute_symbol &&
				token != decorator.close_tag_symbol &&
				token != decorator.single_tag_symbol &&
				token != decorator.equal_attribute_symbol &&
				token != decorator.doc_info_last_symbol)
			{
				data.push_to_attribute_name(std::forward<CharT>(token));
			}
			else if (token == decorator.open_attribute_symbol)
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

			return Attribute_name_state{};
		}
	};

	//*************************************************************************



	/**************************************************************************
	* @brief Функтор для обработки значения аттрибута XML тега
	**************************************************************************/
	struct Attribute_value_functor final
	{
		/**********************************************************************
		* @brief Функция функтора для парсинга
		*
		* @param[in] token - символ или ссылка(укзаатель) на символ
		* @param[in] data - контроллер данных
		* @param[in] decorator - объект декоратор с необходимыми символами
		*
		* @return новое состояние парсера в зависимости от текущего символа
		**********************************************************************/
		template<typename CharT, typename DecorT, typename DataT>
		Base_state operator()(CharT&& token, DataT& data,
			const DecorT& decorator) const
		{
			if (token != decorator.close_attribute_symbol &&
				token != decorator.close_tag_symbol &&
				token != decorator.line_break_symbol)
			{
				data.push_to_attribute_value(std::forward<CharT>(token));
			}
			else if (token == decorator.close_attribute_symbol)
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

			return Attribute_value_state{};
		}
	};

	//*************************************************************************



	/**************************************************************************
	* @brief Функтор для символа начала XML комментария
	**************************************************************************/
	struct Comment_functor final
	{
		/**********************************************************************
		* @brief Функция функтора для парсинга
		*
		* @param[in] token - символ или ссылка(укзаатель) на символ
		* @param[in] data - контроллер данных
		* @param[in] decorator - объект декоратор с необходимыми символами
		*
		* @return новое состояние парсера в зависимости от текущего символа
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
	* @brief Функтор для начала парсинга XML документа
	**************************************************************************/
	struct Start_functor final
	{
		/**********************************************************************
		* @brief Функция функтора для парсинга
		*
		* @param[in] token - символ или ссылка(укзаатель) на символ
		* @param[in] data - контроллер данных
		* @param[in] decorator - объект декоратор с необходимыми символами
		*
		* @return новое состояние парсера в зависимости от текущего символа
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
	**************************************************************************/
	struct Document_info_functor final
	{
		/**********************************************************************
		* @brief Функция функтора для парсинга
		*
		* @param[in] token - символ или ссылка(укзаатель) на символ
		* @param[in] data - контроллер данных
		* @param[in] decorator - объект декоратор с необходимыми символами
		*
		* @return новое состояние парсера в зависимости от текущего символа
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