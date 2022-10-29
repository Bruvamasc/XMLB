/******************************************************************************
* @file
* Данный файл объявляет и определяет структура Декоратора
*
* @author Bruvamasc
* @date   2022-08-25
*
* @todo Подумать над возможными изменениями Node_tree_impl, code_converter
* ///< Указывает, что элемент недоступен для использования
*
******************************************************************************/

#ifndef XMLB_DECORATOR_H
#define XMLB_DECORATOR_H

namespace XMLB { namespace detail {

	/**************************************************************************
	* @brief Структура, которая хранит в себе необходимые символы для
	* форматирования вывода и ввода
	*
	* @todo Возможно разрешить насследование
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