/******************************************************************************
* @file
* ������ ���� ��������� � ����������� ��������� ��� XML ���������
*
* @author Bruvamasc
* @date   2022-08-25
*
* @todo �������
* ///< ���������, ��� ������� ���������� ��� �������������
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

	/// ������ ����������� ��������� ��� ���������� ����� �����, �������
	/// ���������� � ������� '<' � ������������� �������� '>'. ���� ����
	/// ��������� ����, �� �������� ����� �����������
	const std::regex kTag_Element_template{"<([^<>]+)>"};

	/// ������ ����������� ��������� ��� ���������� ����� ����
	const std::regex kOpen_tag_template{ R"(<([^\s\.\-!?,=>%\$#@"'+~()*&\^|\/\\]+)\s?)" };

	/// ������ ����������� ��������� ��� ���������� ���������� ����
	const std::regex kTag_parameters_template{ R"(\s([^\s\.\-!?=,>%$#@"'+~)(*&\^|\/\\]+="[^\s"]+"))"};

	/// ������ ����������� ��������� ��� ���������� ����� ��������� ����
	const std::regex kClose_tag_template{ R"(<\/([^\s\.\-!?,=>%$#@\"'+~()*&\^|\/\\]+))"};

	/// ������ ����������� ��������� ��� ���������� ����� ���������� ����
	const std::regex kSingle_tag_template{ R"(/{1}>)" };

	/// ������ ����������� ��������� ��� ���������� ��������, ������ ����
	const std::regex kText_template{ R"(>(.+)<)" };

	/// ������ ����������� ��������� ��� ���������� ������ XML ���������
	const std::regex kVersion_tag_template{ R"(version\s*=\s*"(\d+\.?\d*))"};

	/// ������ ����������� ��������� ��� ���������� ��������� XML ���������
	const std::regex kEncoding_tag_template{ R"(encoding\s*=\s*"(.+)\")" };
}

#endif // !XMLB_CONSTANTS_H