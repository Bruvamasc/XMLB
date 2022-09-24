/******************************************************************************
* @file
* ������ ���� �������� �������� ������������ �������(����) XML ���������
*
* @author Bruvamasc
* @date   2022-08-25
*
* @todo �������
* ///< ���������, ��� ������� ���������� ��� �������������
*
******************************************************************************/

#ifndef XMLB_PARSER_H
#define XMLB_PARSER_H

#include <string>
#include <regex>
#include <queue>
#include <stack>

#include "XMLB_Node.h"
#include "XMLB_Constants.h"
#include "XMLB_Utility.h"

namespace XMLB
{
	template<typename Iter>
	struct Tag_range
	{
		Iter first;
		Iter last;
		Node::Ptr node;
	};



	/**************************************************************************
	* @brief ������ ������� ������������ ������������������ ����� � XML
	* ���������
	*
	* @param[in] first - ������ ������������������(����������)
	* @param[in] last - ����� ������������������(����������)
	*
	* @return ����� ��������� � XML ����� � ���� ��������� ������, ���� ���
	* ����
	**************************************************************************/
	template<typename Iter>
	Node::Ptr parse_to_node(Iter first, Iter last)
	{
		using Tag = Tag_range<Iter>;

		Node::Ptr result{ nullptr };
		bool is_container_correct = true;

		std::stack<Tag> tags;
		Tag* last_parent = nullptr;

		std::smatch tag_match;
		std::smatch open_match;
		std::smatch close_match;
		std::smatch single_match;

		//������� ��� ����. � ������, ���� ���� ��������, �� ������ �������
		//���������. ��� �� ���������. ���� ��� ��������� �� ���������
		//��������, ��� �� ���������� ���� ��������, ��� ���� ������ ��������
		//��� ����������, �� ��������� ���� � ��������� ������ �� ������������
		for (; first != last; ++first)
		{
			//���������, �������� �� ������� ������ �����. ���� �� ��������, ��
			//������� �� �����
			if (!std::regex_search(*first, tag_match, kTag_Element_template))
			{
				is_container_correct = false;
				break;
			}

			//���������, �������� �� ������� �������� ������ ���������
			//��������� ���������� ��������. ���� ��, �� �������������
			//������ ���������� ��������.
			if (last_parent && first == last_parent->last)
			{
				last_parent = nullptr;

				if (tags.size() > 1)
				{
					auto last_tag = std::move(tags.top().node);
					tags.pop();
					tags.top().node->add_child(std::move(last_tag));
					last_parent = &tags.top();
				}
				else if (tags.size() == 1)
				{
					result = std::move(tags.top().node);
					tags.pop();
				}
			}

			bool is_one_tag = false;
			bool is_single_tag = false;

			//���������, ���� �� �������� ��� � ������� ������� � ������ ��
			//�� ������� �������� ���. ���� ����� ��� � ���� ������ ������, ��
			//���� ����������� ���
			if (std::regex_search(*first, open_match, kOpen_tag_template))
			{
				//���������, �������� �� ��� ��������� �����
				if (std::regex_search(*first,
					single_match,
					kSingle_tag_template))
				{
					is_single_tag = true;
				}
				else
				{
					is_one_tag = true;
				}
			}

			//���� ������ �������� ��� � �� �� ���������, �� ���� ����������� ���
			if (is_one_tag)
			{
				auto tag_iter = first;

				//���� ��������������� �������� ���
				for (; tag_iter != last; ++tag_iter)
				{
					if (std::regex_search(*tag_iter,
						close_match,
						kClose_tag_template) &&
						open_match[1] == close_match[1])
					{
						break;
					}
				}

				//���� �������������� �������� ��� �� ��� ������, �� ������� ��
				//�����
				if (tag_iter == last)
				{
					is_container_correct = false;
					break;
				}

				//������� ���������� ����� ������� � ������ ��������� ����.
				//���� �� ������ 1, �� ������������ ���� ���, ��������� � ����
				//�����, � �������� ��������� ���������� �������� �� ���� ���
				if (std::distance(first, tag_iter) > 1)
				{
					tags.push(
						Tag{ first,
						tag_iter,
						create_node(open_match[1], *first) }
					);

					//���� ��������� �������� ����, �� ������������� ���
					//��������� ���������� ���� �� ����� tags
					if (last_parent)
					{
						tags.top().node->set_parent(&(*(last_parent->node)));
					}

					//�������� ��������� �� ���������� ��������
					last_parent = &tags.top();
				}
				//� ��������� ������, ������� ����, ��� ��� ���������� � ����
				//�����
				else
				{
					if (last_parent)
					{
						last_parent->node->add_child
						(
							std::move(create_node(open_match[1], *first))
						);
					}
				}
			}
			//���� �� ��� ��������� ���
			else if (is_single_tag)
			{
				//���� ���������� �������� ���, ������ ��� ������ �
				//������������ ��� � ���������. �������� result � ���������
				//����
				if (!last_parent)
				{
					result = std::move(create_node(open_match[1], *first));
					break;
				}
				//� ��������� ������, ������� ����, ��� ��� ���������� � ����
				//�����
				else
				{
					last_parent->node->add_child
					(
						std::move(create_node(open_match[1], *first))
					);
				}
			}
		}

		if (!is_container_correct)
		{
			result.reset(nullptr);
		}

		return result;
	}
}

#endif // !XMLB_PARSER_H

//
////���������, ���� �� ��������� ���
//			else if (std::regex_search(*first,
//			single_match,
//			kSingle_tag_template))
//			{
//			//���� ���������� �������� ���, ������ ��� ������ �
//			//������������ ��� � ���������. �������� result � ���������
//			//����
//			if (!last_parent)
//			{
//				result = std::move(create_node(single_match[1], *first));
//				break;
//			}
//			//� ��������� ������, ������� ����, ��� ��� ���������� � ����
//			//�����
//			else
//			{
//				last_parent->node->add_child
//				(
//					std::move(create_node(single_match[1], *first))
//				);
//			}
//			}