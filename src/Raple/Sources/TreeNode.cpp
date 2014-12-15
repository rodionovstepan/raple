//-------------------------------------
#include "../Headers/TreeNode.h"
//-------------------------------------

namespace Raple
{
	TreeNode::TreeNode(NodeType nodeType)
	{
		init(nodeType);
	}

	TreeNode::TreeNode(NodeType nodeType, const Token *token, TreeNode *left, TreeNode *right)
	{
		init(nodeType);

		SetToken(token);
		AddChild(left);
		AddChild(right);
	}

	TreeNode::~TreeNode()
	{
		if(_token != 0)
			delete(_token);

		// recursively deleting
		TreeNode *item = firstChild();
		if (item != 0)
		{
			while (item)
			{
				TreeNode *next = item->Next;

				delete item;
				item = next;
			}
		}
	}

	void TreeNode::init(NodeType nodeType)
	{
		Next = 0;
		Prev = 0;
		Parent = 0;

		_token = new Token();
		_token->Length = 0;
		_token->Position = 0;
		_token->Type = ttUnknown;

		_nodeType = nodeType;
	}

	void TreeNode::AddChild(TreeNode *const childNode)
	{
		if (childNode != 0)
		{
			childNode->Parent = this;
			childNode->Next = 0;

			if (lastChild() == 0)
				childNode->Prev = 0;
			else
			{
				childNode->Prev = lastChild();
				lastChild()->Next = childNode;
			}
		}
		
		_childs.push_back(childNode);
	}

	void TreeNode::SetToken(const Token *token)
	{
		_token->Length = token->Length;
		_token->Position = token->Position;
		_token->Type = token->Type;
	}

	void TreeNode::UpdateData(int position, int length, int row)
	{
		if(position != 0 || length != 0)
		{
			if(_token->Length == 0 && _token->Position == 0)
			{
				_token->Length = length;
				_token->Position = position;
			}
			else
			{
				if(length + position > _token->Length + _token->Position)
					_token->Length = position - _token->Position + length;
				else if(_token->Position > position)
				{
					_token->Position = position;
					_token->Length = _token->Position - position + _token->Length;
				}
			}
		}

		_token->Row = row;
	}
}