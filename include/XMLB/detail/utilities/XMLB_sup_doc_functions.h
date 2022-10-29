/******************************************************************************
* @file
* Данный файл объявляет и определяет вспомагательные шаблонные классы,
* структуры, функции
*
* @author Bruvamasc
* @date   2022-08-25
*
* @todo Подумать над возможными изменениями Node_tree_impl, code_converter
* ///< Указывает, что элемент недоступен для использования
*
******************************************************************************/

#ifndef XMLB_SUP_DOC_FUNCTIONS_H
#define XMLB_SUP_DOC_FUNCTIONS_H

#include "XMLB/XMLB_Document.h"
#include "XMLB/XMLB_Node.h"
#include "XMLB/detail/XMLB_Decorator.h"
#include "XMLB/detail/parser/XMLB_Parser_data_controller.h"
#include "XMLB/detail/parser/XMLB_Parser_functors.h"
#include "XMLB/detail/parser/XMLB_Parser.h"

namespace XMLB
{
	template<typename CharT>
	class Document;
}

namespace XMLB { namespace detail {

	template<typename CharT>
	using default_decorator = Decorator<CharT>;

	//*************************************************************************

	template<typename CharT>
	using default_data_controller = Parser_data_controller<CharT>;

	//*************************************************************************

	template<typename CharT, typename DecorT, typename DataT>
	using default_parser = Parser<CharT, DecorT, DataT>;

	//*************************************************************************

	template<typename ParserT>
	inline ParserT create_default_parser()
	{
		unsigned int default_functor_count = 11;

		ParserT parser{ default_functor_count };

		parser.add_functor(Start_functor{});
		parser.add_functor(Open_tag_functor{});
		parser.add_functor(Close_tag_functor{});
		parser.add_functor(Last_tag_functor{});
		parser.add_functor(Single_tag_functor{});
		parser.add_functor(Name_tag_functor{});
		parser.add_functor(Value_tag_functor{});
		parser.add_functor(Attribute_name_functor{});
		parser.add_functor(Attribute_value_functor{});
		parser.add_functor(Comment_functor{});
		parser.add_functor(Document_info_functor{});

		return parser;
	}

	//*************************************************************************

	template<typename StringT, typename ContainerT>
	inline decltype(auto) create_node(StringT&& name, StringT&& value,
		ContainerT&& attribute_names, ContainerT&& attribute_values)
	{
		using symbol_type = typename StringT::value_type;
		using node_type = Node<symbol_type>;
		using node_pointer = typename Node<symbol_type>::Ptr;
		using node_attribute = Node_attribute<symbol_type>;

		node_pointer result = std::make_unique<node_type>(
			std::forward<StringT>(name), std::forward<StringT>(value));

		auto attr_names_beg = attribute_names.begin();
		auto attr_names_end = attribute_names.end();

		auto attr_values_beg = attribute_values.begin();
		auto attr_values_end = attribute_values.end();

		for (; attr_names_beg != attr_names_end &&
			attr_values_beg != attr_values_end;
			++attr_names_beg, ++attr_values_beg)
		{
			result->add_attribute(node_attribute{
				std::move(*attr_names_beg), std::move(*attr_values_beg) });
		}

		return result;
	}

	//*************************************************************************

	template<typename ContainerT>
	inline decltype(auto) create_root_node_from_container(
		ContainerT&& container)
	{
		using tag_container_type =
			typename std::decay_t<ContainerT>::value_type;
		using symbol_type = symbol_type_t<tag_container_type>;

		using iterator_type =
			iterator_t<typename std::decay_t<ContainerT>::value_type::
			child_container_type>;

		using node_type = Node<symbol_type>;
		using node_pointer = typename Node<symbol_type>::Ptr;
		using tag_type = Tag_range<iterator_type, node_pointer>;

		using std::swap;

		node_pointer result{ nullptr };

		if (container.size())
		{
			auto&& source_root_tag = container.top();

			std::stack<tag_type> tag_nodes;

			node_pointer tag_node = create_node(
				std::move(source_root_tag.name),
				std::move(source_root_tag.value),
				source_root_tag.attribute_names,
				source_root_tag.attribute_values);

			tag_nodes.push(tag_type{
				source_root_tag.childs.begin(),
				source_root_tag.childs.end(),
				std::move(tag_node) });

			auto first = tag_nodes.top().first;
			auto last = tag_nodes.top().last;

			while (first != last)
			{
				node_pointer temp_tag = create_node(
					std::move((*first).name),
					std::move((*first).value),
					(*first).attribute_names,
					(*first).attribute_values);

				if ((*first).childs.size())
				{
					tag_nodes.top().first = first;

					tag_nodes.push(tag_type{ (*first).childs.begin(),
						(*first).childs.end(), std::move(temp_tag) });

					first = tag_nodes.top().first;
					last = tag_nodes.top().last;
				}
				else
				{
					tag_nodes.top().node->add_child(std::move(temp_tag));

					++first;
				}

				while (first == last && tag_nodes.size() > 1)
				{
					auto temp = std::move(tag_nodes.top().node);
					tag_nodes.pop();
					tag_nodes.top().node->add_child(std::move(temp));

					++tag_nodes.top().first;

					first = tag_nodes.top().first;
					last = tag_nodes.top().last;
				}
			}

			swap(result, tag_nodes.top().node);
		}

		return result;
	}

	//*************************************************************************

	template<typename ContainerT>
	inline decltype(auto) create_doc_from_info(ContainerT&& doc_info)
	{
		using tag_container_type =
			typename std::decay_t<ContainerT>::value_type;
		using symbol_type = symbol_type_t<tag_container_type>;
		using document_type = Document<symbol_type>;
		using document_pointer = typename Document<symbol_type>::Ptr;

		document_pointer result{ nullptr };

		if (doc_info.size())
		{
			const unsigned int only_doc_version = 1;
			const unsigned int doc_version_and_encoding = 2;

			auto&& source_doc_info = doc_info.top();

			if (source_doc_info.attribute_names.size() ==
				only_doc_version)
			{
				auto doc_version = to_float<symbol_type>(
					*source_doc_info.attribute_values.begin());

				result = std::move(
					std::make_unique<document_type>(doc_version));
			}
			else if (source_doc_info.attribute_names.size() ==
				doc_version_and_encoding)
			{
				auto&& doc_version = to_float<symbol_type>(
					*source_doc_info.attribute_values.begin());

				auto&& doc_encoding =
					*(++source_doc_info.attribute_values.begin());

				result = std::move(std::make_unique<document_type>(
					doc_version, doc_encoding));
			}
		}

		if (!result)
		{
			result = std::move(std::make_unique<document_type>());
		}

		return result;
	}

}} // namespace XMLB::detail

#endif // !XMLB_SUP_DOC_FUNCTIONS_H