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

#include "XMLB_Node.h"
#include "XMLB_Constants.h"

namespace XMLB
{
	/**************************************************************************
	* @brief Перечесления кодировок
	**************************************************************************/
	enum class Encode_type
	{
		UTF_8,
		UTF_16,
		UTF_32
	};

	//*************************************************************************



	/**************************************************************************
	* @brief Данная функция конвертирует версию XML документа в строку, без
	* лишних нулей.
	*
	* @details Например 1.003400 -> "1.0034". 1.0000000 -> "1.0"
	*
	* @param[in] version - значение версии XML документа. Значение должно быть
	* не отрицательным
	*
	* @return строка содержащая версию документа
	**************************************************************************/
	template<typename CharT>
	inline std::basic_string<CharT> version_to_string(float version)
	{
		if (version < 0)
		{
			version *= -1.f;
		}

		auto result = std::to_string(version);

		auto dot_pos = result.find_first_of('.');
		auto last_number_pos = result.find_first_not_of('0', dot_pos + 1);
		auto first_zero_pos = result.find_first_of('0', dot_pos);

		if (last_number_pos != std::string::npos &&
			first_zero_pos < last_number_pos)
		{
			result.erase(last_number_pos + 1);
		}
		else
		{
			result.erase(first_zero_pos);
		}

		if constexpr (std::is_same_v<char, CharT>)
		{
			return result;
		}
		else
		{
			code_converter<CharT, char> converter;

			return converter.convert_to_in(result);
		}
	}

	//*************************************************************************



	/**************************************************************************
	* @brief Данная функция ищет во входной строке версию XML документа.
	*
	* @param[in] input - строка, в которой может хранится версия XML документа
	* "...version="X.YZ"..." с любым количеством пробелов. Версия должна быть
	* не отрицательной
	*
	* @return если версия была найдена во входной строке и удалось
	* сконвертировать, вернет значение версии XML документа. В противном случае
	* вернет 1.0
	**************************************************************************/
	template<typename CharT>
	inline float string_to_version(const std::basic_string<CharT>& input)
	{
		auto result = 1.f;

		std::smatch version_match;

		if (std::regex_search(input, version_match, kVersion_tag_template) &&
			version_match.size() >= 2)
		{
			result = std::stof(version_match[1]);
		}

		return result;
	}

	//*************************************************************************



	/**************************************************************************
	* @brief Данная функция конвертирует кодировку XML документа в строку
	*
	* @param[in] encoding - кодировка XML документа.
	*
	* @return строка значущая кодировку XML документа
	**************************************************************************/
	template<typename CharT>
	inline std::basic_string<CharT> encoding_to_string(Encode_type doc_encode)
	{
		std::basic_string<CharT> result;

		if (doc_encode == Encode_type::UTF_32)
		{
			result.reserve(6);
			result += 'U';
			result += 'T';
			result += 'F';
			result += '-';
			result += '3';
			result += '2';
		}
		else if (doc_encode == Encode_type::UTF_16)
		{
			result.reserve(6);
			result += 'U';
			result += 'T';
			result += 'F';
			result += '-';
			result += '1';
			result += '6';
		}
		else
		{
			result.reserve(5);
			result += 'U';
			result += 'T';
			result += 'F';
			result += '-';
			result += '8';
		}

		return result;
	}

	//*************************************************************************



	/**************************************************************************
	* @brief Данная функция ищет во входной строке кодировку XML документа.
	*
	* @param[in] input - строка, в которой может хранится версия XML документа
	* "...encoding="XYZ"..." с любым количеством пробелов.
	*
	* @return кодировку XML документа. Если кодировка не была найдена во
	* входной строке, то вернет кодировку UTF-8
	**************************************************************************/
	template<typename CharT>
	inline Encode_type string_to_encoding(
		const std::basic_string<CharT>& input)
	{
		Encode_type result = Encode_type::UTF_8;

		std::smatch encoding_match;

		if (std::regex_search(input, 
			encoding_match, 
			kEncoding_tag_template) &&
			encoding_match.size() >= 2)
		{
			auto&& encode_str = encoding_match[1].str();

			if (encode_str == "UTF-16")
			{
				result = Encode_type::UTF_16;
			}
			else if (encode_str == "UTF-32")
			{
				result = Encode_type::UTF_32;
			}
		}

		return result;
	}

	//*************************************************************************



	/**************************************************************************
	* @brief Данная функция ищет во входной строке кодировку XML документа
	*
	* @param[in] file_name - имя файла
	*
	* @return кодировку XML файла, если она была найдена. В противном случае
	* UTF-8
	**************************************************************************/
	inline Encode_type get_file_encode(const std::string& file_name)
	{
		Encode_type result = Encode_type::UTF_8;

		std::ifstream file{ file_name };

		if (file.is_open())
		{
			std::string temp_string;

			std::getline(file, temp_string);

			result = string_to_encoding(temp_string);

			file.close();
		}

		return result;
	}

	//*************************************************************************

} // namespace XMLB

#endif // !XMLB_UTILITY_H