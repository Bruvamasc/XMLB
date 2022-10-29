/******************************************************************************
* @file
* Данный файл объявляет и определяет структуру с данными для создания XML узлов
* из прочитанных данных, и классы контроллеры данных (предоставляют интерфейс
* для функторов парсера)
* 
* @todo Нужно добавить хранение XMLschema и подумать над уменьшением размера
* объекта хранения информации о документе
*
* @author Bruvamasc
* @date   2022-08-25
*
******************************************************************************/

#ifndef XMLB_PARSER_DATA_CONTROLLER_H
#define XMLB_PARSER_DATA_CONTROLLER_H

#include <string>
#include <string_view>
#include <list>
#include <stack>

namespace XMLB { namespace detail {

	/**************************************************************************
	* @brief Структура для хранения временных данных. Используется для
	* создания XML узлов
	*
	* @details Базовая, общая структура. Для хранения содержимого,
	* использует строко-подобные контейнеры
	**************************************************************************/
	template<typename CharT>
	struct Tag_data
	{
		using symbol_type = CharT;
		using symbol_pointer = std::nullptr_t;

		using string_type = std::basic_string<symbol_type>;
		using string_wrapper = std::basic_string_view<symbol_type>;

		using attr_container_type = std::list<string_type>;
		using child_container_type = std::list<Tag_data<symbol_type>>;



		string_type name;
		string_type value;
		attr_container_type attribute_names;
		attr_container_type attribute_values;
		child_container_type childs;
	};

	//*************************************************************************



	/**************************************************************************
	* @brief Структура для хранения временных данных. Используется для
	* создания XML узлов
	*
	* @details Специализация для forward и выше итераторов и указателей.
	* Соответственно, в структуре хранятся не сами строко-подобные
	* контейнеры, а невладеющие объекты (view концепт)
	**************************************************************************/
	template<typename CharT>
	struct Tag_data<CharT*>
	{
		using symbol_type = std::decay_t<CharT>;
		using symbol_pointer = const symbol_type*;

		using string_type = std::basic_string<symbol_type>;
		using string_wrapper = std::basic_string_view<symbol_type>;

		using attr_container_type = std::list<string_wrapper>;
		using child_container_type = std::list<Tag_data<symbol_pointer>>;



		string_wrapper name;
		string_wrapper value;
		attr_container_type attribute_names;
		attr_container_type attribute_values;
		child_container_type childs;
	};

	//*************************************************************************



	/**************************************************************************
	* @brief Класс, который хранит в себе временные XML данные и
	* предоставляет интерфейс для управления этими данными.
	*
	* @details Базовый, общий класс. Для хранения содержимого, использует
	* строко-подобные контейнеры. Стандартно используется при чтении
	* данных при помощи итераторо категории input_iterator
	**************************************************************************/
	template<typename CharT>
	class Parser_data_controller
	{
	public:
		using symbol_type = CharT;
		using symbol_pointer = const symbol_type*;
		using symbol_reference = const symbol_type&;
		using string_type = std::basic_string<symbol_type>;
		using string_wrapper = std::basic_string_view<symbol_type>;
		using size_type =
			typename Tag_data<symbol_type>::attr_container_type::size_type;

		using tag_type = Tag_data<symbol_type>;
		using tags_container_type = std::stack<tag_type>;
		using attr_container_type =
			typename Tag_data<symbol_type>::attr_container_type;

		// Конструкторы, деструкторы и т.п.

		Parser_data_controller() = default;

		Parser_data_controller(const Parser_data_controller&) = default;
		Parser_data_controller& operator=(const Parser_data_controller&) =
			default;

		Parser_data_controller(Parser_data_controller&&) = default;
		Parser_data_controller& operator=(Parser_data_controller&&) =
			default;
		~Parser_data_controller() = default;

		// Проверки состояния

		bool is_tags_empty() const noexcept;

		/**********************************************************************
		* @brief Проверка на корректность тега. Если закрывающий тег есть,
		* то проверяется он. В противном случае проверяется открывающий
		**********************************************************************/
		bool is_correct_tag_name() const noexcept;

		bool is_correct_tag_attributes() const noexcept;

		/**********************************************************************
		* @brief Если есть закрывающий тег, то он сравинвается с последним
		* добавленным тегом. В противном случае сравнивается открывающим
		**********************************************************************/
		bool is_tag_name_equal_parent_name() const noexcept;

		// Модификаторы объектов-тегов

		/**********************************************************************
		* @brief Создать и добавить тег, как информацию о документе
		**********************************************************************/
		void add_tag_as_doc_info();

		/**********************************************************************
		* @brief Создать и добавить тег, как дочерний в последний
		* добавленный тег
		**********************************************************************/
		void add_tag_as_child();

		/**********************************************************************
		* @brief Создать и добавить тег, как родителя. То есть сделать
		* его последним добавленным тегом
		**********************************************************************/
		void add_tag_as_parent();

		/**********************************************************************
		* @brief Переместить последний тег, к предыдущему тегу в качетсве
		* дочернего тега
		**********************************************************************/
		void pop_to_parent();

		/**********************************************************************
		* @brief Создать и добавить в контейнер с атрибутами атрибут из
		* текущего имени и значения атрибута
		**********************************************************************/
		void add_attribute();

		// Модификаторы временных данных тега

		void push_to_tag_name(symbol_type token);
		void push_to_tag_value(symbol_type token);
		void push_to_tag_last_name(symbol_type token);
		void push_to_attribute_name(symbol_type token);
		void push_to_attribute_value(symbol_type token);
		void push_to_comment(symbol_type token);

		void pop_from_tag_name();
		void pop_from_tag_value();
		void pop_from_tag_last_name();
		void pop_from_attribute_name();
		void pop_from_attribute_value();
		void pop_from_comment();

		// Функции очистки

		void clear_tag_name() noexcept;
		void clear_tag_value() noexcept;
		void clear_tag_last_name() noexcept;
		void clear_attribute_name() noexcept;
		void clear_attribute_value() noexcept;
		void clear_attributes_buffers() noexcept;
		void clear_comment() noexcept;
		void clear_all_buffers() noexcept;

		// Ддоступ к данным

		string_wrapper get_tag_name() const;
		string_wrapper get_tag_value() const;
		string_wrapper get_tag_last_name() const;
		string_wrapper get_current_attribute_name() const;
		string_wrapper get_current_attribute_value() const;
		string_wrapper get_comment() const;
		string_wrapper get_attribute_name(size_type index) const;
		string_wrapper get_attribute_value(size_type index) const;

		size_type get_attribute_names_size() const;
		size_type get_attribute_values_size() const;

		tags_container_type& get_doc_info();
		tags_container_type& get_tags();

	private:
		tag_type create_tag();

	private:
		tags_container_type m_tags;

		tags_container_type m_doc_info;

		attr_container_type m_attribute_names_buffer;
		attr_container_type m_attribute_values_buffer;

		string_type m_attribute_name_buffer;
		string_type m_attribute_value_buffer;

		string_type m_tag_name_buffer;
		string_type m_tag_value_buffer;
		string_type m_tag_last_name_buffer;

		string_type m_comment_buffer;
	};

	//*************************************************************************



	/**************************************************************************
	* @brief Класс, который хранит в себе временные XML данные и
	* предоставляет интерфейс для управления этими данными.
	*
	* @details Специализация для forward и выше итераторов и указателей.
	* Соответственно, в структуре хранятся не сами строко-подобные
	* контейнеры, а невладеющие объекты (view концепт)
	**************************************************************************/
	template<typename CharT>
	class Parser_data_controller<CharT*>
	{
	public:
		using symbol_type = std::decay_t<CharT>;
		using symbol_pointer = const symbol_type*;
		using symbol_reference = const symbol_type&;
		using string_type = std::basic_string<symbol_type>;
		using string_wrapper = std::basic_string_view<symbol_type>;
		using size_type =
			typename Tag_data<symbol_type>::attr_container_type::size_type;

		using tag_type = Tag_data<symbol_pointer>;
		using tags_container_type = std::stack<tag_type>;
		using attr_container_type =
			typename Tag_data<symbol_pointer>::attr_container_type;

		// Конструкторы, деструкторы и т.п.

		Parser_data_controller() = default;

		Parser_data_controller(const Parser_data_controller&) = default;
		Parser_data_controller& operator=(const Parser_data_controller&) =
			default;

		Parser_data_controller(Parser_data_controller&&) = default;
		Parser_data_controller& operator=(Parser_data_controller&&) =
			default;
		~Parser_data_controller() = default;

		// Проверки состояния

		bool is_tags_empty() const noexcept;

		/**********************************************************************
		* @brief Проверка на корректность тега. Если закрывающий тег есть,
		* то проверяется он. В противном случае проверяется открывающий
		**********************************************************************/
		bool is_correct_tag_name() const noexcept;

		//Проверяются размеры контейнеров с атрибутами
		bool is_correct_tag_attributes() const noexcept;

		/**********************************************************************
		* @brief Если есть закрывающий тег, то он сравинвается с последним
		* добавленным тегом. В противном случае сравнивается открывающим
		**********************************************************************/
		bool is_tag_name_equal_parent_name() const noexcept;

		// Модификаторы объектов-тегов

		/**********************************************************************
		* @brief Создать и добавить тег, как информацию о документе
		**********************************************************************/
		void add_tag_as_doc_info();

		/**********************************************************************
		* @brief Создать и добавить тег, как дочерний в последний
		* добавленный тег
		**********************************************************************/
		void add_tag_as_child();

		/**********************************************************************
		* @brief Создать и добавить тег, как родителя. То есть сделать
		* его последним добавленным тегом
		**********************************************************************/
		void add_tag_as_parent();

		/**********************************************************************
		* @brief Переместить последний тег, к предыдущему тегу в качетсве
		* дочернего тега
		**********************************************************************/
		void pop_to_parent();

		/**********************************************************************
		* @brief Создать и добавить в контейнер с атрибутами атрибут из
		* текущего имени и значения атрибута
		**********************************************************************/
		void add_attribute();

		// Модификаторы временных данных тега

		void push_to_tag_name(symbol_reference token);
		void push_to_tag_value(symbol_reference token);
		void push_to_tag_last_name(symbol_reference token);
		void push_to_attribute_name(symbol_reference token);
		void push_to_attribute_value(symbol_reference token);
		void push_to_comment(symbol_reference token);

		void pop_from_tag_name();
		void pop_from_tag_value();
		void pop_from_tag_last_name();
		void pop_from_attribute_name();
		void pop_from_attribute_value();
		void pop_from_comment();

		// Функции очистки

		void clear_tag_name() noexcept;
		void clear_tag_value() noexcept;
		void clear_tag_last_name() noexcept;
		void clear_attribute_name() noexcept;
		void clear_attribute_value() noexcept;
		void clear_attributes_buffers() noexcept;
		void clear_comment() noexcept;
		void clear_all_buffers() noexcept;

		// Ддоступ к данным

		string_wrapper get_tag_name() const;
		string_wrapper get_tag_value() const;
		string_wrapper get_tag_last_name() const;
		string_wrapper get_current_attribute_name() const;
		string_wrapper get_current_attribute_value() const;
		string_wrapper get_comment() const;
		string_wrapper get_attribute_name(size_type index) const;
		string_wrapper get_attribute_value(size_type index) const;

		size_type get_attribute_names_size() const;
		size_type get_attribute_values_size() const;

		tags_container_type& get_doc_info();
		tags_container_type& get_tags();

	private:
		tag_type create_tag();

	private:
		tags_container_type m_tags;

		tags_container_type m_doc_info;

		attr_container_type m_attribute_names_buffer;
		attr_container_type m_attribute_values_buffer;

		symbol_pointer m_attribute_name_first = nullptr;
		symbol_pointer m_attribute_name_last = nullptr;

		symbol_pointer m_attribute_value_first = nullptr;
		symbol_pointer m_attribute_value_last = nullptr;

		symbol_pointer m_tag_name_first = nullptr;
		symbol_pointer m_tag_name_last = nullptr;

		symbol_pointer m_tag_value_first = nullptr;
		symbol_pointer m_tag_value_last = nullptr;

		symbol_pointer m_tag_last_name_first = nullptr;
		symbol_pointer m_tag_last_name_last = nullptr;

		string_type m_comment_buffer;
	};

	//*************************************************************************

	

	/**************************************************************************
	*				Parse_data_controller<CharT> IMPLEMENTATION
	**************************************************************************/

	template<typename CharT>
	inline bool Parser_data_controller<CharT>::is_tags_empty() const noexcept
	{
		return m_tags.empty();
	}

	//*************************************************************************

	template<typename CharT>
	inline bool Parser_data_controller<CharT>::is_correct_tag_name() const
		noexcept
	{
		if (m_tag_last_name_buffer.size())
		{
			return m_tag_name_buffer == m_tag_last_name_buffer;
		}
		else
		{
			return m_tag_name_buffer.size();
		}
	}

	//*************************************************************************

	template<typename CharT>
	inline bool Parser_data_controller<CharT>::is_correct_tag_attributes()
		const noexcept
	{
		return m_attribute_names_buffer.size() ==
			m_attribute_values_buffer.size();
	}

	//*************************************************************************

	template<typename CharT>
	inline bool Parser_data_controller<CharT>::
		is_tag_name_equal_parent_name() const noexcept
	{
		bool result = false;

		if (!m_tags.empty())
		{
			if (m_tag_last_name_buffer.size())
			{
				result = m_tag_last_name_buffer == m_tags.top().name;
			}
			else if (m_tag_name_buffer.size())
			{
				result = m_tag_name_buffer == m_tags.top().name;
			}
		}

		return result;
	}

	//*************************************************************************

	template<typename CharT>
	inline void Parser_data_controller<CharT>::add_tag_as_doc_info()
	{
		auto&& tmp_tag = create_tag();

		if (!m_doc_info.empty())
		{
			m_doc_info.pop();
		}

		m_doc_info.push(std::move(tmp_tag));
	}

	//*************************************************************************

	template<typename CharT>
	inline void Parser_data_controller<CharT>::add_tag_as_child()
	{
		auto&& tmp_tag = create_tag();

		if (m_tags.empty())
		{
			m_tags.push(std::move(tmp_tag));
		}
		else
		{
			m_tags.top().childs.push_back(std::move(tmp_tag));
		}
	}

	//*************************************************************************

	template<typename CharT>
	inline void Parser_data_controller<CharT>::add_tag_as_parent()
	{
		auto&& tmp_tag = create_tag();

		m_tags.push(std::move(tmp_tag));
	}

	//*************************************************************************

	template<typename CharT>
	inline void Parser_data_controller<CharT>::pop_to_parent()
	{
		unsigned int minimum_tags_count_to_pop = 2;

		if (m_tags.size() >= minimum_tags_count_to_pop)
		{
			auto tmp_tag = std::move(m_tags.top());

			m_tags.pop();

			m_tags.top().childs.push_back(std::move(tmp_tag));
		}
	}

	//*************************************************************************

	template<typename CharT>
	inline void Parser_data_controller<CharT>::add_attribute()
	{
		//Копирую в контейнеры имя и значение аттрибута, а не перемещаю
		//так как предполагаю, что копирование несколько раз, будет лучше
		//чем потом дальнешие постоянные выделения памяти
		m_attribute_names_buffer.push_back(m_attribute_name_buffer);
		m_attribute_values_buffer.push_back(m_attribute_value_buffer);

		clear_attribute_name();
		clear_attribute_value();
	}

	//*************************************************************************

	template<typename CharT>
	inline void Parser_data_controller<CharT>::push_to_tag_name(
		symbol_type token)
	{
		m_tag_name_buffer.push_back(token);
	}

	//*************************************************************************

	template<typename CharT>
	inline void Parser_data_controller<CharT>::push_to_tag_value(
		symbol_type token)
	{
		m_tag_value_buffer.push_back(token);
	}

	//*************************************************************************

	template<typename CharT>
	inline void Parser_data_controller<CharT>::push_to_tag_last_name(
		symbol_type token)
	{
		m_tag_last_name_buffer.push_back(token);
	}

	//*************************************************************************

	template<typename CharT>
	inline void Parser_data_controller<CharT>::push_to_attribute_name(
		symbol_type token)
	{
		m_attribute_name_buffer.push_back(token);
	}

	//*************************************************************************

	template<typename CharT>
	inline void Parser_data_controller<CharT>::push_to_attribute_value(
		symbol_type token)
	{
		m_attribute_value_buffer.push_back(token);
	}

	//*************************************************************************

	template<typename CharT>
	inline void Parser_data_controller<CharT>::
		push_to_comment(symbol_type token)
	{
		m_comment_buffer.push_back(token);
	}

	//*************************************************************************

	template<typename CharT>
	inline void Parser_data_controller<CharT>::pop_from_tag_name()
	{
		m_tag_name_buffer.pop_back();
	}

	//*************************************************************************

	template<typename CharT>
	inline void Parser_data_controller<CharT>::pop_from_tag_value()
	{
		m_tag_value_buffer.pop_back();
	}

	//*************************************************************************

	template<typename CharT>
	inline void Parser_data_controller<CharT>::pop_from_tag_last_name()
	{
		m_tag_last_name_buffer.pop_back();
	}

	//*************************************************************************

	template<typename CharT>
	inline void Parser_data_controller<CharT>::pop_from_attribute_name()
	{
		m_attribute_name_buffer.pop_back();
	}

	//*************************************************************************

	template<typename CharT>
	inline void Parser_data_controller<CharT>::pop_from_attribute_value()
	{
		m_attribute_value_buffer.pop_back();
	}

	//*************************************************************************

	template<typename CharT>
	inline void Parser_data_controller<CharT>::pop_from_comment()
	{
		m_comment_buffer.pop_back();
	}

	//*************************************************************************

	template<typename CharT>
	inline void Parser_data_controller<CharT>::clear_tag_name() noexcept
	{
		m_tag_name_buffer.clear();
	}

	//*************************************************************************

	template<typename CharT>
	inline void Parser_data_controller<CharT>::clear_tag_value() noexcept
	{
		m_tag_value_buffer.clear();
	}

	//*************************************************************************

	template<typename CharT>
	inline void Parser_data_controller<CharT>::clear_tag_last_name()
		noexcept
	{
		m_tag_last_name_buffer.clear();
	}

	//*************************************************************************

	template<typename CharT>
	inline void Parser_data_controller<CharT>::clear_attribute_name()
		noexcept
	{
		m_attribute_name_buffer.clear();
	}

	//*************************************************************************

	template<typename CharT>
	inline void Parser_data_controller<CharT>::clear_attribute_value()
		noexcept
	{
		m_attribute_value_buffer.clear();
	}

	//*************************************************************************

	template<typename CharT>
	inline void Parser_data_controller<CharT>::clear_attributes_buffers()
		noexcept
	{
		m_attribute_names_buffer.clear();
		m_attribute_values_buffer.clear();
	}

	//*************************************************************************

	template<typename CharT>
	inline void Parser_data_controller<CharT>::clear_comment() noexcept
	{
		m_comment_buffer.clear();
	}

	//*************************************************************************

	template<typename CharT>
	inline void Parser_data_controller<CharT>::clear_all_buffers() noexcept
	{
		clear_tag_name();
		clear_tag_value();
		clear_tag_last_name();
		clear_attribute_name();
		clear_attribute_value();
		clear_attributes_buffers();
		clear_comment();
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Parser_data_controller<CharT>::string_wrapper
		Parser_data_controller<CharT>::get_tag_name() const
	{
		return string_wrapper{ m_tag_name_buffer };
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Parser_data_controller<CharT>::string_wrapper
		Parser_data_controller<CharT>::get_tag_value() const
	{
		return string_wrapper{ m_tag_value_buffer };
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Parser_data_controller<CharT>::string_wrapper
		Parser_data_controller<CharT>::get_tag_last_name() const
	{
		return string_wrapper{ m_tag_last_name_buffer };
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Parser_data_controller<CharT>::string_wrapper
		Parser_data_controller<CharT>::get_current_attribute_name() const
	{
		return string_wrapper{ m_attribute_name_buffer };
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Parser_data_controller<CharT>::string_wrapper
		Parser_data_controller<CharT>::get_current_attribute_value() const
	{
		return string_wrapper{ m_attribute_value_buffer };
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Parser_data_controller<CharT>::string_wrapper
		Parser_data_controller<CharT>::get_comment() const
	{
		return string_wrapper{ m_comment_buffer };
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Parser_data_controller<CharT>::string_wrapper
		Parser_data_controller<CharT>::get_attribute_name(size_type index) 
		const
	{
		using std::advance;
		using std::begin;

		if (index < m_attribute_names_buffer.size())
		{
			auto&& iter = begin(m_attribute_names_buffer);
			advance(iter, index);

			return string_wrapper{ *iter };
		}
		else
		{
			return string_wrapper{};
		}
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Parser_data_controller<CharT>::string_wrapper
		Parser_data_controller<CharT>::get_attribute_value(size_type index) 
		const
	{
		using std::advance;
		using std::begin;

		if (index < m_attribute_values_buffer.size())
		{
			auto&& iter = begin(m_attribute_values_buffer);
			advance(iter, index);

			return string_wrapper{ *iter };
		}
		else
		{
			return string_wrapper{};
		}
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Parser_data_controller<CharT>::size_type
		Parser_data_controller<CharT>::get_attribute_names_size() const
	{
		return m_attribute_names_buffer.size();
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Parser_data_controller<CharT>::size_type
		Parser_data_controller<CharT>::get_attribute_values_size() const
	{
		return m_attribute_values_buffer.size();
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Parser_data_controller<CharT>::tags_container_type&
		Parser_data_controller<CharT>::get_doc_info()
	{
		return m_doc_info;
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Parser_data_controller<CharT>::tags_container_type&
		Parser_data_controller<CharT>::get_tags()
	{
		return m_tags;
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Parser_data_controller<CharT>::tag_type
		Parser_data_controller<CharT>::create_tag()
	{
		//Создают временный тег копированием, без использования move из-за
		//того, что я предполагаю, что скопировать в общем случае будет
		//лучше, чем при добавлении символов в буфферы заново выделать
		//выделять новую память постоянно
		tag_type tmp_tag
		{
			m_tag_name_buffer,
			m_tag_value_buffer,
			m_attribute_names_buffer,
			m_attribute_values_buffer
		};

		clear_tag_name();
		clear_tag_last_name();
		clear_tag_value();
		clear_attributes_buffers();

		return tmp_tag;
	}

	//*************************************************************************


	
	/**************************************************************************
	*				Parse_data_controller<CharT*> IMPLEMENTATION
	**************************************************************************/

	template<typename CharT>
	inline bool Parser_data_controller<CharT*>::is_tags_empty() const noexcept
	{
		return m_tags.empty();
	}

	//*************************************************************************

	template<typename CharT>
	inline bool Parser_data_controller<CharT*>::is_correct_tag_name() const
		noexcept
	{
		using std::begin;
		using std::end;

		bool result = false;

		if ((m_tag_last_name_last - m_tag_last_name_first) > 0 &&
			(m_tag_name_last - m_tag_name_first) <=
			(m_tag_last_name_last - m_tag_last_name_first))
		{
			result = std::equal(m_tag_name_first, m_tag_name_last,
				m_tag_last_name_first, m_tag_last_name_last);
		}
		else
		{
			result = (m_tag_name_last - m_tag_name_first) > 0;
		}

		return result;
	}

	//*************************************************************************

	template<typename CharT>
	inline bool Parser_data_controller<CharT*>::is_correct_tag_attributes()
		const noexcept
	{
		return m_attribute_names_buffer.size() ==
			m_attribute_values_buffer.size();
	}

	//*************************************************************************

	template<typename CharT>
	inline bool Parser_data_controller<CharT*>::
		is_tag_name_equal_parent_name() const noexcept
	{
		using std::begin;
		using std::end;

		bool result = false;

		if ((m_tag_last_name_last - m_tag_last_name_first) > 0)
		{
			result = std::equal(m_tag_last_name_first, m_tag_last_name_last,
				begin(m_tags.top().name), end(m_tags.top().name));
		}
		else if ((m_tag_name_last - m_tag_name_first) > 0)
		{
			result = std::equal(m_tag_name_first, m_tag_name_last,
				begin(m_tags.top().name), end(m_tags.top().name));
		}

		return result;
	}

	//*************************************************************************

	template<typename CharT>
	inline void Parser_data_controller<CharT*>::add_tag_as_doc_info()
	{
		auto&& tmp_tag = create_tag();

		if (!m_doc_info.empty())
		{
			m_doc_info.pop();
		}

		m_doc_info.push(std::move(tmp_tag));
	}

	//*************************************************************************

	template<typename CharT>
	inline void Parser_data_controller<CharT*>::add_tag_as_child()
	{
		auto&& tmp_tag = create_tag();

		if (m_tags.empty())
		{
			m_tags.push(std::move(tmp_tag));
		}
		else
		{
			m_tags.top().childs.push_back(std::move(tmp_tag));
		}
	}

	//*************************************************************************

	template<typename CharT>
	inline void Parser_data_controller<CharT*>::add_tag_as_parent()
	{
		auto&& tmp_tag = create_tag();

		m_tags.push(std::move(tmp_tag));
	}

	//*************************************************************************

	template<typename CharT>
	inline void Parser_data_controller<CharT*>::pop_to_parent()
	{
		unsigned int minimum_tags_count_to_pop = 2;

		if (m_tags.size() >= minimum_tags_count_to_pop)
		{
			auto tmp_tag = std::move(m_tags.top());

			m_tags.pop();

			m_tags.top().childs.push_back(std::move(tmp_tag));
		}
	}

	//*************************************************************************

	template<typename CharT>
	inline void Parser_data_controller<CharT*>::add_attribute()
	{
		using std::end;

		m_attribute_names_buffer.emplace(end(m_attribute_names_buffer),
			m_attribute_name_first,
			m_attribute_name_last - m_attribute_name_first);

		m_attribute_values_buffer.emplace(end(m_attribute_values_buffer),
			m_attribute_value_first,
			m_attribute_value_last - m_attribute_value_first);

		clear_attribute_name();
		clear_attribute_value();
	}

	//*************************************************************************

	template<typename CharT>
	inline void Parser_data_controller<CharT*>::push_to_tag_name(
		symbol_reference token)
	{
		if (!m_tag_name_first)
		{
			m_tag_name_first = &token;
			m_tag_name_last = &token;
			++m_tag_name_last;
		}
		else
		{
			++m_tag_name_last;
		}
	}

	//*************************************************************************

	template<typename CharT>
	inline void Parser_data_controller<CharT*>::push_to_tag_value(
		symbol_reference token)
	{
		if (!m_tag_value_first)
		{
			m_tag_value_first = &token;
			m_tag_value_last = &token;
			++m_tag_value_last;
		}
		else
		{
			++m_tag_value_last;
		}
	}

	//*************************************************************************

	template<typename CharT>
	inline void Parser_data_controller<CharT*>::push_to_tag_last_name(
		symbol_reference token)
	{
		if (!m_tag_last_name_first)
		{
			m_tag_last_name_first = &token;
			m_tag_last_name_last = &token;
			++m_tag_last_name_last;
		}
		else
		{
			++m_tag_last_name_last;
		}
	}

	//*************************************************************************

	template<typename CharT>
	inline void Parser_data_controller<CharT*>::push_to_attribute_name(
		symbol_reference token)
	{
		if (!m_attribute_name_first)
		{
			m_attribute_name_first = &token;
			m_attribute_name_last = &token;
			++m_attribute_name_last;
		}
		else
		{
			++m_attribute_name_last;
		}
	}

	//*************************************************************************

	template<typename CharT>
	inline void Parser_data_controller<CharT*>::push_to_attribute_value(
		symbol_reference token)
	{
		if (!m_attribute_value_first)
		{
			m_attribute_value_first = &token;
			m_attribute_value_last = &token;
			++m_attribute_value_last;
		}
		else
		{
			++m_attribute_value_last;
		}
	}

	//*************************************************************************

	template<typename CharT>
	inline void Parser_data_controller<CharT*>::push_to_comment(
		symbol_reference token)
	{
		m_comment_buffer.push_back(token);
	}

	//*************************************************************************

	template<typename CharT>
	inline void Parser_data_controller<CharT*>::pop_from_tag_name()
	{
		if (m_tag_name_last != m_tag_name_first)
		{
			--m_tag_name_last;
		}
	}

	//*************************************************************************

	template<typename CharT>
	inline void Parser_data_controller<CharT*>::pop_from_tag_value()
	{
		if (m_tag_value_last != m_tag_value_first)
		{
			--m_tag_value_last;
		}
	}

	//*************************************************************************

	template<typename CharT>
	inline void Parser_data_controller<CharT*>::pop_from_tag_last_name()
	{
		if (m_tag_last_name_last != m_tag_last_name_first)
		{
			--m_tag_last_name_last;
		}
	}

	//*************************************************************************

	template<typename CharT>
	inline void Parser_data_controller<CharT*>::pop_from_attribute_name()
	{
		if (m_attribute_name_last != m_attribute_name_first)
		{
			--m_attribute_name_last;
		}
	}

	//*************************************************************************

	template<typename CharT>
	inline void Parser_data_controller<CharT*>::pop_from_attribute_value()
	{
		if (m_attribute_value_last != m_attribute_value_first)
		{
			--m_attribute_value_last;
		}
	}

	//*************************************************************************

	template<typename CharT>
	inline void Parser_data_controller<CharT*>::pop_from_comment()
	{
		m_comment_buffer.pop_back();
	}

	//*************************************************************************

	template<typename CharT>
	inline void Parser_data_controller<CharT*>::clear_tag_name() noexcept
	{
		m_tag_name_first = m_tag_name_last = nullptr;
	}

	//*************************************************************************

	template<typename CharT>
	inline void Parser_data_controller<CharT*>::clear_tag_value() noexcept
	{
		m_tag_value_first = m_tag_value_last = nullptr;
	}

	//*************************************************************************

	template<typename CharT>
	inline void Parser_data_controller<CharT*>::clear_tag_last_name() noexcept
	{
		m_tag_last_name_first = m_tag_last_name_last = nullptr;
	}

	//*************************************************************************

	template<typename CharT>
	inline void Parser_data_controller<CharT*>::clear_attribute_name() noexcept
	{
		m_attribute_name_first = m_attribute_name_last = nullptr;
	}

	//*************************************************************************

	template<typename CharT>
	inline void Parser_data_controller<CharT*>::clear_attribute_value()
		noexcept
	{
		m_attribute_value_first = m_attribute_value_last = nullptr;
	}

	//*************************************************************************

	template<typename CharT>
	inline void Parser_data_controller<CharT*>::clear_attributes_buffers()
		noexcept
	{
		m_attribute_names_buffer.clear();
		m_attribute_values_buffer.clear();
	}

	//*************************************************************************

	template<typename CharT>
	inline void Parser_data_controller<CharT*>::clear_comment() noexcept
	{
		m_comment_buffer.clear();
	}

	//*************************************************************************

	template<typename CharT>
	inline void Parser_data_controller<CharT*>::clear_all_buffers()
		noexcept
	{
		clear_tag_name();
		clear_tag_value();
		clear_tag_last_name();
		clear_attribute_name();
		clear_attribute_value();
		clear_attributes_buffers();
		clear_comment();
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Parser_data_controller<CharT*>::
		string_wrapper Parser_data_controller<CharT*>::get_tag_name() const
	{
		return string_wrapper(m_tag_name_first,
			m_tag_name_last - m_tag_name_first);
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Parser_data_controller<CharT*>::
		string_wrapper Parser_data_controller<CharT*>::get_tag_value() const
	{
		return string_wrapper(m_tag_value_first,
			m_tag_value_last - m_tag_value_first);
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Parser_data_controller<CharT*>::
		string_wrapper Parser_data_controller<CharT*>::get_tag_last_name()
		const
	{
		return string_wrapper(m_tag_last_name_first,
			m_tag_last_name_last - m_tag_last_name_first);
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Parser_data_controller<CharT*>::
		string_wrapper Parser_data_controller<CharT*>::
		get_current_attribute_name() const
	{
		return string_wrapper(m_attribute_name_first,
			m_attribute_name_last - m_attribute_name_first);
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Parser_data_controller<CharT*>::
		string_wrapper Parser_data_controller<CharT*>::
		get_current_attribute_value() const
	{
		return string_wrapper(m_attribute_value_first,
			m_attribute_value_last - m_attribute_value_first);
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Parser_data_controller<CharT*>::
		string_wrapper Parser_data_controller<CharT*>::get_comment() const
	{
		return string_wrapper{ m_comment_buffer };
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Parser_data_controller<CharT*>::string_wrapper
		Parser_data_controller<CharT*>::get_attribute_name(size_type index) 
		const
	{
		using std::advance;
		using std::begin;

		if (index < m_attribute_names_buffer.size())
		{
			auto&& iter = begin(m_attribute_names_buffer);
			advance(iter, index);

			return string_wrapper{ *iter };
		}
		else
		{
			return string_wrapper{};
		}
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Parser_data_controller<CharT*>::string_wrapper
		Parser_data_controller<CharT*>::get_attribute_value(size_type index) 
		const
	{
		using std::advance;
		using std::begin;

		if (index < m_attribute_values_buffer.size())
		{
			auto&& iter = begin(m_attribute_values_buffer);
			advance(iter, index);

			return string_wrapper{ *iter };
		}
		else
		{
			return string_wrapper{};
		}
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Parser_data_controller<CharT*>::size_type
		Parser_data_controller<CharT*>::get_attribute_names_size() const
	{
		return m_attribute_names_buffer.size();
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Parser_data_controller<CharT*>::size_type
		Parser_data_controller<CharT*>::get_attribute_values_size() const
	{
		return m_attribute_values_buffer.size();
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Parser_data_controller<CharT*>::tags_container_type&
		Parser_data_controller<CharT*>::get_doc_info()
	{
		return m_doc_info;
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Parser_data_controller<CharT*>::
		tags_container_type& Parser_data_controller<CharT*>::get_tags()
	{
		return m_tags;
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Parser_data_controller<CharT*>::tag_type
		Parser_data_controller<CharT*>::create_tag()
	{
		//Создают временный тег копированием, без использования move из-за
		//того, что я предполагаю, что скопировать в общем случае будет
		//лучше, чем при добавлении символов в буфферы заново выделать
		//выделять новую память постоянно. Это касается контейнеров
		//аттрибутов
		tag_type tmp_tag
		{
			string_wrapper(m_tag_name_first, 
			m_tag_name_last - m_tag_name_first),

			string_wrapper(m_tag_value_first,
			m_tag_value_last - m_tag_value_first),

			m_attribute_names_buffer,
			m_attribute_values_buffer
		};

		clear_tag_name();
		clear_tag_last_name();
		clear_tag_value();
		clear_attributes_buffers();

		return tmp_tag;
	}

	//*************************************************************************

}} // namespace XMLB::detail

#endif // !XMLB_PARSER_DATA_CONTROLLER_H