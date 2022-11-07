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



#ifndef XMLB_SUP_TYPES_H
#define XMLB_SUP_TYPES_H



namespace XMLB { namespace detail {

	//*************************************************************************
	// @brief Вспомагательная структура, для функции
	// create_doc_from_docyment
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



	//*************************************************************************
	// @brief Вспомагательная структура, для "дерева" XML узлов.
	// Является деталью реализации!
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