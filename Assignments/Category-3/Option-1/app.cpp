#include "./app.h"
#include <iostream>
#include "NewStuff/TAVL.h"
#include "NewStuff/TBST.h"
#include <sstream>
#include <fstream>
#include <PrevAssignments/TQueue.h>
#include <SharedLib.h>
#include <PrevAssignments/TStack.h>
#include "search.h"

struct TEmployee {
	std::string firstName;
	std::string LastName;
	uint32_t id=0;
};

std::ostream& operator<<(std::ostream& os, const TEmployee& dt)
{
	os << dt.id << " " << dt.firstName << " " << dt.LastName;
	return os;
}

typedef TAVL<uint32_t, TEmployee*> EmployeeAVL;
typedef TBST<uint32_t, TEmployee*> EmployeeBST;

namespace Dump {
	int dumpCount = 0;

	template <typename T = TAVL<uint32_t, TEmployee*>* >
	void Dump(T a) {
		std::ofstream f(std::string(std::to_string(dumpCount++) + ".plantuml"));
		f << "@startuml\n" << a->Plantuml().str() << "\n@enduml";
		f.close();
	}
};

EmployeeAVL* list = nullptr;
EmployeeBST* bstList = nullptr;

bool NameRead(
	const int aIndex,
	const int aTotalCount,
	const std::string& aFirstName,
	const std::string& aLastName
) {
	auto a = new TEmployee;
	a->firstName = aFirstName;
	a->LastName = aLastName;
	a->id = rand() % UINT32_MAX;

	if (list == nullptr) {
		list = new TAVL<uint32_t, TEmployee*>(a->id, a, nullptr);
		bstList = new EmployeeBST(a->id, a, nullptr);
	} else {
		while (!list->Insert(a->id, a)) {
			a->id = rand() % UINT32_MAX;
		}
		bstList->Insert(a->id, a);
	}
	list = list->GetRoot();
	
	std::cout << "Added: " << *a << "\n";
	if (aIndex <= 10) { return true; }
	return false;
}

int RunApp(int argc, char* argv[]) {
	// Get a different random number each time the program runs
	srand(time(0));

	//readNamesFromFile("C:/Users/Filip/Code/IKT203-portfolio/Assignment-03/random_names.txt", &NameRead);
	std::string filename(CMAKE_CURRENT_SOURCE_DIR);
	filename.append("/Assignment-03/random_names.txt");
	readNamesFromFile(filename, & NameRead);
	
	Dump::Dump<>(list);
	Dump::Dump<EmployeeBST*>(bstList);

	std::cout << "\nIn order traversal of AVL\n";
	list->InOrderTraversal();

	std::cout << "\nIn order traversal of BST\n";
	bstList->InOrderTraversal();

	std::cout << "\nPre order traversal of AVL\n";
	list->PreOrderTraversal();
	
	std::cout << "\nPre order traversal of BST\n";
	bstList->PreOrderTraversal();

	std::cout << "\nPost order traversal AVL\n";
	list->PostOrderTraversal();
	
	std::cout << "\nPost order traversal of BST\n";
	bstList->PostOrderTraversal();

	std::cout << "\nBFS over AVL\n";
	auto avlbst = new TBFS< TAVL<uint32_t, TEmployee*>* >;
	avlbst->Main(list);
	delete avlbst;

	std::cout << "\nBFS over BST\n";
	auto bstbfs = new TBFS< EmployeeBST *>;
	bstbfs->Main(bstList);
	delete bstbfs;
	return 0;
}
