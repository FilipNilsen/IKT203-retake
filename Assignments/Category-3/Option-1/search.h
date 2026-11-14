#pragma once

template <typename T, typename T2 = TStack<T> >
class TSearch {
protected:
	/** @brief List of visited elements */
	TBST<uint32_t, uint32_t*>* visited = nullptr;
	T2* stack;
	void Add(T node) {
		// Prevent adding nullptr
		if (node == nullptr) { return; }

		// Check if visited needs initialisation
		if (visited == nullptr) {
			visited = new TBST<uint32_t, uint32_t*>(node->GetValue(), nullptr, nullptr);
			visited->Insert(node->GetValue(), new uint32_t(node->GetValue()));
			stack->Add(node);
			return;
		}

		//Check if visited
		if (visited->Search(node->GetValue()) != nullptr) { return; }

		// Add to visited list
		visited->Insert(node->GetValue(), new uint32_t(node->GetValue()));

		// Add to stack
		stack->Add(node);
	}
	TSearch() { stack = new T2; }
	~TSearch() {
		if (visited != nullptr) {
			visited->RecursiveDelete();
		}
		delete stack;
	}
};

template <typename T>
class TInOrderTraversal: public TSearch<T> {
public:
	TInOrderTraversal() : TSearch<T>() {};
	void Main(T list) {
		while (1) {
			std::cout << *(list->GetContent()) << "\n";
			Add(list->Right());
			Add(list);
			Add(list->Left());
			list = this->stack->Take();
			if (this->stack->IsEmpty()) { return; }
		}
	}
};

template <typename T>
class TPreOrderTraversal : public TSearch<T> {
public:
	TPreOrderTraversal() : TSearch<T>() {};
	void Main(T list) {
		while (1) {
			std::cout << *(list->GetContent()) << "\n";
			Add(list);
			Add(list->Left());
			Add(list->Right());
			list = this->stack->Take();
			if (this->stack->IsEmpty()) { return; }
		}
	}
};

template <typename T>
class TPostOrderTraversal : public TSearch<T> {
public:
	TPostOrderTraversal() : TSearch<T>() {};
	void Main(T list) {
		while (1) {
			std::cout << *(list->GetContent()) << "\n";
			Add(list->Left());
			Add(list->Right());
			Add(list);
			list = this->stack->Take();
			if (this->stack->IsEmpty()) { return; }
		}
	}
};

template <typename T>
class TBFS : public TSearch<T, TQueue<T> > {
public:
	TBFS() : TSearch<T, TQueue<T>>() {};
	void Main(T list) {
		this->stack->Enqueue(list);
		do {
			list = this->stack->Dequeue();
			std::cout << "Now at: " << *(list->GetContent()) << "\n";
			if (list->Left() != nullptr)
				this->stack->Enqueue(list->Left());
			if (list->Right() != nullptr)
				this->stack->Enqueue(list->Right());
		} while (!this->stack->IsEmpty());
	}
};