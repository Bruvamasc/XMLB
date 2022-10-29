/******************************************************************************
* @file
* Данный файл объявляет и определяет класс code_converter для конвертиации
* из одной разрядности типа char в другую
*
* @author Bruvamasc
* @date   2022-08-25
*
* @todo Подумать над возможными изменениями Node_tree_impl, code_converter
* ///< Указывает, что элемент недоступен для использования
*
******************************************************************************/

#ifndef XMLB_CODE_CONVERTER_H
#define XMLB_CODE_CONVERTER_H

#include <string>
#include <codecvt>
#include <locale>

namespace XMLB
{
	/**************************************************************************
	* @brief Класс, конвертирующий символы из одной кодировки в другую
	*
	* @todo Подумать над тем, нужно ли создавать копию, если тип символов у
	* входной строки и конвертированной одинаковые
	*
	* @param[in] IntertT - тип символов строки, который больше или равен
	* размеру типа ExternT
	* @param[in] ExternT - тип символов строки, который меньше или равен
	* размеру типа InternT
	**************************************************************************/
	template<typename InternT,
		typename ExternT,
		typename StateT = std::mbstate_t,
		std::enable_if_t<std::is_fundamental_v<InternT>&&
		std::is_fundamental_v<ExternT>&&
		std::is_integral_v<InternT>&&
		std::is_integral_v<ExternT> &&
		sizeof(InternT) >= sizeof(ExternT), std::nullptr_t> = nullptr>
	class code_converter
	{
	public:
		using intern_type = InternT;
		using extern_type = ExternT;
		using state_type = StateT;

		using out_string = std::basic_string<extern_type>;
		using in_string = std::basic_string<intern_type>;



		code_converter() { init(); };
		code_converter(const code_converter&) = delete;
		code_converter& operator=(const code_converter&) = delete;
		code_converter(code_converter&&) = delete;
		code_converter& operator=(code_converter&&) = delete;

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