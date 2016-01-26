#ifndef _DomBuilder_TREENODE_H_
#define _DomBuilder_TREENODE_H_

#include <string>
#include <map>
#include "Node.h"
#include <iostream>

#define NODE_WEIGHT // for Constor2.0 by LiQiang: 是否考虑节点权重

using namespace std;

class ParserNode;

namespace DomBuilder
{
	/************************************************************************/
	/* for Indilator by Faneshion(fanyixing@software.ict.ac.cn)             */
	/************************************************************************/
	enum BlockType // 块类型 
	{
		NOTBLOCK,
		NULLBLOCK,
		LINKBLOCK,
		INFOBLOCK,
		EMPTYBLOCK,
		OTHERBLOCK
	};
	// ----------end of "for Indilator by Faneshion------------
	namespace HTML
	{
		class TreeNode : public Node
		{
		public:
			TreeNode(const string &strText, bool aIsContainer = false, const bool aIsComment = false)
			{
				// Dad comes first!
				TreeNode();
				text(strText);
				closingText(strText);
				offset(0);
				length(0);
				isTag(true);
				isComment(aIsComment);
				tagName(extractTagName(strText));

				// It's my time!
				isContainer(aIsContainer);
				attributesCount(0);
			}

			TreeNode()
			{
				// Dad comes first
				Node();
				offset(0);
				length(0);
				isTag(true);
				isComment(false);
				isContainer(false);
				attributesCount(0);
#ifdef NODE_WEIGHT
				this->char_length = 0.0;
#else
				this->char_length = 0;
#endif

				this->tag_number = 0;
				this->density = 0.0;
				this->density_sum = 0.0;
				this->node_weight = 1.0;
				this->mark = 0;
				this->is_max_density_sum_node = true;

				this->m_AdjXpathDis = 0;
				this->m_bBlockTag = NULLBLOCK;
			}

			TreeNode(const TreeNode& ori)
			{
				//std::cout << "fan " << endl;
				if (this != &ori)
				{
					offset(ori.offset());
					length(ori.length());
					isTag(ori.isTag());
					isComment(ori.isComment());
					isContainer(ori.isContainer());
					this->char_length = ori.char_length;
					this->tag_number = ori.tag_number;
					this->density = ori.density;
					this->density_sum = ori.density_sum;
					this->node_weight = ori.node_weight;
					this->mark = ori.mark;
					this->is_max_density_sum_node = ori.is_max_density_sum_node;
					text(ori.text());

					this->m_AdjXpathDis = ori.m_AdjXpathDis;
					this->m_bBlockTag = ori.m_bBlockTag;

					this->mAttributes.clear();
					std::map<std::string, std::string>::const_iterator i = ori.mAttributes.begin();
					while (i != ori.mAttributes.end()) {
						this->mAttributes.insert(make_pair(i->first, i->second));
						i++;
					}
					if (mAttributes.size() > 0)
					{
						updateMText();
					}
				}
			}

			bool operator==(const TreeNode& ori)const
			{
				if (this->tagName() != ori.tagName())
				{
					return false;
				}
				if (this->attributesCount() != ori.attributesCount())
				{
					return false;
				}
				return true;
			}

			TreeNode& operator=(const TreeNode& ori)
			{
				if (this != &ori)
				{
					offset(ori.offset());
					length(ori.length());
					isTag(ori.isTag());
					isComment(ori.isComment());
					isContainer(ori.isContainer());
					this->char_length = ori.char_length;
					this->tag_number = ori.tag_number;
					this->density = ori.density;
					this->density_sum = ori.density_sum;
					this->node_weight = ori.node_weight;
					this->mark = ori.mark;
					this->is_max_density_sum_node = ori.is_max_density_sum_node;
					text(ori.text());

					this->m_AdjXpathDis = ori.m_AdjXpathDis;
					this->m_bBlockTag = ori.m_bBlockTag;

					this->mAttributes.clear();
					std::map<std::string, std::string>::const_iterator i = ori.mAttributes.begin();
					while (i != ori.mAttributes.end()) {
						this->mAttributes.insert(make_pair(i->first, i->second));
						i++;
					}
					if (mAttributes.size() > 0)
					{
						updateMText();
					}
				}
				return *this;
			}

			~TreeNode()
			{
				this->mAttributes.clear();
			}

			// override some unsuitable behaviors of dad
			// new times need more functions
			inline void text(const std::string& text)
			{
				this->mText = text;
				tagName(extractTagName(text));
				if (isContainer())
				{
					closingText("</" + tagName() + ">");
				}
				else
				{
					closingText("");
				}
			}
			inline const std::string& text() const { return this->mText; }

			const bool isContainer() const
			{
				return this->mIsContainer;
			}
			void isContainer(const bool bContainer)
			{
				this->mIsContainer = bContainer;
				if (bContainer)
				{
					isTag(bContainer);
					closingText("</" + tagName() + ">");
				}
				else
				{
					closingText("");
				}
			}

			inline void attributesCount(const int mAttrCount)
			{
				this->mAttrCount = mAttrCount;
			}
			inline const int attributesCount() const
			{
				return this->mAttrCount;
			}

			void parseAttributes()
			{
				return;
			}

			std::pair<bool, std::string> attribute(const std::string &attr) const
			{
				std::map<std::string, std::string>::const_iterator i = this->mAttributes.find(attr);
				if (i != this->mAttributes.end()) {
					return make_pair(true, i->second);
				}
				else {
					return make_pair(false, std::string());
				}
			}

			const std::map<std::string, std::string>& attributes() const
			{
				return this->mAttributes;
			}


			void addAttribute(string key, string value)
			{
				mAttributes.insert(make_pair(key, value));
				// Instead parsing attribute from mText, update the mText when attributes change!
				updateMText();
				mAttrCount++;
			}

			void setAttribute(const std::string &key, const std::string &value)
			{
				std::map<std::string, std::string>::const_iterator i = this->mAttributes.find(key);
				if (i != this->mAttributes.end()) {
					this->mAttributes[key] = value;
				}
				else {
					return;
				}
				// Instead parsing attribute from mText, update the mText when attributes change!
				updateMText();
			}

			bool removeAttribute(const std::string& attrKey)
			{
				if (attrKey.empty())
				{
					return true;
				}
				std::map<std::string, std::string>::iterator i = this->mAttributes.find(attrKey);
				if (i != this->mAttributes.end()) {
					this->mAttributes.erase(i);
					mAttrCount--;
					updateMText();
					return true;
				}
				return false;
			}

		private:
			inline string extractTagName(const string& strText)
			{
				string tagName("");
				size_t beginPos = string::npos;
				size_t endPos = string::npos;
				beginPos = strText.find_first_of("<");
				if (string::npos == beginPos || '!' == strText[beginPos + 1]) // Lose her, lose the world!
				{
					tagName = strText;
					return tagName;
				}
				beginPos = (string::npos == beginPos) ? 0 : beginPos;
				endPos = strText.find_first_of(" ");
				endPos = (string::npos == endPos) ? strText.find_first_of(">") : endPos;
				tagName = (string::npos == endPos) ? strText.substr(beginPos) : strText.substr(beginPos + 1, endPos - beginPos - 1);
				return tagName;
			}

			inline void updateMText()
			{
				if (!isTag() && !isComment())
				{
					return;
				}
				mText.clear();
				mText = "<" + tagName();
				std::map<std::string, std::string>::const_iterator iterBegin = this->mAttributes.begin();
				std::map<std::string, std::string>::const_iterator iterEnd = this->mAttributes.end();
				this->mAttrCount = 0;
				while (iterBegin != iterEnd)
				{
					mText += " " + iterBegin->first + "=\"" + iterBegin->second + "\"";
					iterBegin++;
					mAttrCount++;
				}
				mText += ">";
			}

		private:
			bool mIsContainer;
			int mAttrCount;

			/************************************************************************/
			/* for Constor2.0 by Liqiang(liqiang@software.ict.ac.cn                 */
			/************************************************************************/
#ifdef NODE_WEIGHT
		public:
			inline void SetCharLength(double char_length) { this->char_length = char_length; }
		public:
			inline double GetCharLength() { return this->char_length; }
		public:
			inline void SetLinkCharLength(double LinkChar_Length) { this->LinkChar_length = LinkChar_Length; }
		public:
			inline double GetLinkCharLength() { return this->LinkChar_length; }
#else
		public:
			inline void SetCharLength(int char_length){this->char_length = char_length;}
			inline int GetCharLength(){return this->char_length;}
			inline void SetLinkCharLength(int LinkChar_length){this->LinkChar_length = LinkChar_length;}
			inline int GetLinkCharLength(){return this->LinkChar_length;}
#endif
		public:
			inline void SetTagNumber(int tag_number) { this->tag_number = tag_number; }
		public:
			inline void SetDensity(double density) { this->density = density; }
		public:
			inline void SetNodeWeight(double node_weight) { this->node_weight = node_weight; }
		public:
			inline void SetDensitySum(double density_sum) { this->density_sum = density_sum; }
		public:
			inline void SetMaxDensitySumMark(bool changes){ this->is_max_density_sum_node = changes; }
		public:
			inline void SetMark(int mark){ this->mark = mark; }
		public:
			inline void SetLinkTagNumber(int LinkTag_Number){ this->LinkTag_number = LinkTag_Number; }
		public:
			inline int GetTagNumber(){ return this->tag_number; }
		public:
			inline int GetLinkTagNumber(){ return this->LinkTag_number; }
		public:
			inline double GetNodeWeight(){ return this->node_weight; }
		public:
			inline double GetDensity(){ return this->density; }
		public:
			inline double GetDensitySum(){ return this->density_sum; }
		public:
			inline bool IsMaxDensitySumNode(){ return this->is_max_density_sum_node; }
		public:
			inline int GetMark(){ return this->mark; }

		public:
#ifdef NODE_WEIGHT
			double char_length;
			double LinkChar_length;
#else
			int char_length;
			int LinkChar_length;
#endif
			int tag_number;
			int LinkTag_number;
			double density;
			double density_sum;
			double node_weight;
			int line;
			bool is_max_density_sum_node; // 是否为density sum最大的节点
			int mark; // 1 for content, 0 for noise, 2 for both
			// -------------end of "for Constor2.0 by Liqiang--------------------

			/************************************************************************/
			/* for Indilator by Faneshion(fanyixing@software.ict.ac.cn)             */
			/************************************************************************/
		public:
			int m_AdjXpathDis; /* 邻接节点的xpath的差值 */
			BlockType m_bBlockTag; /* 节点块类型 */
			// ----------end of "for Indilator by Faneshion------------

			/************************************************************************/
			/* for MASK by yujun(yujun@software.ict.ac.cn)                          */
			/************************************************************************/
		public:
			inline void attributes(const std::map<std::string, std::string>& m) // 重置属性值
			{
				this->mAttributes = m;
				updateMText();
			}
			// ----------end of "for MASK by yujun"--------------
		};
	}
}


#endif
