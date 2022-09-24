#include "XMLB_Node.h"

//#include <iostream>

namespace XMLB
{
	/**************************************************************************
	*								NODE
	**************************************************************************/

	Node::Node(const std::string& name, const std::string& value)
		:m_name{ name },
		m_value{ value },
		m_parent{ nullptr }
	{
		//std::cout << "Node: name == " << m_name << ", value == " << m_value << " - Default Ctor\n";
	}

	//*************************************************************************

	Node::Node(const Node& node)
		:m_name{ node.m_name },
		m_value{ node.m_value },
		m_attributes{ node.m_attributes.cbegin(), node.m_attributes.cend() },
		m_parent{ node.m_parent }
	{
		for (auto& elem : node.m_childs)
		{
			m_childs.push_back(std::make_unique<Node>(*elem));
			m_childs.back()->set_parent(this);
		}

		//std::cout << "Node: name == " << m_name << ", value == " << m_value << " - Copy Ctor\n";
	}

	//*************************************************************************

	Node& Node::operator=(const Node& node)
	{
		if (this != &node)
		{
			Node temp_node(node);

			swap(temp_node);
		}

		//std::cout << "Node: name == " << m_name << ", value == " << m_value << " - Copy assigment\n";

		return *this;
	}

	//*************************************************************************

	Node::Node(Node&& node) noexcept
		:m_name{ std::move(node.m_name) },
		m_value{ std::move(node.m_value) },
		m_attributes{ std::move(node.m_attributes) },
		m_childs{ std::move(node.m_childs) },
		m_parent{ std::move(node.m_parent) }
	{
		//std::cout << "Node: name == " << m_name << ", value == " << m_value << " - Move Ctor\n";
	}

	//*************************************************************************

	Node& Node::operator=(Node&& node) noexcept
	{
		if (this != &node)
		{
			swap(node);
		}

		//std::cout << "Node: name == " << m_name << ", value == " << m_value << " - Move assigment\n";

		return *this;
	}

	//*************************************************************************

	Node::~Node()
	{
		//std::cout << "Node: name == " << m_name << ", value == " << m_value << " - Dtor\n";
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
		for (auto it = m_attributes.begin(), end = m_attributes.end();
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
		m_childs.push_back(std::make_unique<Node>(node));
		m_childs.back()->set_parent(this);
	}

	//*************************************************************************

	void Node::add_child(Node&& node)
	{
		m_childs.push_back(std::make_unique<Node>(std::move(node)));
		m_childs.back()->set_parent(this);
	}

	//*************************************************************************

	void Node::add_child(Ptr node)
	{
		if (!node)
		{
			return;
		}

		m_childs.push_back(std::move(node));
		m_childs.back()->set_parent(this);
	}

	//*************************************************************************

	void Node::erase_child(std::size_t index) noexcept
	{
		if (index < m_childs.size())
		{
			m_childs.erase(std::next(m_childs.cbegin(), index));
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
				m_childs.erase(it);

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
				m_childs.erase(it);

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

	void Node::set_parent(const Node* parent) noexcept
	{
		m_parent = parent;
	}

	//*************************************************************************

	const Node* Node::get_parent() const noexcept
	{
		return m_parent;
	}

	//*************************************************************************

	void Node::swap(Node& node) noexcept
	{
		std::swap(m_name, node.m_name);
		std::swap(m_value, node.m_value);
		std::swap(m_attributes, node.m_attributes);
		std::swap(m_childs, node.m_childs);
		std::swap(m_parent, node.m_parent);
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