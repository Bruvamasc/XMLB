/******************************************************************************
* @file
* Данный файл объявляет и определяет класс парсера XML документов
*
* @author Bruvamasc
* @date   2022-08-25
*
******************************************************************************/

#ifndef XMLB_PARSER_H
#define XMLB_PARSER_H

#include <vector>
#include <functional>

#include "XMLB/detail/parser/XMLB_Parser_states.h"

namespace XMLB { namespace detail {

	/**************************************************************************
	* @brief Парсер XML документов
	*
	* @details Сам парсер не парсит данные. Он просто передаёт входные
	* данные функторам, которые он содержит. Функторы можно добавлять и
	* удалять. Для скорости специально выбран контейнер вектор для хранения
	* функторов. Поэтому индекс функтора должен соответствовать своему
	* состоянию (Base_state)
	**************************************************************************/
	template<typename InputT, typename DecorT, typename DataT>
	class Parser final
	{
	public:
		using input_symbol_type = InputT;
		using symbol_type = std::decay_t<input_symbol_type>;
		using state_type = Base_state;
		using data_controller_type = DataT;
		using decorator_type = DecorT;

		using functor_type = std::function<state_type(input_symbol_type,
			data_controller_type&, const decorator_type&)>;

		using tags_container_type = typename DataT::tags_container_type;

		// Конструкторы, деструкторы и т.п.

		Parser(std::size_t functors_count = 0);
		Parser(const Parser&) = default;
		Parser& operator=(const Parser&) = default;
		Parser(Parser&&) = default;
		Parser& operator=(Parser&&) = default;
		~Parser() = default;

		// Основные методы

		template<typename CharT>
		bool parse(CharT&& symbol, const DecorT& decorator);

		tags_container_type& get_doc_info();
		tags_container_type& get_tags();

		// Управление функторами

		///@todo Добавить проверку на совместимость типов функторов
		template<typename FunctorT>
		void add_functor(FunctorT functor);

		void erase_functor(std::size_t idnex);

		std::size_t get_functor_count() const noexcept;

	private:
		data_controller_type m_data;
		std::vector<functor_type> m_functors;
		state_type m_current_state;
	};

	//*************************************************************************

	

	/**************************************************************************
	*					Parser<CharT, DecorT, DataT> IMPLEMENTATION
	**************************************************************************/

	template<typename InputT, typename DecorT, typename DataT>
	Parser<InputT, DecorT, DataT>::Parser(std::size_t functors_count)
		:m_current_state{ Start_state{} }
	{
		m_functors.reserve(functors_count);
	}

	//*************************************************************************

	template<typename InputT, typename DecorT, typename DataT>
	template<typename CharT>
	inline bool Parser<InputT, DecorT, DataT>::parse(CharT&& symbol,
		const DecorT& decorator)
	{
		bool result = true;

		m_current_state = m_functors[m_current_state](
			std::forward<CharT>(symbol), m_data, decorator);

		if (m_current_state >= m_functors.size())
		{
			result = false;
		}

		return result;
	}

	//*************************************************************************

	template<typename InputT, typename DecorT, typename DataT>
	inline typename Parser<InputT, DecorT, DataT>::tags_container_type&
		Parser<InputT, DecorT, DataT>::get_doc_info()
	{
		return m_data.get_doc_info();
	}

	//*************************************************************************

	template<typename InputT, typename DecorT, typename DataT>
	inline typename Parser<InputT, DecorT, DataT>::tags_container_type&
		Parser<InputT, DecorT, DataT>::get_tags()
	{
		return m_data.get_tags();
	}

	//*************************************************************************

	template<typename InputT, typename DecorT, typename DataT>
	template<typename FunctorT>
	inline void Parser<InputT, DecorT, DataT>::add_functor(FunctorT functor)
	{
		m_functors.emplace_back(functor);
	}

	//*************************************************************************

	template<typename InputT, typename DecorT, typename DataT>
	inline void Parser<InputT, DecorT, DataT>::erase_functor(
		std::size_t index)
	{
		m_functors.erase(std::next(begin(m_functors), index));
	}

	//*************************************************************************

	template<typename InputT, typename DecorT, typename DataT>
	inline std::size_t Parser<InputT, DecorT, DataT>::get_functor_count()
		const noexcept
	{
		return m_functors.size();
	}

	//*************************************************************************

}} // namespace XMLB::detail

#endif // !XMLB_PARSER_H