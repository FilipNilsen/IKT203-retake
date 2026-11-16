#pragma once
#include <cfloat>

class TNode;
std::ostream& operator<<(std::ostream& os, TNode& node);

class TNode {
private:
	std::string name;
	uint32_t index;
	float shortestPath = FLT_MAX;
public:
	TNode(uint32_t _index, std::string _name):index(_index), name(_name) {};
	std::string GetName() { return name; }
	uint32_t GetIndex() { return index; }
	TNode* SetShortestPath(float newShortestPath) {
		shortestPath = newShortestPath; 
		return this;
	}

	TNode* UpdateShortestPath(float newVal) {
		if (newVal < shortestPath) {
			std::cout << "Cost to " << name << " reduced from "<< shortestPath << " to " << newVal << "\n";
			shortestPath = newVal;
		}
		return this;
	}

	float GetShortestPath() { return shortestPath; }
	TNode* Reset() {
		shortestPath = FLT_MAX;
		return this;
	}

	bool operator>(TNode& r)
	{
		return this->GetShortestPath() > r.GetShortestPath();
	}

	bool operator<(TNode& r)
	{
		return this->GetShortestPath() < r.GetShortestPath();
	}

	bool operator==(TNode& r)
	{
		return this->GetShortestPath() == r.GetShortestPath();
	}
};

std::ostream& operator<<(std::ostream& os, TNode& node) {
	os << node.GetName() << " has cost: " << node.GetShortestPath();
	return os;
}