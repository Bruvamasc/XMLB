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
	struct Node_attribute final
	{
		std::string name;
		std::string value;
	};

	//*************************************************************************



	/**************************************************************************
	* @brief Узел XML структуры
	**************************************************************************/
	class Node final
	{
	public:
		using Ptr = std::unique_ptr<Node>;
		using attr_iterator = std::list<Node_attribute>::iterator;
		using attr_const_iterator = std::list<Node_attribute>::const_iterator;
		using node_iterator = std::list<Ptr>::iterator;
		using node_const_iterator = std::list<Ptr>::const_iterator;
		using tree_node = Node_tree_impl<Node>;
		using iterator = Node_iterator_impl<Node, Node>;
		using const_iterator = Node_iterator_impl<const Node, Node>;
		

		Node(const std::string& name, const std::string& value = "");

		Node(const Node& node);
		Node& operator=(const Node& node);

		Node(Node&& node) noexcept;
		Node& operator=(Node&& node) noexcept;

		~Node();

		// Работа с именем тега

		void set_name(const std::string& name);
		const std::string& get_name() const & noexcept;

		// Работа со значением тега

		void set_value(const std::string& value);
		const std::string& get_value() const & noexcept;

		// Работа с аттрибутами тега

		void add_attribute(const Node_attribute& attribute);
		void add_attribute(Node_attribute&& attribute);

		void erase_attribute(std::size_t index);
		void erase_attribute(attr_iterator attr_iter);
		void erase_attribute(attr_const_iterator attr_iter);

		std::size_t attr_size() const noexcept;

		attr_iterator attr_begin() noexcept;
		attr_iterator attr_end() noexcept;

		attr_const_iterator attr_begin() const noexcept;
		attr_const_iterator attr_end() const noexcept;

		attr_const_iterator attr_cbegin() const noexcept;
		attr_const_iterator attr_cend() const noexcept;

		// Работа с дочерними node

		void add_child(const Node& node);
		void add_child(Node&& node);
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

		std::size_t child_size() const noexcept;

		// Блок с итераторами

		iterator begin() noexcept;
		iterator end() noexcept;

		const_iterator begin() const noexcept;
		const_iterator end() const noexcept;

		const_iterator cbegin() const noexcept;
		const_iterator cend() const noexcept;

		// Работа с родителем

		const Node* get_parent() const noexcept;

		// Вспомагательные функции

		void swap(Node& node) noexcept;

	private:
		tree_node* find_last_tree_node() const;
		void connect_tree_nodes(tree_node* prev_node);

		void erase_element(std::size_t index, node_const_iterator iterator);
		void erase_first_or_last_child(node_const_iterator erase_iterator);
		void erase_one_child(node_const_iterator erase_iterator, 
			node_const_iterator next_iterator);

	private:
		std::string m_name;
		std::string m_value;
		std::list<Node_attribute> m_attributes;
		std::list<Ptr> m_childs;
		mutable tree_node m_tree_node;
	};

	//*************************************************************************



	/**************************************************************************
	*								FUNCTIONS
	**************************************************************************/

	bool operator==(const Node& lhs, const Node& rhs);
	bool operator!=(const Node& lhs, const Node& rhs);

	//*************************************************************************

} // namespace XMLB

//*****************************************************************************

namespace std
{
	template<>
	inline void swap<XMLB::Node>(XMLB::Node& lhs, XMLB::Node& rhs) noexcept
	{
		lhs.swap(rhs);
	}

	//*************************************************************************

	template<>
	inline void swap<XMLB::Node_attribute>(
		XMLB::Node_attribute& lhs, XMLB::Node_attribute& rhs) noexcept
	{
		swap(lhs.name, rhs.name);
		swap(lhs.value, rhs.value);
	}

	//*************************************************************************

} // namespace std

#endif // !XMLB_NODE_H