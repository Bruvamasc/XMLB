/******************************************************************************
* @file
* Данный файл объявляет и определяет итератор для XML узлов. Итератор является
* деталью реализации и настоятельно рекомендуется использовать его отдельно
* от классов, которые сами создают объекты данного итератора.
*
* @author Bruvamasc
* @date   2022-08-25
*
* @todo Нужно подумать, как добавить режим дебага и оператор инкремента
* ///< Указывает, что элемент недоступен для использования
*
******************************************************************************/

#ifndef XMLB_NODE_ITERATOR_IMPL_H
#define XMLB_NODE_ITERATOR_IMPL_H

namespace XMLB
{
	/**************************************************************************
	* @brief XML итератор
	**************************************************************************/
	template<class T>
	class Node_iterator_impl final
	{
	public:
		using iterator_type = T;
		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = iterator_type;
		using difference_type = std::ptrdiff_t;
		using reference = iterator_type&;
		using pointer = iterator_type*;

		Node_iterator_impl(iterator_type* ptr = nullptr);

		Node_iterator_impl(const Node_iterator_impl& iter);
		Node_iterator_impl& operator=(const Node_iterator_impl& iter);

		Node_iterator_impl(Node_iterator_impl&& iter) noexcept;
		Node_iterator_impl& operator=(Node_iterator_impl&& iter) noexcept;

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
		iterator_type* m_ptr;
		unsigned int m_offset;
	};



	/**************************************************************************
	*						NODE_ITERATOR IMPLEMENTATION
	**************************************************************************/

	template<class T>
	inline Node_iterator_impl<T>::Node_iterator_impl(iterator_type* ptr)
		:m_ptr{ ptr },
		m_offset { 0 }
	{

	}

	//*************************************************************************

	template<class T>
	inline Node_iterator_impl<T>::Node_iterator_impl(
		const Node_iterator_impl& iter)
		:m_ptr{ iter.m_ptr },
		m_offset{ iter.m_offset }
	{

	}

	//*************************************************************************

	template<class T>
	inline Node_iterator_impl<T>&Node_iterator_impl<T>::operator=(
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

	template<class T>
	inline Node_iterator_impl<T>::Node_iterator_impl(
		Node_iterator_impl&& iter) noexcept
		:m_ptr{ std::move(iter.m_ptr) },
		m_offset{ std::move(iter.m_offset) }
	{

	}

	//*************************************************************************

	template<class T>
	inline Node_iterator_impl<T>& Node_iterator_impl<T>::operator=(
		Node_iterator_impl&& iter) noexcept
	{
		if (this != &iter)
		{
			swap(iter);
		}

		return *this;
	}

	//*************************************************************************

	template<class T>
	inline bool Node_iterator_impl<T>::operator==(
		const Node_iterator_impl& iter) const noexcept
	{
		return m_ptr == iter.m_ptr;
	}

	//*************************************************************************

	template<class T>
	inline bool Node_iterator_impl<T>::operator!=(
		const Node_iterator_impl& iter) const noexcept
	{
		return !(*this == iter);
	}

	//*************************************************************************

	template<class T>
	inline typename Node_iterator_impl<T>::reference
		Node_iterator_impl<T>::operator*() const
	{
		if (m_ptr)
		{
			return *m_ptr;
		}
		else
		{
			throw std::exception("Node_iterator point to nullptr");
		}
	}

	//*************************************************************************

	template<class T>
	inline typename Node_iterator_impl<T>::pointer
		Node_iterator_impl<T>::operator->() const
	{
		if (m_ptr)
		{
			return m_ptr;
		}
		else
		{
			throw std::exception("Node_iterator point to nullptr");
		}
	}

	//*************************************************************************

	template<class T>
	inline Node_iterator_impl<T>& Node_iterator_impl<T>::operator++()
	{
		if (m_ptr && m_ptr->child_size())
		{
			m_ptr = m_ptr->child_begin()->get();
			++m_offset;
		}
		else if (m_ptr && !m_ptr->child_size())
		{
			auto tmp_ptr = m_ptr;
			auto tmp_parent_ptr = tmp_ptr->get_parent();

			while (tmp_ptr && tmp_parent_ptr)
			{
				auto found_Typeator = tmp_parent_ptr->child_end();

				for (auto it = tmp_parent_ptr->child_begin(),
					end = tmp_parent_ptr->child_end();
					it != end;
					++it)
				{
					if (it->get() == tmp_ptr)
					{
						found_Typeator = it;
						++found_Typeator;
						break;
					}
				}

				if (found_Typeator != tmp_parent_ptr->child_end())
				{
					m_ptr = found_Typeator->get();

					break;
				}
				else
				{
					tmp_ptr = tmp_parent_ptr;
					tmp_parent_ptr = tmp_ptr->get_parent();

					--m_offset;
				}
			}

			if (!tmp_ptr || !tmp_parent_ptr)
			{
				m_ptr = nullptr;

				m_offset = 0;
			}
		}
		else
		{
			m_ptr = nullptr;

			m_offset = 0;
		}

		return *this;
	}

	//*************************************************************************

	template<class T>
	inline Node_iterator_impl<T> Node_iterator_impl<T>::operator++(int)
	{
		Node_iterator_impl<T> temp_iterator{ *this };

		++(*this);

		return temp_iterator;
	}

	//*************************************************************************

	template<class T>
	inline Node_iterator_impl<T>& Node_iterator_impl<T>::operator--()
	{
		

		return *this;
	}

	//*************************************************************************

	template<class T>
	inline Node_iterator_impl<T> Node_iterator_impl<T>::operator--(int)
	{
		Node_iterator_impl<T> temp_iterator{ *this };

		--(*this);

		return temp_iterator;
	}

	//*************************************************************************

	template<class T>
	inline unsigned int Node_iterator_impl<T>::get_offset() const noexcept
	{
		return m_offset;
	}

	//*************************************************************************

	template<class T>
	inline void Node_iterator_impl<T>::swap(Node_iterator_impl& iter) noexcept
	{
		std::swap(m_ptr, iter.m_ptr);
		std::swap(m_offset, iter.m_offset);
	}

	//*************************************************************************



	/**************************************************************************
	*								FUNCTIONS
	**************************************************************************/

	template<class T>
	inline void swap(Node_iterator_impl<T>& lhs, 
		Node_iterator_impl<T>& rhs) noexcept
	{
		lhs.swap(rhs);
	}

	//*************************************************************************

} // namespace XMLB

#endif // !XMLB_NODE_ITERATOR_IMPL_H





/**************************************************************************
	* @brief Класс, узла XML структуры
	*
	* @details Деструктор объявлен виртуальным. Можно наследоваться от данного
	* класса и расширять свойства задачи. Выбран класс, а не структура с
	* расчетом на перспективу и расширения - чтобы сохранить инкапсуляцию при
	* наследовании
	*
	* @todo Нужно реализовать добавление дочерних узлов с учётом порядка. Чтобы
	* соблюдался инфариант XML структуры
	**************************************************************************/


	/**************************************************************************
		* @brief Данная функция проверяет корректность даты
		*
		* @param[in] day - проверяемый день
		* @param[in] month - проверяемый месяц
		* @param[in] year - проверяемый год
		*
		* @return true - если всё корректно, false - если не корректно
		**************************************************************************/