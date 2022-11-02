#include <fstream>
#include <string>

#include "XMLB/XMLB.h"

template<typename CharT>
struct My_decorator
{
	using symbol_type = CharT;

	//Change open_tag_symol, close_tag_symol and equal_attribute_symbol

	symbol_type fill_symbol = '\t';
	symbol_type white_space_symbol = ' ';
	symbol_type open_tag_symbol = '#';
	symbol_type close_tag_symbol = '%';
	symbol_type single_tag_symbol = '/';
	symbol_type last_tag_symbol = '/';
	symbol_type open_attribute_symbol = '\"';
	symbol_type close_attribute_symbol = '\"';
	symbol_type equal_attribute_symbol = '@';
	symbol_type comment_symbol = '!';
	symbol_type comment_last_symbol = '-';
	symbol_type line_break_symbol = '\n';
	symbol_type tab_symbol = '\t';
	symbol_type carriage_symbol = '\r';
	symbol_type doc_info_symbol = '?';
	symbol_type doc_info_last_symbol = '?';

	//others symbols in my decorator for my parser
};

int main()
{
	//-------------------------------------------------------------------------
	// EXAMPLE 1. Read XML file with default decorator. Then save XML document
	// with custom decorator
	//-------------------------------------------------------------------------

	std::string load_file_name_1{ "sample_load_1.xml" };
	std::string save_file_name_1{ "sample_save_1.xml" };
	
	std::ifstream in_file_1{ load_file_name_1 };
	in_file_1.unsetf(std::ios_base::skipws);

	if (in_file_1)
	{
		auto doc = XMLB::load_from(std::istream_iterator<char>{in_file_1}, 
			std::istream_iterator<char>{});

		in_file_1.close();

		if (doc)
		{
			std::ofstream out_file_1{ save_file_name_1 };

			if (out_file_1)
			{
				//Create here or in function custom decorator
				My_decorator<char> custom_decorator;

				XMLB::save_to(*doc, std::ostream_iterator<char>{out_file_1}, 
					custom_decorator);

				out_file_1.close();
			}
		}
	}

	//-------------------------------------------------------------------------
	// EXAMPLE 2. Load XML file with custom decorator and save XML document
	// with default decorator
	//-------------------------------------------------------------------------

	std::string load_file_name_2{ "sample_save_1.xml" };
	std::string save_file_name_2{ "sample_save_2.xml" };

	std::ifstream in_file_2{ load_file_name_2 };
	in_file_2.unsetf(std::ios_base::skipws);

	if (in_file_2)
	{
		//You can modify default decorator;
		XMLB::u8Decorator custrom_decorator{};

		custrom_decorator.open_tag_symbol = '#';
		custrom_decorator.close_tag_symbol = '%';
		custrom_decorator.equal_attribute_symbol = '@';

		auto doc = XMLB::load_from(std::istream_iterator<char>{in_file_2}, 
			std::istream_iterator<char>{}, custrom_decorator);

		in_file_2.close();

		if (doc)
		{
			std::ofstream out_file_2{ save_file_name_2 };

			if (out_file_2)
			{
				XMLB::save_to(*doc, std::ostream_iterator<char>{out_file_2});

				out_file_2.close();
			}
		}

	}

	return 0;
}