#ifndef __TREE_NODE_H_INCLUDED__
#define __TREE_NODE_H_INCLUDED__


//-------------------------------------
#include "Token.h"
#include "NodeDefinition.h"
//-------------------------------------
#include <vector>
//-------------------------------------


namespace Raple
{
	class TreeNode
	{
		Token *_token;
		NodeType _nodeType;

		std::vector<TreeNode *> _childs;

	public:

		TreeNode(NodeType nodeType);
		TreeNode(NodeType nodeType, const Token *token, TreeNode *left, TreeNode *right);
		~TreeNode();

		//-------------------------
		// As a properties
		TreeNode *Next;
		TreeNode *Prev;
		TreeNode *Parent;

		void AddChild(TreeNode *const childNode);
		void SetToken(const Token *token);
		void UpdateData(int position, int length);

		Token *GetToken() const
		{
			return _token;
		}

		NodeType GetType() const
		{
			return _nodeType;
		}

		void SetType(NodeType nodeType)
		{
			_nodeType = nodeType;
		}

		unsigned int ChildCount() const
		{
			return _childs.size();
		}

		TreeNode *GetChild(unsigned int index) const
		{
			unsigned int count = ChildCount();

			if (count && index < count)
				return _childs[index];

			return (TreeNode *)0;
		}

	private:

		TreeNode *lastChild() const
		{
			if (ChildCount())
				return _childs.back();

			return (TreeNode *)0;
		}

		TreeNode *firstChild() const
		{
			if (ChildCount())
				return _childs.front();

			return (TreeNode *)0;
		}

		void init(NodeType nodeType);
	};
}


#endif //__TREE_NODE_H_INCLUDED__