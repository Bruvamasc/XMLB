/******************************************************************************
* @file
* ƒанный файл объ€вл€ет и определе€ет константы дл€ XML структуры
*
* @author Bruvamasc
* @date   2022-08-25
*
* @todo “јјј“ј“
* ///< ”казывает, что элемент недоступен дл€ использовани€
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

	/// Ўаблон регул€рного выражени€ дл€ нахождени€ любых тегов, которые
	/// начинаютс€ с символа '<' и заканчиваютс€ символом '>'. ≈сли есть
	/// вложенные теги, то найдетс€ самый внутрненний
	const std::regex kTag_Element_template{"<([^<>]+)>"};

	/// Ўаблон регул€рного выражени€ дл€ нахождени€ имени тега
	const std::regex kOpen_tag_template{ R"(<([^\s\.\-!?,=>%\$#@"'+~()*&\^|\/\\]+)\s?)" };

	/// Ўаблон регул€рного выражени€ дл€ нахождени€ аттрибутов тега
	const std::regex kTag_parameters_template{ R"(\s([^\s\.\-!?=,>%$#@"'+~)(*&\^|\/\\]+="[^\s"]+"))"};

	/// Ўаблон регул€рного выражени€ дл€ нахождени€ имени закрытого тега
	const std::regex kClose_tag_template{ R"(<\/([^\s\.\-!?,=>%$#@\"'+~()*&\^|\/\\]+))"};

	/// Ўаблон регул€рного выражени€ дл€ нахождени€ имени одиночного тега
	const std::regex kSingle_tag_template{ R"(/{1}>)" };

	/// Ўаблон регул€рного выражени€ дл€ нахождени€ значени€, текста тега
	const std::regex kText_template{ R"(>(.+)<)" };

	/// Ўаблон регул€рного выражени€ дл€ нахождени€ версии XML документа
	const std::regex kVersion_tag_template{ R"(version\s*=\s*"(\d+\.?\d*))"};

	/// Ўаблон регул€рного выражени€ дл€ нахождени€ кодировки XML документа
	const std::regex kEncoding_tag_template{ R"(encoding\s*=\s*"(.+)\")" };
}

#endif // !XMLB_CONSTANTS_H