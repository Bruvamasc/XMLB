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

#ifndef XMLB_TEMPLATES_H
#define XMLB_TEMPLATES_H

#include <string>
#include <codecvt>
#include <locale>

namespace XMLB
{
	/**************************************************************************
	* @brief Вернуть категорию итератора
	**************************************************************************/
	template<typename Iter>
	using iterator_category_t = 
		typename std::iterator_traits<Iter>::iterator_category;

	//*************************************************************************



	/**************************************************************************
	* @brief Вернуть тип содержимого у итератора
	**************************************************************************/
	template<typename Iter>
	using iterator_value_t = typename std::iterator_traits<Iter>::value_type;

	//*************************************************************************



	/**************************************************************************
	* @brief Вернуть тип тип ссылки содержимого у итератора
	**************************************************************************/
	template<typename Iter>
	using iterator_reference_t = 
		typename std::iterator_traits<Iter>::reference;

	//*************************************************************************



	/**************************************************************************
	* @brief Есть ли у типа оператор++()
	**************************************************************************/
	template<typename T>
	using is_has_operator_plus_plus = decltype(++std::declval<T&>());

	//*************************************************************************



	/**************************************************************************
	* @brief Есть ли у типа оператор*()
	**************************************************************************/
	template<typename T>
	using is_has_operator_indirect_conversion = 
		decltype(*std::declval<T&>());

	//*************************************************************************



	/**************************************************************************
	* @brief Есть ли у типа оператор==(T)
	**************************************************************************/
	template<typename T>
	using is_has_operator_self_equality = 
		decltype(std::declval<T&>().operator==(std::declval<T&>()));

	//*************************************************************************



	/**************************************************************************
	* @brief Вспомагательная структура, для реализации функции parse_to_node
	* Является деталью реализации!
	**************************************************************************/
	template<typename Iter, typename T>
	struct Tag_range_impl final
	{
		Iter first;				//Итератор на начало диапазона
		Iter last;				//Итератор на конец диапазона
		T node;					//XML узел
	};

	//*************************************************************************

	template<typename Iter, typename T>
	inline void swap(Tag_range_impl<Iter, T>& lhs,
		Tag_range_impl<Iter, T>& rhs) noexcept
	{
		std::swap(lhs.first, rhs.first);
		std::swap(lhs.last, rhs.last);
		std::swap(lhs.node, rhs.node);
	}

	//*************************************************************************



	/**************************************************************************
	* @brief Вспомагательная структура, для "дерева" XML узлов.
	* Является деталбю реализации!
	**************************************************************************/
	template<typename T>
	struct Node_tree_impl final
	{
		T* element = nullptr;					//Указатель на текущий узел
		Node_tree_impl<T>* parent = nullptr;	//Указатель на родителя узала
		Node_tree_impl<T>* next = nullptr;		//Указатель на следующий узел
		Node_tree_impl<T>* prev = nullptr;		//Указатель на предыдущий узел
	};

	//*************************************************************************

	template<typename T>
	inline void swap(Node_tree_impl<T>& lhs, Node_tree_impl<T>& rhs) noexcept
	{
		std::swap(lhs.next, rhs.next);
		std::swap(lhs.prev, rhs.prev);
		std::swap(lhs.parent, rhs.parent);
		std::swap(lhs.element, rhs.element);
	}

	//*************************************************************************



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
		std::enable_if_t<std::is_fundamental_v<InternT> && 
		std::is_fundamental_v<ExternT> && 
		std::is_integral_v<InternT> && 
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

	//*************************************************************************

} // namespace XMLB

#endif // !XMLB_TEMPLATES_H