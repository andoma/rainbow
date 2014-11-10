// Copyright (c) 2010-14 Bifrost Entertainment AS and Tommy Nguyen
// Distributed under the MIT License.
// (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)

#ifndef COMMON_TREENODE_H_
#define COMMON_TREENODE_H_

#include <functional>

#include "Common/Vector.h"

/// A barebone implementation of a tree node.
///
/// This class does not hold any data and is meant to be sub-classed.
template<typename T>
class TreeNode : private NonCopyable<TreeNode<T>>
{
public:
	/// Adds a node as child.
	void add_child(T *);

	/// Removes node from the tree and deletes it.
	void remove();

	/// Removes a child node.
	void remove_child(T *);

	/// Recursively calls function \p f on \p node and its children.
	template<typename U, typename F, typename... Args>
	friend void for_each(U *node, F&& f, Args&&... args);

protected:
	T *parent_;            ///< This node's parent.
	Vector<T*> children_;  ///< This node's children.

	TreeNode();
	~TreeNode();
};

template<typename T>
void TreeNode<T>::add_child(T *node)
{
	if (node->parent_)
		node->parent_->children_.remove(node);
	node->parent_ = static_cast<T*>(this);
	children_.push_back(node);
}

template<typename T>
void TreeNode<T>::remove()
{
	parent_->remove_child(static_cast<T*>(this));
}

template<typename T>
void TreeNode<T>::remove_child(T *node)
{
	if (!node)
		return;
	children_.remove(node);
	delete node;
}

template<typename T>
TreeNode<T>::TreeNode() : parent_(nullptr)
{
	static_assert(std::is_base_of<TreeNode, T>::value,
	              "T must be a subclass of TreeNode");
}

template<typename T>
TreeNode<T>::~TreeNode()
{
	for (auto child : children_)
		delete child;
}

template<typename T, typename F, typename... Args>
void for_each(T *node, F&& f, Args&&... args)
{
	using U = typename std::decay<T>::type;
	static_assert(std::is_base_of<TreeNode<U>, U>::value,
	              "T must be a subclass of TreeNode");
	static_assert(std::is_convertible<F, std::function<void(T*, Args&&...)>>::value,
	              "function type void(T*, Args&&...) required");

	f(node, std::forward<Args>(args)...);
	for (auto child : node->children_)
		for_each(child, f, std::forward<Args>(args)...);
}

#endif
