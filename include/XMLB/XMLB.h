/******************************************************************************
* @file
* Данный файл просто включает все нужные файлы для работы мини-библиоткеи XMLB.
*
* @author Bruvamasc
* @date   2022-09-17
*
* @todo Нужно подумать, как добавить режим дебага
* ///< Указывает, что элемент недоступен для использования
*
******************************************************************************/

#ifndef XMLB_H
#define XMLB_H

#include "XMLB/XMLB_Node.h"
#include "XMLB/XMLB_Document.h"
#include "XMLB/XMLB_utility.h"

namespace XMLB
{
	using u8Node = Node<char>;
	using u16Node = Node<char16_t>;
	using u32Node = Node<char32_t>;
	using wNode = Node<wchar_t>;

	using u8Document = Document<char>;
	using u16Document = Document<char16_t>;
	using u32Document = Document<char32_t>;
	using wDocument = Document<wchar_t>;

	using u8Decorator = detail::Decorator<char>;
	using u16Decorator = detail::Decorator<char16_t>;
	using u32Decorator = detail::Decorator<char32_t>;
	using wDecorator = detail::Decorator<wchar_t>;
}

#endif // !XMLB_H