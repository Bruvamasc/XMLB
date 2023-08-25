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



#ifndef XMLB_NODE_H
#define XMLB_NODE_H

#include <string>
#include <string_view>
#include <list>
#include <memory>

#include "XMLB/detail/XMLB_Node_iterator.h"
#include "XMLB/detail/XMLB_Diagnostic_iterator.h"
#include "XMLB/detail/traits/XMLB_Type_traits.h"
#include "XMLB/detail/traits/XMLB_Type_methods_traits.h"



namespace XMLB
{
	/**************************************************************************
	* @brief Структура для хранения XML атрибута
	* 
	* @ingroup general
	* 
	* @tparam CharT - тип символов
	**************************************************************************/
	template<typename CharT>
	struct Node_attribute final
	{
		using symbols_type = CharT;
		using string_type = std::basic_string<CharT>;
		using string_wrapper = std::basic_string_view<CharT>;

		string_type name;				///<Имя атрибута
		string_type value;				///<Значение атрибута
	};

	//*************************************************************************



	/**************************************************************************
	* @brief XML узел
	* 
	* @ingroup general
	* 
	* @tparam CharT - тип символов
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

		using first_level_iterator = typename std::list<Ptr>::iterator;
		using first_level_const_iterator =
			typename std::list<Ptr>::const_iterator;

		using iterator = detail::Node_iterator<node_type>;
		using const_iterator = detail::Node_const_iterator<node_type>;



		/// @name Конструкторы, деструктор
		/// @{
		/**********************************************************************
		* @param name - имя узла
		* @param value - значение узла
		**********************************************************************/
		Node(const string_type& name,
			const string_type& value = string_type{});

		/**********************************************************************
		* @param name - имя узла
		* @param value - значение узла
		**********************************************************************/
		Node(string_type&& name, string_type&& value = string_type{}) noexcept;

		Node(const Node& node);
		Node& operator=(const Node& node);

		Node(Node&& node) noexcept;
		Node& operator=(Node&& node) noexcept;

		~Node() = default;
		/// @}



		/// @name Методы для работы с именем узла
		/// @{
		/**********************************************************************
		* @brief Изменить имя узла
		*
		* @param name - новое имя узла
		**********************************************************************/
		void set_name(const string_type& name);

		/**********************************************************************
		* @brief Изменить имя узла
		*
		* @param name - новое имя узла
		**********************************************************************/
		void set_name(string_type&& name) noexcept;

		/**********************************************************************
		* @brief Получить имя узла
		*
		* @return невладеющий объект-обертку строки с именем узла
		**********************************************************************/
		string_wrapper get_name() const & noexcept;
		/// @}



		/// @name Методы для работы со значением узла
		/// @{
		/**********************************************************************
		* @brief Изменить значение узла
		*
		* @param value - новое значение узла
		**********************************************************************/
		void set_value(const string_type& value);

		/**********************************************************************
		* @brief Изменить значение узла
		*
		* @param value - новое значение узла
		**********************************************************************/
		void set_value(string_type&& value) noexcept;

		/**********************************************************************
		* @brief Получить значение узла
		*
		* @return невладеющий объект-обертку строки со значением узла
		**********************************************************************/
		string_wrapper get_value() const & noexcept;
		/// @}



		/// @name Методы для работы с атрибутами узла
		/// @{
		/**********************************************************************
		* @brief Добавить атрибут
		* 
		* @param attribute - атрибута
		*
		* @return текущий узел
		**********************************************************************/
		node_type& add_attribute(const attribute_type& attribute) &;

		/**********************************************************************
		* @brief Добавить атрибут
		*
		* @param attribute - атрибут
		*
		* @return текущий узел
		**********************************************************************/
		node_type& add_attribute(attribute_type&& attribute) &;
		
		/**********************************************************************
		* @brief Найти атрибут
		*
		* @param attribute_name - название атрибута
		*
		* @return текущий узел
		**********************************************************************/
		attr_iterator find_attribute(const string_type& attribute_name);
		
		/**********************************************************************
		* @brief Найти атрибут
		*
		* @param attribute_name - название атрибута
		*
		* @return текущий узел
		**********************************************************************/
		attr_const_iterator find_attribute(const string_type& attribute_name) 
			const;

		/**********************************************************************
		* @brief Удалить атрибут
		*
		* @param index - позиция атрибута
		*
		* @return итератор на атрибут после удаленного атрибута. Если не
		* удалось удалить атрибут, то возвращает итератор на attr_end()
		**********************************************************************/
		attr_iterator erase_attribute(std::size_t index);

		/**********************************************************************
		* @brief Удалить атрибут
		*
		* @param attribute - итератор на атрибут
		*
		* @return итератор на атрибут после удаленного атрибута. Если не
		* удалось удалить атрибут, то возвращает итератор на attr_end()
		**********************************************************************/
		attr_iterator erase_attribute(attr_const_iterator attribute);

		/**********************************************************************
		* @brief Удалить атрибуты
		*
		* @param attribute_first - итератор на первый атрибут
		* @param attribute_last - итератор за последним атрибутом
		*
		* @return итератор на атрибут после последнего удаленного атрибута. 
		* Если не удалось удалить атрибуты, то возвращает итератор на 
		* attr_end() или на атрибут, который после последнего успешно 
		* удаленного атрибута
		**********************************************************************/
		attr_iterator erase_attribute(attr_const_iterator attribute_fisrt,
			attr_const_iterator attribute_last);

		/**********************************************************************
		* @brief Получить количество атрибутов
		*
		* @return количество атрибутов
		**********************************************************************/
		size_type attr_size() const noexcept;

		attr_iterator attr_begin() noexcept;
		attr_iterator attr_end() noexcept;

		attr_const_iterator attr_begin() const noexcept;
		attr_const_iterator attr_end() const noexcept;

		attr_const_iterator attr_cbegin() const noexcept;
		attr_const_iterator attr_cend() const noexcept;
		/// @}



		/// @name Методы для работы с дочерними узлами
		/// @{
		/**********************************************************************
		* @brief Добавить дочерний узел
		* 
		* @param node - узел, который нужно добавить
		*
		* @return добавленный дочерний узел. Если узел не был добавлен,
		* возращает текущий узел
		**********************************************************************/
		node_type& add_child(const node_type& node) &;

		/**********************************************************************
		* @brief Добавить дочерний узел
		*
		* @param node - узел, который нужно добавить
		*
		* @return добавленный дочерний узел. Если узел не был добавлен,
		* возращает текущий узел
		**********************************************************************/
		node_type& add_child(node_type&& node) &;

		/**********************************************************************
		* @brief Добавить дочерний узел
		*
		* @param node - узел, который нужно добавить
		*
		* @return добавленный дочерний узел. Если узел не был добавлен,
		* возращает текущий узел
		**********************************************************************/
		node_type& add_child(Ptr node) &;


		/**********************************************************************
		* @brief Удалить дочерний узел
		*
		* @details Так как все узлы, которые являются дочерними узлами дочерних
		* узлов текущего узла и т.д., то можно удалить любой дочерний узел в
		* диапазоне [0, size())
		* 
		* @param index - позиция дочернего узла
		*
		* @return итератор на следующий узел после удаленного. Если узел не был
		* удален, возвращает итератор на end()
		**********************************************************************/
		iterator erase_child(std::size_t index);

		/**********************************************************************
		* @brief Удалить дочерний узел
		*
		* @details Так как все узлы, которые являются дочерними узлами дочерних
		* узлов текущего узла и т.д., то можно удалить любой дочерний узел в
		* диапазоне [begin(), end())
		*
		* @param node - итератор указывающий на узел, который нужно удалить
		*
		* @return итератор на следующий узел после удаленного. Если узел не был
		* удален, возвращает итератор на end()
		**********************************************************************/
		iterator erase_child(const_iterator node);

		/**********************************************************************
		* @brief Удалить дочерние узлы
		*
		* @details Так как все узлы, которые являются дочерними узлами дочерних
		* узлов текущего узла и т.д., то можно удалить любой дочерний узел в
		* диапазоне [begin(), end())
		*
		* @param node_first - итератор указывающий на первый удаляемый узел
		* @param node_last - итератор указывающий за последний удаляемый узел
		*
		* @return итератор на следующий узел после последнего удаленного. Если 
		* узлы не были удалены, возвращает итератор на end() или на следующий
		* после последнего успешно удаленного узла
		**********************************************************************/
		iterator erase_child(const_iterator node_first, 
			const_iterator node_last);

		first_level_iterator first_level_begin() noexcept;
		first_level_iterator first_level_end() noexcept;

		first_level_const_iterator first_level_begin() const noexcept;
		first_level_const_iterator first_level_end() const noexcept;

		first_level_const_iterator first_level_cbegin() const noexcept;
		first_level_const_iterator first_level_cend() const noexcept;

		iterator begin() noexcept;
		iterator end() noexcept;

		const_iterator begin() const noexcept;
		const_iterator end() const noexcept;

		const_iterator cbegin() const noexcept;
		const_iterator cend() const noexcept;
		/// @}



		/// @name Вспомогательные методы
		/// @{
		/**********************************************************************
		* @brief Получить количество дочерних узлов первого уровня
		*
		* @return количество дочерних узлов первого уровня
		**********************************************************************/
		size_type child_size() const noexcept;

		/**********************************************************************
		* @brief Получить общее количество дочерних узлов
		*
		* @return общее количество дочерних узлов, не включая текущий узел
		**********************************************************************/
		size_type size() const noexcept;

		/**********************************************************************
		* @brief Получить родителя текущего узла
		*
		* @return nullptr - если родителя нет. В противном случае вернет
		* указатель на родителя
		**********************************************************************/
		const node_type* get_parent() const noexcept;

		/**********************************************************************
		* @brief Обменять данные
		*
		* @param node - правый Node
		**********************************************************************/
		void swap(node_type& node) noexcept;
		/// @}



		/// @name Методы поиска
		/// @{
		/**********************************************************************
		* @brief Найти узел
		*
		* @details Перегрузка для работы с указателями на строки, например:
		* const char*, const wchar_t*
		*
		* @tparam ContT - тип контейнера
		* @param container - имя узла, который нужно найти
		* @param offset - итератор, с которого начать искать
		*
		* @return итератор на узел - если он был найден. В противном случае
		* возвращает итератор указывающий на end()
		**********************************************************************/
		template<typename ContT, 
			std::enable_if_t<std::is_same_v<ContT, symbol_type> &&
			std::is_constructible_v<string_wrapper, const ContT*>, 
			std::nullptr_t> = nullptr>
		iterator find(const ContT* container, 
			const_iterator offset = const_iterator{ nullptr });

		/**********************************************************************
		* @brief Найти узел(ы)
		*
		* @details Перегрузка для работы с контейнерами содержащие простые
		* типы, например: std::string, std::vector<char>, std::string_view
		*
		* @tparam ContT - тип контейнера
		* @param container - список имён узлов, которые нужно найти
		* @param offset - итератор, с которого начать искать
		*
		* @return итератор на узел - если он был найден. В противном случае
		* возвращает итератор указывающий на end()
		**********************************************************************/
		template<typename ContT,
			std::enable_if_t<
			
			detail::is_has_begin_and_end_v<ContT> &&

			detail::is_has_value_type_v<ContT> &&

			std::is_constructible_v<string_wrapper,
			std::add_pointer_t<std::remove_reference_t<
			detail::dereferenced_t<detail::const_iterator_t<ContT>>>>> &&

			std::is_same_v<detail::universal_value_type_t<ContT>, symbol_type>,

			std::nullptr_t> = nullptr>
		iterator find(const ContT& container, 
			const_iterator offset = const_iterator{ nullptr });

		/**********************************************************************
		* @brief Найти узел(ы)
		*
		* @details Перегрузка для работы с контейнерами содержащие
		* строко-подобные типы, например: std::vector<std::string>,
		* std::vector<const char*>, std::initializer_list<const char*>,
		* std::initializer_list<std::string>
		*
		* @tparam ContT - тип контейнера
		* @param container - список имён узлов, которые нужно найти
		* @param offset - итератор, с которого начать искать
		*
		* @return итератор на узел - если он был найден. В противном случае
		* возвращает итератор указывающий на end()
		**********************************************************************/
		template<typename ContT = std::initializer_list<string_wrapper>,
			std::enable_if_t<

			detail::is_has_begin_and_end_v<ContT> &&

			detail::is_has_value_type_v<ContT> &&

			!std::is_arithmetic_v<detail::value_type_t<ContT>> &&

			(std::is_constructible_v<string_wrapper,
				std::decay_t<
				detail::dereferenced_t<detail::const_iterator_t<ContT>>>> ||

				std::is_constructible_v<string_wrapper,
				detail::dereferenced_t<detail::const_iterator_t<ContT>>>) &&

			std::is_same_v<std::remove_const_t<
			detail::universal_value_type_t<ContT>>, symbol_type>,

			std::nullptr_t> = nullptr>
		iterator find(const ContT& container, 
			const_iterator offset = const_iterator{ nullptr });

		/**********************************************************************
		* @brief Найти узел
		*
		* @details Перегрузка для работы с указателями на строки, например:
		* const char*, const wchar_t*
		*
		* @tparam ContT - тип контейнера
		* @param container - имя узла, который нужно найти
		* @param offset - итератор, с которого начать искать
		*
		* @return константный итератор на узел - если он был найден. 
		* В противном случае возвращает константній итератор указывающий 
		* на end()
		**********************************************************************/
		template<typename ContT,
			std::enable_if_t<std::is_same_v<ContT, symbol_type>&&
			std::is_constructible_v<string_wrapper, const ContT*>,
			std::nullptr_t> = nullptr>
		const_iterator find(const ContT* container,
			const_iterator offset = const_iterator{ nullptr }) const;

		/**********************************************************************
		* @brief Найти узел(ы)
		*
		* @details Перегрузка для работы с контейнерами содержащие простые
		* типы, например: std::string, std::vector<char>, std::string_view
		*
		* @tparam ContT - тип контейнера
		* @param container - список имён узлов, которые нужно найти
		* @param offset - итератор, с которого начать искать
		*
		* @return константный итератор на узел - если он был найден. 
		* В противном случае возвращает константній итератор указывающий 
		* на end()
		**********************************************************************/
		template<typename ContT,
			std::enable_if_t<

			detail::is_has_begin_and_end_v<ContT>&&

			detail::is_has_value_type_v<ContT>&&

			std::is_constructible_v<string_wrapper,
			std::add_pointer_t<std::remove_reference_t<
			detail::dereferenced_t<detail::const_iterator_t<ContT>>>>>&&

			std::is_same_v<detail::universal_value_type_t<ContT>, symbol_type>,

			std::nullptr_t> = nullptr>
		const_iterator find(const ContT& container, 
			const_iterator offset = const_iterator{ nullptr }) const;

		/**********************************************************************
		* @brief Найти узел(ы)
		*
		* @details Перегрузка для работы с контейнерами содержащие
		* строко-подобные типы, например: std::vector<std::string>,
		* std::vector<const char*>, std::initializer_list<const char*>,
		* std::initializer_list<std::string>
		*
		* @tparam ContT - тип контейнера
		* @param container - список имён узлов, которые нужно найти
		* @param offset - итератор, с которого начать искать
		*
		* @return константный итератор на узел - если он был найден. 
		* В противном случае возвращает константній итератор указывающий 
		* на end()
		**********************************************************************/
		template<typename ContT = std::initializer_list<string_wrapper>,
			std::enable_if_t<

			detail::is_has_begin_and_end_v<ContT>&&

			detail::is_has_value_type_v<ContT> &&

			!std::is_arithmetic_v<detail::value_type_t<ContT>> &&

			(std::is_constructible_v<string_wrapper,
				std::decay_t<
				detail::dereferenced_t<detail::const_iterator_t<ContT>>>> ||

				std::is_constructible_v<string_wrapper,
				detail::dereferenced_t<detail::const_iterator_t<ContT>>>) &&

			std::is_same_v<std::remove_const_t<
			detail::universal_value_type_t<ContT>>, symbol_type>,

			std::nullptr_t> = nullptr>
		const_iterator find(const ContT& container, 
			const_iterator offset = const_iterator{ nullptr }) const;

		//*********************************************************************
		/// @}

	private:
		tree_node* find_last_tree_node() const;
		void connect_tree_nodes(tree_node* current_node, tree_node* add_node);

		iterator erase_element(const_iterator node);
		iterator erase_element(const_iterator node_first, 
			const_iterator node_last);

		iterator find_element(const_iterator first,
			const_iterator last, string_wrapper tag_name) const;

		void update_size(int size) noexcept;

	private:
		string_type m_name;
		string_type m_value;
		std::list<attribute_type> m_attributes;
		std::list<Ptr> m_childs;
		size_type m_size;
		mutable tree_node m_tree_node;
	};

	//*************************************************************************



	//*************************************************************************
	//							NODE IMPLEMENTATION
	//*************************************************************************

	template<typename CharT>
	inline Node<CharT>::Node(const string_type& name, const string_type& value)
		:m_name{ name }, m_value{ value }, m_size{ 0 }
	{
		m_tree_node.element = this;
	}

	//*************************************************************************

	template<typename CharT>
	inline Node<CharT>::Node(string_type&& name, string_type&& value) noexcept
		:m_name{ std::move(name) }, m_value{ std::move(value) }, m_size{ 0 }	
	{
		m_tree_node.element = this;
	}

	//*************************************************************************

	template<typename CharT>
	inline Node<CharT>::Node(const Node<CharT>& node)
		:m_name{ node.m_name },
		m_value{ node.m_value },
		m_attributes{ node.m_attributes.cbegin(), node.m_attributes.cend() },
		m_size{ 0 }
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
		m_attributes{ std::move(node.m_attributes) },
		m_childs{ std::move(node.m_childs) },
		m_size{ 0 }
	{
		m_tree_node.element = this;

		node.m_tree_node.next = nullptr;
		node.m_tree_node.prev = nullptr;
		node.m_tree_node.parent = nullptr;

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

		auto last_child = find_last_tree_node();

		if (last_child != &m_tree_node)
		{
			last_child->next = &m_tree_node;
			m_tree_node.prev = last_child;
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
	inline void Node<CharT>::set_name(string_type&& name) noexcept
	{
		m_name = std::move(name);
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
	inline void Node<CharT>::set_value(string_type&& value) noexcept
	{
		m_value = std::move(value);
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
	inline typename Node<CharT>::node_type& 
		Node<CharT>::add_attribute(const attribute_type& attribute) &
	{
		m_attributes.push_back(attribute);

		return *this;
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Node<CharT>::node_type& 
		Node<CharT>::add_attribute(attribute_type&& attribute) &
	{
		m_attributes.push_back(std::move(attribute));

		return *this;
	}
	
	//*************************************************************************

	template<typename CharT>
	inline typename Node<CharT>::attr_iterator 
		Node<CharT>::find_attribute(const string_type& attribute_name)
	{
		using std::begin;
		using std::end;
		
		auto&& attr_founder = [&](auto&& attribute)
		{
			return attribute.name == attribute_name;
		};
		
		return std::find_if(begin(m_attributes), end(m_attributes), 
			attr_founder);
	}
	
	//*************************************************************************

	template<typename CharT>
	inline typename Node<CharT>::attr_const_iterator 
		Node<CharT>::find_attribute(const string_type& attribute_name) const
	{
		using std::begin;
		using std::end;
		
		auto&& attr_founder = [&](auto&& attribute)
		{
			return attribute.name == attribute_name;
		};
		
		return std::find_if(begin(m_attributes), end(m_attributes), 
			attr_founder);
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Node<CharT>::attr_iterator 
		Node<CharT>::erase_attribute(std::size_t index)
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
		attr_const_iterator attr_iter)
	{
		return m_attributes.erase(attr_iter);
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Node<CharT>::attr_iterator 
		Node<CharT>::erase_attribute(attr_const_iterator attribute_fisrt, 
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
	inline typename Node<CharT>::node_type& 
		Node<CharT>::add_child(const node_type& node) &
	{
		tree_node* last_tree_node = find_last_tree_node();

		m_childs.push_back(std::make_unique<Node<CharT>>(node));

		connect_tree_nodes(
			last_tree_node, &m_childs.back().get()->m_tree_node);

		return *m_childs.back();
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Node<CharT>::node_type&
		Node<CharT>::add_child(node_type&& node) &
	{
		if (this == &node)
		{
			return *this;
		}

		tree_node* last_tree_node = find_last_tree_node();

		m_childs.push_back(std::make_unique<Node<CharT>>(std::move(node)));

		connect_tree_nodes(
			last_tree_node, &m_childs.back().get()->m_tree_node);

		return *m_childs.back();
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Node<CharT>::node_type& 
		Node<CharT>::add_child(Ptr node) &
	{
		if (!node || this == node.get())
		{
			return *this;
		}

		tree_node* last_tree_node = find_last_tree_node();

		m_childs.push_back(std::move(node));

		connect_tree_nodes(
			last_tree_node, &m_childs.back().get()->m_tree_node);

		return *m_childs.back();
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Node<CharT>::iterator Node<CharT>::erase_child(
		std::size_t index)
	{
		auto result = end();

		if (index < m_childs.size())
		{
			auto erase_iter = std::next(cbegin(), index);

			result = erase_element(erase_iter);
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
	inline typename Node<CharT>::first_level_iterator 
		Node<CharT>::first_level_begin() noexcept
	{
		return m_childs.begin();
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Node<CharT>::first_level_iterator
		Node<CharT>::first_level_end() noexcept
	{
		return m_childs.end();
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Node<CharT>::first_level_const_iterator
		Node<CharT>::first_level_begin() const noexcept
	{
		return m_childs.begin();
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Node<CharT>::first_level_const_iterator
		Node<CharT>::first_level_end() const noexcept
	{
		return m_childs.end();
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Node<CharT>::first_level_const_iterator
		Node<CharT>::first_level_cbegin() const noexcept
	{
		return m_childs.cbegin();
	}

	//*************************************************************************

	template<typename CharT>
	inline typename Node<CharT>::first_level_const_iterator
		Node<CharT>::first_level_cend() const noexcept
	{
		return m_childs.cend();
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
	inline typename Node<CharT>::size_type Node<CharT>::size() const
		noexcept
	{
		return m_size;
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
		//start -> swap_element -> end

		auto lhs_start = m_tree_node.prev;
		auto lhs_end = find_last_tree_node()->next;
		auto lhs_parent = m_tree_node.parent;	

		auto rhs_start = node.m_tree_node.prev;
		auto rhs_end = node.find_last_tree_node()->next;
		auto rhs_parent = node.m_tree_node.parent;

		swap(m_name, node.m_name);
		swap(m_value, node.m_value);
		swap(m_attributes, node.m_attributes);
		swap(m_childs, node.m_childs);
		swap(m_size, node.m_size);
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

		//Получем новый узел, который является самым последним дочерним узлом
		//или же текущим

		auto lhs_last_child = find_last_tree_node();
		auto rhs_last_child = node.find_last_tree_node();


		//Коннектим начальные узлы текущего объекта

		if (lhs_start && lhs_start != rhs_last_child)
		{
			lhs_start->next = &m_tree_node;
			m_tree_node.prev = lhs_start;
		}
		else
		{
			lhs_last_child->next = &m_tree_node;
			m_tree_node.prev = lhs_last_child;
		}

		if (lhs_end && lhs_end != rhs_start)
		{
			lhs_end->prev = lhs_last_child;
			lhs_last_child->next = lhs_end;
		}

		if (&m_tree_node == lhs_last_child)
		{
			m_tree_node.prev = nullptr;
			m_tree_node.next = nullptr;
		}

		m_tree_node.parent = lhs_parent;

		//Коннектим узлы в переданном node

		if (rhs_start && rhs_start != lhs_last_child)
		{
			rhs_start->next = &node.m_tree_node;
			node.m_tree_node.prev = rhs_start;
		}
		else
		{
			rhs_last_child->next = &node.m_tree_node;
			node.m_tree_node.prev = rhs_last_child;
		}

		if (rhs_end && rhs_end != lhs_start)
		{
			rhs_end->prev = rhs_last_child;
			rhs_last_child->next = rhs_end;
		}

		if (&node.m_tree_node == rhs_last_child)
		{
			node.m_tree_node.prev = nullptr;
			node.m_tree_node.next = nullptr;
		}

		node.m_tree_node.parent = rhs_parent;

		//Обновляем общее количество узлов
		
		int lhs_size_old = static_cast<int>(node.m_size);
		int rhs_size_old = static_cast<int>(m_size);

		int lhs_size_new = rhs_size_old;
		int rhs_size_new = lhs_size_old;

		if (node.m_tree_node.parent)
		{
			node.m_tree_node.parent->element->update_size(
				rhs_size_new - rhs_size_old);
		}

		if (m_tree_node.parent)
		{
			m_tree_node.parent->element->update_size(
				lhs_size_new - lhs_size_old);
		}
	}

	//*************************************************************************

	template<typename CharT>
	template<typename ContT,
		std::enable_if_t<std::is_same_v<ContT, CharT>&&

		std::is_constructible_v<
		std::basic_string_view<CharT>, const ContT*>,

		std::nullptr_t>>
		inline typename Node<CharT>::iterator Node<CharT>::find(
			const ContT* container, const_iterator offset)
	{
		using std::begin;
		using std::end;

		iterator first = this->begin();
		iterator last = this->end();

		if (offset != const_iterator{ nullptr } &&
			offset._is_in_sequences(this->begin()))
		{
			first = iterator{ &offset->m_tree_node };
		}

		if (offset == last)
		{
			first = last;
		}

		for (; first != last;)
		{
			first = find_element(first, last, string_wrapper{ container });

			if (first != last)
			{
				break;
			}
		}

		return first;
	}

	//*************************************************************************

	template<typename CharT>
	template<typename ContT,
		std::enable_if_t<

		detail::is_has_begin_and_end_v<ContT>&&

		detail::is_has_value_type_v<ContT>&&

		std::is_constructible_v<std::basic_string_view<CharT>,
		std::add_pointer_t<std::remove_reference_t<
		detail::dereferenced_t<detail::const_iterator_t<ContT>>>>>&&

		std::is_same_v<detail::universal_value_type_t<ContT>, CharT>,

		std::nullptr_t>>
		inline typename Node<CharT>::iterator Node<CharT>::find(
			const ContT& container, const_iterator offset)
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

		for (; first != last;)
		{
			first = find_element(first, last,
				string_wrapper{ &*container.begin(), container.size() });

			if (first != last)
			{
				break;
			}
		}

		return first;
	}

	//*************************************************************************

	template<typename CharT>
	template<typename ContT,
		std::enable_if_t<

		detail::is_has_begin_and_end_v<ContT>&&

		detail::is_has_value_type_v<ContT> &&

		!std::is_arithmetic_v<detail::value_type_t<ContT>> &&

		(std::is_constructible_v<std::basic_string_view<CharT>,
			std::decay_t<
			detail::dereferenced_t<detail::const_iterator_t<ContT>>>> ||

			std::is_constructible_v<std::basic_string_view<CharT>,
			detail::dereferenced_t<detail::const_iterator_t<ContT>>>) &&

		std::is_same_v<std::remove_const_t<
		detail::universal_value_type_t<ContT>>, CharT>,

		std::nullptr_t>>
		inline typename Node<CharT>::iterator Node<CharT>::find(
			const ContT& container, const_iterator offset)
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
		std::enable_if_t<std::is_same_v<ContT, CharT>&&

		std::is_constructible_v<
		std::basic_string_view<CharT>, const ContT*>,

		std::nullptr_t>>
		inline typename Node<CharT>::const_iterator Node<CharT>::find(
			const ContT* container, const_iterator offset) const
	{
		using std::begin;
		using std::end;

		const_iterator first = this->begin();
		const_iterator last = this->end();

		if (offset != const_iterator{ nullptr } &&
			offset._is_in_sequences(this->begin()))
		{
			first = iterator{ &offset->m_tree_node };
		}

		if (offset == last)
		{
			first = last;
		}

		for (; first != last;)
		{
			first = find_element(first, last, string_wrapper{ container });

			if (first != last)
			{
				break;
			}
		}

		return first;
	}

	//*************************************************************************

	template<typename CharT>
	template<typename ContT,
		std::enable_if_t<

		detail::is_has_begin_and_end_v<ContT>&&

		detail::is_has_value_type_v<ContT>&&

		std::is_constructible_v<std::basic_string_view<CharT>,
		std::add_pointer_t<std::remove_reference_t<
		detail::dereferenced_t<detail::const_iterator_t<ContT>>>>>&&

		std::is_same_v<detail::universal_value_type_t<ContT>, CharT>,

		std::nullptr_t>>
		inline typename Node<CharT>::const_iterator Node<CharT>::find(
			const ContT& container, const_iterator offset) const
	{
		using std::begin;
		using std::end;

		const_iterator first = this->begin();
		const_iterator last = this->end();

		auto word_it = begin(container);
		auto word_end = end(container);

		if (offset != const_iterator{ nullptr } &&
			offset._is_in_sequences(this->begin()))
		{
			first = cosnt_iterator{ &offset->m_tree_node };
		}

		if (offset == last)
		{
			first = last;
		}

		for (; first != last;)
		{
			first = find_element(first, last,
				string_wrapper{ &*container.begin(), container.size() });

			if (first != last)
			{
				break;
			}
		}

		return first;
	}

	//*************************************************************************

	template<typename CharT>
	template<typename ContT,
		std::enable_if_t<

		detail::is_has_begin_and_end_v<ContT>&&

		detail::is_has_value_type_v<ContT> &&

		!std::is_arithmetic_v<detail::value_type_t<ContT>> &&

		(std::is_constructible_v<std::basic_string_view<CharT>,
			std::decay_t<
			detail::dereferenced_t<detail::const_iterator_t<ContT>>>> ||

			std::is_constructible_v<std::basic_string_view<CharT>,
			detail::dereferenced_t<detail::const_iterator_t<ContT>>>) &&

		std::is_same_v<std::remove_const_t<
		detail::universal_value_type_t<ContT>>, CharT>,

		std::nullptr_t>>
		inline typename Node<CharT>::const_iterator Node<CharT>::find(
			const ContT& container, const_iterator offset) const
	{
		using std::begin;
		using std::end;

		const_iterator first = this->begin();
		const_iterator last = this->end();

		auto word_it = begin(container);
		auto word_end = end(container);

		if (offset != const_iterator{ nullptr } &&
			offset._is_in_sequences(this->begin()))
		{
			first = const_iterator{ &offset->m_tree_node };
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
	inline void Node<CharT>::connect_tree_nodes(tree_node* current_node, 
		tree_node* add_node)
	{
		if (!add_node)
		{
			return;
		}

		//Следующей узел, после текущего
		tree_node* next_node = current_node->next;

		//Последний дочерний узел у добавочного узла
		tree_node* last_add_node = add_node->element->find_last_tree_node();

		//Коннектим между собой текущий, добавочный и следующий узлы

		current_node->next = add_node;
		add_node->prev = current_node;

		if (next_node)
		{
			last_add_node->next = next_node;
			next_node->prev = last_add_node;
		}
		else
		{
			last_add_node->next = current_node;
			current_node->prev = last_add_node;
		}
		
		//Меняем родителя у добавочного узла
		add_node->parent = &m_tree_node;

		//Увелививаем общее количество узлов
		update_size(static_cast<int>(add_node->element->m_size + 1));
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

		//Удаляем элемент и уменьшаем общее количество узлов.
		//Временное решение с const_castom...
		Node* parent = const_cast<Node*>(node->get_parent());

		parent->m_childs.erase(erase_iterator);

		//В родителях тоже корректируем количество узлов
		tree_node* parent_tree = node->m_tree_node.parent;

		//Уменьшаем общее количество узлов
		update_size(static_cast<int>(-1 * node->m_size + 1));

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
		string_wrapper tag_name) const
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

	template<typename CharT>
	inline void Node<CharT>::update_size(int size) noexcept
	{
		m_size += size;

		auto current_parent = m_tree_node.parent;

		while (current_parent)
		{	
			current_parent->element->update_size(size);
			
			current_parent = current_parent->parent;
		}
	}

	//*************************************************************************



	//*************************************************************************
	//						NODE SUPPORT FUNCTIONS
	//*************************************************************************

	/**************************************************************************
	* @brief Проверить на валидность XML узел
	* 
	* @ingroup general
	*
	* @param node - XML узел
	*
	* @return диагностический объект, который преобразуется в true, если была
	* найдена ошибка и false если ошибок не было найдено
	**************************************************************************/
	template<typename CharT>
	inline bool is_correct(const Node<CharT>& node)
	{
		bool result = true;

		if (!node.get_name().size())
		{
			result = false;
		}
		else if (node.attr_size())
		{
			auto attr_it = node.attr_cbegin();
			auto attr_end = node.attr_cend();

			for (; attr_it != attr_end; ++attr_it)
			{
				if (!attr_it->name.size())
				{
					result = false;

					break;
				}
			}
		}

		return result;
	}

	//*************************************************************************

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



	/**************************************************************************
	* @brief Обменять данные двух Node_attribute
	*
	* @tparam CharT - тип символов
	*
	* @param lhs - первый Node_attribute
	* @param rhs - второй Node_attribute
	**************************************************************************/
	template<typename CharT>
	inline void swap(Node_attribute<CharT>& lhs, Node_attribute<CharT>& rhs) 
		noexcept
	{
		using std::swap;

		swap(lhs.name, rhs.name);
		swap(lhs.value, rhs.value);
	}

	//*************************************************************************



	/**************************************************************************
	* @brief Обменять данные двух Node
	*
	* @tparam CharT - тип символов
	*
	* @param lhs - первый Node
	* @param rhs - второй Node
	**************************************************************************/
	template<typename CharT>
	inline void swap(Node<CharT>& lhs, Node<CharT>& rhs) noexcept
	{
		lhs.swap(rhs);
	}

	//*************************************************************************

} // namespace XMLB

#endif // !XMLB_NODE_H