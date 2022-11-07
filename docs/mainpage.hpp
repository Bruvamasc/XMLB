////////////////////////////////////////////////////////////
/// \mainpage XMLB Документация
///
/// \section start_welcome Приветствие
/// Данная секция не законена. <br/>
///
/// \section short_example Короткий пример
/// Вот простой и короткий пример, который показывает как легко и быстро
/// использовать XMLB библиотеку:
///
/// \code
///
///	#include <fstream>
///	#include <string>
///
///	#include "XMLB/XMLB.h"
///
///	int main()
///	{
///		std::string save_file_name_1{ "sample_save_1.xml" };
///
///		XMLB::u8Document doc{ };
///
///		auto general_node = std::make_unique<XMLB::u8Node>("HOUSE");
///		general_node->add_attribute({ "height", "10.m" });
///		general_node->add_attribute({ "widht", "5.m" });
///		general_node->add_attribute({ "lenght", "4.m" });
///
///		auto wall_1 = std::make_unique<XMLB::u8Node>("front_wall");
///		wall_1->add_child(XMLB::u8Node{ "door", "Очень красивая дверь!" });
///		wall_1->add_attribute(XMLB::u8Node_attribute{ "color", "green" });
///
///		XMLB::u8Node wall_2{ "left_wall" };
///		wall_2.add_child({ "window", "Чистое окно!" });
///		wall_2.add_attribute(XMLB::u8Node_attribute{ "color", "white" });
///
///		auto wall_3 = std::make_unique<XMLB::u8Node>("back_wall");
///		wall_3->add_child(XMLB::u8Node{ "furnace", "Что делает печь на стене?" });
///		wall_3->add_attribute(XMLB::u8Node_attribute{ "color", "blue" });
///
///		XMLB::u8Node wall_4{ "right_wall" };
///		wall_4.add_child({ "window", "Ещё одно чистое окно!" });
///		wall_4.add_attribute(XMLB::u8Node_attribute{ "color", "red" });
///
///		general_node->add_child(std::move(wall_1));
///		general_node->add_child(wall_2);
///		general_node->add_child(std::move(wall_3));
///		general_node->add_child(std::move(wall_4));
///
///		doc.root(std::move(general_node));
///
///		doc.set_version(1.1f);
///		doc.set_encoding_type("UTF-16");
///
///		auto found_back_wall = doc.find("back_wall");
///
///		auto found_right_window = doc.find({ "right_wall", "window" },
///			found_back_wall);
///
///		//If we found back_wall
///		if (found_back_wall != doc.end())
///		{
///			found_back_wall->add_attribute({ "price", "3000$" });
///		}
///
///		//If we found right window
///		if (found_right_window != doc.end())
///		{
///			found_right_window->add_child(XMLB::u8Node{ "glass" }).
///				add_attribute({ "color", "yellow" });
///
///			found_right_window->add_child({ "window_frame" }).
///				add_attribute({ "manufacturer", "Steel and Air" }).
///				add_child({ "sealant", "Хороший уплотнитель!" });
///		}
///
///		std::ofstream out_file_1{ save_file_name_1 };
///
///		if (out_file_1)
///		{
///			XMLB::save_to(doc, std::ostream_iterator<char>{out_file_1});
///
///			out_file_1.close();
///		}
/// 
///		return 0;
///	}
///
/// \endcode
////////////////////////////////////////////////////////////