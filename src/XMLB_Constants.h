/******************************************************************************
* @file
* Данный файл объявл¤ет и определе¤ет константы дл¤ XML структуры
*
* @author Bruvamasc
* @date   2022-08-25
*
* @todo “јјј“ј“
* ///< Указывает, что элемент недоступен дл¤ использовани¤
*
******************************************************************************/

#ifndef XMLB_CONSTANTS_H
#define XMLB_CONSTANTS_H

#include <string>
#include <regex>

namespace XMLB
{
	constexpr char kTab = '\t';
	constexpr char kSpace = ' ';
	constexpr char kOpen_tag = '<';
	constexpr char kClose_tag = '>';
	constexpr char kSlash = '/';

	/// Шаблон регулярного выражени¤ дл¤ нахождени¤ любых тегов, которые
	/// начинаютс¤ с символа '<' и заканчиваютс¤ символом '>'. если есть
	/// вложенные теги, то найдется самый внутрненний
	const std::regex kTag_Element_template{"<([^<>]+)>"};

	/// Шаблон регулярного выражения для нахождения имени тега
	const std::regex kOpen_tag_template
	{ 
		R"(<([^\s\.\-!?,=>%\$#@"'+~()*&\^|\/\\]+)\s?)" 
	};

	/// Шаблон регул¤рного выражения для нахождения аттрибутов тега
	const std::regex kTag_parameters_template
	{ 
		R"(\s([^\s\.\-!?=,>%$#@"'+~)(*&\^|\/\\]+="[^\s"]+"))"
	};

	/// Шаблон регулярного выражения для нахождения имени закрытого тега
	const std::regex kClose_tag_template
	{ 
		R"(<\/([^\s\.\-!?,=>%$#@\"'+~()*&\^|\/\\]+))"
	};

	/// Шаблон регулярного выражения для нахождения имени одиночного тега
	const std::regex kSingle_tag_template{ R"(/{1}>)" };

	/// Шаблон регулярного выражения для нахождения значения, текста тега
	const std::regex kText_template{ R"(>(.+)<)" };

	/// Шаблон регулярного выражения дл¤ нахождения версии XML документа
	const std::regex kVersion_tag_template{ R"(version\s*=\s*"(\d+\.?\d*))"};

	/// Шаблон регулярного выражения для нахождения кодировки XML документа
	const std::regex kEncoding_tag_template{ R"(encoding\s*=\s*"(.+)\")" };
}

#endif // !XMLB_CONSTANTS_H