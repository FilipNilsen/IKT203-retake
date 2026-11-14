#pragma once

/**
@tparam C Child class type.
*/
template <typename C, typename K, typename T>
class TTreeBase {
protected:
	explicit TTreeBase(K key, C* parent = nullptr);
	explicit TTreeBase(K key, T data, C* parent = nullptr);
	C* right = nullptr;
	C* left = nullptr;
	C* parent = nullptr;
	T content = nullptr;
	K value;
public:
	C* Right() { return right; }
	C* Left() { return left; }
	T GetContent() { return content; }
	K GetValue() { return value; }
	K GetKey() { return value; }
	void InOrderTraversal();
	void PostOrderTraversal();
	void PreOrderTraversal();

	/** @test Does this work with the AVL tree? */
	void RecursiveDelete();

	/**
	@returns Pointer to root node
	*/
	C* GetRoot();
};

template<typename C, typename K, typename T>
TTreeBase<C, K, T>::TTreeBase(K key, C* parent)
{
	this->value = key;
	this->parent = parent;
}

template<typename C, typename K, typename T>
TTreeBase<C, K, T>::TTreeBase(K key, T data, C* parent)
{
	this->value = key;
	this->parent = parent;
	this->content = data;
}

template<typename C,typename K, typename T>
void TTreeBase<C,K, T>::InOrderTraversal()
{
	if (left != nullptr)
		left->InOrderTraversal();
	std::cout << *content << std::endl;
	if (right != nullptr)
		right->InOrderTraversal();
}

template<typename C, typename K, typename T>
void TTreeBase<C,K, T>::PostOrderTraversal()
{
	if (left != nullptr)
		left->PostOrderTraversal();
	if (right != nullptr)
		right->PostOrderTraversal();
	std::cout << *content << std::endl;
}

template<typename C, typename K, typename T>
void TTreeBase<C,K, T>::PreOrderTraversal()
{
	std::cout << *content << std::endl;
	if (left != nullptr)
		left->PreOrderTraversal();
	if (right != nullptr)
		right->PreOrderTraversal();
}

template<typename C, typename K, typename T>
void TTreeBase<C, K, T>::RecursiveDelete()
{
	if (left != nullptr) {
		left->RecursiveDelete();
	}
	if (right != nullptr) {
		right->RecursiveDelete();
	}
	return delete this;
}

template<typename C, typename K, typename T>
C* TTreeBase<C, K, T>::GetRoot()
{
	if (parent == nullptr) {
		// I really hope this works :/
		return static_cast<C*>(this);
		//return this;
	}
	else {
		return parent->GetRoot();
	}
}
