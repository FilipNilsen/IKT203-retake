#pragma once
#include "TBST.h"

/**
@tparam T Type for data contained in queue. Needs to be a pointer type.
@tparam K Datatype for key values.
@bug Does not handle duplicate keys for the moment.
*/
template <typename K, typename T>
class TPriorityQueue {
private:
	TBST<K,T>* bst = nullptr;
public:
	T Dequeue() {
		if (bst == nullptr) {
			return nullptr;
		}
		//std::cout << bst->Plantuml().str();
		auto node = bst;
		while (1) {
			if (node->Left() == nullptr) {
				T rvalue = node->GetContent();

				if (node == bst) {
					bst = node->Right();
				}

				delete node;
				return rvalue;
			}
			else { node = node->Left(); }
		}
	}
	bool Enqueue(K key, T value) {
		if (bst == nullptr) {
			bst = new TBST<K, T>(key, value, nullptr);
			return true;
		}
		return bst->Insert(key, value);
	}
};