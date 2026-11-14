template <typename T>
void PreOrderTraversal(T list) {
	auto stack = TStack<T>();
	while (1) {
		if (list->Left() != nullptr)
		{
			stack.Add(list->Left());
		}
		stack.Add(list);
		if (list->Right() != nullptr)
		{
			stack.Add(list->Right());
		}
		list = stack.Take();
		std::cout << list << "\n";
	}
}