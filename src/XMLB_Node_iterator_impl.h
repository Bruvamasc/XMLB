/******************************************************************************
* @file
* Данный файл объявляет и определяет итератор для XML узлов. Итератор является
* деталью реализации и настоятельно рекомендуется использовать его отдельно
* от классов, которые сами создают объекты данного итератора.
*
* @author Bruvamasc
* @date   2022-08-25
*
* @todo Нужно подумать, как добавить режим дебага; Оператор инкремента;
* Подумать, как изменить хранение и нужно ли Node_tree_impl в итератор.
* Подумать над операторами и конструкторами для итератора с константными типами
* ///< Указывает, что элемент недоступен для использования
*
******************************************************************************/

#ifndef XMLB_NODE_ITERATOR_IMPL_H
#define XMLB_NODE_ITERATOR_IMPL_H

#include "XMLB_Templates.h"

namespace XMLB
{
	/**************************************************************************
	* @brief XML итератор
	**************************************************************************/
	template<typename T, typename N>
	class Node_iterator_impl final
	{
	public:
		using value_type = T;
		using iterator_category = std::bidirectional_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using reference = value_type&;
		using pointer = value_type*;

		Node_iterator_impl(Node_tree_impl<N>* ptr = nullptr);

		Node_iterator_impl(const Node_iterator_impl& iter);
		Node_iterator_impl& operator=(const Node_iterator_impl& iter);

		Node_iterator_impl(Node_iterator_impl&& iter) noexcept;
		Node_iterator_impl& operator=(Node_iterator_impl&& iter) noexcept;

		template<typename S, typename K = S>
		Node_iterator_impl(Node_iterator_impl<S, K>&& iter) noexcept;

		template<typename S, typename K = S>
		Node_iterator_impl& operator=(Node_iterator_impl<S, K>&& iter) noexcept;

		bool operator==(const Node_iterator_impl& node_iter) const noexcept;
		bool operator!=(const Node_iterator_impl& node_iter) const noexcept;

		reference operator*() const;
		pointer operator->() const;

		Node_iterator_impl& operator++();
		Node_iterator_impl operator++(int);

		Node_iterator_impl& operator--();
		Node_iterator_impl operator--(int);

		/**********************************************************************
		* @brief Данная функция возвращает размер отступа, таба, офсета
		* XML тега в контейнере
		*
		* @return размер отступа, таба, офсета XML тега в контейнере
		**********************************************************************/
		unsigned int get_offset() const noexcept;

		void swap(Node_iterator_impl& iter) noexcept;

	private:
		Node_tree_impl<N>* m_ptr;
		unsigned int m_offset;
	};



	/**************************************************************************
	*						NODE_ITERATOR IMPLEMENTATION
	**************************************************************************/

	template<typename T, typename N>
	inline Node_iterator_impl<T, N>::Node_iterator_impl(Node_tree_impl<N>* ptr)
		:m_ptr{ ptr },
		m_offset { 0 }
	{

	}

	//*************************************************************************

	template<typename T, typename N>
	inline Node_iterator_impl<T, N>::Node_iterator_impl(
		const Node_iterator_impl& iter)
		:m_ptr{ iter.m_ptr },
		m_offset{ iter.m_offset }
	{

	}

	//*************************************************************************

	template<typename T, typename N>
	inline Node_iterator_impl<T, N>& Node_iterator_impl<T, N>::operator=(
		const Node_iterator_impl& iter)
	{
		if (this != &iter)
		{
			m_ptr = iter.m_ptr;
			m_offset = iter.m_offset;
		}

		return *this;
	}

	//*************************************************************************

	template<typename T, typename N>
	inline Node_iterator_impl<T, N>::Node_iterator_impl(
		Node_iterator_impl&& iter) noexcept
		:m_ptr{ std::forward(iter.m_ptr) },
		m_offset{ std::forward(iter.m_offset) }
	{

	}

	//*************************************************************************

	template<typename T, typename N>
	inline Node_iterator_impl<T, N>& Node_iterator_impl<T, N>::operator=(
		Node_iterator_impl&& iter) noexcept
	{
		if (this != &iter)
		{
			swap(iter);
		}

		return *this;
	}

	//*************************************************************************

	template<typename T, typename N>
	inline bool Node_iterator_impl<T, N>::operator==(
		const Node_iterator_impl& iter) const noexcept
	{
		return m_ptr == iter.m_ptr;
	}

	//*************************************************************************

	template<typename T, typename N>
	inline bool Node_iterator_impl<T, N>::operator!=(
		const Node_iterator_impl& iter) const noexcept
	{
		return !(*this == iter);
	}

	//*************************************************************************

	template<typename T, typename N>
	inline typename Node_iterator_impl<T, N>::reference
		Node_iterator_impl<T, N>::operator*() const
	{
		if (m_ptr)
		{
			return *m_ptr->element;
		}
		else
		{
			throw std::exception{ "Node_iterator point to nullptr" };
		}
	}

	//*************************************************************************

	template<typename T, typename N>
	inline typename Node_iterator_impl<T, N>::pointer
		Node_iterator_impl<T, N>::operator->() const
	{
		if (m_ptr)
		{
			return m_ptr->element;
		}
		else
		{
			throw std::exception{ "Node_iterator point to nullptr" };
		}
	}

	//*************************************************************************

	template<typename T, typename N>
	inline Node_iterator_impl<T, N>& Node_iterator_impl<T, N>::operator++()
	{
		//Проверяем, есть ли следующий элемент в узле. Если его не существует
		//то зануляем текущий указатель
		if (!m_ptr->next)
		{
			m_ptr = nullptr;
			m_offset = 0;

			return *this;
		}
		//В противном случае, итерируемся
		else
		{
			//Если у текущего узла нет родителя - значит он первый. Прибавляем
			//единицу к значению таба(оступа)
			if (!m_ptr->parent)
			{
				++m_offset;
			}
			//Если текущий узел является родителем следующего. Прибавляем
			//единицу к значению таба(оступа)
			else if (m_ptr == m_ptr->next->parent)
			{
				++m_offset;
			}
			//Если родитель текущего узла не равне родителю следующего - значит
			//текущий узел является последним дочерним узлом родителя. Нужно
			//отнять N-ое количество табов(оступов)
			else if (m_ptr->parent != m_ptr->next->parent)
			{
				//Временные переменные для удобства
				auto temp_ptr = m_ptr->parent;
				auto currect_parent = m_ptr->next->parent;

				//Повторяем цикл до тех пор, пока временный temp_ptr не будет
				//указывать на родитлея следующего узла.
				while (temp_ptr->parent && temp_ptr != currect_parent)
				{
					temp_ptr = temp_ptr->parent;

					if (m_offset > 0)
					{
						--m_offset;
					}
				}
			}
			
			//Присваимвам текущему укзателю следующий
			m_ptr = m_ptr->next;
		}

		return *this;
	}

	//*************************************************************************

	template<typename T, typename N>
	inline Node_iterator_impl<T, N> Node_iterator_impl<T, N>::operator++(int)
	{
		Node_iterator_impl<T> temp_iterator{ *this };

		++(*this);

		return temp_iterator;
	}

	//*************************************************************************

	template<typename T, typename N>
	inline Node_iterator_impl<T, N>& Node_iterator_impl<T, N>::operator--()
	{
		

		return *this;
	}

	//*************************************************************************

	template<typename T, typename N>
	inline Node_iterator_impl<T, N> Node_iterator_impl<T, N>::operator--(int)
	{
		Node_iterator_impl<T> temp_iterator{ *this };

		--(*this);

		return temp_iterator;
	}

	//*************************************************************************

	template<typename T, typename N>
	inline unsigned int Node_iterator_impl<T, N>::get_offset() const noexcept
	{
		return m_offset;
	}

	//*************************************************************************

	template<typename T, typename N>
	inline void Node_iterator_impl<T, N>::swap(Node_iterator_impl& iter) noexcept
	{
		using std::swap;

		swap(m_ptr, iter.m_ptr);
		swap(m_offset, iter.m_offset);
	}

	//*************************************************************************



	/**************************************************************************
	*								FUNCTIONS
	**************************************************************************/

	template<typename T, typename N>
	inline void swap(Node_iterator_impl<T, N>& lhs, 
		Node_iterator_impl<T, N>& rhs) noexcept
	{
		lhs.swap(rhs);
	}

	//*************************************************************************

} // namespace XMLB

#endif // !XMLB_NODE_ITERATOR_IMPL_H