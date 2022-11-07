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



#ifndef XMLB_DECORATOR_H
#define XMLB_DECORATOR_H



namespace XMLB { namespace detail {

	/**************************************************************************
	* @brief Структура, которая хранит в себе необходимые символы для 
	* синтаксического анализатора входных XML данных и для вывода готовой XML
	* структуры
	* 
	* @ingroup secondary
	* 
	* @tparam CharT - тип символов
	*
	* @todo Возможно, убрать некоторые символы и разрешить наследование
	**************************************************************************/
	template<typename CharT>
	struct Decorator final
	{
		using symbol_type = CharT;

		///Символ-заполнитель перед началом тегов
		symbol_type fill_symbol = '\t';

		///Символ оступа между словами
		symbol_type white_space_symbol = ' ';

		///Символ открытия тега
		symbol_type open_tag_symbol = '<';

		///Символ закрытия тега
		symbol_type close_tag_symbol = '>';

		///Символ перед символом закрытия одиночного тега
		symbol_type single_tag_symbol = '/';

		///Символ после открытия завершающего тега
		symbol_type last_tag_symbol = '/';

		///Символ перед значением аттрибута
		symbol_type open_attribute_symbol = '\"';

		///Символ после значения аттрибута
		symbol_type close_attribute_symbol = '\"';

		///Символ после имени аттрибута
		symbol_type equal_attribute_symbol = '=';

		///Символ начала комментария
		symbol_type comment_symbol = '!';

		///Символ окончания комментария
		symbol_type comment_last_symbol = '-';

		///Символ перевода строки
		symbol_type line_break_symbol = '\n';

		///Символ табуляции
		symbol_type tab_symbol = '\t';

		///Символ перевода каретки
		symbol_type carriage_symbol = '\r';

		///Символ начала информации о документе
		symbol_type doc_info_symbol = '?';

		///Символ конца информации о документе
		symbol_type doc_info_last_symbol = '?';
	};

}} // namespace XMLB::detail

#endif // !XMLB_DECORATOR_H