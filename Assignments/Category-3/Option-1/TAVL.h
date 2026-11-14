#pragma once
#include "TTreeBase.h"
#include <cstdint>
#include <sstream>
#include <stdexcept>
#include <iostream>

/**
@brief AVL-tree
@note Could have inherited from TBST, but there where issues with that so I didn't.
@warning The rotation function change what is the root node, workaround is the GetRoot function.
@tparam T Data that is stored in AVL-tree, Needs to be a pointer type.
@tparam K Datatype used for key, must support comparison with <,>,==,!=.
*/
template <typename K, typename T>
class TAVL: public TTreeBase<TAVL<K,T>, K, T> {
private:
	uint16_t leftSubtreeHeight = 0;
	uint16_t rightSubtreeHeight = 0;
	int GetBalance();
	uint16_t GetHeight();

	/**
	@brief Checks if AVL is balanced and calls correct balance function if nessecary. Propegates upwards with recursive calls.
	*/
	void Balance();

	void RotateRight();
	void RotateLeft();

	/**
	@brief Replace the left or right node with a new one.
	@param old Pointer to either right or left node of this.
	@param _new Pointer that should replace old.
	@returns false if old didn't match with the address of left or right.
	*/
	bool ReplaceWith(TAVL* old, TAVL* _new);

	/**
	@brief To be called by a child to update its parents height.

	This will recurse up the tree to update height for all nodes above.
	@param child Pointer to the child node
	@param height The height of the child
	*/
	void HeightOfChildIs(TAVL* child, uint16_t height);

	/**
	@brief Check the height downward in the tree.
	*/
	uint16_t RecalculateHeight();

	/**
	@brief Sets up the node

	Some features in the constructor required the parent node to know the address of its child.
	Because child nodes usually were set up in this form: `left = new TAVL`.
	The parent would not know the address of the child until after the constructor is done.
	*/
	void Initialize();

public:
	/**
	@param key The value to use in the tree
	@param parent Pointer to parent. Defaults to nullptr.
	@note You should run Initialize() afterwards.
	*/
	TAVL(K key, TAVL* parent = nullptr) : TTreeBase<TAVL<K,T>, K, T>(key, parent) {};

	TAVL(K key, T data, TAVL* parent = nullptr) : TTreeBase<TAVL<K,T>, K, T>(key, data, parent) {};

	/**
	@returns false if a node with a value same as the key exists.
	@todo Return the address of the new root node.
	*/
	bool Insert(K key, T data);

	/**
	@brief Generates a stringstream with code for a plantuml object diagram of the BST
	*/
	std::stringstream Plantuml();
};

template<typename K, typename T>
inline int TAVL<K, T>::GetBalance()
{
	return rightSubtreeHeight - leftSubtreeHeight;
}

template<typename K, typename T>
uint16_t TAVL<K, T>::GetHeight()
{
	if (leftSubtreeHeight > rightSubtreeHeight) {
		return leftSubtreeHeight;
	}
	return rightSubtreeHeight;
}

template<typename K, typename T>
bool TAVL<K, T>::ReplaceWith(TAVL* old, TAVL* _new)
{
	if (this->left == old) { this->left = _new; return true; }
	if (this->right == old) { this->right = _new; return true; }
	return false;
}

template<typename K, typename T>
void TAVL<K, T>::Balance()
{
	if (GetBalance() <= -2) {
		if (this->left != nullptr && this->left->GetBalance() >= 1) {
			//std::cout << "LR rotate ";
			this->left->RotateLeft();
			RotateRight();
		}
		else {
			//std::cout << "R roate ";
			RotateRight();
		}

		if (this->parent != nullptr)
		{
			this->parent->RecalculateHeight();
			this->parent->HeightOfChildIs(this, GetHeight());
		}
		else
		{
			RecalculateHeight();
		}
	}
	if (GetBalance() >= 2) {
		if (this->right->GetBalance() <= -1) {
			//std::cout << "RL roatate ";
			this->right->RotateRight();
			RotateLeft();
		}
		else
		{
			//std::cout << "LL rotate ";
			RotateLeft();
		}
		
		if (this->parent != nullptr)
		{
			this->parent->RecalculateHeight();
			this->parent->HeightOfChildIs(this, GetHeight());
		}
		else
		{
			RecalculateHeight();
		}
	}
	if (this->parent != nullptr)
		this->parent->Balance();
}

template<typename K, typename T>
void TAVL<K, T>::RotateRight()
{
	/*
	Start:
	  P
	  |
	  c
	 / \
	a
   / \
	  b
	End:
	P
	|
	a
   / \
	  c
	 / \
	b
	*/
	auto P = this->parent;
	auto c = this;
	auto a = this->left;
	// if a is a nullptr somethis has gone horibly wrong.
	auto b = this->left->right;
	
	a->right = c;
	c->parent = a;

	c->left = b;
	if (b!=nullptr)
		b->parent = c;

	if (P!=nullptr)
		P->ReplaceWith(c, a);
	a->parent = P;
}

template<typename K, typename T>
void TAVL<K, T>::RotateLeft()
{
	/*
	Start:
	P
	|
	a
   / \
	  c
	 / \
	b
	End:
	  P
	  |
	  c
	 / \
	a
   / \
	  b
	*/
	auto a = this;
	auto c = this->right;
	//If c is a nullptr some thing is wrong
	auto b = c->left;
	auto P = this->parent;

	if (P!= nullptr)
		P->ReplaceWith(a, c);
	c->parent = P;

	c->left = a;
	a->parent = c;

	a->right = b;
	if(b!=nullptr)
		b->parent = a;
}

template<typename K, typename T>
void TAVL<K, T>::HeightOfChildIs(TAVL<K, T>* child, uint16_t height)
{
	if (child == this->right) {
			rightSubtreeHeight = height+1;
	}
	else if (child == this->left) {
			leftSubtreeHeight = height+1;
	}
	else {
		throw std::logic_error("child param is not a pointer to a child of this TAVL.");
	}
	
	if (this->parent != nullptr) {
		return this->parent->HeightOfChildIs(this, GetHeight());
	}
}

template<typename K, typename T>
uint16_t TAVL<K, T>::RecalculateHeight()
{
	if (this->right == nullptr)
		rightSubtreeHeight = 0;
	else
		rightSubtreeHeight = this->right->RecalculateHeight()+1;

	if (this->left == nullptr)
		leftSubtreeHeight = 0;
	else
		leftSubtreeHeight = this->left->RecalculateHeight()+1;

	return GetHeight();
}

template<typename K, typename T>
void TAVL<K, T>::Initialize()
{
	if (this->parent != nullptr) {
		this->parent->HeightOfChildIs(this, GetHeight());
	}
	Balance();
}

template<typename K, typename T>
bool TAVL<K, T>::Insert(K key, T data)
{
	if (this->content == nullptr) {
		Initialize();
		this->content = data;
		return true;
	}
	else if (key > this->value) {
		if (this->right == nullptr) {
			this->right = new TAVL<K, T>(key, this);
		}
		return this->right->Insert(key, data);
	}
	else if (key < this->value) {
		if (this->left == nullptr) {
			this->left = new TAVL<K, T>(key,this);
		}
		return this->left->Insert(key, data);
	}
	//throw std::logic_error("This condition should not be reached!");
	return false;
}

template<typename K, typename T>
std::stringstream TAVL<K, T>::Plantuml()
{
	auto s = std::stringstream();
	s << "object " << this << " {\nvalue: int = " << this->value << "\ncontent: T = " << *this->content << "\nheight = " << this->GetHeight() << "\nbalance factor = " << this->GetBalance() <<  "\n}\n";
	if (this->left != nullptr) {
		s << this->left->Plantuml().str();
		s << this << " --> " << this->left << ": left\n";
	}
	if (this->right != nullptr) {
		s << (this->right->Plantuml().str());
		s << this << " --> " << this->right << ": right\n";
	}
	return s;
}