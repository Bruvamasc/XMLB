/******************************************************************************
* @file
* Данный файл объявляет и определяет вспомагательные шаблонные классы,
* структуры, функции
*
* @author Bruvamasc
* @date   2022-08-25
*
* @todo Подумать над возможными изменениями Node_tree_impl, code_converter
* ///< Указывает, что элемент недоступен для использования
*
******************************************************************************/

#ifndef XMLB_SUP_FUNCTIONS_H
#define XMLB_SUP_FUNCTIONS_H

#include <utility>

namespace XMLB { namespace detail {

	/**************************************************************************
	* @brief Функция для сравнения объекта с аргументами
	*
	* @param[in] obj - объект, который будет сравинваться
	* @param[in] op - бинарный оператор сравнения
	* @param[in] args - аргументы, с которыми будет сравинваться obj
	*
	* @return true - если хоть один аргумент равен сравниваемому объекту, в
	* противном случае - false
	**************************************************************************/
	template<typename T, typename BinaryPredicateT, typename ... ArgT>
	inline constexpr bool is_any_of(T&& obj, BinaryPredicateT op,
		ArgT&& ... args)
	{
		return (op(std::forward<decltype(obj)>(obj),
			std::forward<decltype(args)>(args)) || ...);
	}

	//*************************************************************************



	/**************************************************************************
	* @brief Функция для сравнения объекта с аргументами
	*
	* @param[in] obj - объект, который будет сравинваться
	* @param[in] op - бинарный оператор сравнения
	* @param[in] args - аргументы, с которыми будет сравинваться obj
	*
	* @return true - если все аргументы равны сравниваемому объекту, в
	* противном случае - false
	**************************************************************************/
	template<typename T, typename BinaryPredicateT, typename ... ArgT>
	inline constexpr bool is_all_of(T&& obj, BinaryPredicateT op,
		ArgT&& ... args)
	{
		return (op(std::forward<decltype(obj)>(obj),
			std::forward<decltype(args)>(args)) && ...);
	}

	//*************************************************************************



	/**************************************************************************
	* @brief Функция чтения данных, если итератор указывает на контейнер
	**************************************************************************/
	template<typename IterT, typename DecorT, typename ParserT>
	inline void load_from_iterator_and_container(IterT first, IterT last,
		const DecorT& decorator, ParserT&& parser)
	{
		for (; first != last; ++first)
		{
			for (auto&& token : *first)
			{
				if (!parser.parse(token, decorator))
				{
					break;
				}
			}
		}
	}

	//*************************************************************************



	/**************************************************************************
	* @brief Функция чтения данных, если итератор указывает на простой тип
	**************************************************************************/
	template<typename IterT, typename DecorT, typename ParserT>
	inline void load_from_iterator_and_char(IterT first, IterT last,
		const DecorT& decorator, ParserT&& parser)
	{
		for (; first != last; ++first)
		{
			if (!parser.parse(*first, decorator))
			{
				break;
			}
		}
	}

	//*************************************************************************



	/**************************************************************************
	* @brief Вспомагательная функция для функций save_to. Данная функция
	* записывает из переданного контейнера (строки или символов) в выходной
	* итератор (объект)
	*
	* @param[in] string - контейнер, строка с символами
	* @param[in] out - итератор в который будут записываться символы
	**************************************************************************/
	template<typename StringT, typename OutT>
	inline void copy_symbol_from_container(StringT&& string, OutT&& out)
	{
		using std::begin;
		using std::end;

		auto first = begin(string);
		auto last = end(string);

		for (; first != last; ++first, ++out)
		{
			*out = *first;
		}
	}

	//*************************************************************************



	/**************************************************************************
	* @brief Вспомагательная функция для функций save_to. Записывает в
	* выходной итератор (объект), указанное количество раз символ
	*
	* @param[in] symbol - символ, который нужно записать
	* @param[in] count - сколько раз нужно записать символ
	* @param[in] out - итератор в который будет записываться символ
	**************************************************************************/
	template<typename CharT, typename OutT>
	inline void copy_symbol_n(CharT symbol, std::size_t count, OutT&& out)
	{
		while (count)
		{
			*out = symbol;
			++out;
			--count;
		}
	}

	//*************************************************************************

}} // namespace XMLB::detail

#endif // !XMLB_SUP_FUNCTIONS_H