#include "XMLB_Node.h"

#include <iostream>
#include <iomanip>

namespace XMLB
{
	/**************************************************************************
	*								NODE
	**************************************************************************/

	Node::Node(const std::string& name, const std::string& value)
		:m_name{ name },
		m_value{ value }
	{
		m_tree_node.element = this;
		std::cout << "Node: name == " << m_name;
		std::cout << ", value == " << m_value << " - Default Ctor\n";
	}

	//*************************************************************************

	Node::Node(const Node& node)
		:m_name{ node.m_name },
		m_value{ node.m_value },
		m_attributes{ node.m_attributes.cbegin(), node.m_attributes.cend() }
	{
		m_tree_node.element = this;

		for (auto& elem : node.m_childs)
		{
			add_child(*elem);
		}

		std::cout << "Node: name == " << m_name;
		std::cout << ", value == " << m_value << " - Copy Ctor\n";
	}

	//*************************************************************************

	Node& Node::operator=(const Node& node)
	{
		if (this != &node)
		{
			Node temp_node(node);

			swap(temp_node);
		}

		std::cout << "Node: name == " << m_name;
		std::cout << ", value == " << m_value << " - Copy assigment\n";

		return *this;
	}

	//*************************************************************************

	Node::Node(Node&& node) noexcept
		:m_name{ std::move(node.m_name) },
		m_value{ std::move(node.m_value) },
		m_attributes{ std::move(node.m_attributes) }
	{
		m_tree_node.element = this;

		for (auto&& elem : node.m_childs)
		{
			add_child(std::move(elem));
		}

		std::cout << "Node: name == " << m_name;
		std::cout << ", value == " << m_value << " - Move Ctor\n";
	}

	//*************************************************************************

	Node& Node::operator=(Node&& node) noexcept
	{
		if (this != &node)
		{
			swap(node);
		}

		std::cout << "Node: name == " << m_name;
		std::cout << ", value == " << m_value << " - Move assigment\n";

		return *this;
	}

	//*************************************************************************

	Node::~Node()
	{
		std::cout << "Node: name == " << m_name;
		std::cout << ", value == " << m_value << " - Dtor\n";
	}

	//*************************************************************************

	void Node::set_name(const std::string& name)
	{
		m_name = name;
	}

	//*************************************************************************

	const std::string& Node::get_name() const & noexcept
	{
		return m_name;
	}

	//*************************************************************************

	void Node::set_value(const std::string& value)
	{
		m_value = value;
	}

	//*************************************************************************

	const std::string& Node::get_value() const & noexcept
	{
		return m_value;
	}

	//*************************************************************************

	void Node::add_attribute(const Node_attribute& attribute)
	{
		m_attributes.push_back(attribute);
	}

	//*************************************************************************

	void Node::add_attribute(Node_attribute&& attribute)
	{
		m_attributes.push_back(std::move(attribute));
	}

	//*************************************************************************

	void Node::erase_attribute(std::size_t index)
	{
		if (index < m_attributes.size())
		{
			m_attributes.erase(std::next(m_attributes.cbegin(), index));
		}
	}

	//*************************************************************************

	void Node::erase_attribute(attr_iterator attr_iter)
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

	void Node::erase_attribute(attr_const_iterator attr_iter)
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

	std::size_t Node::attr_size() const noexcept
	{
		return m_attributes.size();
	}

	//*************************************************************************

	Node::attr_iterator Node::attr_begin() noexcept
	{
		return m_attributes.begin();
	}

	//*************************************************************************

	Node::attr_iterator Node::attr_end() noexcept
	{
		return m_attributes.end();
	}

	//*************************************************************************

	Node::attr_const_iterator Node::attr_begin() const noexcept
	{
		return m_attributes.begin();
	}

	//*************************************************************************

	Node::attr_const_iterator Node::attr_end() const noexcept
	{
		return m_attributes.end();
	}

	//*************************************************************************

	Node::attr_const_iterator Node::attr_cbegin() const noexcept
	{
		return m_attributes.cbegin();
	}

	//*************************************************************************

	Node::attr_const_iterator Node::attr_cend() const noexcept
	{
		return m_attributes.cend();
	}

	//*************************************************************************

	void Node::add_child(const Node& node)
	{
		tree_node* last_tree_node = find_last_tree_node();

		m_childs.push_back(std::make_unique<Node>(node));

		connect_tree_nodes(last_tree_node);
	}

	//*************************************************************************

	void Node::add_child(Node&& node)
	{
		tree_node* last_tree_node = find_last_tree_node();

		m_childs.push_back(std::make_unique<Node>(std::move(node)));
		
		connect_tree_nodes(last_tree_node);
	}

	//*************************************************************************

	void Node::add_child(Ptr node)
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

	void Node::erase_child(std::size_t index) noexcept
	{
		if (index < m_childs.size())
		{
			erase_element(index, std::next(m_childs.cbegin(), index));
		}
	}

	//*************************************************************************

	void Node::erase_child(node_iterator node_iter) noexcept
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

	void Node::erase_child(node_const_iterator node_iter) noexcept
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

	Node::node_iterator Node::child_begin() noexcept
	{
		return m_childs.begin();
	}

	//*************************************************************************

	Node::node_iterator Node::child_end() noexcept
	{
		return m_childs.end();
	}

	//*************************************************************************

	Node::node_const_iterator Node::child_begin() const noexcept
	{
		return m_childs.begin();
	}

	//*************************************************************************

	Node::node_const_iterator Node::child_end() const noexcept
	{
		return m_childs.end();
	}

	//*************************************************************************

	Node::node_const_iterator Node::child_cbegin() const noexcept
	{
		return m_childs.cbegin();
	}

	//*************************************************************************

	Node::node_const_iterator Node::child_cend() const noexcept
	{
		return m_childs.cend();
	}

	//*************************************************************************

	std::size_t Node::child_size() const noexcept
	{
		return m_childs.size();
	}

	//*************************************************************************

	Node::iterator Node::begin() noexcept
	{
		return iterator{ &m_tree_node };
	}

	//*************************************************************************

	Node::iterator Node::end() noexcept
	{
		return iterator{ find_last_tree_node()->next };
	}

	//*************************************************************************

	Node::const_iterator Node::begin() const noexcept
	{
		return const_iterator{&m_tree_node};
	}

	//*************************************************************************

	Node::const_iterator Node::end() const noexcept
	{
		return const_iterator{ find_last_tree_node()->next };
	}

	//*************************************************************************

	Node::const_iterator Node::cbegin() const noexcept
	{
		return const_iterator{ &m_tree_node };
	}

	//*************************************************************************

	Node::const_iterator Node::cend() const noexcept
	{
		return const_iterator{ find_last_tree_node()->next };
	}

	//*************************************************************************

	const Node* Node::get_parent() const noexcept
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

	void Node::swap(Node& node) noexcept
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

	Node::tree_node* Node::find_last_tree_node() const
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

	void Node::connect_tree_nodes(tree_node* prev_node)
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

	void Node::erase_element(std::size_t index, node_const_iterator iterator)
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

	void Node::erase_first_or_last_child(node_const_iterator erase_iterator)
	{
		//Элемент, которй нужно удалить и который будет указывать на последний
		//элемент, который является дочерним этого элемент
		Node* erase_child = erase_iterator->get();

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

	void Node::erase_one_child(node_const_iterator erase_iterator,
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
	*								FUNCTIONS
	**************************************************************************/

	bool operator==(const Node& lhs, const Node& rhs)
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

	bool operator!=(const Node& lhs, const Node& rhs)
	{
		return !(lhs == rhs);
	}

	//*************************************************************************

} // namepsace XMLB




//void Node::swap(Node& node) noexcept
//{
//	using std::swap;
//
//	//Получаем необходимые начальные узлы текущего элемента
//
//	//Узел, который идёт перед текущим элементом
//	auto lhs_first_node = m_tree_node.prev;
//
//	//Узел родитель текущего элемент
//	auto lhs_parent_node = m_tree_node.parent;
//
//	//Узел, который идёт после самого последнего дочернего узла 
//	//текущего элемента
//	auto lhs_last_node = find_last_tree_node()->next;
//
//	//Получаем необходимые начальные узлы элемента node
//
//	//Узел, который идёт перед элементом node
//	auto rhs_first_node = node.m_tree_node.prev;
//
//	//Узел родитель элемента node
//	auto rhs_parent_node = node.m_tree_node.parent;
//
//	//Узел, который идёт после самого последнего дочернего узла 
//	//элемента node
//	auto rhs_last_node = node.find_last_tree_node()->next;
//
//	swap(m_tree_node, node.m_tree_node);
//	swap(m_name, node.m_name);
//	swap(m_value, node.m_value);
//	swap(m_attributes, node.m_attributes);
//	swap(m_childs, node.m_childs);
//	//swap(m_tree_node, node.m_tree_node);
//
//	//Получем новый узел, который является самым последним узлом 
//	//или же текущим
//	auto lhs_new_last_node = find_last_tree_node();
//
//	//Получем новый узел, который является самым последним узлом 
//	//или же текущим node
//	auto rhs_new_last_node = node.find_last_tree_node();
//
//	//Соеденяем узлы текущего элемента
//	if (lhs_first_node)
//	{
//		lhs_first_node->next = &m_tree_node;
//		m_tree_node.prev = lhs_first_node;
//	}
//	else
//	{
//		m_tree_node.prev = nullptr;
//	}
//
//	m_tree_node.parent = lhs_parent_node;
//
//	if (lhs_last_node)
//	{
//		lhs_last_node->prev = lhs_new_last_node;
//		lhs_new_last_node->next = lhs_last_node;
//	}
//	else
//	{
//		m_tree_node.next = nullptr;
//	}
//
//	//Соеденяем узлы элемента node
//	if (rhs_first_node)
//	{
//		rhs_first_node->next = &node.m_tree_node;
//		node.m_tree_node.prev = rhs_first_node;
//	}
//	else
//	{
//		node.m_tree_node.prev = nullptr;
//	}
//
//	node.m_tree_node.parent = rhs_parent_node;
//
//	if (rhs_last_node)
//	{
//		rhs_last_node->prev = rhs_new_last_node;
//		rhs_new_last_node->next = rhs_last_node;
//	}
//	else
//	{
//		rhs_new_last_node->next = nullptr;
//	}
//
//}