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



#ifndef XMLB_PARSER_STATES_H
#define XMLB_PARSER_STATES_H



namespace XMLB { namespace detail {

	/**************************************************************************
	* @brief Класс, который является базовым классом для состояний 
	* синтаксического анализатора входных XML данных
	* 
	* @ingroup secondary
	*
	* @details Данный класс преобразуется в unsigned int. Данный подход был
	* выбран по той причине, чтобы можно было добавлять новые состояния (enum
	* class не дал бы такой возможности). Также этот вариант выбран чтобы не
	* использовать виртуальные функции. То есть, дочерние классы просто
	* вызывают конструктор этого класса с определенным числом(ключем)
	**************************************************************************/
	class Base_state
	{
	public:
		explicit Base_state(unsigned int key) noexcept : m_key{ key } {};

		operator unsigned int() const noexcept { return m_key; };

	private:
		unsigned int m_key;
	};

	//*************************************************************************



	/**************************************************************************
	* @brief Класс начального, стартового состояния синтаксического
	* анализатора. Равен 0
	* 
	* @ingroup secondary
	**************************************************************************/
	class Start_state : public Base_state
	{
	public:
		Start_state() noexcept :Base_state(0) {};
	};

	//*************************************************************************



	/**************************************************************************
	* @brief Класс состояния начала, открытия XML тега. Равен 1
	* 
	* @ingroup secondary
	**************************************************************************/
	class Open_tag_state : public Base_state
	{
	public:
		Open_tag_state() noexcept :Base_state(1) {};
	};

	//*************************************************************************



	/**************************************************************************
	* @brief Класс состояния конца, закрытия XML тега. Равен 2
	* 
	* @ingroup secondary
	**************************************************************************/
	class Close_tag_state : public Base_state
	{
	public:
		Close_tag_state() noexcept :Base_state(2) {};
	};

	//*************************************************************************



	/**************************************************************************
	* @brief Класс состояния последнего, закрывающего XML тега. Равен 3
	* 
	* @ingroup secondary
	**************************************************************************/
	class Last_tag_state : public Base_state
	{
	public:
		Last_tag_state() noexcept :Base_state(3) {};
	};

	//*************************************************************************



	/**************************************************************************
	* @brief Класс состояния одиночного XML тега. Равен 4
	* 
	* @ingroup secondary
	**************************************************************************/
	class Single_tag_state : public Base_state
	{
	public:
		Single_tag_state() noexcept :Base_state(4) {};
	};

	//*************************************************************************



	/**************************************************************************
	* @brief Класс состояния имени XML тегаю Равен 5
	* 
	* @ingroup secondary
	**************************************************************************/
	class Name_tag_state : public Base_state
	{
	public:
		Name_tag_state() noexcept :Base_state(5) {};
	};

	//*************************************************************************



	/**************************************************************************
	* @brief Класс состояния значения XML тега. Равен 6
	* 
	* @ingroup secondary
	**************************************************************************/
	class Value_tag_state : public Base_state
	{
	public:
		Value_tag_state() noexcept :Base_state(6) {};
	};

	//*************************************************************************



	/**************************************************************************
	* @brief Класс состояния имени атрибута XML тега. Равен 7
	* 
	* @ingroup secondary
	**************************************************************************/
	class Attribute_name_state : public Base_state
	{
	public:
		Attribute_name_state() noexcept :Base_state(7) {};
	};

	//*************************************************************************



	/**************************************************************************
	* @brief Класс состояния значения атрибута XML тега. Равен 8
	* 
	* @ingroup secondary
	**************************************************************************/
	class Attribute_value_state : public Base_state
	{
	public:
		Attribute_value_state() noexcept :Base_state(8) {};
	};

	//*************************************************************************



	/**************************************************************************
	* @brief Класс состояния XML комментария. Равен 9
	* 
	* @ingroup secondary
	**************************************************************************/
	class Comment_state : public Base_state
	{
	public:
		Comment_state() noexcept :Base_state(9) {};
	};

	//*************************************************************************



	/**************************************************************************
	* @brief Класс состояния информации о XML документе. Равен 10
	* 
	* @ingroup secondary
	**************************************************************************/
	class Document_info_state : public Base_state
	{
	public:
		Document_info_state() noexcept :Base_state(10) {};
	};

	//*************************************************************************



	/**************************************************************************
	* @brief Класс состояния ошибки. Равен max(unsigned int)
	* 
	* @ingroup secondary
	**************************************************************************/
	class Error_state : public Base_state
	{
	public:
		Error_state() noexcept :Base_state(-1) {};
	};

	//*************************************************************************

}} // namespace XMLB::detail

#endif // !XMLB_PARSER_STATES_H