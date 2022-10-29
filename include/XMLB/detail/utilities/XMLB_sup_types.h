/******************************************************************************
* @file
* Данный файл объявляет и определяет вспомагательные шаблонные классы,
* структуры, функции
*
* @author Bruvamasc
* @date   2022-08-25
*
* @todo Подумать над возможными изменениями Node_tree_impl, code_converter
* ///< Указывает, что элемент недоступен для использования
*
******************************************************************************/

#ifndef XMLB_SUP_TYPES_H
#define XMLB_SUP_TYPES_H

namespace XMLB { namespace detail {

	/**************************************************************************
	* @brief Вспомагательная структура, для функции
	* create_doc_from_docyment
	**************************************************************************/
	template<typename Iter, typename T>
	struct Tag_range final
	{
		Iter first;				//Итератор на начало диапазона
		Iter last;				//Итератор на конец диапазона
		T node;					//XML узел
	};

	//*************************************************************************

	template<typename Iter, typename T>
	inline void swap(Tag_range<Iter, T>& lhs, Tag_range<Iter, T>& rhs) noexcept
	{
		using std::swap;

		swap(lhs.first, rhs.first);
		swap(lhs.last, rhs.last);
		swap(lhs.node, rhs.node);
	}

	//*************************************************************************



	/**************************************************************************
	* @brief Вспомагательная структура, для "дерева" XML узлов.
	* Является деталью реализации!
	**************************************************************************/
	template<typename T>
	struct Node_tree final
	{
		T* element = nullptr;				//Указатель на текущий узел
		Node_tree<T>* parent = nullptr;		//Указатель на родителя узала
		Node_tree<T>* next = nullptr;		//Указатель на следующий узел
		Node_tree<T>* prev = nullptr;		//Указатель на предыдущий узел
	};

	//*************************************************************************

	template<typename T>
	inline void swap(Node_tree<T>& lhs, Node_tree<T>& rhs) noexcept
	{
		using std::swap;

		swap(lhs.next, rhs.next);
		swap(lhs.prev, rhs.prev);
		swap(lhs.parent, rhs.parent);
		swap(lhs.element, rhs.element);
	}

	//*************************************************************************

}} // namespace XMLB::detail

#endif // !XMLB_SUP_TYPES_H