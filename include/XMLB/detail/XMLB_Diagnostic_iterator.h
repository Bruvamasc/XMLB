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



#ifndef XMLB_DIAGNOSTIC_ITERATOR_H
#define XMLB_DIAGNOSTIC_ITERATOR_H

#include "XMLB/detail/traits/XMLB_Type_traits.h"



namespace XMLB { namespace detail {

	/**************************************************************************
	* @brief Данный класс является вспомагательным, для функция is_correct
	*
	* @details Данный класс просто обертка над типом итератора и состоянием.
	* Может и уже использутеся в функии is_corret в качестве возвращаемого
	* объекта. Также объект данного класса может конвертироваться в тип
	* хранимого итератора, что даёт возможность его использовать как обычный
	* итератор, где он нужен.
	* 
	* @tparam IterT - тип итератора
	**************************************************************************/
	template<typename IterT>
	class Diagnostic_iterator final
	{
	public:
		using value_type = value_type_t<IterT>;
		using pointer = iterator_pointer_t<IterT>;
		using reference = iterator_reference_t<IterT>;


		/// @name Конструкторы, деструктор
		/// @{
		Diagnostic_iterator() :m_iterator{}, m_status{ false } {};
		Diagnostic_iterator(IterT iterator, bool status)
			:m_iterator{ iterator }, m_status{ status } {};
		Diagnostic_iterator(const Diagnostic_iterator&) = default;
		Diagnostic_iterator& operator=(const Diagnostic_iterator&) = default;
		Diagnostic_iterator(Diagnostic_iterator&&) = default;
		Diagnostic_iterator& operator=(Diagnostic_iterator&&) = default;
		~Diagnostic_iterator() = default;
		/// @}


		/// @name Методы доступа
		/// @{
		pointer operator->()
		{
			return m_iterator.operator->();
		}

		operator IterT() const
			noexcept(noexcept(std::is_nothrow_copy_constructible_v<IterT>))
		{
			return m_iterator;
		}
		/// @}



		/// @name Методы проверки состояния
		/// @{
		operator bool() const noexcept
		{
			return m_status;
		}
		/// @}



		/// @name Вспомагательные методы
		/// @{
		/**********************************************************************
		* @brief Обменять данные
		* 
		* @param rhs - правый Diagnostic_iterator
		**********************************************************************/
		void swap(Diagnostic_iterator& rhs) 
			noexcept(noexcept(
				(std::is_nothrow_move_constructible_v<IterT> && 
					std::is_nothrow_move_assignable_v<IterT>) ||
				(std::is_nothrow_copy_constructible_v<IterT> && 
					std::is_nothrow_copy_assignable_v<IterT>)))
		{
			using std::swap;

			swap(m_iterator, rhs.m_iterator);
			swap(m_status, rhs.m_status);
		}
		/// @}

	private:
		IterT m_iterator;
		bool m_status;
	};

	//*************************************************************************



	//*************************************************************************
	//					DIAGNOSTIC_ITERATOR SUPPORT FUNCTIONS
	//*************************************************************************

	/**************************************************************************
	* @brief Обменять данные двух Diagnostic_iterator
	* 
	* @tparam IterT - тип итератора
	* 
	* @param lhs - первый Diagnostic_iterator
	* @param rhs - второй Diagnostic_iterator
	**************************************************************************/
	template<typename IterT>
	inline void swap(Diagnostic_iterator<IterT>& lhs,
		Diagnostic_iterator<IterT>& rhs) noexcept(noexcept(
			std::declval<Diagnostic_iterator<IterT>>().swap(
				std::declval<Diagnostic_iterator<IterT>()>)))
	{
		using std::swap;

		lhs.swap(rhs);
	}

	//*************************************************************************

}} // namespace XMLB::detail

#endif // !XMLB_FWD_H