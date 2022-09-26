/******************************************************************************
* @file
* Данный файл объявлет основной строительный элемент(узел) XML документа
*
* @author Bruvamasc
* @date   2022-08-25
*
* @todo ТАААТАТ
* ///< Указывает, что элемент недоступен для использования
*
******************************************************************************/

#ifndef XMLB_PARSER_H
#define XMLB_PARSER_H

#include <string>
#include <regex>
#include <queue>
#include <stack>

#include "XMLB_Node.h"
#include "XMLB_Constants.h"
#include "XMLB_Utility.h"
#include "XMLB_Templates.h"

namespace XMLB
{
	/**************************************************************************
	* @brief Данная функция конвертирует последовательность строк в XML
	* структуру
	*
	* @param[in] first - начало последовательности(контейнера)
	* @param[in] last - конец последовательности(контейнера)
	*
	* @return умный указатель с XML узлом и всем дочерними узлами, если они
	* есть
	**************************************************************************/
	template<typename Iter,
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
	Node::Ptr parse_to_node(Iter first, Iter last)
	{
		using Tag = Tag_range_impl<Iter>;

		Node::Ptr result{ nullptr };
		bool is_container_correct = true;

		std::stack<Tag> tags;
		Tag* last_parent = nullptr;

		std::smatch tag_match;
		std::smatch open_match;
		std::smatch close_match;
		std::smatch single_match;

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
				is_container_correct = false;
				break;
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

			bool is_one_tag = false;
			bool is_single_tag = false;

			//Проверяем, есть ли открытый тег в текущей строчке и сейчас не
			//не ищеться закрытый тег. Если такой тег в этой строке найден, то
			//ищем закрывающий тег
			if (std::regex_search(*first, open_match, kOpen_tag_template))
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

			//Если найден открытый тег и он не одиночный, то ищем закрывающий тег
			if (is_one_tag)
			{
				auto tag_iter = first;

				//Ищем соответствующий закрытый тег
				for (; tag_iter != last; ++tag_iter)
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

				//Считаем расстояние между началом и концом диапазона тега.
				//Если он больше 1, то конструируем этот тег, добавляем в стек
				//тегов, и изменяем указатель последнего родителя на этот тег
				if (std::distance(first, tag_iter) > 1)
				{
					tags.push(
						Tag{ first,
						tag_iter,
						create_node(open_match[1], *first) }
					);

					//Если последний родитель есть, до устанавливаем его
					//родителем созданного тега из стека tags
					if (last_parent)
					{
						tags.top().node->set_parent(&(*(last_parent->node)));
					}

					//Изменяем указатель на последнего родителя
					last_parent = &tags.top();
				}
				//в противном случае, создаем узел, тег без добавления в стек
				//тегов
				else
				{
					if (last_parent)
					{
						last_parent->node->add_child
						(
							std::move(create_node(open_match[1], *first))
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
					result = std::move(create_node(open_match[1], *first));
					break;
				}
				//В противном случае, создаем узел, тег без добавления в стек
				//тегов
				else
				{
					last_parent->node->add_child
					(
						std::move(create_node(open_match[1], *first))
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
}

#endif // !XMLB_PARSER_H