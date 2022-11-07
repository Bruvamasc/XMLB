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



#ifndef XMLB_UTILITY_H
#define XMLB_UTILITY_H

#include <string>
#include <string_view>

#include "XMLB/detail/XMLB_fwd.h"



namespace XMLB
{
	/**************************************************************************
	* @brief Конвертировать строку в float
	* 
	* @ingroup general
	*
	* @tparam CharT - тип символов
	* @param str - строка, которую нужно конвертировать
	*
	* @return 0.0 - если не удасться конвертировать строку. В противном случае
	* конвертированное число, которое тоже может быть равно 0.0
	* 
	* @todo Нужно переписать данную функцию, так как она пока-что как
	* временное решение. Нужно добавить hex, oct и binary
	**************************************************************************/
	template<typename CharT>
	inline float to_float(std::basic_string_view<CharT> str)
	{
		using symbol_type = CharT;
		using size_type =
			typename std::basic_string_view<symbol_type>::size_type;

		float result = 0.f;

		if (str.size())
		{
			const unsigned int base = 10;
			const unsigned int start_ascii_for_dec = 48;
			const unsigned int dot_count = 1;
			const symbol_type dot_symbol = '.';
			const symbol_type minus_symbol = '-';

			auto found_minus = str.find_first_of(minus_symbol);
			auto found_dot = str.find_first_of(dot_symbol);

			size_type head_count = 0;
			size_type tail_count = 0;

			if (found_dot != std::basic_string_view<CharT>::npos)
			{
				head_count = found_dot;
				tail_count = str.size() - dot_count - head_count;
			}
			else
			{
				head_count = str.size();
			}

			if (found_minus != std::basic_string_view<CharT>::npos)
			{
				--head_count;
			}

			float head = 0.f;
			float tail = 0.f;

			float start_base = 1.f;
			size_type digit_position = found_dot - 1;

			while (head_count)
			{
				float digit = static_cast<float>(str[digit_position] -
					start_ascii_for_dec);

				head += digit * start_base;
				start_base *= base;

				--head_count;
				--digit_position;
			}

			start_base = base;
			digit_position = found_dot + 1;

			while (tail_count)
			{
				float digit = static_cast<float>(str[digit_position] -
					start_ascii_for_dec);

				tail += digit > 0 ? digit / start_base : 0.f;
				start_base *= base;

				--tail_count;
				++digit_position;
			}

			result = head + tail;

			if (found_minus != std::basic_string_view<CharT>::npos)
			{
				result *= -1.f;
			}
		}

		return result;
	}

	//*************************************************************************



	/**************************************************************************
	* @brief Конвертировать float в строку
	* 
	* @ingroup general
	*
	* @tparam CharT - тип сиволов
	* @param value - число, которое нужно конвертировать
	* @param base - основание числа
	* 
	* @return строку содержащую переданное число
	* 
	* @todo Нужно переписать данную функцию, так как она пока-что как
	* временное решение. Нужно добавить hex, oct и binary
	**************************************************************************/
	template<typename CharT>
	inline std::basic_string<CharT> to_string(float value, int base = 10)
	{
		using std::begin;
		using std::end;

		using symbol_type = CharT;
		using string_type = std::basic_string<symbol_type>;
		using size_type = typename std::basic_string<symbol_type>::size_type;

		const unsigned int digit_count = 7;
		const unsigned int dot_count = 1;
		const unsigned int sign_count = 1;
		const unsigned int start_ascii_for_hex = 87;
		const unsigned int start_ascii_for_dec = 48;
		const symbol_type dot_symbol = '.';
		const symbol_type minus_symbol = '-';

		string_type result;
		int result_char_count;
		size_type head_count = 1;
		int digit_position = 1;

		// Корректируем размер результирующей строки и значение вспомагательной
		// позиции цыфры

		if (value < 0)
		{
			result_char_count = digit_count + dot_count + sign_count;
			result.resize(result_char_count, 
				static_cast<char>(start_ascii_for_dec));

			digit_position = 0;
		}
		else
		{
			result_char_count = digit_count + dot_count;
			result.resize(result_char_count, 
				static_cast<char>(start_ascii_for_dec));
		}	

		// Инициализируем целую и дробные части входного числа

		int head = static_cast<int>(std::abs(value));

		{
			int temp_base = base;

			while (head > temp_base)
			{
				temp_base *= base;
				++head_count;
			}
		}

		float tail_raw = (std::abs(value) - static_cast<float>(head));
		float tail_count = static_cast<float>(
			std::pow(base, digit_count - head_count));

		int tail = static_cast<int>(std::round(tail_raw * tail_count));

		// Конвертация целой части в строку

		do
		{
			int digit = head % base;
			char symbol = static_cast<char>(digit + start_ascii_for_dec);

			result[head_count - digit_position] = symbol;
			--result_char_count;
			++digit_position;

			head /= base;

		} while (head && result_char_count >= 0);

		// Добавляем знак, если он нужен, и точку
		
		if (value < 0)
		{
			result.front() = minus_symbol;
			--result_char_count;
		}

		result[result.size() - result_char_count] = dot_symbol;
		--result_char_count;

		// Конвертация дробной части в строку

		digit_position = 1;

		while (tail && result_char_count > 0)
		{
			int digit = tail % base;
			char symbol = static_cast<char>(digit + start_ascii_for_dec);

			result[result.size() - digit_position] = symbol;
			--result_char_count;
			++digit_position;

			tail /= base;

		};

		return result;
	}

	//*************************************************************************



	/**************************************************************************
	* @brief Обрезать версию XML документа в строку с точностью до последней 
	* значащей цыфры в дробной части
	* 
	* @ingroup general
	*
	* @tparam CharT - тип символов
	* @param verstion - версия XML документа
	*
	* @return строку с обрезанной версией XML документа
	* 
	* @todo Нужно переписать данную функцию, так как она пока-что как
	* временное решение.
	**************************************************************************/
	template<typename CharT>
	inline std::basic_string<CharT> cut_doc_version(float version)
	{
		using symbol_type = CharT;
		using size_type = typename std::basic_string<symbol_type>::size_type;

		auto result = to_string<symbol_type>(version);

		const symbol_type dot_symbol = '.';
		const symbol_type zero_digit_symbol = '0';

		auto found_dot = result.find_first_of(dot_symbol);

		size_type not_zero_digit_index = std::basic_string<symbol_type>::npos;

		for (auto index = found_dot + 1; index != result.size(); ++index)
		{
			if (result[index] != zero_digit_symbol)
			{
				not_zero_digit_index = index;
			}
		}

		if (not_zero_digit_index != std::basic_string<symbol_type>::npos)
		{
			result.erase(not_zero_digit_index + 1);
		}
		else
		{
			result.erase(found_dot + 2);
		}

		result.shrink_to_fit();

		return result;
	}

	//*************************************************************************



	/**************************************************************************
	* @brief Получить суммарное количество символов, необходимое для хранения 
	* готового XML документа
	* 
	* @ingroup general
	*
	* @tparam CharT - тип символов
	* @param document - XML документ
	* @param fill_status - false - если не учитывать заполняющие символы(табы)
	* @param line_break_status - false - если не учитывать перевод на новую
	* строку
	*
	* @return количество необходимых символов для хранения готового XML
	* документа
	* 
	* @todo Нужно переписать данную функцию, так как она пока-что как
	* временное решение.
	**************************************************************************/
	template<typename CharT>
	inline std::size_t symbols_count(
		const XMLB::Document<CharT>& document, bool fill_status = true, 
		bool line_break_status = true)
	{
		using symbol_type = CharT;
		std::size_t result = 0;

		constexpr auto kSpace = 1;
		constexpr auto kOpen_tag = 1;
		constexpr auto kClose_tag = 1;
		constexpr auto kOpen_attr = 1;
		constexpr auto kClose_attr = 1;
		constexpr auto kEqual_attr = 1;
		constexpr auto kDoc_info = 1;
		constexpr auto kDoc_last_info = 1;
		constexpr auto kLine_break = line_break_status ? 1 : 0;

		constexpr auto kXml = 3;
		constexpr auto kVersion = 7;
		constexpr auto kEncoding = 8;

		result += kOpen_tag;
		result += kDoc_info;
		result += kXml;

		result += kSpace;
		result += kVersion;
		result += kEqual_attr;
		result += kOpen_attr;
		result += cut_doc_version<symbol_type>(document.get_version()).size();
		result += kClose_attr;

		result += kSpace;
		result += kEncoding;
		result += kEqual_attr;
		result += kOpen_attr;
		result += document.get_encoding_type().size();
		result += kClose_attr;

		result += kDoc_last_info;
		result += kClose_tag;
		result += kLine_break;

		result += symbols_count(document.cbegin(), document.cend(), 
			fill_status, line_break_status);

		return result;
	}

	//*************************************************************************



	/**************************************************************************
	* @brief Получить суммарное количество символов, необходимое для хранения 
	* готового XML документа
	* 
	* @ingroup general
	*
	* @tparam CharT - тип символов
	* @param first - итератор на начало XML последовательности
	* @param last - итератор на конец XML последовательности
	* @param fill_status - false - если не учитывать заполняющие символы(табы)
	* @param line_break_status - false - если не учитывать перевод на новую
	* строку
	*
	* @return количество необходимых символов для хранения готового XML
	* документа
	* 
	* @todo Нужно переписать данную функцию, так как она пока-что как
	* временное решение.
	**************************************************************************/
	template<typename CharT>
	inline std::size_t symbols_count(
		detail::Node_const_iterator<CharT> first, 
		detail::Node_const_iterator<CharT> last,
		bool fill_status = true,
		bool line_break_status = true)
	{
		using const_iterator = detail::Node_const_iterator<CharT>;

		std::size_t result = 0;

		if (first == last)
		{
			return result;
		}

		constexpr auto kFill = fill_status ? 1 : 0;
		constexpr auto kSpace = 1;
		constexpr auto kOpen_tag = 1;
		constexpr auto kClose_tag = 1;
		constexpr auto kSingle_tag = 1;
		constexpr auto kLast_tag = 1;
		constexpr auto kOpen_attr = 1;
		constexpr auto kClose_attr = 1;
		constexpr auto kEqual_attr = 1;
		constexpr auto kLine_break = line_break_status ? 1 : 0;
		constexpr auto kTab = 1;
		constexpr auto kCarriage = 1;

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
					result += node_groups.top().get_offset() * kFill;

					result += kOpen_tag;
					result += kLast_tag;
					result += node_groups.top()->get_name().size();
					result += kClose_tag;
					result += kLine_break;

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
			result += first.get_offset() * kFill;

			//Заносим в файл сам тег и его информацию
			result += kOpen_tag;
			result += first->get_name().size();

			//Если у XML тега есть аттрибуты, то записываем в файл
			//их имена и значения
			if (first->attr_size())
			{
				for (auto at_it = first->attr_begin(),
					at_end = first->attr_end();
					at_it != at_end;
					++at_it)
				{
					result += kSpace;

					result += at_it->name.size();
					result += kEqual_attr;

					result += kOpen_attr;
					result += at_it->value.size();
					result += kClose_attr;
				}
			}

			//Если у XML тега нет дочерних узлов и есть значение, то
			//записываем это значение и доп. символы по шаблону
			if (!first->child_size() && first->get_value().size())
			{
				result += kClose_tag;
				result += first->get_value().size();

				result += kOpen_tag;
				result += kLast_tag;

				result += first->get_name().size();
				result += kClose_tag;
			}
			//Если у XML тега нет дочерних узлов и нет значения, то
			//записываем доп. символы по шаблону одиночного XML тега
			else if (!first->child_size())
			{
				result += kSpace;
				result += kSingle_tag;
				result += kClose_tag;
			}
			//В противном случае, просто записываем в файл символ
			//закрытия XML тега
			else
			{
				result += kClose_tag;
			}

			result += kLine_break;
		}

		//Если остались ещё незакрытые теги, то заносим в файл их закрытие
		while (!node_groups.empty())
		{
			//Записываем нужное количество табов для тега
			result += node_groups.top().get_offset() * kFill;

			result += kOpen_tag;
			result += kLast_tag;
			result += node_groups.top()->get_name().size();
			result += kClose_tag;
			result += kLine_break;

			node_groups.pop();
		}

		return result;
	}

	//*************************************************************************



	/**************************************************************************
	* @brief Получить суммарное количество символов, необходимое для хранения
	* готового XML документа
	* 
	* @ingroup general
	*
	* @tparam CharT - тип символов
	* @param node - XML узел
	* @param fill_status - false - если не учитывать заполняющие символы(табы)
	* @param line_break_status - false - если не учитывать перевод на новую
	* строку
	*
	* @return количество необходимых символов для хранения готового XML
	* документа
	* 
	* @todo Нужно переписать данную функцию, так как она пока-что как
	* временное решение.
	**************************************************************************/
	template<typename CharT>
	inline std::size_t symbols_count(const Node<CharT>& node,
		bool fill_status = true,
		bool line_break_status = true)
	{
		using const_iterator = detail::Node_const_iterator<CharT>;

		std::size_t result = 0;

		constexpr auto kFill = fill_status ? 1 : 0;
		constexpr auto kSpace = 1;
		constexpr auto kOpen_tag = 1;
		constexpr auto kClose_tag = 1;
		constexpr auto kSingle_tag = 1;
		constexpr auto kLast_tag = 1;
		constexpr auto kOpen_attr = 1;
		constexpr auto kClose_attr = 1;
		constexpr auto kEqual_attr = 1;
		constexpr auto kLine_break = line_break_status ? 1 : 0;
		constexpr auto kTab = 1;
		constexpr auto kCarriage = 1;

		result += kOpen_tag;
		result += node.get_name().size();

		//Если у XML тега есть аттрибуты, то записываем в файл
		//их имена и значения
		if (node.attr_size())
		{
			for (auto at_it = node.attr_begin(),
				at_end = node.attr_end();
				at_it != at_end;
				++at_it)
			{
				result += kSpace;

				result += at_it->name.size();
				result += kEqual_attr;

				result += kOpen_attr;
				result += at_it->value.size();
				result += kClose_attr;
			}
		}

		//Если у XML тега нет дочерних узлов и есть значение, то
		//записываем это значение и доп. символы по шаблону
		if (!node.child_size() && node.get_value().size())
		{
			result += kClose_tag;
			result += node.get_value().size();

			result += kOpen_tag;
			result += kLast_tag;

			result += node.get_name().size();
			result += kClose_tag;
		}
		//Если у XML тега нет дочерних узлов и нет значения, то
		//записываем доп. символы по шаблону одиночного XML тега
		else if (!node.child_size())
		{
			result += kSpace;
			result += kSingle_tag;
			result += kClose_tag;
		}
		//В противном случае, просто записываем в файл символ
		//закрытия XML тега
		else
		{
			result += kClose_tag;

			result += kLine_break;
			result += kFill;

			result += kOpen_tag;
			result += kLast_tag;
			result += node.get_name().size();
			result += kClose_tag;
		}

		result += symbols_count(node.begin(), node.end(),
			fill_status, line_break_status);

		return result;
	}

	//*************************************************************************

} // namespace XMLB

#endif // !XMLB_UTILITY_H