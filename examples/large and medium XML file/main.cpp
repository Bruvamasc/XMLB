#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define DBG_NEW new( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define newDBG_NEW

#include <fstream>
#include <string>
#include <chrono>

#include "XMLB/XMLB.h"

class Timer
{
public:
	using clock_t = std::chrono::high_resolution_clock;
	using second_t = std::chrono::duration<double, std::ratio<1> >;

	Timer() : m_point(clock_t::now())
	{

	}

	void reset()
	{
		m_point = clock_t::now();
	}

	double elapsed() const
	{
		return std::chrono::duration_cast<second_t>(clock_t::now() - m_point).count();
	}

private:
	std::chrono::time_point<clock_t> m_point;
};

int main()
{
	//-------------------------------------------------------------------------
	// EXAMPLE 1. Load two XML files, find in they some tags and swap it
	//-------------------------------------------------------------------------

	//Результаты +- нормальные, но нужно подумать над ускорением, так как в
	//предыдущем ченовом варианта, загрузка в RELESE 
	//для большого файла была = 1.40836, а для среднего = 0.452555

	//11 286 307 symbols/bytes DEBUG
	//Load and parse from file (istream_iterator) - 20.2824
	//Load and parse from buffer (std::vector) - 21.5387
	//Save to file - 3.76143

	//11 286 307 symbols/bytes RELESE
	//Load and parse from file (istream_iterator) - 3.81051
	//Load and parse from buffer (std::vector) - 3.30794
	//Save to file - 0.831036
	std::string large_load_file_name{ "sample_large.xml" };
	std::string large_save_file_name{ "sample_large_save.xml" };

	//5 302 611 symbols/bytes DEBUG
	//Load and parse from file (istream_iterator) - 10.8433
	//Load and parse from buffer (std::vector) -  11.54578?
	//Save to file - 1.80689

	//5 302 611 symbols/bytes RELESE
	//Load and parse from file (istream_iterator) - 1.64537
	//Load and parse from buffer (std::vector) - 1.6124
	//Save to file - 1.24418
	std::string medium_load_file_name{ "sample_medium.xml" };
	std::string medium_save_file_name{ "sample_medium_save.xml" };

	std::ifstream in_file;
	in_file.unsetf(std::ios_base::skipws);

	Timer timer;

	//in_file.open(medium_load_file_name);

	//if (in_file)
	//{
	//	/*in_file.seekg(0, std::ios_base::end);

	//	std::vector<char> buffer;
	//	buffer.reserve(in_file.tellg());

	//	in_file.seekg(0, std::ios_base::beg);

	//	timer.reset();

	//	std::copy(std::istream_iterator<char>{in_file}, std::istream_iterator<char>{}, std::back_inserter(buffer));

	//	auto medium_doc = XMLB::load_from(std::cbegin(buffer), std::cend(buffer));*/

	//	auto medium_doc = XMLB::load_from(std::istream_iterator<char>{in_file},
	//		std::istream_iterator<char>{});

	//	std::cout << "MEDIUM LOAD == " << timer.elapsed() << '\n';

	//	in_file.close();

	//	if (medium_doc)
	//	{
	//		std::ofstream out_file{ medium_save_file_name };

	//		if (out_file)
	//		{
	//			timer.reset();

	//			XMLB::save_to(*medium_doc,
	//				std::ostream_iterator<char>{out_file});

	//			std::cout << "MEDIUM SAVE == " << timer.elapsed() << '\n';

	//			out_file.close();
	//		}
	//	}
	//}

	in_file.open(large_load_file_name);

	if (in_file)
	{
		in_file.seekg(0, std::ios_base::end);

		std::vector<char> buffer;
		buffer.reserve(in_file.tellg());

		in_file.seekg(0, std::ios_base::beg);

		timer.reset();

		std::copy(std::istream_iterator<char>{in_file}, std::istream_iterator<char>{}, std::back_inserter(buffer));

		auto large_doc = XMLB::load_from(std::cbegin(buffer), std::cend(buffer));

		/*auto large_doc = XMLB::load_from(std::istream_iterator<char>{in_file}, 
			std::istream_iterator<char>{});*/

		std::cout << "LARGE LOAD == " << timer.elapsed() << '\n';

		in_file.close();

		if (large_doc)
		{
			std::ofstream out_file{ large_save_file_name };

			if (out_file)
			{
				timer.reset();

				XMLB::save_to(*large_doc, 
					std::ostream_iterator<char>{out_file});

				std::cout << "LARGE SAVE == " << timer.elapsed() << '\n';

				out_file.close();
			}
		}
	}

	_CrtDumpMemoryLeaks();

	return 0;
}