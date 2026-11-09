#include <iostream>
#include "./app.h"
#include "SharedLib.h"
#include <NewStuff/TBST.h>
#include <NewStuff/PriorityQueue.h>
#include "TNode.h"
static constexpr std::string_view AssignmentOption = "Option 2: Data Center Network Monitor.";

TBST< std::string, TNode* >* nodes = nullptr;
TBST< uint32_t, TNode* >* nodesIdx = nullptr;
float edges[50][50] = { 0 };

/**
 * @brief Callback function to process one node.
 */
static bool NodeReadCallback(const int aIndex, const int aTotalCount, const std::string& aNode)
{
	std::cout << "Loading Node " << (aIndex + 1) << " of " << aTotalCount << ": " << aNode << "\n";
	auto node = new TNode(aIndex, aNode);
	
	if (nodes == nullptr) {
		nodes = new TBST<std::string, TNode*>(aNode, node, nullptr);
		nodesIdx = new TBST<uint32_t, TNode* >(aIndex, node, nullptr);
		return true;
	}

	nodes->Insert(aNode, node);
	nodesIdx->Insert(aIndex, node);

	// Return true to continue reading
	return true;
}

/**
 * @brief Callback function to process one edge.
 */
static bool EdgeReadCallback(const int aIndex, const int aTotalCount, const std::string& aFromNode, const std::string& aToNode, float aWeight)
{
	std::cout << "  Loading Edge " << (aIndex + 1) << " of " << aTotalCount << ": "
		<< aFromNode << " -> " << aToNode << " (Weight: " << aWeight << ")\n";

	edges[ nodes->Search(aFromNode)->GetIndex() ][nodes->Search(aToNode)->GetIndex() ] = aWeight;

	// Return true to continue reading
	return true;
}

void Dijkstra(std::string start="Oslo") {
	auto pq = new TPriorityQueue<float, TNode*>;
	TNode* node = nodes->Search(start);
	node->UpdateShortestPath(0);
	TNode* neighbour= nullptr;
	float costSoFar = 0;
	TBST<uint32_t, TNode*>* visited = new TBST<uint32_t, TNode* >(node->GetIndex(), node, nullptr);

	while (1) {
		std::cout << "At node " << *node << "\n";
		auto index = node->GetIndex();
		for (size_t i = 0; i < 50; ++i) {
			if (edges[index][i] != 0) {
				neighbour = nodesIdx->Search(i)->UpdateShortestPath(costSoFar + edges[index][i]);
				std::cout << "Has neighbour " << *neighbour << "\n";
				
				if (visited->Search(i) == nullptr) {
					pq->Enqueue(
						neighbour->GetShortestPath(),
						neighbour);
				}
			}
		}
		neighbour = pq->Dequeue();
		if (neighbour == nullptr) { 
			std::cout << "\nSummary:\n";
			for (size_t i = 0; i < 50; ++i) {
				node = nodesIdx->Search(i);
				if (node != nullptr)
					std::cout << *node << std::endl;
			}
			return;
		}
		visited->Insert(neighbour->GetIndex(), neighbour);
		
		costSoFar = neighbour->GetShortestPath();
		node = neighbour;
		std::cout << std::endl;
	}

	
}

int RunApp()
{
	setlocale(LC_ALL, ".UTF8");

	std::cout << AssignmentOption << std::endl;

	// Path to the graph data file
	//std::string filename = "F:\\IKT203\\VisualStudio\\DATA\\city_graph.txt";
	std::string filename = std::string(CMAKE_CURRENT_SOURCE_DIR).append("/Assignment-04/city_graph.txt");

	std::cout << "Reading graph from file: " << filename << "\n\n";

	// Call the utility function with both callbacks
	readGraphFromFile(filename, NodeReadCallback, EdgeReadCallback);

	std::cout << "\nFinished reading graph.\nType in the city you want to go to:" << std::endl;

	std::string start;
	std::getline(std::cin, start);

	Dijkstra(start);
	return 0;
}

int RunApp(int argc, char* argv[]) {

	return 0;
}