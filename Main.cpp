#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define DBG_NEW new( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define newDBG_NEW

#include <iostream>
#include <string>
#include <fstream>

#include <list>

#include "XMLB/XMLB.h"

template<typename CharT>
struct TEST final
{
	using symbol_type = CharT;

	///Символ-заполнитель перед началом тегов
	symbol_type fill_symbol = '\t';

	///Символ оступа между словами
	symbol_type white_space_symbol = ' ';

	///Символ открытия тега
	symbol_type open_tag_symbol = '#';

	///Символ закрытия тега
	symbol_type close_tag_symbol = '#';

	///Символ перед символом закрытия одиночного тега
	symbol_type single_tag_symbol = '/';

	///Символ после открытия завершающего тега
	symbol_type last_tag_symbol = '/';

	///Символ перед значением аттрибута
	symbol_type open_attribute_symbol = '\"';

	///Символ после значения аттрибута
	symbol_type close_attribute_symbol = '\"';

	///Символ после имени аттрибута
	symbol_type equal_attribute_symbol = '=';

	///Символ начала комментария
	symbol_type comment_symbol = '!';

	///Символ окончания комментария
	symbol_type comment_last_symbol = '-';

	///Символ перевода строки
	symbol_type line_break_symbol = '\n';

	///Символ табуляции
	symbol_type tab_symbol = '\t';

	///Символ перевода каретки
	symbol_type carriage_symbol = '\r';

	///Символ начала информации о документе
	symbol_type doc_info_symbol = '?';

	///Символ конца информации о документе
	symbol_type doc_info_last_symbol = '?';
};

//template<typename CharT>
//void print_some_info(XMLB::Document<CharT>& doc)
//{
//	std::cout << "DOC version: " << doc.get_version() << '\n';
//	std::cout << "DOC encode: " << 
//		XMLB::encoding_to_string<char>(XMLB::get_doc_encode(doc)) << '\n';
//	std::cout << "DOC nodes size: " << std::distance(doc.cbegin(), doc.cend());
//	std::cout << '\n' << '\n';
//}

void print_file_saved_or_not(const std::string& file_name, bool status)
{
	if (status)
	{
		std::cout << "File: " << file_name << " was saved!" << '\n';
	}
	else
	{
		std::cout << "File: " << file_name << " was not saved!" << '\n';
	}
}

void print_file_loaded_or_not(const std::string& file_name, bool status)
{
	if (status)
	{
		std::cout << "File: " << file_name << " was loaded!" << '\n';
	}
	else
	{
		std::cout << "File: " << file_name << " was not loaded!" << '\n';
	}
}

int main()
{
	//using Container = XMLB::detail::Tag_data<char>;
	//using symbol_type = char;
	//using iterator_type =
	//	XMLB::detail::iterator_t<typename std::decay_t<Container>::child_container_type>;
	//using node_type = XMLB::Node<symbol_type>;
	//using node_pointer = typename XMLB::Node<symbol_type>::Ptr;
	//

	//using iter = std::list<XMLB::detail::Tag_data<symbol_type>>::iterator;
	//using tag_type = XMLB::detail::Tag_range<iter, node_pointer>;

	//std::stack<tag_type> tags;

	//auto l = tags.top().first;


	/**************************************************************************
	*								ТЕСТЫ
	**************************************************************************/

	std::string load_file_name_2{ "sample_load_1.xml" };
	std::string save_file_name_2{ "sample_save_1.xml" };

	std::ifstream in_file{ load_file_name_2 };
	in_file.unsetf(std::ios_base::skipws);

	using symbol_type = char;

	if (in_file.is_open())
	{
		auto&& doc = XMLB::load_from(std::istream_iterator<symbol_type>(in_file), std::istream_iterator<symbol_type>{});

		in_file.close();

		if (doc)
		{
			std::cout << "File was loaded!" << '\n';

			/*std::vector<std::basic_string<symbol_type>> buffer;

			buffer.reserve(14);

			if (XMLB::save_to(*doc, std::back_inserter(buffer)))
			{
				std::cout << "Buffer was saved!" << '\n';
				std::cout << "Buffer size == " << buffer.size();
			}*/

			auto found_composition = doc->get_general_node()->find({ "composition" });

			if (found_composition != doc->end())
			{
				auto found_ingrediend = doc->get_general_node()->find({ "ingredient" }, found_composition);

				std::vector<std::string> words
				{
					"Это точно не мука!",
					"Хмм.... дрожжи?",
					"А если я люблю холодную воду?!"
				};

				auto word_it = words.cbegin();

				while (found_ingrediend != doc->end() && word_it != words.cend())
				{
					found_ingrediend->set_value(*word_it);

					++word_it;
					
					found_ingrediend = doc->get_general_node()->find({ "ingredient" }, ++found_ingrediend);
				}
			}

			std::ofstream out_file{ save_file_name_2 };

			if (out_file.is_open())
			{
				if (XMLB::save_to(*doc, std::ostream_iterator<symbol_type>{out_file}))
				{
					std::cout << "File was saved!" << '\n';
				}

				out_file.close();
			}
		}
	}

	

	/**************************************************************************
	*									ТЕСТЫ
	**************************************************************************/

	_CrtDumpMemoryLeaks();

	return 0;
}


////Считываем с файла XML данные и создаём XML документ в UTF-16. Сам XML
//	//файл, конечно же, может хранится в другой кодировке
//auto&& file_2 = XMLB::load_from_file<char16_t>(load_file_name_2);
//
////Проверяем, был ли создан XML документ
//if (file_2)
//{
//	//Просто выводи некоторую информацию о документе
//	print_some_info(*file_2);
//
//	auto temp_iter = file_2->begin();
//	auto&& found_iter = temp_iter->find
//	(
//		std::initializer_list<std::u16string>
//	{
//		u"composition", u"ingredient"
//	}
//	);
//
//	if (found_iter != file_2->end())
//	{
//		XMLB::u16Node::Ptr temp_node = std::make_unique<XMLB::u16Node>(u"cost");
//		temp_node->add_attribute({ u"currency", u"EURO" });
//		temp_node->add_child(XMLB::u16Node{ u"sale", u"15%" });
//		temp_node->add_child(XMLB::u16Node{ u"general", u"3000" });
//
//		found_iter->add_child(std::move(temp_node));
//	}
//
//	/*auto found_erase_first = file_2->get_general_node().find({ u"instructions", u"step" });
//
//	if (found_erase_first != file_2->end() && found_iter != file_2->end())
//	{
//		using std::swap;
//
//		swap(*found_erase_first, *(--found_iter));
//	}*/
//
//	/*auto found_erase_first = file_2->get_general_node().find({u"instructions", u"step"});
//
//	auto found_erase_last = found_erase_first;
//
//	while (found_erase_last != file_2->end())
//	{
//		found_erase_last = file_2->get_general_node().find({ u"step" }, std::next(found_erase_last));
//	}
//
//	if (found_erase_first != file_2->end())
//	{
//		file_2->get_general_node().erase_child(found_erase_first, found_erase_last);
//	}*/
//
//	//Сохраняем в файл загруженный XML документ
//	if (XMLB::save_to_file(*file_2, save_file_name_2))
//	{
//		print_file_saved_or_not(save_file_name_2, true);
//	}
//	else
//	{
//		print_file_saved_or_not(save_file_name_2, false);
//	}
//}
//else
//{
//	print_file_loaded_or_not(save_file_name_2, false);
//}