#pragma once
#include "TTreeBase.h"
#include <cstdint>
#include <sstream>
#include <stdexcept>
#include <iostream>

/** 
* @brief Binary search tree.
* @tparam T Data that is stored in BST, Needs to be a pointer type.
* @tparam K Datatype used for key, must support comparison with <,>,==.
* @bug Does not handle comparisons between std::strings that contain ���.
*/
template <typename K, typename T>
class TBST: public TTreeBase<TBST<K,T>, K, T> {
public:
	/**
	@param key The value to use in the tree
	@param parent Pointer to parent. Defaults to nullptr.
	*/
	explicit TBST(K key, TBST* parent = nullptr) : TTreeBase<TBST<K,T>,K,T>(key, parent) {};
	explicit TBST(K key, T data, TBST* parent = nullptr) : TTreeBase<TBST<K,T>,K,T>(key, data, parent) {};

	~TBST();

	/**
	@brief Replace the left or right node with a new one.
	@param old Pointer to either right or left node of this.
	@param _new Pointer that should replace old.
	@returns false if old didn't match with the address of left or right.
	*/
	bool ReplaceWith(TBST* old, TBST* _new);

	/**
	@returns false if a node with a value same as the key exists.
	*/
	bool Insert(K key, T data);
	bool Insert(TBST*);

	/**
	@brief Delete content in node with a value same as key.
	@note This does not delete the TBST object itself, just the content that it points to.
	@param key Key to search for.
	*/
	void Delete(K key);

	T Search(K key);

	/**
	@brief Generates a stringstream with code for a plantuml object diagram of the BST
	*/
	std::stringstream Plantuml();
};

#include <sstream>

template<typename K, typename T>
inline TBST<K,T>::~TBST()
{
	//std::cout << "TBST Destructor.\n";
	if (this->right == nullptr) {
		if (this->left == nullptr) {
			// left and right is empty
			if (this->parent != nullptr)
				this->parent->ReplaceWith(this, nullptr);
			return;
		}
		// left is still there
		if (this->parent != nullptr)
			this->parent->ReplaceWith(this, this->left);
		this->left->parent = this->parent;
		return;
	}
	if (this->left == nullptr) {
		// right is still there
		if(this->parent != nullptr)
			this->parent->ReplaceWith(this, this->right);
		this->right->parent = this->parent;
		return;
	}
	// both are here
	// We're just replacing this with left because we need to replace it with something.
	if(this->parent!=nullptr)
		this->parent->ReplaceWith(this, this->left);
	this->left->parent = this->parent;
	this->left->Insert(this->right);
	return;
}

template<typename K, typename T>
bool TBST<K,T>::ReplaceWith(TBST* old, TBST* _new)
{
	if (this->left == old) { this->left = _new; return true; }
	if (this->right == old) { this->right = _new; return true; }
	std::stringstream s;
	s << old << " is not a child of " << this;
	throw std::logic_error(s.str());
	return false;
}

template<typename K, typename T>
bool TBST<K,T>::Insert(K key, T data)
{
	if (this->content == nullptr) {
		this->content = data;
		return true;
	}
	else if (key > this->value) {
		if (this->right == nullptr) {
			this->right = new TBST<K,T>(key,this);
		}
		return this->right->Insert(key, data);
	}
	else if (key < this->value) {
		if (this->left == nullptr) {
			this->left = new TBST<K,T>(key,this);
		}
		return this->left->Insert(key, data);
	}
	//std::logic_error("This condition should not be reached!");
	return false;
}

template<typename K, typename T>
bool TBST<K,T>::Insert(TBST* bst)
{
	auto key = bst->value;
	if (key > this->value) {
		if (this->right == nullptr) {
			this->right = bst;
			bst->parent = this;
			return true;
		}
		return this->right->Insert(bst);
	}
	else if (key < this->value) {
		if (this->left == nullptr) {
			this->left = bst;
			bst->parent = this;
			return true;
		}
		return this->left->Insert(bst);
	}
	//std::logic_error("This condition should not be reached!");
	return false;
}

template<typename K, typename T>
void TBST<K,T>::Delete(K key)
{
	if (key == this->value) {
		delete this->content;
		return;
	}
	else if (key > this->value) {
		return this->right->Delete(key);
	}
	else if (key < this->value) {
		return this->left->Delete(key);
	}
}

template<typename K, typename T>
T TBST<K,T>::Search(K key)
{
	if (key == this->value) {
		return this->content;
	}
	else if (key > this->value) {
		if (this->right == nullptr) { return nullptr; }
		return this->right->Search(key);
	}
	else if (key < this->value) {
		if (this->left == nullptr) { return nullptr; }
		return this->left->Search(key);
	}
	return nullptr;
}

template<typename K, typename T>
std::stringstream TBST<K,T>::Plantuml()
{
	auto s = std::stringstream();
	s << "object " << this << " {\nvalue: K = " << this->value << "\ncontent: T = " << *this->content <<"\nleft: TBST =" << this->left << "\nright:TBST = " << this->right << "\n}\n";
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