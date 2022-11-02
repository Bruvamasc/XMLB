#include <fstream>
#include <string>

#include "XMLB/XMLB.h"

//-----------------------------------------------------------------------------
// Parser functors, states. You can declaration your XMLB::detail::Parser, or
// XMLB::detail::Parser_data_controller. You can too modify default Parser use
// you functors, states. At this time, Parser store functors in vector so it
// is necessary that your functors be in the correct sequence. In the future, 
// this may change.
// 
// Вы можете определять свои XMLB::detail::Parser или 
// XMLB::detail::Parser_data_controller. Также вы можете модифицировать
// стандратны Parser используя ваши функторы, состояния. На текущий момент,
// Parser хранит все функторы в векторе, поэтому ваши функторы должны быть в
// правильной последовательности.
// 
// В этом примере, я заменю в стандартном парсере функтор, отвечающий за
// парсинг атрибутов тегов. Я создам один функтор, который будет игнорировать
// все атрибуты. Также можно собрать стандартный Parser используя только ваши 
// функторы
// 
// Пока что, это экспериментально, так как нужно подумать над заменой вектора,
// чтобы не думать о порядке и оставалась таже скорость
//-----------------------------------------------------------------------------

struct my_attribute_functor
{
	template<typename CharT, typename DecorT, typename DataT>
	XMLB::detail::Base_state operator()(CharT&& token, DataT& data,
		const DecorT& decorator) const
	{
		if (token == decorator.close_tag_symbol)
		{
			return XMLB::detail::Close_tag_state{};
		}
		else if (token == decorator.doc_info_last_symbol)
		{
			return XMLB::detail::Document_info_state{};
		}

		//Так как в XMLB библиотеке Base_state для имени атрибута равен 7, а
		//Base_state для значения атрибута равен 8. Так как я заменю этим
		//функтором целых два, мне нужно будет добавить функтор затычку. Для
		//это подойдет даже один из функторов, которые заменил мой текущий.
		//
		//Не обязательно создвать свой Base_state для текущего функтора. Можно
		//использовать стандартный.

		return XMLB::detail::Attribute_name_state{};
	}
};

struct my_doc_info_functor
{

};

//Для удобства, определяем функцию, которая создасти и модифицирует стандартный
//Parser. Данная функция будет работать только для итераторов категории
//input_iterator. Для forward_iterator или выше, нужна использовать CharT* в
//data_controller
template<typename CharT>
inline decltype(auto) create_my_parser()
{
	using namespace XMLB::detail;
	using decorator = default_decorator<CharT>;
	using data_contoller = default_data_controller<CharT>;

	//В стандартном Parser количество функторов равно 11
	unsigned int default_functor_count = 11;

	Parser<CharT, decorator, data_contoller> my_parser{ 
		default_functor_count };
	
	my_parser.add_functor(Start_functor{});							//0
	my_parser.add_functor(Open_tag_functor{});						//1
	my_parser.add_functor(Close_tag_functor{});						//2
	my_parser.add_functor(Last_tag_functor{});						//3
	my_parser.add_functor(Single_tag_functor{});					//4
	my_parser.add_functor(Name_tag_functor{});						//5
	my_parser.add_functor(Value_tag_functor{});						//6

	//Мой кастомный функторы
	my_parser.add_functor(my_attribute_functor{});					//7
	//Attribute_value_functor - в данном случае просто "заглушка"
	my_parser.add_functor(Attribute_value_functor{});				//8

	my_parser.add_functor(Comment_functor{});						//9
	my_parser.add_functor(Document_info_functor{});					//10

	return my_parser;

}


int main()
{
	//-------------------------------------------------------------------------
	// EXAMPLE 1. Load XML file with default settings and then save it to
	// new file for comparison
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
				XMLB::save_to(*doc, std::ostream_iterator<char>{out_file_1});

				out_file_1.close();
			}
		}
	}

	//-------------------------------------------------------------------------
	// EXAMPLE 2. Load XML file with custom parser and save it to new file
	//-------------------------------------------------------------------------

	std::string load_file_name_2{ "sample_load_1.xml" };
	std::string save_file_name_2{ "sample_save_2.xml" };

	std::ifstream in_file_2{ load_file_name_2 };
	in_file_2.unsetf(std::ios_base::skipws);

	if (in_file_2)
	{
		//Create custom parser
		auto custom_parser = create_my_parser<char>();

		auto doc = XMLB::load_from(std::istream_iterator<char>{in_file_2}, 
			std::istream_iterator<char>{}, 
			XMLB::detail::default_decorator<char>{},
			custom_parser);

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