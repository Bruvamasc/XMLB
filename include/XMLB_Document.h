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

#ifndef XMLB_DOCUMENT_H
#define XMLB_DOCUMENT_H

#include <fstream>
#include <stack>
#include <iomanip>
#include <string>
#include <memory>

#include "XMLB_Node.h"
#include "XMLB_Constants.h"
#include "XMLB_Utility.h"
#include "XMLB_Parser.h"

namespace XMLB
{
	/**************************************************************************
	* @brief XML документ, структура
	*
	* @details 
	*
	* @todo Нужно подумать, над реализацией замены родителя
	**************************************************************************/
	template<typename CharT>
	class Document final
	{
	public:
		using encoding_type = CharT;
		using node_type = Node<encoding_type>;
		using document_type = Document<CharT>;
		using size_type = typename Node<encoding_type>::size_type;

		using Ptr = std::unique_ptr<document_type>;

		using iterator = typename Node<encoding_type>::iterator;
		using const_iterator = typename Node<encoding_type>::const_iterator;

		// Конструкторы, деструкторы и т.п.

		Document(float version);

		Document(const Document& doc);
		Document& operator=(const Document& doc);

		Document(Document&& doc) noexcept;
		Document& operator=(Document&& doc) noexcept;

		~Document() = default;

		// Работа с XML корнем у другими узлами

		void set_general_node(const node_type& node);
		void set_general_node(node_type&& node);
		void set_general_node(std::unique_ptr<node_type> node);
		const node_type& get_general_node() const & noexcept;

		// Работа с версией документа

		void set_version(float version) noexcept;
		float get_version() const noexcept;

		// Очистка, удаление XML структуры

		void clear() noexcept;

		// Блок с итераторами

		iterator begin() noexcept;
		iterator end() noexcept;

		const_iterator begin() const noexcept;
		const_iterator end() const noexcept;

		const_iterator cbegin() const noexcept;
		const_iterator cend() const noexcept;

		// Вспомагательные функции

		void swap(Document&& doc) noexcept;

	private:
		std::unique_ptr<node_type> m_parent;
		float m_version;
	};

	//*************************************************************************



	/**************************************************************************
	*							DOCUMENT IMPLEMENTATION
	**************************************************************************/

	template<typename CharT>
	inline Document<CharT>::Document(float version)
		:m_parent{ std::make_unique<typename Document<CharT>::node_type>(
			typename node_type::string_type{}) },
		m_version{ version }
	{

	}

	//*************************************************************************

	template<typename CharT>
	inline Document<CharT>::Document(const Document& doc)
		:m_parent{ std::make_unique<typename Document<CharT>::node_type>(
			*doc.m_parent) },
		m_version{ doc.m_version }
	{

	}

	//*************************************************************************

	template<typename CharT>
	inline Document<CharT>& Document<CharT>::operator=(const Document& doc)
	{
		if (this != &doc)
		{
			Document temp_doc(doc);

			swap(std::forward(temp_doc));
		}

		return *this;
	}

	//*************************************************************************

	template<typename CharT>
	inline Document<CharT>::Document(Document&& doc) noexcept
		:m_parent{ std::move(doc.m_parent) },
		m_version{ std::move(doc.m_version) }
	{

	}

	//*************************************************************************

	template<typename CharT>
	inline Document<CharT>& Document<CharT>::operator=(Document&& doc) noexcept
	{
		if (this != &doc)
		{
			swap(doc);
		}

		return *this;
	}

	//*************************************************************************

	template<typename CharT>
	inline void Document<CharT>::set_general_node(const node_type& node)
	{
		using std::swap;

		auto in_parent = std::make_unique<Node<CharT>>(node);

		swap(m_parent, in_parent);
	}

	//*************************************************************************

	template<typename CharT>
	inline void Document<CharT>::set_general_node(node_type&& node)
	{
		using std::swap;

		auto in_parent = std::make_unique<Node<CharT>>(std::forward(node));

		swap(m_parent, in_parent);
	}

	//*************************************************************************

	template<typename CharT>
	inline void Document<CharT>::set_general_node(
		std::unique_ptr<node_type> node)
	{
		using std::swap;

		swap(m_parent, node);
	}

	//*************************************************************************

	template<typename CharT>
	inline const typename Document<CharT>::node_type& 
		Document<CharT>::get_general_node() const& noexcept
	{
		return *m_parent.get();
	}

	//*************************************************************************

	template<typename CharT>
	inline void Document<CharT>::set_version(float version) noexcept
	{
		m_version = version;
	}

	//*************************************************************************

	template<typename CharT>
	inline float Document<CharT>::get_version() const noexcept
	{
		return m_version;
	}

	//*************************************************************************

	template<typename CharT>
	inline void Document<CharT>::clear() noexcept
	{
		m_parent.reset(std::make_unique<node_type>("Empty XMLB"));
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Document<CharT>::iterator Document<CharT>::begin() noexcept
	{
		return m_parent->begin();
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Document<CharT>::iterator Document<CharT>::end() noexcept
	{
		return m_parent->end();
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Document<CharT>::const_iterator Document<CharT>::begin() 
		const noexcept
	{
		return m_parent->cbegin();
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Document<CharT>::const_iterator Document<CharT>::end() 
		const noexcept
	{
		return m_parent->cend();
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Document<CharT>::const_iterator Document<CharT>::cbegin() 
		const noexcept
	{
		return m_parent->cbegin();
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Document<CharT>::const_iterator Document<CharT>::cend()
		const noexcept
	{
		return m_parent->cend();
	}

	//*************************************************************************

	template<typename CharT>
	inline void Document<CharT>::swap(Document&& doc) noexcept
	{
		using std::swap;

		swap(m_parent, doc.m_parent);
		swap(m_version, doc.m_version);
	}

	//*************************************************************************



	/**************************************************************************
	*						SAVE AND LOAS FUNCTIONS
	**************************************************************************/

	/**************************************************************************
	* @brief Данная функция сохраняет переданный документ в файл
	*
	* @param[in] doc - XML документ, который нужно сохранить
	* @param[in] file_name - имя файла, в который нужно сохранить документ
	*
	* @return true - если файл успешно сохранен, в противном случае - false
	**************************************************************************/
	template<typename CharT>
	inline bool save_to_file(const Document<CharT>& doc,
		const std::string& file_name)
	{
		bool result = false;

		std::basic_ofstream<CharT> file{ file_name, std::ios_base::binary };

		if (file.is_open())
		{
			auto&& doc_encode = get_doc_encode(doc);
			auto version = version_to_string<CharT>(doc.get_version());
			auto encoding = encoding_to_string<CharT>(doc_encode);

			//Контейнер итераторова, чтобы правильно закрывать теги с потомками
			std::stack<typename Node<CharT>::const_iterator> node_grous;

			//Записываем в файл первую строчку и информацией о документе
			file << kOpen_tag << "?xml version=\"" << version << "\" ";
			file << "encoding=\"" << encoding << "\"?";
			file << kClose_tag;

			for (auto first = doc.cbegin(), last = doc.cend(); 
				first != last; 
				++first)
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
						file << std::setfill(static_cast<CharT>(kTab));
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
				if (first->child_size())
				{
					node_grous.push(first);
				}

				//Устанавливаем нужное количество табов для тега
				file << std::setfill(static_cast<CharT>(kTab));
				file << std::setw(first.get_offset() + 1);

				//Заносим в файл сам тег и его информацию
				file << kOpen_tag << first->get_name();

				//Если у XML тега есть аттрибуты, то записываем в файл
				//их имена и значения
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

				//Если у XML тега нет дочерних узлов и есть значение, то
				//записываем это значение и доп. символы по шаблону
				if (!first->child_size() && first->get_value().size())
				{
					file << kClose_tag;
					file << first->get_value() << kOpen_tag;
					file << kSlash << first->get_name();
					file << kClose_tag;
				}
				//Если у XML тега нет дочерних узлов и нет значения, то
				//записываем доп. символы по шаблону одиночного XML тега
				else if (!first->child_size())
				{
					file << kSpace << kSlash << kClose_tag;
				}
				//В противном случае, просто записываем в файл символ
				//закрытия XML тега
				else
				{
					file << kClose_tag;
				}
			}

			//Если остались ещё незакрытые теги, то заносим в файл их закрытие
			while (!node_grous.empty())
			{
				file << '\n';

				//Устанавливаем нужное количество табов для тега
				file << std::setfill(static_cast<CharT>(kTab));
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



	/**************************************************************************
	* @brief Данная функция загружает из указанного файла данные и создаёт
	* XML структуру
	*
	* @todo Заменить чтение из файла по умолчанию как UTF-8 на нужное - чтобы
	* не создавать лишних строк.
	*
	* @param[in] file_name - имя файла
	*
	* @return умный указатель с XML документом. Если не удалось создать XML
	* структуру, указатель будет равен nullptr
	**************************************************************************/
	template<typename CharT>
	typename Document<CharT>::Ptr load_from_file(const std::string& file_name)
	{
		using std::swap;

		typename Document<CharT>::Ptr result{};

		//Пока что, открываем файл в UTF-8
		std::ifstream file{ file_name, std::ios_base::binary };

		if (file.is_open())
		{
			//Буфер строк, который заполняется данными из файла
			std::vector<std::string> temp_buffer;

			//Буферная строка
			std::string temp_string;

			//Читаем из файла данные. Пока что строками
			while (std::getline(file, temp_string))
			{
				temp_buffer.push_back(temp_string);
			}

			file.close();

			if (temp_buffer.size() > 1)
			{
				//Получаем версию документа из первой строки загруженных файлов
				auto doc_version = string_to_version(temp_buffer[0]);

				{
					auto temp_doc = 
						std::make_unique<Document<CharT>>(doc_version);

					swap(result, temp_doc);
				}

				auto&& general_node = parse_to_node<CharT>
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



	/**************************************************************************
	*							SUPPORT FUNCTIONS
	**************************************************************************/

	template<typename CharT>
	inline bool operator==(const Document<CharT>& lhs,
		const Document<CharT>& rhs)
	{
		bool result = true;

		if (lhs.get_version() != rhs.get_version() ||
			lhs.get_general_node() != rhs.get_general_node())
		{
			result = false;
		}

		return result;
	}

	//*************************************************************************

	template<typename CharT>
	inline bool operator!=(const Document<CharT>& lhs,
		const Document<CharT>& rhs)
	{
		return !(lhs == rhs);
	}

	//*************************************************************************

	template<typename CharT>
	inline void swap(Document<CharT>& lhs, Document<CharT>& rhs) noexcept
	{
		lhs.swap(rhs);
	}

	//*************************************************************************

	template<typename CharT>
	inline Encode_type get_doc_encode(const Document<CharT>& doc)
	{
		if constexpr (std::is_same_v<CharT, char>)
		{
			return Encode_type::UTF_8;
		}
		else if constexpr (std::is_same_v<CharT, char16_t>)
		{
			return Encode_type::UTF_16;
		}
		else if constexpr (std::is_same_v<CharT, char32_t>)
		{
			return Encode_type::UTF_32;
		}
		else if constexpr (std::is_same_v<CharT, wchar_t>)
		{
			if constexpr (sizeof(wchar_t) == sizeof(char16_t))
			{
				return Encode_type::UTF_16;
			}
			else if constexpr (sizeof(wchar_t) == sizeof(char32_t))
			{
				return Encode_type::UTF_32;
			}
			else
			{
				return Encode_type::UTF_8;
			}
		}
		else
		{
			return Encode_type::UTF_8;
		}
	}

	//*************************************************************************

} // namespace XMLB

#endif // !XMLB_DOCUMENT_H