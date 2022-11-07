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



#ifndef XMLB_DOCUMENT_H
#define XMLB_DOCUMENT_H

#include <stack>
#include <memory>
#include <stdexcept>

#include "XMLB_Node.h"
#include "XMLB_Utility.h"
#include "XMLB/detail/XMLB_Decorator.h"
#include "XMLB/detail/parser/XMLB_Parser_states.h"
#include "XMLB/detail/parser/XMLB_Parser_functors.h"
#include "XMLB/detail/parser/XMLB_Parser_data_controller.h"
#include "XMLB/detail/parser/XMLB_Parser.h"
#include "XMLB/detail/traits/XMLB_Type_special_general_traits.h"
#include "XMLB/detail/utilities/XMLB_sup_functions.h"



namespace XMLB
{

	/**************************************************************************
	* @brief XML документ с узлами
	* 
	* @ingroup general
	*
	* @details XML документ, который хранит в себе версию и кодировку
	* документа. Также хранит root узел(тег) с его всеми дочерними узлами
	* 
	* @tparam CharT - тип символов
	* 
	* @todo Нужно подумать, нужно ли вообще хранить кодировку. Так же, возможно
	* стоит приватно наследоваться от Node<CharT>.
	**************************************************************************/
	template<typename CharT>
	class Document final
	{
	public:
		using symbol_type = CharT;
		using string_type = typename Node<symbol_type>::string_type;
		using string_wrapper = typename Node<symbol_type>::string_wrapper;
		using node_type = Node<symbol_type>;
		using node_pointer = typename Node<symbol_type>::Ptr;
		using document_type = Document<CharT>;
		using size_type = typename Node<symbol_type>::size_type;

		using Ptr = std::unique_ptr<document_type>;

		using iterator = typename Node<symbol_type>::iterator;
		using const_iterator = typename Node<symbol_type>::const_iterator;



		/// @name Конструкторы, деструктор
		/// @{
		/**********************************************************************
		* @param version - версия документа
		* @param encoding_type - кодировка документа
		**********************************************************************/
		Document(float version = 1.f, 
			const string_type& encoding_type = 
			detail::to_string<symbol_type>('U', 'T', 'F', '-', '8'));

		Document(float version, string_type&& encoding_type);

		Document(const Document& doc);
		Document& operator=(const Document& doc);

		Document(Document&& doc) noexcept;
		Document& operator=(Document&& doc) noexcept;

		~Document() = default;
		/// @}



		// Работа с XML корнем у другими узлами
		/// @name Методы работы с корневым узлом
		/// @{
		/**********************************************************************
		* @brief Заменить корневой XML узел
		* 
		* @param node - XML узел
		**********************************************************************/
		void root(const node_type& node);

		/**********************************************************************
		* @brief Заменить корневой XML узел
		* 
		* @param node - XML узел
		**********************************************************************/
		void root(node_type&& node);

		/**********************************************************************
		* @brief Заменить корневой XML узел
		*
		* @param node - XML узел
		**********************************************************************/
		void root(node_pointer node);

		/**********************************************************************
		* @brief Получить корневой XML узел
		* 
		* @warning Если корневого узла нет, бросается исключение 
		* std::out_of_range
		* 
		* @return корневой узел, если он есть
		**********************************************************************/
		node_type& root() &;

		/**********************************************************************
		* @brief Получить корневой XML узел
		*
		* @warning Если корневого узла нет, бросается исключение
		* std::out_of_range
		*
		* @return корневой узел, если он есть
		**********************************************************************/
		const node_type& root() const &;
		/// @}



		/// @name Методы для работы с версией документа
		/// @{
		/**********************************************************************
		* @brief Изменить версию документа
		*
		* @param version - новая версия документа
		**********************************************************************/
		void set_version(float version) noexcept;

		/**********************************************************************
		* @brief Получить версию документа
		*
		* @return версию документа
		**********************************************************************/
		float get_version() const noexcept;
		/// @}



		/// @name Методы для работы с кодировкой документа
		/// @{
		/**********************************************************************
		* @brief Изменить кодировку документа
		*
		* @param encoding_type - новая кодировка документа
		**********************************************************************/
		void set_encoding_type(const string_type& encoding_type);

		/**********************************************************************
		* @brief Изменить кодировку документа
		*
		* @param encoding_type - новая кодировка документа
		**********************************************************************/
		void set_encoding_type(string_type&& encoding_type) noexcept;

		/**********************************************************************
		* @brief Получить кодировку документа
		*
		* @return невладеющий объект-обертку строки с кодировкой документа
		**********************************************************************/
		string_wrapper get_encoding_type() const noexcept;
		/// @}



		/// @name Методы доступа итераторами
		/// @{
		iterator begin() noexcept;
		iterator end() noexcept;

		const_iterator begin() const noexcept;
		const_iterator end() const noexcept;

		const_iterator cbegin() const noexcept;
		const_iterator cend() const noexcept;
		/// @}



		/// @name Методы поиска
		/// @{
		/**********************************************************************
		* @brief Найти узел
		*
		* @details Перегрузка для работы с указателями на строки, например:
		* const char*, const wchar_t*
		*
		* @tparam ContT - тип контейнера
		* @param container - имя узла, который нужно найти
		* @param offset - итератор, с которого начать искать
		*
		* @return итератор на узел - если он был найден. В противном случае
		* возвращает итератор указывающий на end()
		**********************************************************************/
		template<typename ContT,
			std::enable_if_t<std::is_same_v<ContT, symbol_type>&&
			std::is_constructible_v<string_wrapper, const ContT*>,
			std::nullptr_t> = nullptr>
		iterator find(const ContT* container,
			const_iterator offset = const_iterator{ nullptr })
		{
			return is_empty() ? end() :
				m_parent->begin()->find(container, offset);
		}

		//*********************************************************************



		/**********************************************************************
		* @brief Найти узел(ы)
		*
		* @details Перегрузка для работы с контейнерами содержащие простые
		* типы, например: std::string, std::vector<char>, std::string_view
		*
		* @tparam ContT - тип контейнера
		* @param container - список имён узлов, которые нужно найти
		* @param offset - итератор, с которого начать искать
		*
		* @return итератор на узел - если он был найден. В противном случае
		* возвращает итератор указывающий на end()
		**********************************************************************/
		template<typename ContT,
			std::enable_if_t<

			detail::is_has_begin_and_end_v<ContT>&&

			detail::is_has_value_type_v<ContT>&&

			std::is_constructible_v<string_wrapper,
			std::add_pointer_t<std::remove_reference_t<
			detail::dereferenced_t<detail::const_iterator_t<ContT>>>>>&&

			std::is_same_v<detail::universal_value_type_t<ContT>, symbol_type>,

			std::nullptr_t> = nullptr>
			iterator find(const ContT & container,
				const_iterator offset = const_iterator{ nullptr })
		{
			return is_empty() ? end() :
				m_parent->begin()->find(container, offset);
		}

		//*********************************************************************



		/**********************************************************************
		* @brief Найти узел(ы)
		*
		* @details Перегрузка для работы с контейнерами содержащие
		* строко-подобные типы, например: std::vector<std::string>,
		* std::vector<const char*>, std::initializer_list<const char*>,
		* std::initializer_list<std::string>
		*
		* @tparam ContT - тип контейнера
		* @param container - список имён узлов, которые нужно найти
		* @param offset - итератор, с которого начать искать
		*
		* @return итератор на узел - если он был найден. В противном случае
		* возвращает итератор указывающий на end()
		**********************************************************************/
		template<typename ContT = std::initializer_list<string_wrapper>,
			std::enable_if_t<

			detail::is_has_begin_and_end_v<ContT>&&

			detail::is_has_value_type_v<ContT> &&

			!std::is_arithmetic_v<detail::value_type_t<ContT>> &&

			(std::is_constructible_v<string_wrapper,
				std::decay_t<
				detail::dereferenced_t<detail::const_iterator_t<ContT>>>> ||

				std::is_constructible_v<string_wrapper,
				detail::dereferenced_t<detail::const_iterator_t<ContT>>>) &&

			std::is_same_v<std::remove_const_t<
			detail::universal_value_type_t<ContT>>, symbol_type>,

			std::nullptr_t> = nullptr>
			iterator find(const ContT & container,
				const_iterator offset = const_iterator{ nullptr })
		{
			return is_empty() ? end() :
				m_parent->begin()->find(container, offset);
		}

		//*********************************************************************



		/**********************************************************************
		* @brief Найти узел
		*
		* @details Перегрузка для работы с указателями на строки, например:
		* const char*, const wchar_t*
		*
		* @tparam ContT - тип контейнера
		* @param container - имя узла, который нужно найти
		* @param offset - итератор, с которого начать искать
		*
		* @return константный итератор на узел - если он был найден. 
		* В противном случае возвращает константній итератор указывающий 
		* на end()
		**********************************************************************/
		template<typename ContT,
			std::enable_if_t<std::is_same_v<ContT, symbol_type>&&
			std::is_constructible_v<string_wrapper, const ContT*>,
			std::nullptr_t> = nullptr>
		const_iterator find(const ContT* container,
			const_iterator offset = const_iterator{ nullptr }) const
		{
			return is_empty() ? cend() :
				m_parent->begin()->find(container, offset);
		}

		//*********************************************************************



		/**********************************************************************
		* @brief Найти узел(ы)
		*
		* @details Перегрузка для работы с контейнерами содержащие простые
		* типы, например: std::string, std::vector<char>, std::string_view
		*
		* @tparam ContT - тип контейнера
		* @param container - список имён узлов, которые нужно найти
		* @param offset - итератор, с которого начать искать
		*
		* @return константный итератор на узел - если он был найден. 
		* В противном случае возвращает константній итератор указывающий 
		* на end()
		**********************************************************************/
		template<typename ContT,
			std::enable_if_t<

			detail::is_has_begin_and_end_v<ContT>&&

			detail::is_has_value_type_v<ContT>&&

			std::is_constructible_v<string_wrapper,
			std::add_pointer_t<std::remove_reference_t<
			detail::dereferenced_t<detail::const_iterator_t<ContT>>>>>&&

			std::is_same_v<detail::universal_value_type_t<ContT>, symbol_type>,

			std::nullptr_t> = nullptr>
			const_iterator find(const ContT & container,
				const_iterator offset = const_iterator{ nullptr }) const
		{
			return is_empty() ? cend() :
				m_parent->begin()->find(container, offset);
		}

		//*********************************************************************



		/**********************************************************************
		* @brief Найти узел(ы)
		*
		* @details Перегрузка для работы с контейнерами содержащие
		* строко-подобные типы, например: std::vector<std::string>,
		* std::vector<const char*>, std::initializer_list<const char*>,
		* std::initializer_list<std::string>
		*
		* @tparam ContT - тип контейнера
		* @param container - список имён узлов, которые нужно найти
		* @param offset - итератор, с которого начать искать
		*
		* @return константный итератор на узел - если он был найден. 
		* В противном случае возвращает константній итератор указывающий 
		* на end()
		**********************************************************************/
		template<typename ContT = std::initializer_list<string_wrapper>,
			std::enable_if_t<

			detail::is_has_begin_and_end_v<ContT>&&

			detail::is_has_value_type_v<ContT> &&

			!std::is_arithmetic_v<detail::value_type_t<ContT>> &&

			(std::is_constructible_v<string_wrapper,
				std::decay_t<
				detail::dereferenced_t<detail::const_iterator_t<ContT>>>> ||

				std::is_constructible_v<string_wrapper,
				detail::dereferenced_t<detail::const_iterator_t<ContT>>>) &&

			std::is_same_v<std::remove_const_t<
			detail::universal_value_type_t<ContT>>, symbol_type>,

			std::nullptr_t> = nullptr>
			const_iterator find(const ContT & container,
				const_iterator offset = const_iterator{ nullptr }) const
		{
			return is_empty() ? cend() :
				m_parent->begin()->find(container, offset);
		}

		//*********************************************************************
		/// @}



		/// @name Вспомагательные методы
		/// @{
		/**********************************************************************
		* @brief Удалить все узлы, включая root узел
		**********************************************************************/
		void clear() noexcept;

		/**********************************************************************
		* @brief Получить количество узлов, включая root узел
		* 
		* @return количество узлов
		**********************************************************************/
		size_type size() const noexcept;

		/**********************************************************************
		* @brief Проверить, пустой ли документа
		* 
		* @return true - если нет ниодного узла, включая root узла. В противном
		* случае false
		**********************************************************************/
		bool is_empty() const noexcept;
		operator bool() const noexcept;

		/**********************************************************************
		* @brief Обменять данные
		*
		* @param doc - правый Document
		**********************************************************************/
		void swap(Document& doc) noexcept;
		/// @}

	private:
		std::unique_ptr<node_type> m_parent;
		string_type m_encoding_type;
		float m_version;
	};

	//*************************************************************************



	//*************************************************************************
	//							DOCUMENT IMPLEMENTATION
	//*************************************************************************

	template<typename CharT>
	inline Document<CharT>::Document(float version,
		const string_type& encoding_type)
		:m_parent{ std::make_unique<typename Document<CharT>::node_type>(
			typename node_type::string_type{}) },
		m_encoding_type{ encoding_type },
		m_version{ version }
	{

	}

	//*************************************************************************

	template<typename CharT>
	inline Document<CharT>::Document(float version,
		string_type&& encoding_type)
		:m_parent{ std::make_unique<typename Document<CharT>::node_type>(
			typename node_type::string_type{}) },
		m_encoding_type{ std::move(encoding_type) },
		m_version{ version }
	{
	}

	//*************************************************************************

	template<typename CharT>
	inline Document<CharT>::Document(const Document& doc)
		:m_parent{ std::make_unique<typename Document<CharT>::node_type>(
			*doc.m_parent) },
		m_encoding_type{ doc.m_encoding_type },
		m_version{ doc.m_version }
	{

	}

	//*************************************************************************

	template<typename CharT>
	inline Document<CharT>& Document<CharT>::operator=(const Document& doc)
	{
		if (this != &doc)
		{
			*this = Document{ doc };
		}

		return *this;
	}

	//*************************************************************************

	template<typename CharT>
	inline Document<CharT>::Document(Document&& doc) noexcept
		:m_parent{ std::move(doc.m_parent) },
		m_encoding_type{ std::move(doc.m_encoding_type) },
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
	inline void Document<CharT>::root(const node_type& node)
	{
		if (m_parent->child_size())
		{
			m_parent->erase_child(0);
		}

		m_parent->add_child(node);
	}

	//*************************************************************************

	template<typename CharT>
	inline void Document<CharT>::root(node_type&& node)
	{
		if (m_parent->child_size())
		{
			m_parent->erase_child(0);
		}

		m_parent->add_child(std::move(node));
	}

	//*************************************************************************

	template<typename CharT>
	inline void Document<CharT>::root(node_pointer node)
	{
		if (m_parent->child_size())
		{
			m_parent->erase_child(0);
		}

		m_parent->add_child(std::move(node));
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Document<CharT>::node_type& Document<CharT>::root() &
	{
		if (is_empty())
		{
			throw std::out_of_range{
				"The document is empty! Can't get root node"};
		}

		return *m_parent->begin();
	}

	//*************************************************************************

	template<typename CharT>
	inline const typename Document<CharT>::node_type& Document<CharT>::root() 
		const &
	{
		if (is_empty())
		{
			throw std::out_of_range{
				"The document is empty! Can't get root node" };
		}

		return m_parent->begin();
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
	inline void Document<CharT>::set_encoding_type(
		const string_type& encoding_type)
	{
		m_encoding_type = encoding_type;
	}

	//*************************************************************************

	template<typename CharT>
	inline void Document<CharT>::set_encoding_type(string_type&& encoding_type)
		noexcept
	{
		m_encoding_type = std::move(encoding_type);
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Document<CharT>::string_wrapper 
		Document<CharT>::get_encoding_type() const noexcept
	{
		return string_wrapper{ m_encoding_type };
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
	inline void Document<CharT>::clear() noexcept
	{
		if (m_parent->child_size())
		{
			m_parent->erase_child(0);
		}
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Document<CharT>::size_type 
		Document<CharT>::size() const noexcept
	{
		return m_parent->size();
	}

	//*************************************************************************

	template<typename CharT>
	inline bool Document<CharT>::is_empty() const noexcept
	{
		return m_parent->child_size() ? false : true;
	}

	//*************************************************************************

	template<typename CharT>
	inline Document<CharT>::operator bool() const noexcept
	{
		return is_empty();
	}

	//*************************************************************************

	template<typename CharT>
	inline void Document<CharT>::swap(Document& doc) noexcept
	{
		using std::swap;

		swap(m_parent, doc.m_parent);
		swap(m_encoding_type, doc.m_encoding_type);
		swap(m_version, doc.m_version);
	}

	//*************************************************************************



	//*************************************************************************
	//						SUPPORT FUNCTIONS FOR DOCUMENT
	//*************************************************************************

	/**************************************************************************
	* @brief Проверить на валидность XML узлы
	* 
	* @ingroup general
	*
	* @param first - начало XML последовательности
	* @param last - конец XML последовательности
	*
	* @return диагностический итератор, который преобразуется в true, если была
	* найдена ошибка и false если ошибок не было найдено
	**************************************************************************/
	template<typename IterT, typename IterU = IterT>
	inline decltype(auto) is_correct(IterT first, IterU last)
	{
		using diagnostic_iterator =
			detail::Diagnostic_iterator<decltype(last)>;

		decltype(last) first_it = first;

		diagnostic_iterator result;

		for (; first_it != last; ++first_it)
		{
			if (!is_correct(*first_it))
			{
				result = diagnostic_iterator{ first_it, true };

				break;
			}
		}

		return result;
	}

	//*************************************************************************



	/**************************************************************************
	* @brief Проверить на валидность XML документ
	* 
	* @ingroup general
	*
	* @param document - XML документ
	*
	* @return диагностический итератор, который преобразуется в true, если была
	* найдена ошибка и false если ошибок не было найдено
	**************************************************************************/
	template<typename DocT>
	inline decltype(auto) is_correct(DocT&& document)
	{
		using diagnostic_iterator =
			detail::Diagnostic_iterator<decltype(document.end())>;

		diagnostic_iterator result =
			is_correct(document.begin(), document.end());

		return result;
	}

	//*************************************************************************

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



	/**************************************************************************
	* @brief Обменять данные двух Document
	*
	* @tparam CharT - тип символов
	*
	* @param lhs - первый Document
	* @param rhs - второй Document
	**************************************************************************/
	template<typename CharT>
	inline void swap(Document<CharT>& lhs, Document<CharT>& rhs) noexcept
	{
		lhs.swap(rhs);
	}

	//*************************************************************************

} // namespace XMLB



namespace XMLB { namespace detail {

	//*************************************************************************
	//				SUPPORT FUNCTIONS FOR SAVE AND LOAD FUNCTIONS
	//*************************************************************************

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

		parser.add_functor(Start_state{}, Start_functor{});
		parser.add_functor(Open_tag_state{}, Open_tag_functor{});
		parser.add_functor(Close_tag_state{}, Close_tag_functor{});
		parser.add_functor(Last_tag_state{}, Last_tag_functor{});
		parser.add_functor(Single_tag_state{}, Single_tag_functor{});
		parser.add_functor(Name_tag_state{}, Name_tag_functor{});
		parser.add_functor(Value_tag_state{}, Value_tag_functor{});
		parser.add_functor(Attribute_name_state{}, Attribute_name_functor{});
		parser.add_functor(Attribute_value_state{}, Attribute_value_functor{});
		parser.add_functor(Comment_state{}, Comment_functor{});
		parser.add_functor(Document_info_state{}, Document_info_functor{});

		return parser;
	}

	//*************************************************************************

	template<typename StringT, typename ContainerT, std::enable_if_t<
		std::is_same_v<std::decay_t<StringT>, 
		std::basic_string<value_type_t<std::decay_t<StringT>>>> &&

		std::is_same_v<value_type_t<std::decay_t<ContainerT>>, 
		std::basic_string<universal_value_type_t<std::decay_t<ContainerT>>>>,

		std::nullptr_t> = nullptr>
	inline decltype(auto) create_node(StringT&& name, StringT&& value,
		ContainerT&& attribute_names, ContainerT&& attribute_values)
	{
		using symbol_type = std::decay_t<typename StringT::value_type>;
		using node_type = Node<symbol_type>;
		using node_pointer = typename Node<symbol_type>::Ptr;
		using node_attribute = typename Node<symbol_type>::attribute_type;
		using string_type = typename Node<symbol_type>::string_type;

		node_pointer result = std::make_unique<node_type>(name, value);

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

	template<typename StringT, typename ContainerT, 
		std::enable_if_t<
		std::is_same_v<std::decay_t<StringT>,
		std::basic_string_view<value_type_t<std::decay_t<StringT>>>>&&

		std::is_same_v<value_type_t<std::decay_t<ContainerT>>,
		std::basic_string_view<
		universal_value_type_t<std::decay_t<ContainerT>>>>,
		
		std::nullptr_t> = nullptr>
	inline decltype(auto) create_node(StringT&& name, StringT&& value,
		ContainerT&& attribute_names, ContainerT&& attribute_values)
	{
		using symbol_type = std::decay_t<typename StringT::value_type>;
		using node_type = Node<symbol_type>;
		using node_pointer = typename Node<symbol_type>::Ptr;
		using node_attribute = typename Node<symbol_type>::attribute_type;
		using string_type = typename Node<symbol_type>::string_type;

		node_pointer result = std::make_unique<node_type>(
			string_type{ name }, string_type{ value });

		auto attr_names_beg = attribute_names.begin();
		auto attr_names_end = attribute_names.end();

		auto attr_values_beg = attribute_values.begin();
		auto attr_values_end = attribute_values.end();

		for (; attr_names_beg != attr_names_end &&
			attr_values_beg != attr_values_end;
			++attr_names_beg, ++attr_values_beg)
		{
			result->add_attribute(node_attribute{
					string_type{ *attr_names_beg }, 
					string_type{ *attr_values_beg} });
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

		using string_type = typename Document<symbol_type>::string_type;

		document_pointer result{ nullptr };

		if (doc_info.size())
		{
			static constexpr unsigned int only_doc_version = 1;
			static constexpr unsigned int doc_version_and_encoding = 2;

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
				auto doc_version = to_float<symbol_type>(
					*source_doc_info.attribute_values.begin());

				auto&& doc_encoding =
					*(++source_doc_info.attribute_values.begin());

				//Вот тут нужно убрать string_type. Нужно выбрать лучший
				//лучший вариант решения
				result = std::move(std::make_unique<document_type>(
					doc_version, string_type(std::move(doc_encoding))));
			}
		}

		if (!result)
		{
			result = std::move(std::make_unique<document_type>());
		}

		return result;
	}

}} // namespace XMLB::detail



namespace XMLB
{

	//*************************************************************************
	//						SAVE AND LOAD FUNCTIONS
	//*************************************************************************

	/// @name Функции сохранения
	/// @{
	/**************************************************************************
	* @brief Скопировать данные из последовательности XML узлов в буфер
	* 
	* @ingroup general
	* 
	* @details Скопировать и сохранить данные в буфер. Данная функция не
	* проверяет последовательность на корректность!
	*
	* @internal
	* @details Данная функция для случаев, когда IterT - итератор с любой
	* категорией или указатель и value_type итератора или указателя IterT -
	* простой символьный тип. Данная функция не проверяет корректность XML
	* узлов
	* @endinternal
	* 
	* @tparam CharT - тип символов
	* @tparam IterT - тип выходного(буфера) итератора
	* @tparam DecorT - тип декоратора
	* 
	* @param first - начальный итератора на XML узел
	* @param last - конечный итератор на XML узел
	* @param out - итератор, в который будут заноситься данные
	* 
	* @todo Нужно оптимизировать данную функцию
	**************************************************************************/
	template<typename CharT, typename IterT,
		typename DecorT = detail::default_decorator<CharT>,
		std::enable_if_t<
		detail::is_back_inserter_iterator_to_symbol_v<IterT, CharT> ||

		detail::is_output_iterator_to_symbol_v<IterT, CharT> ||

		detail::is_input_derived_iterator_or_pointer_to_symbol_v<IterT, CharT>,

		std::nullptr_t> = nullptr>
	inline void save_to(detail::Node_const_iterator<Node<CharT>> first,
		detail::Node_const_iterator<Node<CharT>> last,
		IterT out,
		const DecorT& decorator = DecorT{})
	{
		if (first == last)
		{
			return;
		}

		using symbol_type = CharT;
		using string_type = typename Node<symbol_type>::string_type;
		using string_wrapper = typename Node<symbol_type>::string_wrapper;
		using const_iterator = typename Node<symbol_type>::const_iterator;

		const auto kFill = decorator.fill_symbol;
		const auto kSpace = decorator.white_space_symbol;
		const auto kOpen_tag = decorator.open_tag_symbol;
		const auto kClose_tag = decorator.close_tag_symbol;
		const auto kSingle_tag = decorator.single_tag_symbol;
		const auto kLast_tag = decorator.last_tag_symbol;
		const auto kOpen_attr = decorator.open_attribute_symbol;
		const auto kClose_attr = decorator.close_attribute_symbol;
		const auto kEqual_attr = decorator.equal_attribute_symbol;
		const auto kLine_break = decorator.line_break_symbol;

		//Контейнер итераторова, чтобы правильно закрывать теги с потомками
		std::stack<const_iterator> node_groups;

		for (; first != last; ++first)
		{
			//Проверяем, пустой ли стэк с итераторома-группами
			if (!node_groups.empty())
			{
				//Если текущее значение табуляции равно с последним
				//добавленным в стэк итератором, то заносим в файл
				//закрытие предыдущего тега
				while (first.get_offset() <= node_groups.top().get_offset())
				{
					//Записываем нужное количество табов для тега
					detail::copy_symbol_n(kFill,
						node_groups.top().get_offset(), out);

					*out = kOpen_tag; ++out;
					*out = kLast_tag; ++out;
					detail::copy_symbol_from_container(
						string_wrapper{ node_groups.top()->get_name() }, out);
					*out = kClose_tag; ++out;
					*out = kLine_break; ++out;

					node_groups.pop();
				}
			}

			//Если у текущего тега есть под-теги, то добавляем его
			//в стек в качестве итератора-группы
			if (first->child_size())
			{
				node_groups.push(first);
			}

			//Устанавливаем нужное количество табов для тега
			detail::copy_symbol_n(kFill, first.get_offset(), out);

			//Заносим в файл сам тег и его информацию
			*out = kOpen_tag; ++out;
			detail::copy_symbol_from_container(
				string_wrapper{ first->get_name() }, out);

			//Если у XML тега есть аттрибуты, то записываем в файл
			//их имена и значения
			if (first->attr_size())
			{
				for (auto at_it = first->attr_begin(),
					at_end = first->attr_end();
					at_it != at_end;
					++at_it)
				{
					*out = kSpace; ++out;

					detail::copy_symbol_from_container(
						string_wrapper{ at_it->name }, out);
					*out = kEqual_attr; ++out;

					*out = kOpen_attr; ++out;
					detail::copy_symbol_from_container(
						string_wrapper{ at_it->value }, out);
					*out = kClose_attr; ++out;
				}
			}

			//Если у XML тега нет дочерних узлов и есть значение, то
			//записываем это значение и доп. символы по шаблону
			if (!first->child_size() && first->get_value().size())
			{
				*out = kClose_tag; ++out;
				detail::copy_symbol_from_container(
					string_wrapper{ first->get_value() }, out);

				*out = kOpen_tag; ++out;
				*out = kLast_tag; ++out;

				detail::copy_symbol_from_container(
					string_wrapper{ first->get_name() }, out);
				*out = kClose_tag; ++out;
			}
			//Если у XML тега нет дочерних узлов и нет значения, то
			//записываем доп. символы по шаблону одиночного XML тега
			else if (!first->child_size())
			{
				*out = kSpace; ++out;
				*out = kSingle_tag; ++out;
				*out = kClose_tag; ++out;
			}
			//В противном случае, просто записываем в файл символ
			//закрытия XML тега
			else
			{
				*out = kClose_tag; ++out;
			}

			*out = kLine_break; ++out;
		}

		//Если остались ещё незакрытые теги, то заносим в файл их закрытие
		while (!node_groups.empty())
		{
			//Записываем нужное количество табов для тега
			detail::copy_symbol_n(kFill,
				node_groups.top().get_offset(), out);

			*out = kOpen_tag; ++out;
			*out = kLast_tag; ++out;
			detail::copy_symbol_from_container(
				string_wrapper{ node_groups.top()->get_name() }, out);
			*out = kClose_tag; ++out;
			*out = kLine_break; ++out;

			node_groups.pop();
		}
	}

	//*************************************************************************



	/**************************************************************************
	* @brief Скопировать данные из XML документа в буфер
	* 
	* @ingroup general
	* 
	* @details Скопировать и сохранить данные в буфер. Данная функция не
	* проверяет документ на корректность!
	*
	* @internal
	* @details Данная функция для случаев, когда IterT - итератор с любой
	* категорией или указатель и value_type итератора или указателя IterT -
	* простой символьный тип. Данная функция не проверяет корректность XML
	* документа
	* @endinternal
	* 
	* @tparam CharT - тип символов
	* @tparam IterT - тип выходного(буфера) итератора
	* @tparam DecorT - тип декоратора
	*
	* @param document - XML Документа
	* @param out - итератор, в который будут заноситься данные
	* 
	* @todo Нужно оптимизировать данную функцию
	**************************************************************************/
	template<typename CharT, typename IterT,
		typename DecorT = detail::default_decorator<CharT>,
		std::enable_if_t<
		detail::is_back_inserter_iterator_to_symbol_v<IterT, CharT> ||

		detail::is_output_iterator_to_symbol_v<IterT, CharT> ||

		detail::is_input_derived_iterator_or_pointer_to_symbol_v<IterT, CharT>,

		std::nullptr_t> = nullptr>
	inline void save_to(const Document<CharT>& document, IterT out,
		const DecorT& decorator = DecorT{})
	{
		using std::begin;
		using std::end;

		using symbol_type = CharT;
		using string_type = typename Node<symbol_type>::string_type;
		using string_wrapper = typename Node<symbol_type>::string_wrapper;

		const auto kSpace = decorator.white_space_symbol;
		const auto kOpen_tag = decorator.open_tag_symbol;
		const auto kClose_tag = decorator.close_tag_symbol;
		const auto kOpen_attr = decorator.open_attribute_symbol;
		const auto kClose_attr = decorator.close_attribute_symbol;
		const auto kEqual_attr = decorator.equal_attribute_symbol;
		const auto kLine_break = decorator.line_break_symbol;
		const auto kDoc_info = decorator.doc_info_symbol;
		const auto kDoc_last_info = decorator.doc_info_last_symbol;

		*out = kOpen_tag; ++out;
		*out = kDoc_info; ++out;
		detail::copy_symbol_from_container(
			detail::to_string<symbol_type>('x','m','l'), out);

		*out = kSpace; ++out;
		detail::copy_symbol_from_container(
			detail::to_string<symbol_type>('v', 'e', 'r', 's', 'i', 'o', 'n'), 
			out);
		*out = kEqual_attr; ++out;
		*out = kOpen_attr; ++out;
		detail::copy_symbol_from_container(
			cut_doc_version<symbol_type>(document.get_version()), out);
		*out = kClose_attr; ++out;

		*out = kSpace; ++out;
		detail::copy_symbol_from_container(
			detail::to_string<symbol_type>(
				'e', 'n', 'c', 'o', 'd', 'i', 'n', 'g'), out);
		*out = kEqual_attr; ++out;
		*out = kOpen_attr; ++out;
		detail::copy_symbol_from_container(document.get_encoding_type(), out);
		*out = kClose_attr; ++out;

		*out = kDoc_last_info; ++out;
		*out = kClose_tag; ++out;
		*out = kLine_break; ++out;

		save_to(begin(document), end(document), out, decorator);
	}

	//*************************************************************************



	/**************************************************************************
	* @overload void save_to(detail::Node_const_iterator<Node<CharT>> first,
	* detail::Node_const_iterator<Node<CharT>> last, IterT out,
	* const DecorT& decorator = DecorT{})
	* 
	* @brief Скопировать данные из последовательности XML узлов в буфер
	* 
	* @ingroup general
	* 
	* @details Скопировать и сохранить данные в буфер. Данная функция не
	* проверяет последовательность на корректность!
	*
	* @internal
	* @details Данная функция для случаев, когда IterT - итератор с любой
	* категорией или указатель и value_type итератора или указателя IterT -
	* строко-подобный контейнер. Данная функция не проверяет корректность XML
	* документа
	* @endinternal
	* 
	* @tparam CharT - тип символов
	* @tparam IterT - тип выходного(буфера) итератора
	* @tparam DecorT - тип декоратора
	*
	* @param first - начальный итератора на XML узел
	* @param last - конечный итератор на XML узел
	* @param out - итератор, в который будут заноситься данные
	* 
	* @todo Нужно оптимизировать данную функцию
	**************************************************************************/
	template<typename CharT, typename IterT,
		typename DecorT = detail::default_decorator<CharT>,
		std::enable_if_t<

		detail::is_back_inserter_iterator_to_container_v<
		IterT, typename Document<CharT>::string_type> ||

		detail::is_output_iterator_to_container_v<
		IterT, typename Document<CharT>::string_type> ||

		detail::is_input_derived_iterator_or_pointer_to_container_v<
		IterT, typename Document<CharT>::string_type>,

		std::nullptr_t> = nullptr>
	inline void save_to(detail::Node_const_iterator<Node<CharT>> first,
		detail::Node_const_iterator<Node<CharT>> last,
		IterT out,
		const DecorT& decorator = DecorT{})
	{
		using symbol_type = CharT;
		using string_type = typename Node<symbol_type>::string_type;
		using string_wrapper = typename Node<symbol_type>::string_wrapper;
		using const_iterator = typename Node<symbol_type>::const_iterator;
		using size_type = typename Node<symbol_type>::string_type::size_type;

		if (first == last)
		{
			return;
		}

		const auto kFill = decorator.fill_symbol;
		const auto kSpace = decorator.white_space_symbol;
		const auto kOpen_tag = decorator.open_tag_symbol;
		const auto kClose_tag = decorator.close_tag_symbol;
		const auto kSingle_tag = decorator.single_tag_symbol;
		const auto kLast_tag = decorator.last_tag_symbol;
		const auto kOpen_attr = decorator.open_attribute_symbol;
		const auto kClose_attr = decorator.close_attribute_symbol;
		const auto kEqual_attr = decorator.equal_attribute_symbol;
		const auto kLine_break = decorator.line_break_symbol;

		//Контейнер итераторова, чтобы правильно закрывать теги с потомками
		std::stack<const_iterator> node_groups;

		for (; first != last; ++first)
		{
			//Проверяем, пустой ли стэк с итераторома-группами
			if (!node_groups.empty())
			{
				//Если текущее значение табуляции равно с последним
				//добавленным в стэк итератором, то заносим в файл
				//закрытие предыдущего тега
				while (first.get_offset() <= node_groups.top().get_offset())
				{
					//Количество дополнительных символов в выходной строке
					size_type additional_symbol_count = 4;

					string_type output_string{};

					//Резервируем необходимый размер для выходноый строки
					output_string.reserve(
						node_groups.top().get_offset() +
						additional_symbol_count +
						node_groups.top()->get_name().size());

					//Записываем нужное количество табов для тега
					output_string +=
						string_type(node_groups.top().get_offset(), kFill);

					output_string.push_back(kOpen_tag);
					output_string.push_back(kLast_tag);
					output_string += node_groups.top()->get_name();
					output_string.push_back(kClose_tag);
					output_string.push_back(kLine_break);

					*out = std::move(output_string);
					++out;

					node_groups.pop();
				}
			}



			//Если у текущего тега есть под-теги, то добавляем его
			//в стек в качестве итератора-группы
			if (first->child_size())
			{
				node_groups.push(first);
			}



			//Количество дополнительных символов в выходной строке
			size_type additional_symbol_count = 2;

			//Суммируем количество необходимых символов для резерва в 
			//выходной строке
			if (first->attr_size())
			{
				for (auto at_it = first->attr_begin(),
					at_end = first->attr_end();
					at_it != at_end;
					++at_it)
				{
					additional_symbol_count += 4;
					additional_symbol_count += at_it->name.size();
					additional_symbol_count += at_it->value.size();
				}
			}

			//Продолжаем суммировать необходимое количество символов
			if (!first->child_size() && first->get_value().size())
			{
				additional_symbol_count += 4;
				additional_symbol_count += first->get_value().size();
				additional_symbol_count += first->get_name().size();
			}
			else if (!first->child_size())
			{
				additional_symbol_count += 3;
			}
			else
			{
				additional_symbol_count += 1;
			}

			string_type output_string{};

			output_string.reserve(first.get_offset() + 
				additional_symbol_count + first->get_name().size());



			//Устанавливаем нужное количество табов для тега
			output_string += string_type(first.get_offset(), kFill);

			//Заносим в файл сам тег и его информацию
			output_string.push_back(kOpen_tag);
			output_string += first->get_name();

			//Если у XML тега есть аттрибуты, то записываем в файл
			//их имена и значения
			if (first->attr_size())
			{
				for (auto at_it = first->attr_begin(),
					at_end = first->attr_end();
					at_it != at_end;
					++at_it)
				{
					output_string.push_back(kSpace);

					output_string += at_it->name;
					output_string.push_back(kEqual_attr);

					output_string.push_back(kOpen_attr);
					output_string += at_it->value;
					output_string.push_back(kClose_attr);
				}
			}

			//Если у XML тега нет дочерних узлов и есть значение, то
			//записываем это значение и доп. символы по шаблону
			if (!first->child_size() && first->get_value().size())
			{
				output_string.push_back(kClose_tag);
				output_string += first->get_value();

				output_string.push_back(kOpen_tag);
				output_string.push_back(kLast_tag);

				output_string += first->get_name();
				output_string.push_back(kClose_tag);
			}
			//Если у XML тега нет дочерних узлов и нет значения, то
			//записываем доп. символы по шаблону одиночного XML тега
			else if (!first->child_size())
			{
				output_string.push_back(kSpace);
				output_string.push_back(kSingle_tag);
				output_string.push_back(kClose_tag);
			}
			//В противном случае, просто записываем в файл символ
			//закрытия XML тега
			else
			{
				output_string.push_back(kClose_tag);
			}

			output_string.push_back(kLine_break);

			*out = std::move(output_string);
			++out;
		}



		//Если остались ещё незакрытые теги, то заносим в файл их закрытие
		while (!node_groups.empty())
		{
			//Количество дополнительных символов в выходной строке
			size_type additional_symbol_count = 4;

			string_type output_string{};

			//Резервируем необходимый размер для выходноый строки
			output_string.reserve(
				node_groups.top().get_offset() +
				additional_symbol_count +
				node_groups.top()->get_name().size());

			//Записываем нужное количество табов для тега
			output_string +=
				string_type(node_groups.top().get_offset(), kFill);

			output_string.push_back(kOpen_tag);
			output_string.push_back(kLast_tag);
			output_string += node_groups.top()->get_name();
			output_string.push_back(kClose_tag);
			output_string.push_back(kLine_break);

			*out = std::move(output_string);
			++out;

			node_groups.pop();
		}
	}

	//*************************************************************************



	/**************************************************************************
	* @overload void save_to(const Document<CharT>& document, IterT out,
	* const DecorT& decorator = DecorT{})
	* 
	* @brief Скопировать данные из XML документа в буфер
	* 
	* @ingroup general
	* 
	* @details Скопировать и сохранить данные в буфер. Данная функция не
	* проверяет документ на корректность!
	*
	* @internal
	* @details Данная функция для случаев, когда IterT - итератор с любой
	* категорией или указатель и value_type итератора или указателя IterT -
	* строко-подобный контейнер. Данная функция не проверяет корректность XML
	* документа
	* @endinternal
	* 
	* @tparam CharT - тип символов
	* @tparam IterT - тип выходного(буфера) итератора
	* @tparam DecorT - тип декоратора
	*
	* @param document - XML Документа
	* @param out - итератор, в который будут заноситься данные
	* 
	* @todo Нужно оптимизировать данную функцию
	**************************************************************************/
	template<typename CharT, typename IterT,
		typename DecorT = detail::default_decorator<CharT>,
		std::enable_if_t<

		detail::is_back_inserter_iterator_to_container_v<
		IterT, typename Document<CharT>::string_type> ||

		detail::is_output_iterator_to_container_v<
		IterT, typename Document<CharT>::string_type> ||

		detail::is_input_derived_iterator_or_pointer_to_container_v<
		IterT, typename Document<CharT>::string_type>,

		std::nullptr_t> = nullptr>
	inline void save_to(const Document<CharT>& document, IterT out,
		const DecorT& decorator = DecorT{})
	{
		using std::begin;
		using std::end;

		using symbol_type = CharT;
		using string_type = typename Node<symbol_type>::string_type;
		using string_wrapper = typename Node<symbol_type>::string_wrapper;
		using size_type = typename Node<symbol_type>::string_type::size_type;

		const auto kSpace = decorator.white_space_symbol;
		const auto kOpen_tag = decorator.open_tag_symbol;
		const auto kClose_tag = decorator.close_tag_symbol;
		const auto kOpen_attr = decorator.open_attribute_symbol;
		const auto kClose_attr = decorator.close_attribute_symbol;
		const auto kEqual_attr = decorator.equal_attribute_symbol;
		const auto kLine_break = decorator.line_break_symbol;
		const auto kDoc_info = decorator.doc_info_symbol;
		const auto kDoc_last_info = decorator.doc_info_last_symbol;

		size_type additional_symbol_count = 13;

		string_type kXml = detail::to_string<symbol_type>('x', 'm', 'l');
		string_type kVersion = 
			detail::to_string<symbol_type>('v', 'e', 'r', 's', 'i', 'o', 'n');
		string_type kEncoding = detail::to_string<symbol_type>(
			'e', 'n', 'c', 'o', 'd', 'i', 'n', 'g');
		string_type doc_version = cut_doc_version<symbol_type>(
			document.get_version());

		string_type output_string{};

		//Резервируем необходимый размер для выходноый строки
		output_string.reserve(additional_symbol_count + kXml.size() + 
			kVersion.size() + kEncoding.size() + doc_version.size() + 
			document.get_encoding_type().size());

		output_string.push_back(kOpen_tag);
		output_string.push_back(kDoc_info);
		output_string += kXml;

		output_string.push_back(kSpace);
		output_string += kVersion;
		output_string.push_back(kEqual_attr);
		output_string.push_back(kOpen_attr);
		output_string += doc_version;
		output_string.push_back(kClose_attr);

		output_string.push_back(kSpace);
		output_string += kEncoding;
		output_string.push_back(kEqual_attr);
		output_string.push_back(kOpen_attr);
		output_string += document.get_encoding_type();
		output_string.push_back(kClose_attr);

		output_string.push_back(kDoc_last_info);
		output_string.push_back(kClose_tag);
		output_string.push_back(kLine_break);

		*out = std::move(output_string);
		++out;

		save_to(begin(document), end(document), out, decorator);
	}

	//*************************************************************************
	/// @}



	/// @name Функции загрузки
	/// @{
	/**************************************************************************
	* @brief Прочитать и выполнить синтаксический анализ XML даннх из 
	* последовательности [first, last)
	* 
	* @ingroup general
	*
	* @internal
	* @details Данная функция для случаев, когда IterT - итератор с категорией
	* равной input_iterator и value_type итератора IterT - простой символьный
	* тип.
	* @endinternal
	*
	* @tparam IterT - тип итераторов входной последовательности
	* @tparam DecorT - тип декоратора
	* @tparam ParserT - тип парсера(синтаксического анализатора)
	* @tparam CharT - тип символов
	* 
	* @param first - начало последовательности
	* @param last - конец последовательности
	* @param decorator - декоратор
	* @param parser - парсер(синтаксический анализатор)
	*
	* @return в случае успешного завершения функции - документ с XML узлами. В
	* противном случае, документ со значением nullptr
	* 
	* @todo Нужно оптимизировать данную функцию
	**************************************************************************/
	template<typename IterT,
		typename DecorT =
		detail::default_decorator<
		std::decay_t<detail::type_for_decorator_t<IterT>>>,

		typename ParserT = detail::default_parser<
		detail::symbol_type_t<DecorT>,
		DecorT,
		detail::default_data_controller<detail::symbol_type_t<DecorT>>>,

		typename CharT = detail::symbol_type_t<DecorT>,

		std::enable_if_t<detail::is_input_iterator_and_char_v<
		IterT, DecorT, std::decay_t<ParserT>> &&
		std::is_same_v<detail::symbol_type_t<DecorT>, CharT>,

		std::nullptr_t> = nullptr>
	[[nodiscard]] inline typename Document<CharT>::Ptr load_from(
		IterT first, 
		IterT last,
		const DecorT& decorator = DecorT{},
		ParserT&& parser = detail::create_default_parser<ParserT>())
	{
		using symbol_type = detail::symbol_type_t<DecorT>;
		using document_type = Document<symbol_type>;
		using document_pointer = typename Document<symbol_type>::Ptr;

		document_pointer result{ nullptr };

		detail::load_from_iterator_and_char(first, last, decorator, parser);

		auto&& general_node =
			detail::create_root_node_from_container(parser.get_tags());

		if (general_node)
		{
			result = std::move(
				detail::create_doc_from_info(parser.get_doc_info()));

			result->root(std::move(general_node));
		}

		return result;
	}

	//*************************************************************************



	/**************************************************************************
	* @overload Document<CharT>::Ptr load_from(IterT first, IterT last, 
	* const DecorT& decorator = DecorT{}, 
	* ParserT&& parser = detail::create_default_parser<ParserT>())
	* 
	* @brief Прочитать и выполнить синтаксический анализ XML даннх из 
	* последовательности [first, last)
	* 
	* @ingroup general
	*
	* @internal
	* @details Данная функция для случаев, когда IterT - итератор с категорией
	* равной input_iterator и value_type итератора IterT - контейнер, у
	* которого есть итераторы, и методы begin(), end().
	* @endinternal
	*
	* @tparam IterT - тип итераторов входной последовательности
	* @tparam DecorT - тип декоратора
	* @tparam ParserT - тип парсера(синтаксического анализатора)
	* @tparam CharT - тип символов
	* 
	* @param first - начало последовательности
	* @param last - конец последовательности
	* @param decorator - декоратор
	* @param parser - парсер(синтаксический анализатор)
	*
	* @return в случае успешного завершения функции - документ с XML узлами. В
	* противном случае, документ со значением nullptr
	* 
	* @todo Нужно оптимизировать данную функцию
	**************************************************************************/
	template<typename IterT,

		typename DecorT =
		detail::default_decorator<
		std::decay_t<detail::type_for_decorator_t<IterT>>>,

		typename ParserT = detail::default_parser<
		detail::symbol_type_t<DecorT>,
		DecorT,
		detail::default_data_controller<detail::symbol_type_t<DecorT>>>,

		typename CharT = detail::symbol_type_t<DecorT>,

		std::enable_if_t<detail::is_input_iterator_and_container_v<
		IterT, DecorT, std::decay_t<ParserT>> &&
		std::is_same_v<detail::symbol_type_t<DecorT>, CharT>,

		std::nullptr_t> = nullptr>
	[[nodiscard]] inline typename Document<CharT>::Ptr load_from(
		IterT first, 
		IterT last,
		const DecorT& decorator = DecorT{},
		ParserT&& parser = detail::create_default_parser<ParserT>())
	{
		using symbol_type = detail::symbol_type_t<DecorT>;
		using document_type = Document<symbol_type>;
		using document_pointer = typename Document<symbol_type>::Ptr;

		document_pointer result{ nullptr };

		detail::load_from_iterator_and_container(
			first, last, decorator, parser);

		auto&& general_node =
			detail::create_root_node_from_container(parser.get_tags());

		if (general_node)
		{
			result = std::move(
				detail::create_doc_from_info(parser.get_doc_info()));

			result->root(std::move(general_node));
		}

		return result;
	}

	//*************************************************************************



	/**************************************************************************
	* @overload Document<CharT>::Ptr load_from(IterT first, IterT last, 
	* const DecorT& decorator = DecorT{}, 
	* ParserT&& parser = detail::create_default_parser<ParserT>())
	* 
	* @brief Прочитать и выполнить синтаксический анализ XML даннх из 
	* последовательности [first, last)
	* 
	* @ingroup general
	*
	* @internal
	* @details Данная функция для случаев, когда IterT - итератор с категорией
	* равной или выше forward_iterator, или указатель и value_type итератора
	* IterT - простой символьный тип
	* @endinternal
	* 
	* @tparam IterT - тип итераторов входной последовательности
	* @tparam DecorT - тип декоратора
	* @tparam ParserT - тип парсера(синтаксического анализатора)
	* @tparam CharT - тип символов
	* 
	* @param first - начало последовательности
	* @param last - конец последовательности
	* @param decorator - декоратор
	* @param parser - парсер(синтаксический анализатор)
	*
	* @return в случае успешного завершения функции - документ с XML узлами. В
	* противном случае, документ со значением nullptr
	* 
	* @todo Нужно оптимизировать данную функцию
	**************************************************************************/
	template<typename IterT,

		typename DecorT =
		detail::default_decorator<
		std::decay_t<detail::type_for_decorator_t<IterT>>>,

		typename ParserT = detail::default_parser<
		const detail::symbol_type_t<DecorT>&,
		DecorT,
		detail::default_data_controller<const detail::symbol_type_t<DecorT>*>>,

		typename CharT = detail::symbol_type_t<DecorT>,

		std::enable_if_t<detail::is_forward_iterator_or_pointer_and_char_v<
		IterT, DecorT, std::decay_t<ParserT>> &&
		std::is_same_v<detail::symbol_type_t<DecorT>, CharT>,

		std::nullptr_t> = nullptr>
	[[nodiscard]] inline typename Document<CharT>::Ptr load_from(
		IterT first, 
		IterT last,
		const DecorT & decorator = DecorT{},
		ParserT&& parser = detail::create_default_parser<ParserT>())
	{
		using symbol_type = detail::symbol_type_t<DecorT>;
		using document_type = Document<symbol_type>;
		using document_pointer = typename Document<symbol_type>::Ptr;

		document_pointer result{ nullptr };

		detail::load_from_iterator_and_char(first, last, decorator, parser);

		auto&& general_node =
			detail::create_root_node_from_container(parser.get_tags());

		if (general_node)
		{
			result = std::move(
				detail::create_doc_from_info(parser.get_doc_info()));

			result->root(std::move(general_node));
		}

		return result;
	}

	//*************************************************************************



	/**************************************************************************
	* @overload Document<CharT>::Ptr load_from(IterT first, IterT last, 
	* const DecorT& decorator = DecorT{}, 
	* ParserT&& parser = detail::create_default_parser<ParserT>())
	* 
	* @brief Прочитать и выполнить синтаксический анализ XML даннх из 
	* последовательности [first, last)
	* 
	* @ingroup general
	*
	* @internal
	* @details Данная функция для случаев, когда IterT - итератор с категорией
	* равной или выше forward_iterator, или указатель и value_type итератора
	* IterT - контейнер, у которого есть итераторы, и методы begin(), end().
	* @endinternal
	* 
	* @tparam IterT - тип итераторов входной последовательности
	* @tparam DecorT - тип декоратора
	* @tparam ParserT - тип парсера(синтаксического анализатора)
	* @tparam CharT - тип символов
	* 
	* @param first - начало последовательности
	* @param last - конец последовательности
	* @param decorator - декоратор
	* @param parser - парсер(синтаксический анализатор)
	*
	* @return в случае успешного завершения функции - документ с XML узлами. В
	* противном случае, документ со значением nullptr
	* 
	* @todo Нужно оптимизировать данную функцию
	**************************************************************************/
	template<typename IterT,

		typename DecorT =
		detail::default_decorator<
		std::decay_t<detail::type_for_decorator_t<IterT>>>,

		typename ParserT = detail::default_parser<
		const detail::symbol_type_t<DecorT>&,
		DecorT,
		detail::default_data_controller<const detail::symbol_type_t<DecorT>*>>,

		typename CharT = detail::symbol_type_t<DecorT>,

		std::enable_if_t<
		detail::is_forward_iterator_or_pointer_and_container_v<
		IterT, DecorT, std::decay_t<ParserT>> &&
		std::is_same_v<detail::symbol_type_t<DecorT>, CharT>,

		std::nullptr_t> = nullptr>
	[[nodiscard]] inline typename Document<CharT>::Ptr load_from(
		IterT first, 
		IterT last,
		const DecorT & decorator = DecorT{},
		ParserT&& parser = detail::create_default_parser<ParserT>())
	{
		using symbol_type = detail::symbol_type_t<DecorT>;
		using document_type = Document<symbol_type>;
		using document_pointer = typename Document<symbol_type>::Ptr;

		document_pointer result{ nullptr };

		detail::load_from_iterator_and_container(first, last,
			decorator, parser);

		auto&& general_node =
			detail::create_root_node_from_container(parser.get_tags());

		if (general_node)
		{
			result = std::move(
				detail::create_doc_from_info(parser.get_doc_info()));

			result->root(std::move(general_node));
		}

		return result;
	}

	//*************************************************************************
	/// @}

} // namespace XMLB

#endif // !XMLB_DOCUMENT_H