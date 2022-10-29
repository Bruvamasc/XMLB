/******************************************************************************
* @file
* Данный файл объявляет стандартные классы состояния для класса Парсера XML 
* документа
*
* @author Bruvamasc
* @date   2022-09-20
*
******************************************************************************/

#ifndef XMLB_PARSER_STATES_H
#define XMLB_PARSER_STATES_H

namespace XMLB { namespace detail {

	/**************************************************************************
	* @brief Данный класс и все остальные наследуемые ним классы, являются
	* состояниями парсера. Неявно преобразуются в unsigned int.
	*
	* @details Можно было бы выбрать enum class, но enum class был бы
	* "прибит гвоздями", а так у пользователя или у меня, есть возможность
	* добавлять новые состояния, которые можно сравнивать как обычные
	* unsigned int числа. Также есть возможность расширения информации
	* состояния. Дочерние классы созданы для удобства записи и чтения кода.
	* В итоге, состояния можно хранить как ключи, можно так же использовать
	* их как индексы в массиве и т.д., т.п.
	**************************************************************************/
	class Base_state
	{
	public:
		explicit Base_state(unsigned int key) : m_key{ key } {};

		operator unsigned int() const { return m_key; };

	private:
		unsigned int m_key;
	};

	//*************************************************************************

	class Start_state : public Base_state
	{
	public:
		Start_state() :Base_state(0) {};
	};

	//*************************************************************************

	class Open_tag_state : public Base_state
	{
	public:
		Open_tag_state() :Base_state(1) {};
	};

	//*************************************************************************

	class Close_tag_state : public Base_state
	{
	public:
		Close_tag_state() :Base_state(2) {};
	};

	//*************************************************************************

	class Last_tag_state : public Base_state
	{
	public:
		Last_tag_state() :Base_state(3) {};
	};

	//*************************************************************************

	class Single_tag_state : public Base_state
	{
	public:
		Single_tag_state() :Base_state(4) {};
	};

	//*************************************************************************

	class Name_tag_state : public Base_state
	{
	public:
		Name_tag_state() :Base_state(5) {};
	};

	//*************************************************************************

	class Value_tag_state : public Base_state
	{
	public:
		Value_tag_state() :Base_state(6) {};
	};

	//*************************************************************************

	class Attribute_name_state : public Base_state
	{
	public:
		Attribute_name_state() :Base_state(7) {};
	};

	//*************************************************************************

	class Attribute_value_state : public Base_state
	{
	public:
		Attribute_value_state() :Base_state(8) {};
	};

	//*************************************************************************

	class Comment_state : public Base_state
	{
	public:
		Comment_state() :Base_state(9) {};
	};

	//*************************************************************************

	class Document_info_state : public Base_state
	{
	public:
		Document_info_state() :Base_state(10) {};
	};

	//*************************************************************************

	class Error_state : public Base_state
	{
	public:
		Error_state() :Base_state(-1) {};
	};

	//*************************************************************************

}} // namespace XMLB::detail

#endif // !XMLB_PARSER_STATES_H