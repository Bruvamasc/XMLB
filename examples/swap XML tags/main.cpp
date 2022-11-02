#include <fstream>
#include <string>
#include <vector>

#include "XMLB/XMLB.h"

int main()
{
	//-------------------------------------------------------------------------
	// EXAMPLE 1. Load two XML files, find in they some tags and swap it
	//-------------------------------------------------------------------------

	std::string load_file_name_1{ "sample_load_1.xml" };
	std::string load_file_name_2{ "sample_load_2.xml" };
	std::string save_file_name_1{ "sample_save_1.xml" };
	std::string save_file_name_2{ "sample_save_2.xml" };

	std::ifstream in_file{ load_file_name_1 };
	in_file.unsetf(std::ios_base::skipws);

	XMLB::u8Document::Ptr first_doc{ nullptr };
	XMLB::u8Document::Ptr second_doc{ nullptr };

	if (in_file)
	{
		first_doc = XMLB::load_from(std::istream_iterator<char>{in_file}, 
			std::istream_iterator<char>{});

		in_file.close();
	}

	in_file.open(load_file_name_2);

	if (in_file)
	{
		second_doc = XMLB::load_from(std::istream_iterator<char>{in_file},
			std::istream_iterator<char>{});

		in_file.close();
	}

	if (first_doc && second_doc)
	{
		std::vector<std::string> found_strings
		{
			"right_wall",
			"window_frame"
		};

		auto found_instructions = first_doc->find("instructions");
		auto found_window_frame = second_doc->find(found_strings);

		if (found_instructions != first_doc->end() &&
			found_window_frame != second_doc->end())
		{
			using std::swap;

			swap(*found_instructions, *found_window_frame);

			std::ofstream out_file{ save_file_name_1 };

			if (out_file)
			{
				XMLB::save_to(*first_doc, 
					std::ostream_iterator<char>{out_file});

				out_file.close();
			}

			out_file.open(save_file_name_2);

			if (out_file)
			{
				XMLB::save_to(*second_doc, 
					std::ostream_iterator<char>{out_file});

				out_file.close();
			}
		}
	}

	return 0;
}