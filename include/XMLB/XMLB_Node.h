/******************************************************************************
* @file
* Данный файл объявлет основной строительный элемент(узел) XML документа.
* На текущий момент, является завершенным без дебаг функций
*
* @author Bruvamasc
* @date   2022-09-17
*
* @todo Нужно подумать, как добавить режим дебага; Возможно заменить способ
* хранения дочерних узлов; Возможно заменить Node_tree_impl на что-то другое;
* Придумать, как конструировать итератор, указывающий на конец узлов, но чтобы
* его можно было декрементировать
* ///< Указывает, что элемент недоступен для использования
*
******************************************************************************/

#ifndef XMLB_NODE_H
#define XMLB_NODE_H

#include <string>
#include <string_view>
#include <list>
#include <memory>

#include "XMLB/detail/XMLB_Node_iterator.h"
#include "XMLB/detail/traits/XMLB_Type_traits.h"
#include "XMLB/detail/traits/XMLB_Type_methods_traits.h"

namespace XMLB
{
	/**************************************************************************
	* @brief Аттрибут узла XML структуры
	**************************************************************************/
	template<typename CharT>
	struct Node_attribute final
	{
		std::basic_string<CharT> name;
		std::basic_string<CharT> value;
	};

	//*************************************************************************



	/**************************************************************************
	* @brief Узел XML структуры
	**************************************************************************/
	template<typename CharT>
	class Node final
	{
	public:
		using symbol_type = CharT;
		using string_type = std::basic_string<symbol_type>;
		using string_wrapper = std::basic_string_view<symbol_type>;
		using attribute_type = Node_attribute<symbol_type>;
		using node_type = Node<symbol_type>;
		using tree_node = detail::Node_tree<node_type>;
		using size_type = typename std::list<CharT>::size_type;

		using Ptr = std::unique_ptr<node_type>;

		using attr_iterator = typename std::list<attribute_type>::iterator;
		using attr_const_iterator = 
			typename std::list<attribute_type>::const_iterator;

		using node_iterator = typename std::list<Ptr>::iterator;
		using node_const_iterator = typename std::list<Ptr>::const_iterator;

		using iterator = detail::Node_iterator<node_type>;
		using const_iterator = detail::Node_const_iterator<node_type>;
		
		// Конструкторы, деструкторы и т.п.

		Node(const string_type& name, 
			const string_type& value = string_type{});

		Node(string_type&& name, string_type&& value = string_type{});

		Node(const Node& node);
		Node& operator=(const Node& node);

		Node(Node&& node) noexcept;
		Node& operator=(Node&& node) noexcept;

		~Node() = default;

		// Работа с именем тега

		void set_name(const string_type& name);
		string_wrapper get_name() const & noexcept;

		// Работа со значением тега

		void set_value(const string_type& value);
		string_wrapper get_value() const & noexcept;

		// Работа с аттрибутами тега

		void add_attribute(const attribute_type& attribute);
		void add_attribute(attribute_type&& attribute);

		attr_iterator erase_attribute(std::size_t index);
		attr_iterator erase_attribute(attr_iterator attribute);
		attr_iterator erase_attribute(attr_const_iterator attribute);
		attr_iterator erase_attribute(attr_iterator attribute_first,
			attr_iterator attribute_last);
		attr_iterator erase_attribute(attr_const_iterator attribute_fisrt,
			attr_const_iterator attribute_last);

		size_type attr_size() const noexcept;

		attr_iterator attr_begin() noexcept;
		attr_iterator attr_end() noexcept;

		attr_const_iterator attr_begin() const noexcept;
		attr_const_iterator attr_end() const noexcept;

		attr_const_iterator attr_cbegin() const noexcept;
		attr_const_iterator attr_cend() const noexcept;

		// Работа с дочерними узлами

		/**********************************************************************
		* @brief Данная фукция ищет узел следуя последовательности в списке
		* переданных имён
		* 
		* @todo Вариант с Variadic template и распаковки параметров подходит, 
		* но с ними не будет работать поиск с зарнее созданными контейнерами.
		* Нужно подумать над этим.
		*
		* @param[in] tag_names - список имён тегов
		* @param[in] iter_start - итератор, с которого начать искать
		*
		* @return итератор на узел - если он был найден. В противном случае
		* возвращает итератор указывающий равный end()
		**********************************************************************/
		template<typename ContT = std::initializer_list<string_type>,
			std::enable_if_t<detail::is_has_value_type_v<ContT> &&
			detail::is_has_begin_and_end_v<ContT> && 
			std::is_constructible_v<typename ContT::value_type, string_type>, 
			std::nullptr_t> = nullptr>
		iterator find(ContT&& container, 
			const_iterator offset = const_iterator{ nullptr });

		/**********************************************************************
		* @brief Данная фукция ищет узел следуя последовательности в списке
		* переданных имён
		*
		* @todo Вариант с Variadic template и распаковки параметров подходит,
		* но с ними не будет работать поиск с зарнее созданными контейнерами.
		* Нужно подумать над этим.
		*
		* @param[in] tag_names - список имён тегов
		* @param[in] iter_start - итератор, с которого начать искать
		*
		* @return итератор на узел - если он был найден. В противном случае
		* возвращает итератор указывающий равный end()
		**********************************************************************/
		template<typename ContT = std::initializer_list<string_type>,
			std::enable_if_t<detail::is_has_value_type_v<ContT> &&
			detail::is_has_begin_and_end_v<ContT> &&
			std::is_constructible_v<typename ContT::value_type, string_type>,
			std::nullptr_t> = nullptr>
		const_iterator find(ContT&& container,
			const_iterator offset = const_iterator{ nullptr }) const;

		void add_child(const node_type& node);
		void add_child(node_type&& node);
		void add_child(Ptr node);

		iterator erase_child(std::size_t index);
		iterator erase_child(const_iterator node);
		iterator erase_child(const_iterator node_first, 
			const_iterator node_last);

		iterator begin() noexcept;
		iterator end() noexcept;

		const_iterator begin() const noexcept;
		const_iterator end() const noexcept;

		const_iterator cbegin() const noexcept;
		const_iterator cend() const noexcept;

		size_type child_size() const noexcept;

		// Работа с родителем

		const node_type* get_parent() const noexcept;

		// Вспомагательные функции

		void swap(node_type& node) noexcept;

	private:
		tree_node* find_last_tree_node() const;
		void connect_tree_nodes(tree_node* prev_node);

		iterator erase_element(const_iterator node);
		iterator erase_element(const_iterator node_first, 
			const_iterator node_last);

		iterator find_element(const_iterator first,
			const_iterator last, const string_type& tag_name) const;

	private:
		string_type m_name;
		string_type m_value;
		std::list<attribute_type> m_attributes;
		std::list<Ptr> m_childs;
		mutable tree_node m_tree_node;
	};

	//*************************************************************************



	/**************************************************************************
	*							NODE IMPLEMENTATION
	**************************************************************************/

	template<typename CharT>
	inline Node<CharT>::Node(const string_type& name, const string_type& value)
		:m_name{ name },
		m_value{ value }
	{
		m_tree_node.element = this;
	}

	//*************************************************************************

	template<typename CharT>
	inline Node<CharT>::Node(string_type&& name, string_type&& value)
		:m_name{ std::move(name) },
		m_value{ std::move(value) }
	{
		m_tree_node.element = this;
	}

	//*************************************************************************

	template<typename CharT>
	inline Node<CharT>::Node(const Node<CharT>& node)
		:m_name{ node.m_name },
		m_value{ node.m_value },
		m_attributes{ node.m_attributes.cbegin(), node.m_attributes.cend() }
	{
		m_tree_node.element = this;

		for (auto& elem : node.m_childs)
		{
			add_child(*elem);
		}
	}

	//*************************************************************************

	template<typename CharT>
	inline Node<CharT>& Node<CharT>::operator=(const Node<CharT>& node)
	{
		if (this != &node)
		{
			*this = Node{ node };
		}

		return *this;
	}

	//*************************************************************************

	template<typename CharT>
	inline Node<CharT>::Node(Node<CharT>&& node) noexcept
		:m_name{ std::move(node.m_name) },
		m_value{ std::move(node.m_value) },
		m_attributes{ std::move(node.m_attributes) }
	{
		m_tree_node.element = this;

		for (auto&& elem : node.m_childs)
		{
			add_child(std::move(elem));
		}
	}

	//*************************************************************************

	template<typename CharT>
	inline Node<CharT>& Node<CharT>::operator=(Node<CharT>&& node) noexcept
	{
		if (this != &node)
		{
			swap(node);
		}

		return *this;
	}

	//*************************************************************************

	template<typename CharT>
	inline void Node<CharT>::set_name(const string_type& name)
	{
		m_name = name;
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Node<CharT>::string_wrapper Node<CharT>::get_name() 
		const & noexcept
	{
		return string_wrapper{ m_name };
	}

	//*************************************************************************

	template<typename CharT>
	inline void Node<CharT>::set_value(const string_type& value)
	{
		m_value = value;
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Node<CharT>::string_wrapper Node<CharT>::get_value() 
		const & noexcept
	{
		return string_wrapper{ m_value };
	}

	//*************************************************************************

	template<typename CharT>
	inline void Node<CharT>::add_attribute(const attribute_type& attribute)
	{
		m_attributes.push_back(attribute);
	}

	//*************************************************************************

	template<typename CharT>
	inline void Node<CharT>::add_attribute(attribute_type&& attribute)
	{
		m_attributes.push_back(std::move(attribute));
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Node<CharT>::attr_iterator Node<CharT>::erase_attribute(
		std::size_t index)
	{
		if (index < m_attributes.size())
		{
			return m_attributes.erase(std::next(m_attributes.cbegin(), index));
		}
		else
		{
			return m_attributes.end();
		}
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Node<CharT>::attr_iterator Node<CharT>::erase_attribute(
		attr_iterator attr_iter)
	{
		return m_attributes.erase(attr_iter);
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Node<CharT>::attr_iterator Node<CharT>::erase_attribute(
		attr_const_iterator attr_iter)
	{
		return m_attributes.erase(attr_iter);
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Node<CharT>::attr_iterator Node<CharT>::erase_attribute(
		attr_iterator attribute_first, attr_iterator attribute_last)
	{
		return m_attributes.erase(attribute_first, attribute_last);
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Node<CharT>::attr_iterator Node<CharT>::erase_attribute(
		attr_const_iterator attribute_fisrt, 
		attr_const_iterator attribute_last)
	{
		return m_attributes.erase(attribute_fisrt, attribute_last);
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Node<CharT>::size_type Node<CharT>::attr_size() const 
		noexcept
	{
		return m_attributes.size();
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Node<CharT>::attr_iterator Node<CharT>::attr_begin() 
		noexcept
	{
		return m_attributes.begin();
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Node<CharT>::attr_iterator Node<CharT>::attr_end() noexcept
	{
		return m_attributes.end();
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Node<CharT>::attr_const_iterator Node<CharT>::attr_begin() 
		const noexcept
	{
		return m_attributes.cbegin();
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Node<CharT>::attr_const_iterator Node<CharT>::attr_end() 
		const noexcept
	{
		return m_attributes.cend();
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Node<CharT>::attr_const_iterator Node<CharT>::attr_cbegin() 
		const noexcept
	{
		return m_attributes.cbegin();
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Node<CharT>::attr_const_iterator Node<CharT>::attr_cend() 
		const noexcept
	{
		return m_attributes.cend();
	}

	//*************************************************************************

	template<typename CharT>
	template<typename ContT, 
		std::enable_if_t<detail::is_has_value_type_v<ContT> &&
		detail::is_has_begin_and_end_v<ContT> &&
		std::is_constructible_v<typename ContT::value_type, 
		std::basic_string<CharT>>, std::nullptr_t>
	>
	inline typename Node<CharT>::iterator Node<CharT>::find(ContT&& container, 
		const_iterator offset)
	{
		using std::begin;
		using std::end;

		iterator first = this->begin();
		iterator last = this->end();

		auto word_it = begin(container);
		auto word_end = end(container);

		if (offset != const_iterator{ nullptr } &&
			offset._is_in_sequences(this->begin()))
		{
			first = iterator{ &offset->m_tree_node };
		}

		if (offset == last)
		{
			first = last;
		}

		for (; first != last && word_it != word_end;)
		{
			first = find_element(first, last, *word_it);

			if (first != last)
			{
				++word_it;

				if (word_it != word_end)
				{
					last = first->end();
					first = first->begin();
				}
			}
		}

		return first;
	}

	//*************************************************************************

	template<typename CharT>
	template<typename ContT,
		std::enable_if_t<detail::is_has_value_type_v<ContT>&&
		detail::is_has_begin_and_end_v<ContT>&&
		std::is_constructible_v<typename ContT::value_type,
		std::basic_string<CharT>>, std::nullptr_t>>
		inline typename Node<CharT>::const_iterator Node<CharT>::find(
			ContT&& container, const_iterator offset) const
	{
		using std::begin;
		using std::end;

		auto result = cend();

		auto first = cbegin();
		auto last = result;

		if (offset != const_iterator{ nullptr })
		{
			first = offset;
		}

		auto words_first = begin(container);
		auto words_last = end(container);

		for (; first != last && words_first != words_last; )
		{
			auto found_iter = find_element(first, last, *words_first);

			if (found_iter != last)
			{
				++words_first;

				last = first->cend();
				first = first->cbegin();
			}
			else
			{
				++first;
			}
		}

		if (first != last)
		{
			return first;
		}
		else
		{
			return result;
		}
	}

	//*************************************************************************

	template<typename CharT>
	inline void Node<CharT>::add_child(const node_type& node)
	{
		tree_node* last_tree_node = find_last_tree_node();

		m_childs.push_back(std::make_unique<Node<CharT>>(node));

		connect_tree_nodes(last_tree_node);
	}

	//*************************************************************************

	template<typename CharT>
	inline void Node<CharT>::add_child(node_type&& node)
	{
		tree_node* last_tree_node = find_last_tree_node();

		m_childs.push_back(std::make_unique<Node<CharT>>(std::move(node)));

		connect_tree_nodes(last_tree_node);
	}

	//*************************************************************************

	template<typename CharT>
	inline void Node<CharT>::add_child(Ptr node)
	{
		if (!node)
		{
			return;
		}

		tree_node* last_tree_node = find_last_tree_node();

		m_childs.push_back(std::move(node));

		connect_tree_nodes(last_tree_node);
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Node<CharT>::iterator Node<CharT>::erase_child(
		std::size_t index)
	{
		auto result = end();

		if (index < m_childs.size())
		{
			auto erase_iter = std::next(m_childs.cbegin(), index);

			result = erase_element(const_iterator
				{
					&erase_iter->get()->m_tree_node
				});
		}

		return result;
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Node<CharT>::iterator Node<CharT>::erase_child(
		const_iterator node)
	{
		auto result = end();

		if (node._is_in_sequences(begin()))
		{
			if (node != result)
			{
				result = erase_element(node);
			}
		}

		return result;
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Node<CharT>::iterator Node<CharT>::erase_child(
		const_iterator node_first, const_iterator node_last)
	{
		auto result = end();

		if (node_first != node_last && node_first._is_in_sequences(begin()))
		{
			result = erase_element(node_first, node_last);
		}

		return result;
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Node<CharT>::iterator Node<CharT>::begin() noexcept
	{
		return iterator{ m_tree_node.next };
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Node<CharT>::iterator Node<CharT>::end() noexcept
	{
		return iterator{ find_last_tree_node()->next };
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Node<CharT>::const_iterator Node<CharT>::begin() const 
		noexcept
	{
		return const_iterator{ m_tree_node.next };
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Node<CharT>::const_iterator Node<CharT>::end() const
		noexcept
	{
		return const_iterator{ find_last_tree_node()->next };
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Node<CharT>::const_iterator Node<CharT>::cbegin() const 
		noexcept
	{
		return const_iterator{ m_tree_node.next };
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Node<CharT>::const_iterator Node<CharT>::cend() const 
		noexcept
	{
		return const_iterator{ find_last_tree_node()->next };
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Node<CharT>::size_type Node<CharT>::child_size() const 
		noexcept
	{
		return m_childs.size();
	}

	//*************************************************************************

	template<typename CharT>
	inline const typename Node<CharT>::node_type* Node<CharT>::get_parent() 
		const noexcept
	{
		if (m_tree_node.parent)
		{
			return m_tree_node.parent->element;
		}
		else
		{
			return nullptr;
		}
	}

	//*************************************************************************

	template<typename CharT>
	inline void Node<CharT>::swap(node_type& node) noexcept
	{
		using std::swap;

		//Получаем необходимые начальные узлы текущего элемента

		//Узел, который идёт перед текущим элементом
		auto lhs_first_node = m_tree_node.prev;

		//Узел родитель текущего элемент
		auto lhs_parent_node = m_tree_node.parent;

		//Узел, который идёт после самого последнего дочернего узла 
		//текущего элемента
		auto lhs_last_node = find_last_tree_node()->next;

		//Получаем необходимые начальные узлы элемента node

		//Узел, который идёт перед элементом node
		auto rhs_first_node = node.m_tree_node.prev;

		//Узел родитель элемента node
		auto rhs_parent_node = node.m_tree_node.parent;

		//Узел, который идёт после самого последнего дочернего узла 
		//элемента node
		auto rhs_last_node = node.find_last_tree_node()->next;

		swap(m_name, node.m_name);
		swap(m_value, node.m_value);
		swap(m_attributes, node.m_attributes);
		swap(m_childs, node.m_childs);
		swap(m_tree_node, node.m_tree_node);

		//Присваиваем узлам указатели на текущие элементы
		m_tree_node.element = this;
		node.m_tree_node.element = &node;

		//Конектим первый дочерний элемент с текущим и изменяем им родителя на
		//текущий элемент
		if (m_childs.size())
		{
			m_tree_node.next = &m_childs.front()->m_tree_node;
			m_childs.front()->m_tree_node.prev = &m_tree_node;

			for (auto&& elem : m_childs)
			{
				elem->m_tree_node.parent = &m_tree_node;
			}
		}

		//Конектим первый дочерний элемент с текущим node элементов и изменяем
		//им родителя на текущий node элемент
		if (node.m_childs.size())
		{
			node.m_tree_node.next = &node.m_childs.front()->m_tree_node;
			node.m_childs.front()->m_tree_node.prev = &node.m_tree_node;

			for (auto&& elem : node.m_childs)
			{
				elem->m_tree_node.parent = &node.m_tree_node;
			}
		}

		//Получем новый узел, который является самым последним узлом 
		//или же текущим
		auto lhs_new_last_node = find_last_tree_node();

		//Получем новый узел, который является самым последним узлом 
		//или же текущим node
		auto rhs_new_last_node = node.find_last_tree_node();

		//Соеденяем узлы текущего элемента
		if (lhs_first_node)
		{
			lhs_first_node->next = &m_tree_node;
			m_tree_node.prev = lhs_first_node;
		}
		else
		{
			m_tree_node.prev = nullptr;
		}

		m_tree_node.parent = lhs_parent_node;

		if (lhs_last_node)
		{
			lhs_last_node->prev = lhs_new_last_node;
			lhs_new_last_node->next = lhs_last_node;
		}
		else
		{
			m_tree_node.next = nullptr;
		}

		//Соеденяем узлы элемента node
		if (rhs_first_node)
		{
			rhs_first_node->next = &node.m_tree_node;
			node.m_tree_node.prev = rhs_first_node;
		}
		else
		{
			node.m_tree_node.prev = nullptr;
		}

		node.m_tree_node.parent = rhs_parent_node;

		if (rhs_last_node)
		{
			rhs_last_node->prev = rhs_new_last_node;
			rhs_new_last_node->next = rhs_last_node;
		}
		else
		{
			rhs_new_last_node->next = nullptr;
		}
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Node<CharT>::tree_node* Node<CharT>::find_last_tree_node() 
		const
	{
		//Инициализируем узел, узлом текущего элемента
		tree_node* result = &m_tree_node;

		//Проходимся по всем последним дочерним элементам, чтобы дойти до
		//последнего узла
		while (result->element->m_childs.size())
		{
			result = &result->element->m_childs.back()->m_tree_node;
		}

		return result;
	}

	//*************************************************************************

	template<typename CharT>
	inline void Node<CharT>::connect_tree_nodes(tree_node* prev_node)
	{
		//Укатаель на последний дочерний элемент
		Node* last_child = m_childs.back().get();

		//Указатель на последний дочерний элемент последного дочерного элемента
		//текущего узла
		Node* last_child_element = last_child;

		//Находим элемент, который является последним элементов в списка
		//удаляемого элемента
		while (last_child_element->m_childs.size())
		{
			last_child_element = last_child_element->m_childs.back().get();
		}

		//Узел, который следует после прошлого предыщущего элемента
		tree_node* next_node = prev_node->next;

		//Конектим узлы между собой
		prev_node->next = &last_child->m_tree_node;
		last_child->m_tree_node.prev = prev_node;

		if (!next_node)
		{
			last_child_element->m_tree_node.next = &m_tree_node;
			m_tree_node.prev = &last_child_element->m_tree_node;
		}
		else
		{
			last_child_element->m_tree_node.next = next_node;
			next_node->prev = &last_child_element->m_tree_node;
		}

		last_child->m_tree_node.parent = &m_tree_node;
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Node<CharT>::iterator Node<CharT>::erase_element(
		const_iterator node)
	{
		auto erase_iterator = node->get_parent()->m_childs.begin();

		//Ищем среди дочерних узлов родителя узла, на который указывает
		//итератор. Ищем для того, чтобы удалить его из контейнера std::list
		for (auto it = node->get_parent()->m_childs.begin(),
			end = node->get_parent()->m_childs.end();
			it != end;
			++it)
		{
			if (it->get() == &*node)
			{
				erase_iterator = it;
				break;
			}
		}

		//Элемент, которй нужно удалить и который будет указывать на последний
		//элемент, который является дочерним этого элемент
		Node<CharT>* erase_child = erase_iterator->get();

		//Находим элемент, который является последним элементов в списка
		//удаляемого элемента
		while (erase_child->m_childs.size())
		{
			erase_child = erase_child->m_childs.back().get();
		}

		//Узел, расположенный до элемента, который нужно удалить
		tree_node* prev_node = erase_iterator->get()->m_tree_node.prev;

		//Узел элемента, который идёт после элемента, который нужно удалить
		tree_node* next_node = erase_child->m_tree_node.next;

		//Конектим узлы между собой
		prev_node->next = next_node;

		next_node->prev = prev_node;

		//Удаляем элемент. Временное решение с const_castom...
		Node* parent = const_cast<Node*>(node->get_parent());
		parent->m_childs.erase(erase_iterator);

		return iterator{ next_node };
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Node<CharT>::iterator Node<CharT>::erase_element(
		const_iterator node_first, const_iterator node_last)
	{
		auto result = end();

		if (node_first != node_last)
		{
			result = erase_element(node_first);

			while (result != node_last)
			{
				result = erase_element(result);
			}
		}

		return result;
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Node<CharT>::iterator Node<CharT>::find_element(
		const_iterator first, const_iterator last, 
		const string_type& tag_name) const
	{
		for (; first != last; ++first)
		{
			if (first->get_name() == tag_name)
			{
				return iterator{ &first->m_tree_node };
			}
		}

		return iterator{ &last->m_tree_node };
	}

	//*************************************************************************



	/**************************************************************************
	*						NODE SUPPORT FUNCTIONS
	**************************************************************************/

	template<typename CharT>
	inline bool operator==(const Node<CharT>& lhs, const Node<CharT>& rhs)
	{
		if (lhs.get_name() != rhs.get_name() ||
			lhs.get_value() != rhs.get_value() ||
			lhs.get_parent() != rhs.get_parent() ||
			lhs.attr_size() != rhs.attr_size() ||
			lhs.child_size() != rhs.child_size())
		{
			return false;
		}

		for (auto lhs_it = lhs.attr_cbegin(), lhs_end = lhs.attr_cend(),
			rhs_it = rhs.attr_cbegin();
			lhs_it != lhs_end;
			++lhs_it, ++rhs_it)
		{
			if (lhs_it != rhs_it)
			{
				return false;
			}
		}

		for (auto lhs_it = lhs.child_cbegin(), lhs_end = lhs.child_cend(),
			rhs_it = rhs.child_cbegin();
			lhs_it != lhs_end;
			++lhs_it, ++rhs_it)
		{
			if (lhs_it != rhs_it)
			{
				return false;
			}
		}

		return true;
	}
	
	//*************************************************************************

	template<typename CharT>
	inline bool operator!=(const Node<CharT>& lhs, const Node<CharT>& rhs)
	{
		return !(lhs == rhs);
	}

	//*************************************************************************

	template<typename CharT>
	inline void swap(Node_attribute<CharT>& lhs, Node_attribute<CharT>& rhs) 
		noexcept
	{
		using std::swap;

		swap(lhs.name, rhs.name);
		swap(lhs.value, rhs.value);
	}

	//*************************************************************************

	template<typename CharT>
	inline void swap(Node<CharT>& lhs, Node<CharT>& rhs) noexcept
	{
		lhs.swap(rhs);
	}

	//*************************************************************************

} // namespace XMLB

#endif // !XMLB_NODE_H