/******************************************************************************
* @file
* Данный файл объявлет основной строительный элемент(узел) XML документа
*
* @author Bruvamasc
* @date   2022-08-25
*
* @todo Нужно переделать, после изменения механизма регулярных выражений,
* функцию create_node и parse_to_node, чтобы они сразу создавали XML узлы, а
* не конвертировали данные из UTF-8 в нужную кодировку
* ///< Указывает, что элемент недоступен для использования
*
******************************************************************************/

#ifndef XMLB_PARSER_H
#define XMLB_PARSER_H

#include <string>
#include <regex>
#include <stack>

#include "XMLB_Node.h"
#include "XMLB_Constants.h"
#include "XMLB_Utility.h"
#include "XMLB_Templates.h"

namespace XMLB
{
	/**************************************************************************
	* @brief Данная функция создаёт XML тег, объект
	*
	* @param[in] name - имя тега в кодировке UTF-8
	* @param[in] input - строка, в которой могут быть аттрибуты и/или
	* значение тега в кодировке UTF-8
	*
	* @return умный укаатель с XML узлом
	**************************************************************************/
	template<typename CharT>
	typename Node<CharT>::Ptr create_node(const std::string& name,
		const std::string& input_attributes, const std::string& input_text)
	{
		code_converter<CharT, char> converter;

		typename Node<CharT>::Ptr result
		{
			std::make_unique<Node<CharT>>(converter.convert_to_in(name))
		};

		std::smatch text_match;

		//Проверяем, есть ли аттрибуты в открытом теге. Если есть, то
		//добавляем их
		for (auto it = std::sregex_iterator{
			input_attributes.cbegin(),
			input_attributes.cend(),
			kTag_parameters_template },
			end = std::sregex_iterator{};
			it != end;
			++it)
		{
			auto tmp_string = it->str();

			std::size_t delim_pos[]{ 0, 0, 0, 0 };

			delim_pos[0] = tmp_string.find_first_of(
				' '
			);
			delim_pos[1] = tmp_string.find_first_of(
				'=', ++delim_pos[0]
			);
			delim_pos[2] = tmp_string.find_first_of(
				'\"', delim_pos[1]
			);
			delim_pos[3] = tmp_string.find_first_of(
				'\"', ++delim_pos[2]
			);

			if (delim_pos[1] > delim_pos[0] &&
				delim_pos[2] > delim_pos[1] &&
				delim_pos[3] > delim_pos[2])
			{
				std::basic_string<CharT> attr_name
				{
					converter.convert_to_in(tmp_string.substr(
							delim_pos[0],
							delim_pos[1] - delim_pos[0]))
				};

				std::basic_string<CharT> attr_value
				{
					converter.convert_to_in(tmp_string.substr(
							delim_pos[2],
							delim_pos[3] - delim_pos[2]))
				};

				result->add_attribute(
					Node_attribute<CharT>{ attr_name, attr_value }
				);
			}
		}

		if (&input_attributes == &input_text)
		{
			//Проверяем, есть ли значение тега в строке input
			if (std::regex_search(input_text, text_match, kText_sign_template))
			{
				//Если в тексте присутсвтует больше чем 1 слово, то
				//объеденяю его в одно
				if (text_match.size() >= 2)
				{
					result->set_value(converter.convert_to_in(text_match[1]));
				}
			}
		}
		else
		{
			std::string tag_text_buffer;
			tag_text_buffer.reserve(input_text.size());

			//Проверяем, есть ли значение тега в строке input_text
			for (auto it = std::sregex_iterator{
				input_text.cbegin(), 
				input_text.cend(), 
				kText_template},
				end = std::sregex_iterator{};
				it != end;
				++it)
			{
				tag_text_buffer += it->str();
				tag_text_buffer += ' ';
			}

			if (tag_text_buffer.size())
			{
				tag_text_buffer.erase(std::next(tag_text_buffer.cend(), -1));

				result->set_value(converter.convert_to_in(tag_text_buffer));
			}
		}

		////Проверяем, есть ли значение тега в строке input
		//if (std::regex_search(input, text_match, kText_template))
		//{
		//	//Если в тексте присутсвтует больше чем 1 слово, то
		//	//объеденяю его в одно
		//	if (text_match.size() >= 2)
		//	{
		//		result->set_value(converter.convert_to_in(text_match[1]));
		//	}
		//}

		return result;
	}

	//*************************************************************************



	/**************************************************************************
	* @brief Данная функция конвертирует последовательность строк в XML
	* структуру
	*
	* @param[in] CharT - тип символов строк данных в узлах, который должен
	* получится
	* @param[in] first - начало последовательности строк в UTF-8
	* @param[in] last - конец последовательности строк в UTF-8
	*
	* @return умный указатель с XML узлом и всем дочерними узлами, если они
	* есть
	**************************************************************************/
	template<typename CharT,
		typename Iter,
		typename = std::void_t<is_has_operator_plus_plus<Iter>, 
		is_has_operator_indirect_conversion<Iter>, 
		is_has_operator_self_equality<Iter>>,

		std::enable_if_t<std::is_same_v<std::string, iterator_value_t<Iter>> ||
		std::is_same_v<const std::string, 
		iterator_value_t<Iter>>, std::nullptr_t> = nullptr,

		std::enable_if_t<std::is_same_v<std::string&, 
		iterator_reference_t<Iter>> || std::is_same_v<const std::string&, 
		iterator_reference_t<Iter>>, std::nullptr_t> = nullptr,

		std::enable_if_t<std::is_base_of_v<std::forward_iterator_tag, 
		iterator_category_t<std::remove_reference_t<Iter>>>, 
		std::nullptr_t> = nullptr,

		std::enable_if_t<std::is_copy_assignable_v<Iter>, 
		std::nullptr_t> = nullptr
	>
	typename Node<CharT>::Ptr parse_to_node(Iter first, Iter last)
	{
		using Tag = Tag_range_impl<Iter, typename XMLB::Node<CharT>::Ptr>;

		typename Node<CharT>::Ptr result{ nullptr };
		bool is_container_correct = true;

		std::stack<Tag> tags;
		Tag* last_parent = nullptr;

		std::smatch tag_match;
		std::smatch next_match;
		std::smatch open_match;
		std::smatch close_match;
		std::smatch single_match;

		bool is_one_tag = false;

		//Находим все теги. А именно, если есть открытый, то должен найтись
		//одиночный. Так же одиночный. Если для открытого не находится
		//закрытый, или же одиночного есть закрытый, или есть только закрытый
		//без одиночного, то завершаем цикл и остальные ничего не конструируем
		for (; first != last; ++first)
		{
			//Проверяем, является ли текущая строка тегом. Если не является, то
			//выходим из цикла
			if (!std::regex_search(*first, tag_match, kTag_Element_template))
			{
				continue;
				//is_container_correct = false;
				//break;
			}

			//Проверяем, является ли текущий итератор равным конечному
			//итератору последнего родителя. Если да, то устанавливаем
			//нового последнего родителя.
			if (last_parent && first == last_parent->last)
			{
				last_parent = nullptr;

				if (tags.size() > 1)
				{
					auto last_tag = std::move(tags.top().node);
					tags.pop();
					tags.top().node->add_child(std::move(last_tag));
					last_parent = &tags.top();
				}
				else if (tags.size() == 1)
				{
					result = std::move(tags.top().node);
					tags.pop();
				}
			}

			bool is_single_tag = false;

			//Проверяем, есть ли открытый тег в текущей строчке и сейчас не
			//не ищеться закрытый тег. Если такой тег в этой строке найден, то
			//ищем закрывающий тег
			if (std::regex_search(*first, open_match, kOpen_tag_template) &&
				!is_one_tag)
			{
				//Проверяем, является ли это одиночным тегом
				if (std::regex_search(*first,
					single_match,
					kSingle_tag_template))
				{
					is_single_tag = true;
				}
				else
				{
					is_one_tag = true;
				}
			}

			//Если найден открытый тег и он не одиночный, ищем закрывающий тег
			if (is_one_tag)
			{
				auto tag_iter = first;

				is_one_tag = false;

				//Подсчет дистанции между итератором first и tag_iter.
				//Сделано так, а не через функцию, из-за того, что итератор
				//может быть не std::random_access_iterator_tag
				std::size_t iter_distance = 0;
			
				//Ищем соответствующий закрытый тег
				for (; tag_iter != last; ++tag_iter, ++iter_distance)
				{
					if (std::regex_search(*tag_iter,
						close_match,
						kClose_tag_template) &&
						open_match[1] == close_match[1])
					{
						break;
					}
				}

				//Если соответсвующий закрытый тег не был найден, то выходим из
				//цикла
				if (tag_iter == last)
				{
					is_container_correct = false;
					break;
				}

				
				auto next_iter = first + 1;

				if (next_iter == last)
				{
					is_container_correct = false;
					break;
				}

				//Считаем расстояние между началом и концом диапазона тега.
				//Если он больше 2, то конструируем этот тег, добавляем в стек
				//тегов, и изменяем указатель последнего родителя на этот тег
				if (iter_distance >= 2 && 
					std::regex_search(*next_iter, next_match, kTag_Element_template))
				{
					tags.push(
						Tag{ first,
						tag_iter,
						create_node<CharT>(
							open_match[1].str(), *first, *first)}
					);

					//Изменяем указатель на последнего родителя
					last_parent = &tags.top();
				}
				//Если же расстояние равно 2, то создаем узел, тег без 
				//добавления в стек тегов
				else if (iter_distance == 2)
				{
					if (last_parent)
					{
						last_parent->node->add_child
						(
							std::move(create_node<CharT>(
								open_match[1].str(), *first, *next_iter))
						);
					}
				}
				//в противном случае, создаем узел, тег без добавления в стек
				//тегов
				else
				{
					if (last_parent)
					{
						last_parent->node->add_child
						(
							std::move(create_node<CharT>(
								open_match[1].str(), *first, *first))
						);
					}
				}
			}
			//Если же это одиночный тег
			else if (is_single_tag)
			{
				//Если последнего родителя нет, значит это первый и
				//единственный тег в документе. Изменяем result и завершаем
				//цикл
				if (!last_parent)
				{
					result = std::move(
						create_node<CharT>(
							open_match[1].str(), *first, *first));

					break;
				}
				//В противном случае, создаем узел, тег без добавления в стек
				//тегов
				else
				{
					last_parent->node->add_child
					(
						std::move(create_node<CharT>(
							open_match[1].str(), *first, *first))
					);
				}
			}
		}

		if (!is_container_correct)
		{
			result.reset(nullptr);
		}

		return result;
	}

	//*************************************************************************
}

#endif // !XMLB_PARSER_H