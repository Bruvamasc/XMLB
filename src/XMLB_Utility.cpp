#include "XMLB_Utility.h"

#include <stack>
#include <exception>
#include <iostream>

#include "XMLB_Constants.h"

namespace XMLB
{
	/**************************************************************************
	*                                  UTILITY
	**************************************************************************/

	std::vector<std::string> to_buffer(Node_iterator_impl<const Node> first,
		Node_iterator_impl<const Node> last)
	{
		std::vector<std::string> result;
		result.reserve(std::distance(first, last));

		std::stack<Node_iterator_impl<const Node>> node_grous;

		for (; first != last; ++first)
		{
			//Проверяем, пустой ли стэк с итераторома-группами
			if (!node_grous.empty())
			{
				//Если текущее значение табуляции равно с последним
				//добавленным в стэк итератором, то заносим в строку
				//закрытие предыдущего тега
				if (first.get_offset() == node_grous.top().get_offset())
				{
					//Устанавливаем нужное количество табов для тега
					std::string temp_string(first.get_offset(), kTab);

					temp_string += kOpen_tag;
					temp_string += node_grous.top()->get_name();
					temp_string += kClose_tag;
					temp_string += '\n';

					node_grous.pop();

					result.push_back(std::move(temp_string));
				}
			}

			//Если у текущего тега есть под-теги, то добавляем его
			//в стек в качестве итератора-группы
			//if (it->get()->child_size())
			if (first->child_size())
			{
				node_grous.push(first);
			}

			//Устанавливаем нужное количество табов для тега
			std::string temp_string(first.get_offset(), kTab);

			//Заносим в строку сам тег и его информацию
			temp_string += kOpen_tag;
			temp_string += first->get_name();

			//Если у тега есть атрибуты, заносим их в строку
			if (first->attr_size())
			{
				for (auto it = first->attr_begin(),
					end = first->attr_end();
					it != end;
					++it)
				{
					temp_string += kSpace;
					temp_string += it->name;
					temp_string += '=';
					temp_string += '\"';
					temp_string += it->value;
					temp_string += '\"';
				}
			}

			//Если у тега нет потомков и значение тега не пустое, то
			//добавляем это значение в строку
			if (!first->child_size() &&
				first->get_value().size())
			{
				temp_string += kClose_tag;
				temp_string += first->get_value();
				temp_string += kOpen_tag;
				temp_string += kSlash;
				temp_string += first->get_name();
				temp_string += kClose_tag;
			}
			//Если у тега нет потомков, но и значения нет, то этот тег является
			//одиночным. Заносим признак одиночного тега в строку
			else if (!first->child_size())
			{
				temp_string += kSpace;
				temp_string += kSlash;
				temp_string += kClose_tag;
			}
			//Во всех остальных случаях просто закрываем тег
			else
			{
				temp_string += kClose_tag;
			}

			result.push_back(std::move(temp_string));
		}

		//Если остались ещё не закрытые теги, то заносим в файл их закрытие
		while (!node_grous.empty())
		{
			//Устанавливаем нужное количество табов для тега
			std::string temp_string(node_grous.top().get_offset() + 1, kTab);
			temp_string[0] = '\n';

			temp_string += kOpen_tag;
			temp_string += kSlash;
			temp_string += node_grous.top()->get_name();
			temp_string += kClose_tag;

			node_grous.pop();

			result.push_back(std::move(temp_string));
		}

		return result;
	}

	//*************************************************************************

	std::string version_to_string(float version)
	{
		if (version < 0)
		{
			version *= -1.f;
		}

		std::string result = std::to_string(version);

		auto dot_pos = result.find_first_of('.');
		auto last_number_pos = result.find_first_not_of('0', dot_pos + 1);
		auto first_zero_pos = result.find_first_of('0', dot_pos);

		if (last_number_pos != std::string::npos)
		{
			result.erase(last_number_pos + 1);
		}
		else
		{
			result.erase(first_zero_pos + 1);
		}

		return result;
	}

	//*************************************************************************

	float string_to_version(const std::string& input) noexcept
	{
		auto result = -1.f;

		std::smatch version_match;

		if (std::regex_search(input, version_match, kVersion_tag_template) &&
			version_match.size() >= 2)
		{
			result = std::stof(version_match[1]);
		}

		return result;
	}

	//*************************************************************************

	std::string encoding_to_string(const std::locale& input)
	{
		std::string result = input.name();
		auto dot_pos = result.find_first_of('.');
		result.erase(0, dot_pos + 1);

		return result;
	}

	//*************************************************************************

	std::locale string_to_encoding(const std::string& input)
	{
		std::locale result("ru_RU.UTF-8");

		std::smatch encoding_match;

		if (std::regex_search(input, encoding_match, kEncoding_tag_template) &&
			encoding_match.size() >= 2)
		{
			try
			{
				std::locale tmp_locale("ru_RU." + encoding_match[1].str());

				result = tmp_locale;
			}
			catch (const std::exception& e)
			{
				std::cout << "EXCEPTION: " << e.what() << '\n';
			}
		}

		return result;
	}

	//*************************************************************************

	Node::Ptr create_node(const std::string& name, const std::string& input)
	{
		Node::Ptr result(new Node{ name });

		std::smatch text_match;

		//Проверяем, есть ли аттрибуты в открытом теге. Если есть, то
		//добавляем их
		for (auto it = std::sregex_iterator{
			input.cbegin(), input.cend(), kTag_parameters_template },
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
				result->add_attribute(
					Node_attribute{
						tmp_string.substr(
							delim_pos[0],
							delim_pos[1] - delim_pos[0]),
						tmp_string.substr(
							delim_pos[2],
							delim_pos[3] - delim_pos[2])
					}
				);
			}
		}

		//Проверяем, есть ли значение тега в строке input
		if (std::regex_search(input, text_match, kText_template))
		{
			//Если в тексте присутсвтует больше чем 1 слово, то
			//объеденяю его в одно
			if (text_match.size() >= 2)
			{
				result->set_value(text_match[1]);
			}
		}

		return result;
	}

	//*************************************************************************

} // namespace XMLB