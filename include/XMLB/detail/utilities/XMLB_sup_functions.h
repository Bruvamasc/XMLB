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



#ifndef XMLB_SUP_FUNCTIONS_H
#define XMLB_SUP_FUNCTIONS_H

#include <string>
#include <utility>



namespace XMLB { namespace detail {

	//*************************************************************************
	// @brief Функция для сравнения объекта с аргументами
	//
	// @param obj - объект, который будет сравинваться
	// @param op - бинарный оператор сравнения
	// @param args - аргументы, с которыми будет сравинваться obj
	//
	// @return true - если хоть один аргумент равен сравниваемому объекту, в
	// противном случае - false
	template<typename T, typename BinaryPredicateT, typename ... ArgT>
	inline constexpr bool is_any_of(T&& obj, BinaryPredicateT op,
		ArgT&& ... args)
	{
		return (op(std::forward<decltype(obj)>(obj),
			std::forward<decltype(args)>(args)) || ...);
	}

	//*************************************************************************



	//*************************************************************************
	// @brief Функция для сравнения объекта с аргументами
	//
	// @param obj - объект, который будет сравинваться
	// @param op - бинарный оператор сравнения
	// @param args - аргументы, с которыми будет сравинваться obj
	//
	// @return true - если все аргументы равны сравниваемому объекту, в
	// противном случае - false
	template<typename T, typename BinaryPredicateT, typename ... ArgT>
	inline constexpr bool is_all_of(T&& obj, BinaryPredicateT op,
		ArgT&& ... args)
	{
		return (op(std::forward<decltype(obj)>(obj),
			std::forward<decltype(args)>(args)) && ...);
	}

	//*************************************************************************



	//*************************************************************************

	template<typename T, typename ... Chars>
	inline decltype(auto) to_string(Chars ... symbols)
	{
		return std::basic_string<T>{ static_cast<T>(symbols)... };
	}

	//*************************************************************************



	//*************************************************************************
	// Функция чтения данных, если итератор указывает на контейнер
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



	//*************************************************************************
	// Функция чтения данных, если итератор указывает на простой тип
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



	//*************************************************************************
	// Вспомагательная функция для функций save_to. Данная функция
	// записывает из переданного контейнера (строки или символов) в выходной
	// итератор (объект)
	//
	// @param string - контейнер, строка с символами
	// @param out - итератор в который будут записываться символы
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



	//*************************************************************************
	// @brief Вспомагательная функция для функций save_to. Записывает в
	// выходной итератор (объект), указанное количество раз символ
	//
	// @param symbol - символ, который нужно записать
	// @param count - сколько раз нужно записать символ
	// @param out - итератор в который будет записываться символ
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