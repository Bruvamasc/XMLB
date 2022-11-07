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



#ifndef XMLB_CODE_CONVERTER_H
#define XMLB_CODE_CONVERTER_H

#include <string>
#include <codecvt>
#include <locale>



namespace XMLB
{
	/**************************************************************************
	* @brief Класс, конвертирующий символы из одной кодировки(типа) в другую
	* 
	* @ingroup general
	*
	* @tparam InternT - тип символов, который больше или равен размеру типа 
	* ExternT
	* @tparam ExternT - тип символов, который меньше или равен размеру типа
	* InternT
	* @tparam StateT - тип, который представляет преобразования состояний в 
	* опеределнных наборах правил кодировки
	* 
	* @todo Подумать над тем, нужно ли создавать копию, если тип символов у
	* входной строки и конвертированной одинаковые. Также нужно подумать
	* над интерфейсом класса
	**************************************************************************/
	template<typename InternT,
		typename ExternT,
		typename StateT = std::mbstate_t,
		std::enable_if_t<std::is_fundamental_v<InternT> &&
		std::is_fundamental_v<ExternT>&&
		std::is_integral_v<InternT>&&
		std::is_integral_v<ExternT> &&
		sizeof(InternT) >= sizeof(ExternT), std::nullptr_t> = nullptr>
	class Code_converter
	{
	public:
		using intern_type = InternT;
		using extern_type = ExternT;
		using state_type = StateT;

		using out_string = std::basic_string<extern_type>;
		using in_string = std::basic_string<intern_type>;


		/// @name Конструкторы, деструктор
		/// @{
		Code_converter() { init(); };
		Code_converter(const Code_converter&) = delete;
		Code_converter& operator=(const Code_converter&) = delete;
		Code_converter(Code_converter&&) = delete;
		Code_converter& operator=(Code_converter&&) = delete;
		~Code_converter() = default;
		/// @}


		/// @name Методы конвертирования
		/// @{
		/**********************************************************************
		* @brief Конвертировать строку с символами типа ExternT в строку с 
		* символами типа InternT
		* 
		* @param from_str - строка, которую нужно конвертировать
		*
		* @return преобразованную строку с символами типа InternT
		*
		* @todo Нужно переписать данную функцию, так как она пока-что как
		* временное решение. Нужно добавить hex, oct и binary
		**********************************************************************/
		in_string convert_to_in(const out_string& from_str)
		{
			if constexpr (std::is_same_v<intern_type, extern_type>)
			{
				return from_str;
			}

			in_string result(from_str.size(), '\0');

			const extern_type* from_next = nullptr;
			intern_type* to_next = nullptr;

			state_type state{};

			m_cdvt->in(state,
				&from_str[0], &from_str[from_str.size()], from_next,
				&result[0], &result[result.size()], to_next);

			result.resize(to_next - &result[0]);

			return result;
		}

		//*********************************************************************



		/**********************************************************************
		* @brief Конвертировать строку с символами типа InternT в строку с
		* символами типа ExternT
		*
		* @param from_str - строка, которую нужно конвертировать
		*
		* @return преобразованную строку с символами типа ExternT
		*
		* @todo Нужно переписать данную функцию, так как она пока-что как
		* временное решение. Нужно добавить hex, oct и binary
		**********************************************************************/
		out_string convert_to_out(const in_string& from_str)
		{
			if constexpr (std::is_same_v<intern_type, extern_type>)
			{
				return from_str;
			}

			out_string result(from_str.size() * m_cdvt->max_lenght(), '\0');

			const intern_type* from_next = nullptr;
			extern_type* to_next = nullptr;

			state_type state{};

			m_cdvt->out(state,
				&from_str[0], &from_str[from_str.size()], from_next,
				&result[0], &result[result.size()], to_next);

			result.resize(to_next - &result[0]);

			return result;
		}
		/// @}

	private:
		void init()
		{
			m_cdvt = new std::codecvt<intern_type, extern_type, state_type>;
			m_locale = std::locale(std::locale{}, m_cdvt);
		}

		const std::codecvt<intern_type, extern_type, state_type>* m_cdvt;
		std::locale m_locale;
	};

} // namespace XMLB

#endif // !XMLB_CODE_CONVERTER_H