#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define DBG_NEW new( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define newDBG_NEW

#include <iostream>
#include <string>

#include "XMLB.h"

template<typename CharT>
void print_some_info(XMLB::Document<CharT>& doc)
{
	std::cout << "DOC version: " << doc.get_version() << '\n';
	std::cout << "DOC encode: " << 
		XMLB::encoding_to_string<char>(XMLB::get_doc_encode(doc)) << '\n';
	std::cout << "DOC nodes size: " << std::distance(doc.cbegin(), doc.cend());
	std::cout << '\n' << '\n';
}

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
	/**************************************************************************
	*								ВАРИАНТ 1
	**************************************************************************/

	std::string load_file_name_1{ "sample_load_1.xml" };
	std::string save_file_name_1{ "sample_save_1.xml" };

	//Получаем кодировку файла, если это необходимо. Так как это необязательно
	//Может пригодится, если нужно хранить данные именно в так, как на то
	//указывается в XML файле
	XMLB::Encode_type file_type = XMLB::get_file_encode(load_file_name_1);

	//Считываем с файла XML данные и создаём XML документ в зависимости от
	//указанной кодировки в XML файле
	if (file_type == XMLB::Encode_type::UTF_32)
	{
		XMLB::u32Document::Ptr file_1 = 
			XMLB::load_from_file<char32_t>(load_file_name_1);

		//Проверяем, был ли создан XML документ
		if (file_1)
		{
			//Просто выводи некоторую информацию о документе
			print_some_info(*file_1);

			//Сохраняем в файл загруженный XML документ
			if (XMLB::save_to_file(*file_1, save_file_name_1))
			{
				print_file_saved_or_not(save_file_name_1, true);
			}
			else
			{
				print_file_saved_or_not(save_file_name_1, false);
			}
		}
		else
		{
			print_file_loaded_or_not(save_file_name_1, false);
		}
	}
	else if (file_type == XMLB::Encode_type::UTF_16)
	{
		XMLB::u16Document::Ptr file_1 = 
			XMLB::load_from_file<char16_t>(load_file_name_1);

		//Проверяем, был ли создан XML документ
		if (file_1)
		{
			//Просто выводи некоторую информацию о документе
			print_some_info(*file_1);

			//Сохраняем в файл загруженный XML документ
			if (XMLB::save_to_file(*file_1, save_file_name_1))
			{
				print_file_saved_or_not(save_file_name_1, true);
			}
			else
			{
				print_file_saved_or_not(save_file_name_1, false);
			}
		}
		else
		{
			print_file_loaded_or_not(save_file_name_1, false);
		}
	}
	else
	{
		XMLB::u8Document::Ptr file_1 =
			XMLB::load_from_file<char>(load_file_name_1);

		//Проверяем, был ли создан XML документ
		if (file_1)
		{
			//Просто выводи некоторую информацию о документе
			print_some_info(*file_1);

			//Сохраняем в файл загруженный XML документ
			if (XMLB::save_to_file(*file_1, save_file_name_1))
			{
				print_file_saved_or_not(save_file_name_1, true);
			}
			else
			{
				print_file_saved_or_not(save_file_name_1, false);
			}
		}
		else
		{
			print_file_loaded_or_not(save_file_name_1, false);
		}
	}



	/**************************************************************************
	*								ВАРИАНТ 2
	**************************************************************************/

	std::string load_file_name_2{ "sample_load_2.xml" };
	std::string save_file_name_2{ "sample_save_2.xml" };

	//Считываем с файла XML данные и создаём XML документ в UTF-16. Сам XML
	//файл, конечно же, может хранится в другой кодировке
	auto&& file_2 = XMLB::load_from_file<char16_t>(load_file_name_2);

	//Проверяем, был ли создан XML документ
	if (file_2)
	{
		//Просто выводи некоторую информацию о документе
		print_some_info(*file_2);

		//Сохраняем в файл загруженный XML документ
		if (XMLB::save_to_file(*file_2, save_file_name_2))
		{
			print_file_saved_or_not(save_file_name_2, true);
		}
		else
		{
			print_file_saved_or_not(save_file_name_2, false);
		}
	}
	else
	{
		print_file_loaded_or_not(save_file_name_2, false);
	}



	/*auto&& file_encode_type = XMLB::get_file_encode("test_3.txt");

	auto temp_doc = XMLB::load_from_file<char32_t>("test_3.txt");

	if (temp_doc)
	{
		std::cout << "File: test_2.txt was loaded!" << '\n';

		if (XMLB::save_to_file(*temp_doc, "test_2.txt"))
		{
			std::cout << "File: test_3.txt was saved!" << '\n';
		}
	}*/


	_CrtDumpMemoryLeaks();
























	//using std::swap; 

	//XMLB::Node::Ptr obj_1{ new XMLB::Node{"Object_0", "Object name 0"} };

	//XMLB::Node::Ptr child_obj_1{ new XMLB::Node{"Object_1", "Object name 1"} };
	//XMLB::Node::Ptr child_obj_2{ new XMLB::Node{"Object_2", "Object name 2"} };
	//XMLB::Node::Ptr child_obj_3{ new XMLB::Node{"Object_3", "Object name 3"} };

	//child_obj_2->add_child(std::make_unique<XMLB::Node>("Object_4", "Object name 4"));
	//child_obj_2->add_child(std::make_unique<XMLB::Node>("Object_5", "Object name 5"));

	//XMLB::Node::Ptr ch_hr_obj_1 = std::make_unique<XMLB::Node>("Object_6", "Object name 6");
	//ch_hr_obj_1->add_child(std::make_unique<XMLB::Node>("Object_7", "Object name 7"));
	//ch_hr_obj_1->add_child(std::make_unique<XMLB::Node>("Object_8", "Object name 8"));

	//child_obj_2->add_child(std::move(ch_hr_obj_1));

	//XMLB::Node ch_hr_obj_2{ "Object_9", "Object name 9" };
	//ch_hr_obj_2.add_child(std::move(XMLB::Node{ "Object_10", "Object name 10" }));
	//ch_hr_obj_2.add_child(std::move(XMLB::Node{ "Object_11", "Object name 11" }));

	//XMLB::Node ch_hr_obj_3{ "Object_12", "Object name 12" };
	//ch_hr_obj_3.add_child(std::move(XMLB::Node{ "Object_13", "Object name 13" }));
	//ch_hr_obj_3.add_child(std::move(XMLB::Node{ "Object_14", "Object name 14" }));

	////swap(ch_hr_obj_2, ch_hr_obj_3);

	//obj_1->add_child(std::move(child_obj_1));
	//obj_1->add_child(ch_hr_obj_2);
	//obj_1->add_child(std::move(child_obj_2));
	//obj_1->add_child(std::move(child_obj_3));
	//obj_1->add_child(std::move(ch_hr_obj_3));

	////std::cout << "First step:" << '\n';

	////print_XML_nodes(obj_1->cbegin(), obj_1->cend());

	//auto fisrt = std::next(obj_1->child_begin(), 1);
	//auto second = std::next(obj_1->child_begin(), 2);

	//second = std::next(second->get()->child_begin(), 2);

	//swap(*fisrt->get(), *second->get());

	////std::cout << "Second step:" << '\n';

	////print_XML_nodes(obj_1->cbegin(), obj_1->cend());


	//auto temp_doc = std::move(XMLB::load_from_file("test_1.txt"));

	
	
	////User a = { "asd", "srs", 5 };

	////std::cout << a.name;

	////XMLB::Document doc(std::locale("ru_RU.UTF-8"), 1.00340f);

	////std::unique_ptr<XMLB::Node> company(new XMLB::Node{ "COMPANY" });

	////std::unique_ptr<XMLB::Node> cmp_1(new XMLB::Node{ "HR" });
	//////std::unique_ptr<XMLB::Node> cmp_2(new XMLB::Node{ "ECONOMY" });
	//////std::unique_ptr<XMLB::Node> cmp_3(new XMLB::Node{ "TASKS" });

	////std::unique_ptr<XMLB::Node> user_1_cmp_1(new XMLB::Node{ "USER_1" });
	////user_1_cmp_1->add_child(std::make_unique<XMLB::Node>("NAME", "Vasiliy"));
	////user_1_cmp_1->add_child(std::make_unique<XMLB::Node>("JOB", "HR"));
	////user_1_cmp_1->add_child(std::make_unique<XMLB::Node>("AGE", "32"));

	////std::unique_ptr<XMLB::Node> user_2_cmp_1(new XMLB::Node{ "USER_2" });
	////user_2_cmp_1->add_child(std::make_unique<XMLB::Node>("NAME", "Konstantin"));
	////user_2_cmp_1->add_child(std::make_unique<XMLB::Node>("JOB", "HR"));
	////user_2_cmp_1->add_child(std::make_unique<XMLB::Node>("AGE", "18"));

	////XMLB::Node user_3_cmp_1{ "USER_3" };
	////user_3_cmp_1.add_child(std::make_unique<XMLB::Node>("NAME", "Vladislav Xaarhys there"));
	////user_3_cmp_1.add_child(std::make_unique<XMLB::Node>("JOB", "HR_KKK"));
	////user_3_cmp_1.add_child(std::make_unique<XMLB::Node>("AGE", "65_1000"));

	////cmp_1->add_child(std::move(user_1_cmp_1));
	////cmp_1->add_child(std::make_unique<XMLB::Node>("SINGLE"));
	////cmp_1->add_child(std::move(user_2_cmp_1));
	////cmp_1->add_child(user_3_cmp_1);

	////cmp_1->add_attribute(XMLB::Node_attribute{ "id", "0" });
	////cmp_1->add_attribute(XMLB::Node_attribute{ "cash", "3000" });

	////std::cout << "\n\n\n\n" << '\n';

	////std::cout << "Automatic node USER-3 == " << &user_3_cmp_1 << '\n';
	////std::cout << "Address node Vladislav == " << *user_3_cmp_1.child_begin() << '\n';

	////std::cout << "Address node Vladislav in cmp_1 == " << *((--cmp_1->child_end())->get()->child_begin()) << '\n';

	////std::cout << "\n\n\n\n" << '\n';

	////for (auto it = cmp_1->child_begin(), end = cmp_1->child_end(); it != end; ++it)
	////{
	////	std::cout << "Big address == " << it->get() << '\n';

	////	std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~" << '\n';

	////	for (auto s = it->get()->child_begin(), e = it->get()->child_end(); s != e; ++s)
	////	{
	////		std::cout << "Small address == " << s->get() << '\n';
	////	}

	////	std::cout << "------------------------" << "\n\n";
	////}


	/////*auto user_1_cmp_2 = new XMLB::Node("USER-4");
	////user_1_cmp_2->add_child(new XMLB::Node("NAME", "Jon"));
	////user_1_cmp_2->add_child(new XMLB::Node("JOB", "ECONOMY"));
	////user_1_cmp_2->add_child(new XMLB::Node("AGE", "52"));

	////auto user_2_cmp_2 = new XMLB::Node("USER-5");
	////user_2_cmp_2->add_child(XMLB::Node("NAME", "Bred"));
	////user_2_cmp_2->add_child(XMLB::Node("JOB", "ECONOMY"));
	////user_2_cmp_2->add_child(XMLB::Node("AGE", "97"));

	////auto user_3_cmp_2 = XMLB::Node("USER-6");
	////user_3_cmp_2.add_child(new XMLB::Node("NAME", "Loly"));
	////user_3_cmp_2.add_child(new XMLB::Node("JOB", "ECONOMY"));
	////user_3_cmp_2.add_child(new XMLB::Node("AGE", "16"));

	////cmp_2->add_child(user_1_cmp_2);
	////cmp_2->add_child(user_2_cmp_2);
	////cmp_2->add_child(user_3_cmp_2);

	////cmp_2->add_attribute(XMLB::Node_attribute{ "id", "1" });
	////cmp_2->add_attribute(XMLB::Node_attribute{ "cash", "9000" });



	////auto user_1_cmp_3 = std::make_unique<XMLB::Node>("USER-7");
	////user_1_cmp_3->add_child(std::make_unique<XMLB::Node>("NAME", "Western"));
	////user_1_cmp_3->add_child(std::make_unique<XMLB::Node>("JOB", "TASKS"));
	////user_1_cmp_3->add_child(std::make_unique<XMLB::Node>("AGE", "228"));

	////auto user_2_cmp_3 = std::make_unique<XMLB::Node>("USER-8");
	////user_2_cmp_3->add_child(std::make_unique<XMLB::Node>("NAME", "Kolya"));
	////user_2_cmp_3->add_child(std::make_unique<XMLB::Node>("JOB", "TASKS"));
	////user_2_cmp_3->add_child(std::make_unique<XMLB::Node>("AGE", "45"));

	////auto user_3_cmp_3 = std::make_unique<XMLB::Node>("USER-9");
	////user_3_cmp_3->add_child(std::make_unique<XMLB::Node>("NAME", "Aleks"));
	////user_3_cmp_3->add_child(std::make_unique<XMLB::Node>("JOB", "TASKS"));
	////user_3_cmp_3->add_child(std::make_unique<XMLB::Node>("AGE", "76"));

	////cmp_3->add_child(std::move(user_1_cmp_3));
	////cmp_3->add_child(std::move(user_2_cmp_3));
	////cmp_3->add_child(std::move(user_3_cmp_3));

	////cmp_3->add_attribute(XMLB::Node_attribute{ "id", "2" });
	////cmp_3->add_attribute(XMLB::Node_attribute{ "cash", "332" });*/

	////company->add_child(std::move(cmp_1));
	//////company->add_child(std::move(cmp_2));
	//////company->add_child(std::move(cmp_3));

	////doc.set_general_node(std::move(company));

	////std::cout << "\n\n\n\n" << '\n';

	///*if (XMLB::save_to_file(doc, "test_1.txt"))
	//{
	//	std::cout << "File: test_1.txt was saved!" << '\n';
	//}

	//std::cout << "\n\n\n\n" << '\n';*/

	////auto temp_doc = std::move(XMLB::load_from_file("test_1.txt"));
	//auto temp_doc = XMLB::load_from_file("test_1.txt");

	//std::cout << "\n\n\n\n" << '\n';

	//if (temp_doc)
	//{
	//	std::cout << "File: test_1.txt was loaded!" << '\n';

	//	if (XMLB::save_to_file(*temp_doc, "test_2.txt"))
	//	{
	//		std::cout << "File: test_2.txt was saved!" << '\n';
	//	}
	//}

	//std::cout << "\n\n\n\n" << '\n';
}

/*
	XMLB::Document doc(std::locale("ru_RU.UTF-8"), 1.0);

	std::unique_ptr<XMLB::Node> company(new XMLB::Node("COMPANY"));

	std::unique_ptr<XMLB::Node> cmp_1(new XMLB::Node{ "HR" });
	std::unique_ptr<XMLB::Node> cmp_2(new XMLB::Node{ "ECONOMY" });
	std::unique_ptr<XMLB::Node> cmp_3(new XMLB::Node{ "TASKS" });

	auto user_1_cmp_1 = std::make_unique<XMLB::Node>("USER-1");
	user_1_cmp_1->add_child(std::make_unique<XMLB::Node>("NAME", "Vasiliy"));
	user_1_cmp_1->add_child(std::make_unique<XMLB::Node>("JOB", "HR"));
	user_1_cmp_1->add_child(std::make_unique<XMLB::Node>("AGE", "32"));

	auto user_2_cmp_1 = std::make_unique<XMLB::Node>("USER-2");
	user_2_cmp_1->add_child(std::make_unique<XMLB::Node>("NAME", "Konstantin"));
	user_2_cmp_1->add_child(std::make_unique<XMLB::Node>("JOB", "HR"));
	user_2_cmp_1->add_child(std::make_unique<XMLB::Node>("AGE", "18"));

	auto user_3_cmp_1 = std::make_unique<XMLB::Node>("USER-3");
	user_3_cmp_1->add_child(std::make_unique<XMLB::Node>("NAME", "Vladislav"));
	user_3_cmp_1->add_child(std::make_unique<XMLB::Node>("JOB", "HR"));
	user_3_cmp_1->add_child(std::make_unique<XMLB::Node>("AGE", "65"));

	cmp_1->add_child(std::move(user_1_cmp_1));
	cmp_1->add_child(std::move(user_2_cmp_1));
	cmp_1->add_child(std::move(user_3_cmp_1));

	cmp_1->add_attribute(XMLB::Node_attribute{ "id", "0" });
	cmp_1->add_attribute(XMLB::Node_attribute{ "cash", "3000" });



	auto user_1_cmp_2 = std::make_unique<XMLB::Node>("USER-4");
	user_1_cmp_2->add_child(std::make_unique<XMLB::Node>("NAME", "Jon"));
	user_1_cmp_2->add_child(std::make_unique<XMLB::Node>("JOB", "ECONOMY"));
	user_1_cmp_2->add_child(std::make_unique<XMLB::Node>("AGE", "52"));

	auto user_2_cmp_2 = std::make_unique<XMLB::Node>("USER-5");
	user_2_cmp_2->add_child(std::make_unique<XMLB::Node>("NAME", "Bred"));
	user_2_cmp_2->add_child(std::make_unique<XMLB::Node>("JOB", "ECONOMY"));
	user_2_cmp_2->add_child(std::make_unique<XMLB::Node>("AGE", "97"));

	auto user_3_cmp_2 = std::make_unique<XMLB::Node>("USER-6");
	user_3_cmp_2->add_child(std::make_unique<XMLB::Node>("NAME", "Loly"));
	user_3_cmp_2->add_child(std::make_unique<XMLB::Node>("JOB", "ECONOMY"));
	user_3_cmp_2->add_child(std::make_unique<XMLB::Node>("AGE", "16"));

	cmp_2->add_child(std::move(user_1_cmp_2));
	cmp_2->add_child(std::move(user_2_cmp_2));
	cmp_2->add_child(std::move(user_3_cmp_2));

	cmp_2->add_attribute(XMLB::Node_attribute{ "id", "1" });
	cmp_2->add_attribute(XMLB::Node_attribute{ "cash", "9000" });



	auto user_1_cmp_3 = std::make_unique<XMLB::Node>("USER-7");
	user_1_cmp_3->add_child(std::make_unique<XMLB::Node>("NAME", "Western"));
	user_1_cmp_3->add_child(std::make_unique<XMLB::Node>("JOB", "TASKS"));
	user_1_cmp_3->add_child(std::make_unique<XMLB::Node>("AGE", "228"));

	auto user_2_cmp_3 = std::make_unique<XMLB::Node>("USER-8");
	user_2_cmp_3->add_child(std::make_unique<XMLB::Node>("NAME", "Kolya"));
	user_2_cmp_3->add_child(std::make_unique<XMLB::Node>("JOB", "TASKS"));
	user_2_cmp_3->add_child(std::make_unique<XMLB::Node>("AGE", "45"));

	auto user_3_cmp_3 = std::make_unique<XMLB::Node>("USER-9");
	user_3_cmp_3->add_child(std::make_unique<XMLB::Node>("NAME", "Aleks"));
	user_3_cmp_3->add_child(std::make_unique<XMLB::Node>("JOB", "TASKS"));
	user_3_cmp_3->add_child(std::make_unique<XMLB::Node>("AGE", "76"));

	cmp_3->add_child(std::move(user_1_cmp_3));
	cmp_3->add_child(std::move(user_2_cmp_3));
	cmp_3->add_child(std::move(user_3_cmp_3));

	cmp_3->add_attribute(XMLB::Node_attribute{ "id", "2" });
	cmp_3->add_attribute(XMLB::Node_attribute{ "cash", "332" });

	company->add_child(std::move(cmp_1));
	company->add_child(std::move(cmp_2));
	company->add_child(std::move(cmp_3));

	doc.set_parent(std::move(company));

	doc.save_to_file("test_1.txt");


*/