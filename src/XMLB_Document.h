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

#include <string>
#include <locale>
#include <memory>

#include "XMLB_Node.h"
#include "XMLB_Node_iterator_impl.h"

namespace XMLB
{
	/**************************************************************************
	* @brief XML ��������, ���������
	*
	* @details 
	*
	* @todo ����� ��������, ��� ����������� ������ ��������, std::swap �
	* move ���������
	**************************************************************************/
	class Document final
	{
	public:
		using iterator = Node_iterator_impl<Node>;
		using const_iterator = Node_iterator_impl<const Node>;
		using Ptr = std::unique_ptr<Document>;

		Document(const std::locale& file_locale, float version);

		Document(const Document& doc);
		Document& operator=(const XMLB::Document& doc);

		Document(Document&& doc) noexcept;
		Document& operator=(Document&& doc) noexcept;

		~Document() = default;

		void set_general_node(const Node& node);
		void set_general_node(Node&& node);
		void set_general_node(Node::Ptr node);
		const Node& get_general_node() const & noexcept;

		void set_file_locale(const std::locale& file_locale);
		const std::locale& get_file_locale() const & noexcept;

		void set_version(float version) noexcept;
		float get_version() const noexcept;

		void clear() noexcept;

		iterator begin() noexcept;
		iterator end() noexcept;

		const_iterator begin() const noexcept;
		const_iterator end() const noexcept;

		const_iterator cbegin() const noexcept;
		const_iterator cend() const noexcept;

		void swap(Document& doc) noexcept;

	private:
		Node::Ptr m_parent;
		std::locale m_locale;
		float m_version;
	};

	//*************************************************************************



	/**************************************************************************
	*								FUNCTIONS
	**************************************************************************/

	bool save_to_file(const Document& doc, const std::string& file_name);

	Document::Ptr load_from_file(const std::string& file_name);

	//*************************************************************************

} // namespace XMLB

namespace std
{
	template<>
	inline void swap<XMLB::Document>(XMLB::Document& lhs, XMLB::Document& rhs) 
		noexcept
	{
		lhs.swap(rhs);
	}

} // namespace std