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



#ifndef XMLB_FWD_H
#define XMLB_FWD_H



namespace XMLB
{
	namespace detail
	{
		template<typename T>
		class Node_const_iterator;

		template<typename T>
		class Node_iterator;

		template<typename IterT>
		class Diagnostic_iterator;

		template<typename CharT>
		struct Decorator;
	}

	template<typename CharT>
	class Document;

	template<typename CharT>
	class Node;

	template<typename CharT>
	struct Node_attribute;



	using u8Node_attribute = Node_attribute<char>;
	using u16Node_attribute = Node_attribute<char16_t>;
	using u32Node_attribute = Node_attribute<char32_t>;
	using wNode_attribute = Node_attribute<wchar_t>;



	using u8Node = Node<char>;
	using u16Node = Node<char16_t>;
	using u32Node = Node<char32_t>;
	using wNode = Node<wchar_t>;



	using u8Document = Document<char>;
	using u16Document = Document<char16_t>;
	using u32Document = Document<char32_t>;
	using wDocument = Document<wchar_t>;



	using u8Node_iterator = detail::Node_iterator<char>;
	using u16Node_iterator = detail::Node_iterator<char16_t>;
	using u32Node_iterator = detail::Node_iterator<char32_t>;
	using wNode_iterator = detail::Node_iterator<wchar_t>;



	using u8Node_const_iterator = detail::Node_const_iterator<char>;
	using u16Node_const_iterator = detail::Node_const_iterator<char16_t>;
	using u32Node_const_iterator = detail::Node_const_iterator<char32_t>;
	using wNode_const_iterator = detail::Node_const_iterator<wchar_t>;



	using u8Decorator = detail::Decorator<char>;
	using u16Decorator = detail::Decorator<char16_t>;
	using u32Decorator = detail::Decorator<char32_t>;
	using wDecorator = detail::Decorator<wchar_t>;



	using u8Diagnostic_iterator = detail::Diagnostic_iterator<u8Node_iterator>;

	using u16Diagnostic_iterator = 
		detail::Diagnostic_iterator<u16Node_iterator>;

	using u32Diagnostic_iterator = 
		detail::Diagnostic_iterator<u32Node_iterator>;

	using wDiagnostic_iterator = detail::Diagnostic_iterator<wNode_iterator>;



	using u8Diagnostic_const_iterator = 
		detail::Diagnostic_iterator<u8Node_const_iterator>;

	using u16Diagnostic_const_iterator = 
		detail::Diagnostic_iterator<u16Node_const_iterator>;

	using u32Diagnostic_const_iterator = 
		detail::Diagnostic_iterator<u32Node_const_iterator>;

	using wDiagnostic_const_iterator = 
		detail::Diagnostic_iterator<wNode_const_iterator>;
}

#endif // !XMLB_FWD_H

/******************************************************************************
* @defgroup general Основные классы и функции
* @brief Классы и функции, которые используются в стандартном использовании
* библиотеки
* 
* @defgroup secondary Второстепенные классы и функции
* @brief Классы и функции, которые можно настроить, заменить своими при
* необходимости или использовать
******************************************************************************/