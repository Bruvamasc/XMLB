/******************************************************************************
* @file
* Данный файл объявлет основной строительный элемент(узел) XML документа.
* На текущий момент, является завершенным без дебаг функций
*
* @author Bruvamasc
* @date   2022-09-17
*
* @todo Нужно подумать, как добавить режим дебага. Также нужно подумать, над
* кодировками - возможно, как-то их переделать
* ///< Указывает, что элемент недоступен для использования
*
******************************************************************************/

#ifndef XMLB_UTILITY_H
#define XMLB_UTILITY_H

#include <string>

namespace XMLB
{
	/**************************************************************************
	* @brief Данная функция конвертирует строку в float
	*
	* @todo Нужно переписать данную функцию, так как она пока-что как
	* временное решение. Нужно добавить hex, oct и binary
	*
	* @return 0.0 - если не удасться конвертировать строку. В противном случае
	* конвертированное число, которое тоже может быть равно 0.0
	**************************************************************************/
	template<typename CharT>
	inline float to_float(const std::basic_string<CharT>& value)
	{
		using symbol_type = CharT;
		using size_type = typename std::basic_string<symbol_type>::size_type;

		float result = 0.f;

		if (value.size())
		{
			const unsigned int base = 10;
			const unsigned int start_ascii_for_dec = 48;
			const unsigned int dot_count = 1;
			const symbol_type dot_symbol = '.';
			const symbol_type minus_symbol = '-';

			auto found_minus = value.find_first_of(minus_symbol);
			auto found_dot = value.find_first_of(dot_symbol);

			size_type head_count = 0;
			size_type tail_count = 0;

			if (found_dot != std::basic_string<CharT>::npos)
			{
				head_count = found_dot;
				tail_count = value.size() - dot_count - head_count;
			}
			else
			{
				head_count = value.size();
			}

			if (found_minus != std::basic_string<CharT>::npos)
			{
				--head_count;
			}

			float head = 0.f;
			float tail = 0.f;
			
			float start_base = 1.f;
			size_type digit_position = found_dot - 1;

			while (head_count)
			{
				float digit = static_cast<float>(value[digit_position] - 
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
				float digit = static_cast<float>(value[digit_position] - 
					start_ascii_for_dec);

				tail += digit > 0 ? digit / start_base : 0.f;
				start_base *= base;

				--tail_count;
				++digit_position;
			}

			result = head + tail;

			if (found_minus != std::basic_string<CharT>::npos)
			{
				result *= -1.f;
			}
		}

		return result;
	}

	//*************************************************************************



	/**************************************************************************
	* @brief Данная функция конвертирует float число в строку
	*
	* @todo Нужно переписать данную функцию, так как она пока-что как
	* временное решение. Нужно добавить hex, oct и binary
	* 
	* @return строку с числом
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
	* @brief Данная функция преобразует версию XML документа в строку с 
	* точностью до последнего значащей цыфры в дробной части
	*
	* @todo Нужно переписать данную функцию, так как она пока-что как
	* временное решение.
	*
	* @return строку с обрезанной версией XML документа
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

} // namespace XMLB

#endif // !XMLB_UTILITY_H