// Option 1 part of Assignments/Category-1
// Option 1 (Standard): Console Text Editor.

#include "./app.h"
#include <string>
#include <iostream>
#include <sstream>
#include "DataStructures/TLinkedList.hpp"
#include "DataStructures/TStack.hpp"
#include "VT100.h"
#include <DataStructures/TQueue.hpp>
#include <fstream> 
#include "TAction.h"

// The 
//   Option/1
//     from Address Spcae
//  Text editing program

//#define STACK_DEBUG

class TextEditor {
private:
	TDLinkedList<std::string*> document = TDLinkedList<std::string*>(true);
	TStack<TAction*>* undoStack = new TStack<TAction*>();
	TStack<TAction*>* redoStack = new TStack<TAction*>();
	TQueue<TDLinkedList<std::string*>*>* printQueue = new TQueue<TDLinkedList<std::string*>*>;

	auto resetRedoStack(){
		delete redoStack;
		redoStack = new TStack<TAction*>;
	}

	auto addTextToDocument(std::string* textToAdd, uint16_t lineNo) {
		undoStack->Push(new TAction(Etype::add, textToAdd, lineNo));
		resetRedoStack();
		document.InsertBefore(lineNo, textToAdd);
		//std::cout << "add " << *(std::string*)textToAdd->Content() << " to line " << lineNo << std::endl;
	};

	auto addTextToDocument(std::string* textToAdd) {
		//std::cout << "add " << textToAdd << " to end\n";
		undoStack->Push(new TAction(Etype::add, textToAdd, -1));
		resetRedoStack();
		document.Append(textToAdd);
	};

	auto showDocument() {
		TDLinkedListNode<std::string*>* a = document.GetHead();
		uint16_t d = 0;
		do {
			std::string* b = a->GetContent();
			if (b != nullptr) {
				auto c = (std::string*)b;
				std::cout << ++d << " " << vt100::CHA(3) << " " << *c << std::endl;
			}
			a = a->GetNextElement();
		} while (a != nullptr);
	}

	auto Undo() {
		if (undoStack->IsEmpty()) {
			std::cerr << "Nothing to undo" << std::endl;
			return;
		}
		TAction* action = undoStack->Pop();
		redoStack->Push(action);
	
		action->SwapType();
		action->PreformOnDocument(&document);
#ifdef STACK_DEBUG
		undoStack->Dump();
#endif
		showDocument();
	}

	auto Redo() {
		if (redoStack->IsEmpty()) {
			std::cerr << "Nothing to redo" << std::endl;
			return;
		}
		TAction* action = redoStack->Pop();
		undoStack->Push(action);
		
		action->SwapType();
		action->PreformOnDocument(&document);
#ifdef STACK_DEBUG
		redoStack->Dump();
#endif
		showDocument();
	}

	auto LoadTextFile(std::string filename) {
		using namespace std;
		ifstream f(filename);
		string* s = new string();
		string* r;
		TAction* a;
		while (getline(f, *s)) {
			r = new string(*s);
			a = new TAction(Etype::add, r, -1);
			a->PreformOnDocument(&document);
			undoStack->Push(a);
			s = new string();
		}
		delete s;
		showDocument();
	};

	auto SpoolPrint() {
		TDLinkedListNode<std::string*>* a = document.GetHead();
		auto printDocumentCopy = new TDLinkedList<std::string*>;
		do {
			std::string* b = a->GetContent();
			if (b != nullptr) {
				printDocumentCopy->Append(b);
			}
			a = a->GetNextElement();
		} while (a != nullptr);
		printQueue->Enqueue(printDocumentCopy);
	}

	auto PrintPrint() {
		auto printDocumentCopy = printQueue->Dequeue();
		if (printDocumentCopy == nullptr) { return; }
		TDLinkedListNode<std::string*>* a = printDocumentCopy->GetHead()->GetNextElement();
		do {
			std::string* b = a->GetContent();
			if (b != nullptr) {
				auto c = (std::string*)b;
				std::cout << *c << std::endl;
			}
			a = a->GetNextElement();
		} while (a != nullptr);
		delete printDocumentCopy;
	}

	bool GetUserInput() {
		using namespace std;
		string command;
		string userInput;
		getline(cin, userInput);
		stringstream ss_userInput(userInput);
		ss_userInput >> command;

		if (command == "+") {
			resetRedoStack();
			int32_t lineno;
			auto textToAddToDocument = new string();

			if (ss_userInput >> lineno) {
			}
			else {
				lineno = 0;
			}
			getline(cin, *(std::string*)textToAddToDocument);

			auto action = new TAction(Etype::add, textToAddToDocument, --lineno);
			action->PreformOnDocument(&document);
			undoStack->Push(action);
			return true;
		}

		if (command == "s") {
			showDocument();
			return true;
		}

		if (command == "-") {
			uint16_t lineno;
			resetRedoStack();
			TAction* action = nullptr;
			if (ss_userInput >> lineno) {
				//document.Remove(--lineno); // -- to convert 1 index to 0 index
				action = new TAction(Etype::remove, nullptr, --lineno);
			}
			else {
				//document.RemoveLast();
				action = new TAction(Etype::remove, nullptr, -1);
			}
			action->PreformOnDocument(&document);
			undoStack->Push(action);
			return true;
		}

		if (command == "q") {
			return false;
		}

		if (command == "un") {
			Undo();
			return true;
		}

		if (command == "re") {
			Redo();
			return true;
		}

		if (command == "sp") {
			SpoolPrint();
			return true;
		}

		if (command == "pp") {
			PrintPrint();
			return true;
		}

		return true;
	};

public:
	auto main() {
		//LoadTextFile("C:/Users/Filip/Code/IKT203-portfolio/Assignment-01/readme.txt");
		std::cout<<CMAKE_CURRENT_SOURCE_DIR;
		LoadTextFile(std::string(CMAKE_CURRENT_SOURCE_DIR).append("/Option-1/readme.txt"));
		//LoadTextFile("/home/filip/Development/C/IKT203-portfolio/Assignment-01/readme.txt");
		while (GetUserInput());
	}
};

int RunApp(int argc, char* argv[]) {
	// Implement the Console Text Editor application logic here
	auto textEditor = new TextEditor;
	textEditor->main();
	delete textEditor;
	return 0;
}
