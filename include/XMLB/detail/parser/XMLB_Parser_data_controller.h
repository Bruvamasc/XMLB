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



#ifndef XMLB_PARSER_DATA_CONTROLLER_H
#define XMLB_PARSER_DATA_CONTROLLER_H

#include <string>
#include <string_view>
#include <list>
#include <stack>



namespace XMLB { namespace detail {

	/**************************************************************************
	* @brief Структура, которая хранит временные, результирующие данные 
	* синтаксического анализа входных XML данных
	* 
	* @ingroup secondary
	*
	* @details Данная структура используется при создания XML узлов. То есть
	* данная структура является просто буфером
	*
	* @tparam CharT - тип символов, которые будут храниться
	*
	* @todo Возможно, нужно подумать над оптимизацие данной структуры
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


		
		string_type name;						///<Имя тега
		string_type value;						///<Значение тега
		attr_container_type attribute_names;	///<Имена атрибутов тега
		attr_container_type attribute_values;	///<Значения атрибутов тега
		child_container_type childs;			///<Дочерние узлы тега
	};

	//*************************************************************************



	/**************************************************************************
	* @brief Структура, которая хранит временные, результирующие данные
	* синтаксического анализа входных XML даннхы
	* 
	* @ingroup secondary
	*
	* @details Это специализация для forward и выше итераторов, а также
	* указателей. Данная структура используется при создания XML узлов. То есть
	* данная структура является просто буфером.
	*
	* @tparam CharT - тип символов, которые будут храниться
	*
	* @todo Возможно, нужно подумать над оптимизацие данной структуры
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



		string_wrapper name;					///<Имя тега
		string_wrapper value;					///<Значение тега
		attr_container_type attribute_names;	///<Имена атрибутов тега
		attr_container_type attribute_values;	///<Значения атрибутов тега
		child_container_type childs;			///<Дочерние узлы тега
	};

	//*************************************************************************



	/**************************************************************************
	* @brief Класс, который хранит в себе данные синтаксического анализатора
	* входных XML данных, а также предоставляет интерфейс для работы с ними
	* 
	* @ingroup secondary
	*
	* @details Данный клас используется в функторах, во время синтаксического
	* анализа входных XML даннхы
	*
	* @tparam CharT - тип символов, которые будут храниться
	*
	* @todo Нужно подумать над оптимизацией данного класса
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
		using tags_container_type = std::stack<tag_type, std::list<tag_type>>;
		using attr_container_type =
			typename Tag_data<symbol_type>::attr_container_type;



		/// @name Конструкторы, деструктор
		/// @{
		Parser_data_controller() = default;

		Parser_data_controller(const Parser_data_controller&) = default;
		Parser_data_controller& operator=(const Parser_data_controller&) =
			default;

		Parser_data_controller(Parser_data_controller&&) = default;
		Parser_data_controller& operator=(Parser_data_controller&&) =
			default;
		~Parser_data_controller() = default;
		/// @}



		/// @name Проверки состояния
		/// @{
		/**********************************************************************
		* @brief Проверить, пустой ли контейнер с тегами
		* 
		* @return true - если пустой, в противном случае - false
		**********************************************************************/
		bool is_tags_empty() const noexcept;

		/**********************************************************************
		* @brief Проверить, корректны ли имена тегов
		* 
		* @details Если имя закрывающего тега есть в текущем буфере, то
		* проверяется он. В противном случае проверяется название текущего тега
		* в буфере
		*
		* @return true - если корректны, в противном случае - false
		**********************************************************************/
		bool is_correct_tag_name() const noexcept;

		/**********************************************************************
		* @brief Проверить, равно ли количесто имен и значений атриубтов
		*
		* @return true - если равно, в противном случае - false
		**********************************************************************/
		bool is_correct_tag_attributes() const noexcept;

		/**********************************************************************
		* @brief Проверить, равен ли текущий тег с последним добаленным
		* 
		* @details Если есть имя закрывающего тега в буфере, то он
		* сравинвается с последним добавленным тегом. В противнос случае
		* сравнивается с именем открывающегося тега
		*
		* @return true - если равны, в противном случае - false
		**********************************************************************/
		bool is_tag_name_equal_parent_name() const noexcept;
		/// @}



		/// @name Модификаторы узлов
		/// @{
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
		/// @}



		/// @name Модификаторы текущих буферов
		/// @{
		/**********************************************************************
		* @brief Создать и добавить в контейнер с атрибутами атрибут из
		* текущего имени и значения атрибута
		**********************************************************************/
		void add_attribute();

		/**********************************************************************
		* @brief Поместить символ в буфер имени тега
		* 
		* @param token - символ
		**********************************************************************/
		void push_to_tag_name(symbol_type token);

		/**********************************************************************
		* @brief Поместить символ в буфер значения тега
		*
		* @param token - символ
		**********************************************************************/
		void push_to_tag_value(symbol_type token);

		/**********************************************************************
		* @brief Поместить символ в буфер имени последнего(закрывающего) тега
		*
		* @param token - символ
		**********************************************************************/
		void push_to_tag_last_name(symbol_type token);

		/**********************************************************************
		* @brief Поместить символ в буфер имени атрибута
		*
		* @param token - символ
		**********************************************************************/
		void push_to_attribute_name(symbol_type token);

		/**********************************************************************
		* @brief Поместить символ в буфер значения атрибута
		*
		* @param token - символ
		**********************************************************************/
		void push_to_attribute_value(symbol_type token);

		/**********************************************************************
		* @brief Поместить символ в буфер комментария
		*
		* @param token - символ
		**********************************************************************/
		void push_to_comment(symbol_type token);

		/**********************************************************************
		* @brief Удалить последний элемент из буфера имени тега
		* 
		* @details Вызов данного метода для пустого буфера не сделает ничего
		**********************************************************************/
		void pop_from_tag_name() noexcept;

		/**********************************************************************
		* @brief Удалить последний элемент из буфера значения тега
		*
		* @details Вызов данного метода для пустого буфера не сделает ничего
		**********************************************************************/
		void pop_from_tag_value() noexcept;

		/**********************************************************************
		* @brief Удалить последний элемент из буфера имени последнего
		* (закрывающего) тега
		*
		* @details Вызов данного метода для пустого буфера не сделает ничего
		**********************************************************************/
		void pop_from_tag_last_name() noexcept;

		/**********************************************************************
		* @brief Удалить последний элемент из буфера имени атрибута
		*
		* @details Вызов данного метода для пустого буфера не сделает ничего
		**********************************************************************/
		void pop_from_attribute_name() noexcept;

		/**********************************************************************
		* @brief Удалить последний элемент из буфера значения атрибута
		*
		* @details Вызов данного метода для пустого буфера не сделает ничего
		**********************************************************************/
		void pop_from_attribute_value() noexcept;

		/**********************************************************************
		* @brief Удалить последний элемент из буфера комментария
		*
		* @details Вызов данного метода для пустого буфера не сделает ничего
		**********************************************************************/
		void pop_from_comment() noexcept;
		/// @}



		/// @name Методы очистки
		/// @{
		/**********************************************************************
		* @brief Очистить буфер имени тега
		**********************************************************************/
		void clear_tag_name() noexcept;

		/**********************************************************************
		* @brief Очистить буфер значения тега
		**********************************************************************/
		void clear_tag_value() noexcept;

		/**********************************************************************
		* @brief Очистить буфер имени последнего(закрывающего) тега
		**********************************************************************/
		void clear_tag_last_name() noexcept;

		/**********************************************************************
		* @brief Очистить буфер имени атрибута
		**********************************************************************/
		void clear_attribute_name() noexcept;

		/**********************************************************************
		* @brief Очистить буфер значения атрибута
		**********************************************************************/
		void clear_attribute_value() noexcept;

		/**********************************************************************
		* @brief Очистить буфер атрибутов
		**********************************************************************/
		void clear_attributes_buffers() noexcept;

		/**********************************************************************
		* @brief Очистить буфер комментария
		**********************************************************************/
		void clear_comment() noexcept;

		/**********************************************************************
		* @brief Очистить все буферы
		**********************************************************************/
		void clear_all_buffers() noexcept;
		/// @}



		/// @name Методы доступа к данным
		/// @{
		/**********************************************************************
		* @brief Получить имя текущего тега
		* 
		* @return невладеющий объект-обертку строки с именем тега
		**********************************************************************/
		string_wrapper get_tag_name() const noexcept;

		/**********************************************************************
		* @brief Получить значение текущего тега
		*
		* @return невладеющий объект-обертку строки со значением тега
		**********************************************************************/
		string_wrapper get_tag_value() const noexcept;

		/**********************************************************************
		* @brief Получить имя последнего(закрывающего) тега
		*
		* @return невладеющий объект-обертку строки с именем последнего тега
		**********************************************************************/
		string_wrapper get_tag_last_name() const noexcept;

		/**********************************************************************
		* @brief Получить имя текущего атрибута
		*
		* @return невладеющий объект-обертку строки с именем атрибута
		**********************************************************************/
		string_wrapper get_current_attribute_name() const noexcept;

		/**********************************************************************
		* @brief Получить значение текущего атрибута
		*
		* @return невладеющий объект-обертку строки со значением атрибута
		**********************************************************************/
		string_wrapper get_current_attribute_value() const noexcept;

		/**********************************************************************
		* @brief Получить текущий комментарий
		*
		* @return невладеющий объект-обертку строки с комментарием
		**********************************************************************/
		string_wrapper get_comment() const noexcept;

		/**********************************************************************
		* @brief Получить имя атрибута из контейнера атрибутов
		* 
		* @param index - позиция атрибута
		*
		* @return невладеющий объект-обертку строки с именем атрибута
		**********************************************************************/
		string_wrapper get_attribute_name(size_type index) const noexcept;

		/**********************************************************************
		* @brief Получить значение атрибута из контейнера атрибутов
		* 
		* @param index - позиция атрибута
		*
		* @return невладеющий объект-обертку строки со значением атрибута
		**********************************************************************/
		string_wrapper get_attribute_value(size_type index) const noexcept;

		/**********************************************************************
		* @brief Получить количество имен атрибутов
		*
		* @return количество имен атрибутов
		**********************************************************************/
		size_type get_attribute_names_size() const noexcept;

		/**********************************************************************
		* @brief Получить количество значение атрибутов
		*
		* @return количество значение атрибутов
		**********************************************************************/
		size_type get_attribute_values_size() const noexcept;

		/**********************************************************************
		* @brief Получить контейнер с данными, которые содержат информацию о
		* документе
		*
		* @return контейнер с информацией о документе
		**********************************************************************/
		tags_container_type& get_doc_info() & noexcept;

		/**********************************************************************
		* @brief Получить контейнер с даннми, которые содержат информацию о
		* всех добавленных тегах
		*
		* @return контейнер с тегами
		**********************************************************************/
		tags_container_type& get_tags() & noexcept;
		/// @}

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
	* @brief Класс, который хранит в себе данные синтаксического анализатора 
	* входных XML даннхы, а также предоставляет интерфейс для работы с ними
	* 
	* @ingroup secondary
	*
	* @details  Это специализация для forward и выше итераторов, а также
	* указателей. Данный клас используется в функторах, во время 
	* синтаксического анализа входных XML даннхы
	*
	* @tparam CharT - тип символов, которые будут храниться
	*
	* @todo Нужно подумать над оптимизацией данного класса
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



		/// @name Конструкторы, деструктор
		/// @{
		Parser_data_controller() = default;

		Parser_data_controller(const Parser_data_controller&) = default;
		Parser_data_controller& operator=(const Parser_data_controller&) =
			default;

		Parser_data_controller(Parser_data_controller&&) = default;
		Parser_data_controller& operator=(Parser_data_controller&&) =
			default;
		~Parser_data_controller() = default;
		/// @}



		/// @name Проверки состояния
		/// @{
		/**********************************************************************
		* @brief Проверить, пустой ли контейнер с тегами
		*
		* @return true - если пустой, в противном случае - false
		**********************************************************************/
		bool is_tags_empty() const noexcept;

		/**********************************************************************
		* @brief Проверить, корректны ли имена тегов
		*
		* @details Если имя закрывающего тега есть в текущем буфере, то
		* проверяется он. В противном случае проверяется название текущего тега
		* в буфере
		*
		* @return true - если корректны, в противном случае - false
		**********************************************************************/
		bool is_correct_tag_name() const noexcept;

		/**********************************************************************
		* @brief Проверить, равно ли количесто имен и значений атриубтов
		*
		* @return true - если равно, в противном случае - false
		**********************************************************************/
		bool is_correct_tag_attributes() const noexcept;

		/**********************************************************************
		* @brief Проверить, равен ли текущий тег с последним добаленным
		*
		* @details Если есть имя закрывающего тега в буфере, то он
		* сравинвается с последним добавленным тегом. В противнос случае
		* сравнивается с именем открывающегося тега
		*
		* @return true - если равны, в противном случае - false
		**********************************************************************/
		bool is_tag_name_equal_parent_name() const noexcept;
		/// @}



		/// @name Модификаторы узлов
		/// @{
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
		/// @}



		/// @name Модификаторы текущих буферов
		/// @{
		/**********************************************************************
		* @brief Создать и добавить в контейнер с атрибутами атрибут из
		* текущего имени и значения атрибута
		**********************************************************************/
		void add_attribute();

		/**********************************************************************
		* @brief Поместить символ в буфер имени тега
		*
		* @param token - ссылка на символ
		**********************************************************************/
		void push_to_tag_name(symbol_reference token);

		/**********************************************************************
		* @brief Поместить символ в буфер значения тега
		*
		* @param token - ссылка на символ
		**********************************************************************/
		void push_to_tag_value(symbol_reference token);

		/**********************************************************************
		* @brief Поместить символ в буфер имени последнего(закрывающего) тега
		*
		* @param token - ссылка на символ
		**********************************************************************/
		void push_to_tag_last_name(symbol_reference token);

		/**********************************************************************
		* @brief Поместить символ в буфер имени атрибута
		*
		* @param token - ссылка на символ
		**********************************************************************/
		void push_to_attribute_name(symbol_reference token);

		/**********************************************************************
		* @brief Поместить символ в буфер значения атрибута
		*
		* @param token - ссылка на символ
		**********************************************************************/
		void push_to_attribute_value(symbol_reference token);

		/**********************************************************************
		* @brief Поместить символ в буфер комментария
		*
		* @param token - ссылка на символ
		**********************************************************************/
		void push_to_comment(symbol_reference token);

		/**********************************************************************
		* @brief Удалить последний элемент из буфера имени тега
		*
		* @details Вызов данного метода для пустого буфера не сделает ничего
		**********************************************************************/
		void pop_from_tag_name() noexcept;

		/**********************************************************************
		* @brief Удалить последний элемент из буфера значения тега
		*
		* @details Вызов данного метода для пустого буфера не сделает ничего
		**********************************************************************/
		void pop_from_tag_value() noexcept;

		/**********************************************************************
		* @brief Удалить последний элемент из буфера имени последнего
		* (закрывающего) тега
		*
		* @details Вызов данного метода для пустого буфера не сделает ничего
		**********************************************************************/
		void pop_from_tag_last_name() noexcept;

		/**********************************************************************
		* @brief Удалить последний элемент из буфера имени атрибута
		*
		* @details Вызов данного метода для пустого буфера не сделает ничего
		**********************************************************************/
		void pop_from_attribute_name() noexcept;

		/**********************************************************************
		* @brief Удалить последний элемент из буфера значения атрибута
		*
		* @details Вызов данного метода для пустого буфера не сделает ничего
		**********************************************************************/
		void pop_from_attribute_value() noexcept;

		/**********************************************************************
		* @brief Удалить последний элемент из буфера комментария
		*
		* @details Вызов данного метода для пустого буфера не сделает ничего
		**********************************************************************/
		void pop_from_comment() noexcept;
		/// @}



		/// @name Методы очистки
		/// @{
		/**********************************************************************
		* @brief Очистить буфер имени тега
		**********************************************************************/
		void clear_tag_name() noexcept;

		/**********************************************************************
		* @brief Очистить буфер значения тега
		**********************************************************************/
		void clear_tag_value() noexcept;

		/**********************************************************************
		* @brief Очистить буфер имени последнего(закрывающего) тега
		**********************************************************************/
		void clear_tag_last_name() noexcept;

		/**********************************************************************
		* @brief Очистить буфер имени атрибута
		**********************************************************************/
		void clear_attribute_name() noexcept;

		/**********************************************************************
		* @brief Очистить буфер значения атрибута
		**********************************************************************/
		void clear_attribute_value() noexcept;

		/**********************************************************************
		* @brief Очистить буфер атрибутов
		**********************************************************************/
		void clear_attributes_buffers() noexcept;

		/**********************************************************************
		* @brief Очистить буфер комментария
		**********************************************************************/
		void clear_comment() noexcept;

		/**********************************************************************
		* @brief Очистить все буферы
		**********************************************************************/
		void clear_all_buffers() noexcept;
		/// @}



		/// @name Методы доступа к данным
		/// @{
		/**********************************************************************
		* @brief Получить имя текущего тега
		*
		* @return невладеющий объект-обертку строки с именем тега
		**********************************************************************/
		string_wrapper get_tag_name() const noexcept;

		/**********************************************************************
		* @brief Получить значение текущего тега
		*
		* @return невладеющий объект-обертку строки со значением тега
		**********************************************************************/
		string_wrapper get_tag_value() const noexcept;

		/**********************************************************************
		* @brief Получить имя последнего(закрывающего) тега
		*
		* @return невладеющий объект-обертку строки с именем последнего тега
		**********************************************************************/
		string_wrapper get_tag_last_name() const noexcept;

		/**********************************************************************
		* @brief Получить имя текущего атрибута
		*
		* @return невладеющий объект-обертку строки с именем атрибута
		**********************************************************************/
		string_wrapper get_current_attribute_name() const noexcept;

		/**********************************************************************
		* @brief Получить значение текущего атрибута
		*
		* @return невладеющий объект-обертку строки со значением атрибута
		**********************************************************************/
		string_wrapper get_current_attribute_value() const noexcept;

		/**********************************************************************
		* @brief Получить текущий комментарий
		*
		* @return невладеющий объект-обертку строки с комментарием
		**********************************************************************/
		string_wrapper get_comment() const noexcept;

		/**********************************************************************
		* @brief Получить имя атрибута из контейнера атрибутов
		*
		* @param index - позиция атрибута
		*
		* @return невладеющий объект-обертку строки с именем атрибута
		**********************************************************************/
		string_wrapper get_attribute_name(size_type index) const noexcept;

		/**********************************************************************
		* @brief Получить значение атрибута из контейнера атрибутов
		*
		* @param index - позиция атрибута
		*
		* @return невладеющий объект-обертку строки со значением атрибута
		**********************************************************************/
		string_wrapper get_attribute_value(size_type index) const noexcept;

		/**********************************************************************
		* @brief Получить количество имен атрибутов
		*
		* @return количество имен атрибутов
		**********************************************************************/
		size_type get_attribute_names_size() const noexcept;

		/**********************************************************************
		* @brief Получить количество значение атрибутов
		*
		* @return количество значение атрибутов
		**********************************************************************/
		size_type get_attribute_values_size() const noexcept;

		/**********************************************************************
		* @brief Получить контейнер с данными, которые содержат информацию о
		* документе
		*
		* @return контейнер с информацией о документе
		**********************************************************************/
		tags_container_type& get_doc_info() & noexcept;

		/**********************************************************************
		* @brief Получить контейнер с даннми, которые содержат информацию о
		* всех добавленных тегах
		*
		* @return контейнер с тегами
		**********************************************************************/
		tags_container_type& get_tags() & noexcept;
		/// @}

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

	

	//*************************************************************************
	//				Parse_data_controller<CharT> IMPLEMENTATION
	//*************************************************************************

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
	inline void Parser_data_controller<CharT>::pop_from_tag_name() noexcept
	{
		if (!m_tag_name_buffer.empty())
		{
			m_tag_name_buffer.pop_back();
		}
	}

	//*************************************************************************

	template<typename CharT>
	inline void Parser_data_controller<CharT>::pop_from_tag_value() noexcept
	{
		if (!m_tag_value_buffer.empty())
		{
			m_tag_value_buffer.pop_back();
		}
	}

	//*************************************************************************

	template<typename CharT>
	inline void Parser_data_controller<CharT>::pop_from_tag_last_name() 
		noexcept
	{
		if (!m_tag_last_name_buffer.empty())
		{
			m_tag_last_name_buffer.pop_back();
		}
	}

	//*************************************************************************

	template<typename CharT>
	inline void Parser_data_controller<CharT>::pop_from_attribute_name()
		noexcept
	{
		if (!m_attribute_name_buffer.empty())
		{
			m_attribute_name_buffer.pop_back();
		}
	}

	//*************************************************************************

	template<typename CharT>
	inline void Parser_data_controller<CharT>::pop_from_attribute_value()
		noexcept
	{
		if (!m_attribute_value_buffer.empty())
		{
			m_attribute_value_buffer.pop_back();
		}
	}

	//*************************************************************************

	template<typename CharT>
	inline void Parser_data_controller<CharT>::pop_from_comment() noexcept
	{
		if (!m_comment_buffer.empty())
		{
			m_comment_buffer.pop_back();
		}
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
		Parser_data_controller<CharT>::get_tag_name() const noexcept
	{
		return string_wrapper{ m_tag_name_buffer };
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Parser_data_controller<CharT>::string_wrapper
		Parser_data_controller<CharT>::get_tag_value() const noexcept
	{
		return string_wrapper{ m_tag_value_buffer };
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Parser_data_controller<CharT>::string_wrapper
		Parser_data_controller<CharT>::get_tag_last_name() const noexcept
	{
		return string_wrapper{ m_tag_last_name_buffer };
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Parser_data_controller<CharT>::string_wrapper
		Parser_data_controller<CharT>::get_current_attribute_name() const
		noexcept
	{
		return string_wrapper{ m_attribute_name_buffer };
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Parser_data_controller<CharT>::string_wrapper
		Parser_data_controller<CharT>::get_current_attribute_value() const
		noexcept
	{
		return string_wrapper{ m_attribute_value_buffer };
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Parser_data_controller<CharT>::string_wrapper
		Parser_data_controller<CharT>::get_comment() const noexcept
	{
		return string_wrapper{ m_comment_buffer };
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Parser_data_controller<CharT>::string_wrapper
		Parser_data_controller<CharT>::get_attribute_name(size_type index) 
		const noexcept
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
		const noexcept
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
		noexcept
	{
		return m_attribute_names_buffer.size();
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Parser_data_controller<CharT>::size_type
		Parser_data_controller<CharT>::get_attribute_values_size() const
		noexcept
	{
		return m_attribute_values_buffer.size();
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Parser_data_controller<CharT>::tags_container_type&
		Parser_data_controller<CharT>::get_doc_info() & noexcept
	{
		return m_doc_info;
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Parser_data_controller<CharT>::tags_container_type&
		Parser_data_controller<CharT>::get_tags() & noexcept
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


	
	//*************************************************************************
	//				Parse_data_controller<CharT*> IMPLEMENTATION
	//*************************************************************************

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
	inline void Parser_data_controller<CharT*>::pop_from_tag_name() noexcept
	{
		if (m_tag_name_last > m_tag_name_first)
		{
			--m_tag_name_last;
		}
	}

	//*************************************************************************

	template<typename CharT>
	inline void Parser_data_controller<CharT*>::pop_from_tag_value() noexcept
	{
		if (m_tag_value_last > m_tag_value_first)
		{
			--m_tag_value_last;
		}
	}

	//*************************************************************************

	template<typename CharT>
	inline void Parser_data_controller<CharT*>::pop_from_tag_last_name()
		noexcept
	{
		if (m_tag_last_name_last > m_tag_last_name_first)
		{
			--m_tag_last_name_last;
		}
	}

	//*************************************************************************

	template<typename CharT>
	inline void Parser_data_controller<CharT*>::pop_from_attribute_name()
		noexcept
	{
		if (m_attribute_name_last > m_attribute_name_first)
		{
			--m_attribute_name_last;
		}
	}

	//*************************************************************************

	template<typename CharT>
	inline void Parser_data_controller<CharT*>::pop_from_attribute_value()
		noexcept
	{
		if (m_attribute_value_last > m_attribute_value_first)
		{
			--m_attribute_value_last;
		}
	}

	//*************************************************************************

	template<typename CharT>
	inline void Parser_data_controller<CharT*>::pop_from_comment() noexcept
	{
		if (!m_comment_buffer.empty())
		{
			m_comment_buffer.pop_back();
		}
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
		noexcept
	{
		return string_wrapper(m_tag_name_first,
			m_tag_name_last - m_tag_name_first);
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Parser_data_controller<CharT*>::
		string_wrapper Parser_data_controller<CharT*>::get_tag_value() const
		noexcept
	{
		return string_wrapper(m_tag_value_first,
			m_tag_value_last - m_tag_value_first);
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Parser_data_controller<CharT*>::
		string_wrapper Parser_data_controller<CharT*>::get_tag_last_name()
		const noexcept
	{
		return string_wrapper(m_tag_last_name_first,
			m_tag_last_name_last - m_tag_last_name_first);
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Parser_data_controller<CharT*>::
		string_wrapper Parser_data_controller<CharT*>::
		get_current_attribute_name() const noexcept
	{
		return string_wrapper(m_attribute_name_first,
			m_attribute_name_last - m_attribute_name_first);
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Parser_data_controller<CharT*>::
		string_wrapper Parser_data_controller<CharT*>::
		get_current_attribute_value() const noexcept
	{
		return string_wrapper(m_attribute_value_first,
			m_attribute_value_last - m_attribute_value_first);
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Parser_data_controller<CharT*>::
		string_wrapper Parser_data_controller<CharT*>::get_comment() const
		noexcept
	{
		return string_wrapper{ m_comment_buffer };
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Parser_data_controller<CharT*>::string_wrapper
		Parser_data_controller<CharT*>::get_attribute_name(size_type index) 
		const noexcept
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
		const noexcept
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
		noexcept
	{
		return m_attribute_names_buffer.size();
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Parser_data_controller<CharT*>::size_type
		Parser_data_controller<CharT*>::get_attribute_values_size() const
		noexcept
	{
		return m_attribute_values_buffer.size();
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Parser_data_controller<CharT*>::tags_container_type&
		Parser_data_controller<CharT*>::get_doc_info() & noexcept
	{
		return m_doc_info;
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Parser_data_controller<CharT*>::
		tags_container_type& Parser_data_controller<CharT*>::get_tags() &
		noexcept
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