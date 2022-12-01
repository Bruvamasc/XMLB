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



#ifndef XMLB_PARSER_H
#define XMLB_PARSER_H

#include <vector>
#include <functional>

#include "XMLB/detail/parser/XMLB_Parser_states.h"



namespace XMLB { namespace detail {

	/**************************************************************************
	* @brief Класс, который управляет синтаксическим анализом входных данных.
	* 
	* @ingroup secondary
	*
	* @details Данный класс хранит наборы состояний-команд и данные для
	* выполнения синтаксического анализа. Есть возможность создавать объекты
	* данного класса со своими наборами состояний-команд и классом хранения
	* данных.
	* 
	* @tparam InputT - тип входящих символов символов
	* @tparam DecorT - тип декоратора
	* @tparam DataT - тип контроллера данных
	* 
	* @todo Возможно, нужно подумать над оптимизацией данного класса
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



		/// @name Конструкторы, деструктор
		/// @{
		Parser(std::size_t functors_count = 1);
		Parser(const Parser&) = default;
		Parser& operator=(const Parser&) = default;
		Parser(Parser&&) = default;
		Parser& operator=(Parser&&) = default;
		~Parser() = default;
		/// @}



		/// @name Основные методы
		/// @{
		/**********************************************************************
		* @brief Выполнить синтаксический анализ переданного символа
		*
		* @todo Подумать над оптимизацией данной функии. Возможно, передавать
		* decorator в конструкторе Parser
		*
		* @tparam CharT - тип символа
		* @param symbol - символ
		* @param decorator - декоратор
		* 
		* @return false - если возвращаемого состояния нет в контейнере 
		* состояний или контейнер пустой. В противном случае true 
		**********************************************************************/
		template<typename CharT>
		bool parse(CharT&& symbol, const DecorT& decorator);

		/**********************************************************************
		* @brief Возвращает контейнер с информацией о документе
		*
		* @return контейнер с информацией о документе
		**********************************************************************/
		tags_container_type& get_doc_info() & noexcept;

		/**********************************************************************
		* @brief Возвращает контейнер с тегами
		*
		* @return контейнер с тегами
		**********************************************************************/
		tags_container_type& get_tags() & noexcept;
		/// @}

		

		/// @name Управление функторами
		/// @{
		/**********************************************************************
		* @brief Добавить функтор в парсер
		* 
		* @todo Добавить проверку типа функтора и сортировку
		* 
		* @details Если пасрер уже содержит передаваемое состояние и функтор,
		* то функтор этого состояния заменится на передаваемый. Все функторы и
		* состояния находятся в упорядоченном состоянии от 0 до 
		* max(unsinged int).
		*
		* @tparam FunctorT тип функтора
		* @param state - состояние соответствующее функтору
		* @param functor - функтор
		**********************************************************************/
		template<typename FunctorT>
		void add_functor(state_type state, FunctorT functor);

		/**********************************************************************
		* @brief Удалить функтор
		* 
		* @details Если несуществует функтора под передаваемым индексом, то 
		* функция ничего не сделает
		*
		* @param index - индекс удаляемого функтора
		**********************************************************************/
		void erase_functor(std::size_t idnex);
		
		/**********************************************************************
		* @brief Возвращает количество функторов
		*
		* @return количество функторов
		**********************************************************************/
		std::size_t get_functor_count() const noexcept;
		/// @}

	private:
		data_controller_type m_data;
		std::vector<functor_type> m_functors;
		state_type m_current_state;
	};

	//*************************************************************************

	

	//*************************************************************************
	//				Parser<CharT, DecorT, DataT> IMPLEMENTATION
	//*************************************************************************

	template<typename InputT, typename DecorT, typename DataT>
	Parser<InputT, DecorT, DataT>::Parser(std::size_t functors_count)
		:m_current_state {Start_state{}}
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
			symbol, m_data, decorator);

		if (m_current_state >= m_functors.size() || !m_functors.size())
		{
			result = false;
		}

		return result;
	}

	//*************************************************************************

	template<typename InputT, typename DecorT, typename DataT>
	inline typename Parser<InputT, DecorT, DataT>::tags_container_type&
		Parser<InputT, DecorT, DataT>::get_doc_info() & noexcept
	{
		return m_data.get_doc_info();
	}

	//*************************************************************************

	template<typename InputT, typename DecorT, typename DataT>
	inline typename Parser<InputT, DecorT, DataT>::tags_container_type&
		Parser<InputT, DecorT, DataT>::get_tags() & noexcept
	{
		return m_data.get_tags();
	}

	//*************************************************************************

	template<typename InputT, typename DecorT, typename DataT>
	template<typename FunctorT>
	inline void Parser<InputT, DecorT, DataT>::add_functor(state_type state,
		FunctorT functor)
	{
		if (state < m_functors.size())
		{
			m_functors[state] = std::move(functor);
		}
		else
		{
			m_functors.emplace_back(functor);
		}
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