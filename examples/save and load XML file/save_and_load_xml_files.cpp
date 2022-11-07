#include <fstream>
#include <vector>
#include <string>

#include "XMLB/XMLB.h"

int main()
{
	//Files names for first example
	std::string load_file_name_1{ "sample_load_1.xml" };
	std::string save_file_name_1{ "sample_save_1.xml" };

	//Files names for second example
	std::string load_file_name_2{ "sample_load_2.xml" };
	std::string save_file_name_2{ "sample_save_2.xml" };

	//-------------------------------------------------------------------------
	// EXAMPLE 1. Load from file width std::istream_iterator and save to 
	// std::vector
	//-------------------------------------------------------------------------

	std::ifstream in_file_1{ load_file_name_1 };
	in_file_1.unsetf(std::ios_base::skipws);

	if (in_file_1.is_open())
	{
		auto&& doc = XMLB::load_from(std::istream_iterator<char>(in_file_1), 
			std::istream_iterator<char>{});

		in_file_1.close();

		//Check if doc was created and load information
		if (doc)
		{
			//...
			//You working with XML document for example here and ready to save
			//...

			//Calculate symbols count for std::vector buffer
			auto doc_symbols_count = XMLB::symbols_count(*doc);

			std::vector<char> buffer;
			buffer.reserve(doc_symbols_count);

			//Just for example save to std::vector. You can save too XML 
			//document to file. See in the next example
			XMLB::save_to(*doc, std::back_inserter(buffer));

			std::ofstream out_file_1{ save_file_name_1 };

			if (out_file_1.is_open())
			{
				std::copy(std::begin(buffer), std::end(buffer),
					std::ostream_iterator<char>{out_file_1});

				out_file_1.close();
			}
		}
	}

	//-------------------------------------------------------------------------
	// EXAMPLE 2. Load from std::u16string and save to file
	//-------------------------------------------------------------------------

	std::basic_ifstream<char16_t> in_file_2{ load_file_name_2 };
	in_file_2.unsetf(std::ios_base::skipws);

	if (in_file_2.is_open())
	{
		in_file_2.seekg(0, std::ios_base::end);

		std::u16string buffer;
		buffer.reserve(in_file_2.tellg());

		in_file_2.seekg(0, std::ios_base::beg);

		std::copy(std::istream_iterator<char16_t, char16_t>{in_file_2}, 
			std::istream_iterator<char16_t, char16_t>{}, 
			std::back_inserter(buffer));

		in_file_2.close();

		auto&& doc = XMLB::load_from(std::begin(buffer), std::end(buffer));

		//Check if doc was created and load information
		if (doc)
		{
			//...
			//You working with XML document for example here and ready to save
			//...

			std::basic_ofstream<char16_t> out_file_2{ save_file_name_2 };

			if (out_file_2.is_open())
			{
				XMLB::save_to(*doc, 
					std::ostream_iterator<char16_t, char16_t>{out_file_2});

				out_file_2.close();
			}
		}
	}
	
	return 0;
}