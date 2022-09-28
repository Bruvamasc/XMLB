#include "XMLB_Document.h"

#include <fstream>
#include <algorithm>
#include <vector>
#include <stack>
#include <iomanip>

#include "XMLB_Parser.h"
#include "XMLB_Constants.h"
#include "XMLB_Utility.h"

namespace XMLB
{

	/**************************************************************************
	*								DOCUMENT
	**************************************************************************/

	Document::Document(const std::locale& file_locale, float version)
		:m_parent{ new Node{ "General XMLB" } },
		m_locale{ file_locale },
		m_version{ version }
	{

	}

	//*************************************************************************

	Document::Document(const Document& doc)
		:m_parent{ new Node{ *doc.m_parent.get() } },
		m_locale{ doc.m_locale },
		m_version{ doc.m_version }
	{

	}

	//*************************************************************************

	Document& Document::operator=(const XMLB::Document& doc)
	{
		if (this != &doc)
		{
			Document temp_doc(doc);

			swap(temp_doc);
		}

		return *this;
	}

	//*************************************************************************

	Document::Document(Document&& doc) noexcept
		:m_parent{ std::move(doc.m_parent) },
		m_locale{ std::move(doc.m_locale) },
		m_version{ std::move(doc.m_version) }
	{

	}

	//*************************************************************************

	Document& Document::operator=(Document&& doc) noexcept
	{
		if (this != &doc)
		{
			swap(doc);
		}

		return *this;
	}

	//*************************************************************************

	void Document::set_general_node(const Node& node)
	{
		auto in_parent = std::make_unique<Node>(node);

		std::swap(m_parent, in_parent);
	}

	//*************************************************************************

	void Document::set_general_node(Node&& node)
	{
		auto in_parent = std::make_unique<Node>(std::move(node));

		std::swap(m_parent, in_parent);
	}

	//*************************************************************************

	void Document::set_general_node(Node::Ptr node)
	{
		std::swap(m_parent, node);
	}

	//*************************************************************************

	const Node& Document::get_general_node() const & noexcept
	{
		return *m_parent.get();
	}

	//*************************************************************************

	void Document::set_file_locale(const std::locale& file_locale)
	{
		m_locale = file_locale;
	}

	//*************************************************************************

	const std::locale& Document::get_file_locale() const & noexcept
	{
		return m_locale;
	}

	//*************************************************************************

	void Document::set_version(float version) noexcept
	{
		m_version = version;
	}

	//*************************************************************************

	float Document::get_version() const noexcept
	{
		return m_version;
	}

	//*************************************************************************

	void Document::clear() noexcept
	{
		m_parent.reset(nullptr);
	}

	//*************************************************************************

	Node::iterator Document::begin() noexcept
	{
		return m_parent->begin();
	}

	//*************************************************************************

	Node::iterator Document::end() noexcept
	{
		return m_parent->end();
	}

	//*************************************************************************

	Node::const_iterator Document::begin() const noexcept
	{
		return m_parent->cbegin();
	}

	//*************************************************************************

	Node::const_iterator Document::end() const noexcept
	{
		return m_parent->cend();
	}

	//*************************************************************************

	Node::const_iterator Document::cbegin() const noexcept
	{
		return m_parent->cbegin();
	}

	//*************************************************************************

	Node::const_iterator Document::cend() const noexcept
	{
		return m_parent->cend();
	}

	//*************************************************************************

	void Document::swap(Document& doc) noexcept
	{
		std::swap(m_parent, doc.m_parent);
		std::swap(m_locale, doc.m_locale);
		std::swap(m_version, doc.m_version);
	}

	//*************************************************************************



	/**************************************************************************
	*								FUNCTIONS
	**************************************************************************/

	bool save_to_file(const Document& doc, const std::string& file_name)
	{
		bool result = false;

		std::ofstream file(file_name);
		file.imbue(doc.get_file_locale());

		if (file.is_open())
		{
			std::string version = version_to_string(doc.get_version());
			std::string encoding = encoding_to_string(doc.get_file_locale());			

			//Контейнер итераторова, чтобы правильно закрывать теги с потомками
			std::stack<Node::const_iterator> node_grous;

			//Записываем в файл первую строчку и информацией о документе
			file << kOpen_tag << "?xml version=\"" << version << "\" ";
			file << "encoding=\"" << encoding << "\"?";
			file << kClose_tag;

			for (auto first = doc.cbegin(), last = doc.cend(); first != last; ++first)
			{
				file << '\n';

				//Проверяем, пустой ли стэк с итераторома-группами
				if (!node_grous.empty())
				{
					//Если текущее значение табуляции равно с последним
					//добавленным в стэк итератором, то заносим в файл
					//закрытие предыдущего тега
					while (first.get_offset() <= node_grous.top().get_offset())
					{
						//Устанавливаем нужное количество табов для тега
						file << std::setfill(kTab);
						file << std::setw(node_grous.top().get_offset() + 1);

						file << kOpen_tag << kSlash;
						file << node_grous.top()->get_name();
						file << kClose_tag;
						file << '\n';

						node_grous.pop();
					}
				}

				//Если у текущего тега есть под-теги, то добавляем его
				//в стек в качестве итератора-группы
				//if (it->get()->child_size())
				if(first->child_size())
				{
					node_grous.push(first);
				}

				//Устанавливаем нужное количество табов для тега
				file << std::setfill(kTab);
				file << std::setw(first.get_offset() + 1);

				//Заносим в файл сам тег и его информацию
				file << kOpen_tag << first->get_name();

				if (first->attr_size())
				{
					for (auto at_it = first->attr_begin(),
						at_end = first->attr_end();
						at_it != at_end;
						++at_it)
					{
						file << kSpace << at_it->name << "=";
						file << '\"' << at_it->value << '\"';
					}
				}

				if (!first->child_size() && first->get_value().size())
				{
					file << kClose_tag;
					file << first->get_value() << kOpen_tag;
					file << kSlash << first->get_name();
					file << kClose_tag;
				}
				else if (!first->child_size())
				{
					file << kSpace << kSlash << kClose_tag;
				}
				else
				{
					file << kClose_tag;
				}
			}

			//Если остались ещё не закрытые теги, то заносим в файл их закрытие
			while (!node_grous.empty())
			{
				file << '\n';

				//Устанавливаем нужное количество табов для тега
				file << std::setfill(kTab);
				file << std::setw(node_grous.top().get_offset() + 1);

				file << kOpen_tag << kSlash;
				file << node_grous.top()->get_name();
				file << kClose_tag;

				node_grous.pop();
			}

			file.close();

			result = true;
		}

		return result;
	}

	//*************************************************************************

	Document::Ptr load_from_file(const std::string& file_name)
	{
		Document::Ptr result;

		std::ifstream file(file_name);

		if (file.is_open())
		{
			std::string temp_string;

			std::getline(file, temp_string);

			auto doc_version = string_to_version(temp_string);
			auto doc_encoding = string_to_encoding(temp_string);

			result.reset(new Document{ doc_encoding, doc_version });
			
			file.close();
		}

		if (result)
		{
			std::ifstream file(file_name);
			file.imbue(result->get_file_locale());

			std::vector<std::string> temp_buffer;

			if (file.is_open())
			{
				std::string temp_string;

				while (std::getline(file, temp_string))
				{
					temp_buffer.push_back(temp_string);
				}

				file.close();
			}

			if (temp_buffer.size() > 1)
			{
				Node::Ptr general_node = parse_to_node
				(
					std::next(std::cbegin(temp_buffer)),
					std::cend(temp_buffer)
				);

				if (general_node)
				{
					result->set_general_node(std::move(general_node));
				}
				else
				{
					result.reset(nullptr);
				}
			}
		}

		return result;
	}

	//*************************************************************************

} //namespace XMLB