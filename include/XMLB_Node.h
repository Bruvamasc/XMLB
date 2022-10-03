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
#include <list>
#include <memory>

#include "XMLB_Templates.h"
#include "XMLB_Node_iterator_impl.h"

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
		using encoding_type = CharT;
		using string_type = std::basic_string<encoding_type>;
		using attribute_type = Node_attribute<encoding_type>;
		using node_type = Node<encoding_type>;
		using tree_node = Node_tree_impl<node_type>;
		using size_type = typename std::list<CharT>::size_type;

		using Ptr = std::unique_ptr<node_type>;

		using attr_iterator = typename std::list<attribute_type>::iterator;
		using attr_const_iterator = 
			typename std::list<attribute_type>::const_iterator;

		using node_iterator = typename std::list<Ptr>::iterator;
		using node_const_iterator = typename std::list<Ptr>::const_iterator;

		using iterator = Node_iterator_impl<node_type, node_type>;
		using const_iterator = Node_iterator_impl<const node_type, node_type>;
		
		// Конструкторы, деструкторы и т.п.

		Node(const string_type& name, 
			const string_type& value = string_type{});

		Node(const Node& node);
		Node& operator=(const Node& node);

		Node(Node&& node) noexcept;
		Node& operator=(Node&& node) noexcept;

		~Node() = default;

		// Работа с именем тега

		void set_name(const string_type& name);
		const string_type& get_name() const & noexcept;

		// Работа со значением тега

		void set_value(const string_type& value);
		const string_type& get_value() const & noexcept;

		// Работа с аттрибутами тега

		void add_attribute(const attribute_type& attribute);
		void add_attribute(attribute_type&& attribute);

		void erase_attribute(std::size_t index);
		void erase_attribute(attr_iterator attr_iter);
		void erase_attribute(attr_const_iterator attr_iter);

		size_type attr_size() const noexcept;

		attr_iterator attr_begin() noexcept;
		attr_iterator attr_end() noexcept;

		attr_const_iterator attr_begin() const noexcept;
		attr_const_iterator attr_end() const noexcept;

		attr_const_iterator attr_cbegin() const noexcept;
		attr_const_iterator attr_cend() const noexcept;

		// Работа с дочерними node

		void add_child(const node_type& node);
		void add_child(node_type&& node);
		void add_child(Ptr node);

		void erase_child(std::size_t index) noexcept;
		void erase_child(node_iterator node_iter) noexcept;
		void erase_child(node_const_iterator node_iter) noexcept;

		node_iterator child_begin() noexcept;
		node_iterator child_end() noexcept;

		node_const_iterator child_begin() const noexcept;
		node_const_iterator child_end() const noexcept;

		node_const_iterator child_cbegin() const noexcept;
		node_const_iterator child_cend() const noexcept;

		size_type child_size() const noexcept;

		// Блок с итераторами

		iterator begin() noexcept;
		iterator end() noexcept;

		const_iterator begin() const noexcept;
		const_iterator end() const noexcept;

		const_iterator cbegin() const noexcept;
		const_iterator cend() const noexcept;

		// Работа с родителем

		const node_type* get_parent() const noexcept;

		// Вспомагательные функции

		void swap(node_type&& node) noexcept;

	private:
		tree_node* find_last_tree_node() const;
		void connect_tree_nodes(tree_node* prev_node);

		void erase_element(size_type index, node_const_iterator iterator);
		void erase_first_or_last_child(node_const_iterator erase_iterator);
		void erase_one_child(node_const_iterator erase_iterator, 
			node_const_iterator next_iterator);

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
		//std::cout << "Node: name == " << m_name;
		//std::cout << ", value == " << m_value << " - Default Ctor\n";
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

		//std::cout << "Node: name == " << m_name;
		//std::cout << ", value == " << m_value << " - Copy Ctor\n";
	}

	//*************************************************************************

	template<typename CharT>
	inline Node<CharT>& Node<CharT>::operator=(const Node<CharT>& node)
	{
		if (this != &node)
		{
			Node<CharT> temp_node(node);

			swap(std::forward(temp_node));
		}

		//std::cout << "Node: name == " << m_name;
		//std::cout << ", value == " << m_value << " - Copy assigment\n";

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
			add_child(std::forward(elem));
		}

		//std::cout << "Node: name == " << m_name;
		//std::cout << ", value == " << m_value << " - Move Ctor\n";
	}

	//*************************************************************************

	template<typename CharT>
	inline Node<CharT>& Node<CharT>::operator=(Node<CharT>&& node) noexcept
	{
		if (this != &node)
		{
			swap(std::forward(node));
		}

		//std::cout << "Node: name == " << m_name;
		//std::cout << ", value == " << m_value << " - Move assigment\n";

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
	inline const typename Node<CharT>::string_type& Node<CharT>::get_name() 
		const & noexcept
	{
		return m_name;
	}

	//*************************************************************************

	template<typename CharT>
	inline void Node<CharT>::set_value(const string_type& value)
	{
		m_value = value;
	}

	//*************************************************************************

	template<typename CharT>
	inline const typename Node<CharT>::string_type& Node<CharT>::get_value() 
		const & noexcept
	{
		return m_value;
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
	inline void Node<CharT>::erase_attribute(std::size_t index)
	{
		if (index < m_attributes.size())
		{
			m_attributes.erase(std::next(m_attributes.cbegin(), index));
		}
	}

	//*************************************************************************

	template<typename CharT>
	inline void Node<CharT>::erase_attribute(attr_iterator attr_iter)
	{
		for (auto it = m_attributes.cbegin(), end = m_attributes.cend();
			it != end;
			++it)
		{
			if (it == attr_iter)
			{
				m_attributes.erase(it);
				return;
			}
		}
	}

	//*************************************************************************

	template<typename CharT>
	inline void Node<CharT>::erase_attribute(attr_const_iterator attr_iter)
	{
		for (auto it = m_attributes.cbegin(), end = m_attributes.cend();
			it != end;
			++it)
		{
			if (it == attr_iter)
			{
				m_attributes.erase(it);

				return;
			}
		}
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
	inline void Node<CharT>::erase_child(std::size_t index) noexcept
	{
		if (index < m_childs.size())
		{
			erase_element(index, std::next(m_childs.cbegin(), index));
		}
	}

	//*************************************************************************

	template<typename CharT>
	inline void Node<CharT>::erase_child(node_iterator node_iter) noexcept
	{
		for (auto it = m_childs.begin(), end = m_childs.end();
			it != end;
			++it)
		{
			if (it == node_iter)
			{
				auto index = std::distance(m_childs.begin(), it);

				erase_element(index, it);

				return;
			}
		}
	}

	//*************************************************************************

	template<typename CharT>
	inline void Node<CharT>::erase_child(node_const_iterator node_iter) 
		noexcept
	{
		for (auto it = m_childs.cbegin(), end = m_childs.cend();
			it != end;
			++it)
		{
			if (it == node_iter)
			{
				auto index = std::distance(m_childs.cbegin(), it);

				erase_element(index, it);

				return;
			}
		}
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Node<CharT>::node_iterator Node<CharT>::child_begin() 
		noexcept
	{
		return m_childs.begin();
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Node<CharT>::node_iterator Node<CharT>::child_end() 
		noexcept
	{
		return m_childs.end();
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Node<CharT>::node_const_iterator Node<CharT>::child_begin() 
		const noexcept
	{
		return m_childs.cbegin();
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Node<CharT>::node_const_iterator Node<CharT>::child_end() 
		const noexcept
	{
		return m_childs.cend();
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Node<CharT>::node_const_iterator 
		Node<CharT>::child_cbegin() const noexcept
	{
		return m_childs.cbegin();
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Node<CharT>::node_const_iterator Node<CharT>::child_cend() 
		const noexcept
	{
		return m_childs.cend();
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
	inline typename Node<CharT>::iterator Node<CharT>::begin() noexcept
	{
		return iterator{ &m_tree_node };
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
		return const_iterator{ &m_tree_node };
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
		return const_iterator{ &m_tree_node };
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
	inline void Node<CharT>::swap(node_type&& node) noexcept
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

		//Конектим узлы между собой
		prev_node->next = &last_child->m_tree_node;

		if (prev_node->prev)
		{
			last_child->m_tree_node.prev = prev_node;
		}
		else
		{
			last_child->m_tree_node.prev = &m_tree_node;
		}

		last_child->m_tree_node.prev = prev_node;
		last_child->m_tree_node.parent = &m_tree_node;
	}

	//*************************************************************************

	template<typename CharT>
	inline void Node<CharT>::erase_element(size_type index, 
		node_const_iterator iterator)
	{
		if (index == 0)
		{
			erase_first_or_last_child(iterator);
		}
		else if (index == m_childs.size() - 1)
		{
			erase_first_or_last_child(iterator);
		}
		else
		{
			erase_one_child(iterator, std::next(iterator, 1));
		}
	}

	//*************************************************************************

	template<typename CharT>
	inline void Node<CharT>::erase_first_or_last_child(
		node_const_iterator erase_iterator)
	{
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

		//Удаляем элемент
		m_childs.erase(erase_iterator);
	}

	//*************************************************************************

	template<typename CharT>
	inline void Node<CharT>::erase_one_child(
		node_const_iterator erase_iterator, 
		node_const_iterator next_iterator)
	{
		//Узел, расположенный до элемента, который нужно удалить
		tree_node* prev_node = erase_iterator->get()->m_tree_node.prev;

		//Узел элемента, который идёт после элемента, который нужно удалить
		tree_node* next_node = &next_iterator->get()->m_tree_node;

		//Конектим узлы между собой
		prev_node->next = next_node;

		next_node->prev = prev_node;

		//Удаляем элемент
		m_childs.erase(erase_iterator);
	}

	//*************************************************************************



	/**************************************************************************
	*							SUPPORT FUNCTIONS
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