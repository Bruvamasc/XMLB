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

#ifndef XMLB_NODE_ITERATOR_H
#define XMLB_NODE_ITERATOR_H

#include <iterator>

#include "XMLB/detail/utilities/XMLB_sup_types.h"

namespace XMLB { namespace detail {

	/**************************************************************************
	* @brief Константный итератор XML структуры
	**************************************************************************/
	template<typename T>
	class Node_const_iterator
	{
	public:
		using value_type = T;
		using iterator_category = std::bidirectional_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using reference = const value_type&;
		using pointer = const value_type*;

		// Конструкторы, деструкторы и т.п.

		Node_const_iterator(detail::Node_tree<T>* ptr = nullptr);

		Node_const_iterator(const Node_const_iterator& iter);
		Node_const_iterator& operator=(const Node_const_iterator& iter);

		Node_const_iterator(Node_const_iterator&& iter) noexcept;
		Node_const_iterator& operator=(Node_const_iterator&& iter) noexcept;

		// Функции сравнения

		bool operator==(const Node_const_iterator& node_iter) const noexcept;
		bool operator!=(const Node_const_iterator& node_iter) const noexcept;

		// Основной интерфейс

		reference operator*() const;
		pointer operator->() const;

		Node_const_iterator& operator++();
		Node_const_iterator operator++(int);

		Node_const_iterator& operator--();
		Node_const_iterator operator--(int);

		/**********************************************************************
		* @brief Данная функция возвращает размер отступа, таба, офсета
		* XML тега в контейнере
		*
		* @details Изначально оступ равен нулю - независимо от того, в
		* какой позиции находится инзначальный итератор.
		*
		* @return размер отступа, таба, офсета XML тега в контейнере
		**********************************************************************/
		unsigned int get_offset() const noexcept;

		void swap(Node_const_iterator& iter) noexcept;

		// Детали реализации!

		/**********************************************************************
		* @brief Данная функция проверяет, принадлежит ли текущий итератор
		* последовательности.
		*
		* @todo Не нашел способа, чтобы убрать эту функцию из
		* пользовательского интерфейса. Нужно подумать, как это исправить.
		* Возможно, всё же придется прибегнуть к наследованию или friend
		* функциям
		*
		* @details Функция не сравнивает на равенство begin и end. Она
		* просто итератоивно проверяет по родителям. Если у текущего
		* итератор родитель или один из родителей будет равен родителю
		* последовательности.
		*
		* @param[in] seq_start - итератора на начало последовательности
		*
		* @return true - если принадлежит, в противном случае - false
		**********************************************************************/
		bool _is_in_sequences(Node_const_iterator seq_start) const noexcept;

	protected:
		detail::Node_tree<T>* m_ptr;
		unsigned int m_offset;
	};

	//*************************************************************************



	/**************************************************************************
	* @brief Итератор XML структуры
	**************************************************************************/
	template<typename T>
	class Node_iterator : public Node_const_iterator<T>
	{
	public:
		using base = Node_const_iterator<T>;
		using value_type = T;
		using iterator_category = std::bidirectional_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using reference = value_type&;
		using pointer = value_type*;

		reference operator*() const;
		pointer operator->() const;

		Node_iterator& operator++();
		Node_iterator operator++(int);

		Node_iterator& operator--();
		Node_iterator operator--(int);
	};

	//*************************************************************************



	/**************************************************************************
	*					NODE_CONST_ITERATOR IMPLEMENTATION
	**************************************************************************/

	template<typename T>
	inline Node_const_iterator<T>::Node_const_iterator(
		detail::Node_tree<T>* ptr)
		:m_ptr{ ptr },
		m_offset{ 0 }
	{

	}

	//*************************************************************************

	template<typename T>
	inline Node_const_iterator<T>::Node_const_iterator(
		const Node_const_iterator& iter)
		:m_ptr{ iter.m_ptr },
		m_offset{ iter.m_offset }
	{

	}

	//*************************************************************************

	template<typename T>
	inline Node_const_iterator<T>& Node_const_iterator<T>::operator=(
		const Node_const_iterator& iter)
	{
		if (this != &iter)
		{
			m_ptr = iter.m_ptr;
			m_offset = iter.m_offset;
		}

		return *this;
	}

	//*************************************************************************

	template<typename T>
	inline Node_const_iterator<T>::Node_const_iterator(
		Node_const_iterator&& iter) noexcept
		:m_ptr{ std::move(iter.m_ptr) },
		m_offset{ std::move(iter.m_offset) }
	{

	}

	//*************************************************************************

	template<typename T>
	inline Node_const_iterator<T>& Node_const_iterator<T>::operator=(
		Node_const_iterator&& iter) noexcept
	{
		if (this != &iter)
		{
			swap(iter);
		}

		return *this;
	}

	//*************************************************************************

	template<typename T>
	inline bool Node_const_iterator<T>::operator==(
		const Node_const_iterator& iter) const noexcept
	{
		return m_ptr == iter.m_ptr;
	}

	//*************************************************************************

	template<typename T>
	inline bool Node_const_iterator<T>::operator!=(
		const Node_const_iterator& iter) const noexcept
	{
		return !(*this == iter);
	}

	//*************************************************************************

	template<typename T>
	inline typename Node_const_iterator<T>::reference
		Node_const_iterator<T>::operator*() const
	{
		return *m_ptr->element;
	}

	//*************************************************************************

	template<typename T>
	inline typename Node_const_iterator<T>::pointer
		Node_const_iterator<T>::operator->() const
	{
		return m_ptr->element;
	}

	//*************************************************************************

	template<typename T>
	inline Node_const_iterator<T>& Node_const_iterator<T>::operator++()
	{
		//Если текущий узел является родителем следующего. Прибавляем
		//единицу к значению таба(оступа)
		if (m_ptr == m_ptr->next->parent)
		{
			++m_offset;
		}
		//Если родитель текущего узла не равен родителю следующего - значит
		//текущий узел является последним дочерним узлом родителя. Нужно
		//отнять N-ое количество табов(оступов)
		else if (m_ptr->parent != m_ptr->next->parent)
		{
			//Временные переменные для удобства
			auto current_parent = m_ptr->parent;
			auto next_parent = m_ptr->next->parent;

			//Повторяем цикл до тех пор, пока временный temp_ptr не будет
			//указывать на родителя следующего узла.
			while (current_parent->parent && current_parent != next_parent)
			{
				current_parent = current_parent->parent;

				if (m_offset > 0)
				{
					--m_offset;
				}
			}
		}

		m_ptr = m_ptr->next;

		return *this;
	}

	//*************************************************************************

	template<typename T>
	inline Node_const_iterator<T> Node_const_iterator<T>::operator++(int)
	{
		Node_const_iterator<T> temp_iterator{ *this };

		++(*this);

		return temp_iterator;
	}

	//*************************************************************************

	template<typename T>
	inline Node_const_iterator<T>& Node_const_iterator<T>::operator--()
	{
		if (m_ptr->parent == m_ptr->prev)
		{
			if (m_offset > 0)
			{
				--m_offset;
			}
		}
		else if (m_ptr->prev->parent != m_ptr->parent)
		{
			//Временные переменные для удобства
			auto current_parent = m_ptr->prev->parent;
			auto prev_parent = m_ptr->parent;

			if (m_offset > 0)
			{
				--m_offset;
			}

			//Повторяем цикл до тех пор, пока временный temp_ptr не будет
			//указывать на родителя следующего узла.
			while (current_parent->parent && current_parent != prev_parent)
			{
				current_parent = current_parent->parent;

				++m_offset;
			}
		}

		m_ptr = m_ptr->prev;

		return *this;
	}

	//*************************************************************************

	template<typename T>
	inline Node_const_iterator<T> Node_const_iterator<T>::operator--(int)
	{
		Node_const_iterator<T> temp_iterator{ *this };

		--(*this);

		return temp_iterator;
	}

	//*************************************************************************

	template<typename T>
	inline unsigned Node_const_iterator<T>::get_offset() const noexcept
	{
		return m_offset;
	}

	//*************************************************************************

	template<typename T>
	inline void Node_const_iterator<T>::swap(Node_const_iterator& iter)
		noexcept
	{
		using std::swap;

		swap(m_ptr, iter.m_ptr);
		swap(m_offset, iter.m_offset);
	}

	//*************************************************************************

	template<typename T>
	inline bool Node_const_iterator<T>::_is_in_sequences(
		Node_const_iterator seq_start) const noexcept
	{
		//Суть функции заключается в том, что она поднимается по родителям
		//указателя в текущем итераторе и сравнивает родителя начала в
		//переданном итераторе начала последовательности

		bool result = false;

		if (seq_start.m_ptr && seq_start.m_ptr->parent && m_ptr)
		{
			auto current_parent = m_ptr->parent;

			while (current_parent)
			{
				if (current_parent == seq_start.m_ptr->parent)
				{
					result = true;
				}

				current_parent = current_parent->parent;
			}
		}

		return result;
	}

	//*************************************************************************



	/**************************************************************************
	*					NODE_ITERATOR IMPLEMENTATION
	**************************************************************************/

	template<typename T>
	inline typename Node_iterator<T>::reference Node_iterator<T>::operator*() 
		const
	{
		return *this->m_ptr->element;
	}

	//*************************************************************************

	template<typename T>
	inline typename Node_iterator<T>::pointer Node_iterator<T>::operator->() 
		const
	{
		return this->m_ptr->element;
	}

	//*************************************************************************

	template<typename T>
	inline Node_iterator<T>& Node_iterator<T>::operator++()
	{
		base::operator++();

		return *this;
	}

	//*************************************************************************

	template<typename T>
	inline Node_iterator<T> Node_iterator<T>::operator++(int)
	{
		Node_iterator<T> temp_iterator{ *this };

		base::operator++();

		return temp_iterator;
	}

	//*************************************************************************

	template<typename T>
	inline Node_iterator<T>& Node_iterator<T>::operator--()
	{
		base::operator--();

		return *this;
	}

	//*************************************************************************

	template<typename T>
	inline Node_iterator<T> Node_iterator<T>::operator--(int)
	{
		Node_iterator<T> temp_iterator{ *this };

		base::operator--();

		return temp_iterator;
	}

	//*************************************************************************



	/**************************************************************************
	*					NODE_CONST_ITERATOR SUPPORT FUNCTIONS
	**************************************************************************/

	template<typename T>
	inline void swap(Node_const_iterator<T>& lhs, Node_const_iterator<T>& rhs) 
		noexcept
	{
		lhs.swap(rhs);
	}

	//*************************************************************************

}} // namespace XMLB::detail

#endif // !XMLB_NODE_ITERATOR_H