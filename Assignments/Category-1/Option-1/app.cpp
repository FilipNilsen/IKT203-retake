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
#include "TStringSearch.h"
#include <cstring>

#define INRANGE(element,start,end) element >= start && element <= end

// The 
//   Option/1
//     from Address Spcae
//  Text editing program

//#define STACK_DEBUG

class TextEditor;
typedef void (TextEditor::*TCommand)();

typedef struct {
	char type;
	union {
		int AsInt;
		std::string* AsString;
	};
} cmd_stack_t;

class TextEditor {
private:
	TDLinkedList<std::string*>* document = new TDLinkedList<std::string*>(true);
	TStack<TAction*>* undoStack = new TStack<TAction*>();
	TStack<TAction*>* redoStack = new TStack<TAction*>();
	TQueue<TDLinkedList<std::string*>*>* printQueue = new TQueue<TDLinkedList<std::string*>*>;
	TStack<cmd_stack_t*>* cmdStack = new TStack<cmd_stack_t*>();
	TStringSearch<TCommand>* cmdParser = new TStringSearch<TCommand>();

	public: TextEditor() {
		cmdParser->Insert(&TextEditor::Undo, "undo");
		cmdParser->Insert(&TextEditor::PrintPrint, "print");
		cmdParser->Insert(&TextEditor::SpoolPrint, "spool");
		cmdParser->Insert(&TextEditor::Redo, "redo");
		cmdParser->Insert(&TextEditor::showDocument, "show");
		cmdParser->Insert(&TextEditor::AddLine, "add");
		cmdParser->Insert(&TextEditor::RemoveLine, "remove");
		cmdParser->Insert(&TextEditor::loadTextFileCmd, "load");
	}

	private:

	auto resetRedoStack(){
		delete redoStack;
		redoStack = new TStack<TAction*>;
	}

	auto addTextToDocument(std::string* textToAdd, uint16_t lineNo) {
		undoStack->Push(new TAction(Etype::add, textToAdd, lineNo));
		resetRedoStack();
		document->InsertBefore(lineNo, textToAdd);
		//std::cout << "add " << *(std::string*)textToAdd->Content() << " to line " << lineNo << std::endl;
	};

	auto addTextToDocument(std::string* textToAdd) {
		//std::cout << "add " << textToAdd << " to end\n";
		undoStack->Push(new TAction(Etype::add, textToAdd, -1));
		resetRedoStack();
		document->Append(textToAdd);
	};

	void showDocument() {
		TDLinkedListNode<std::string*>* a = document->GetHead();
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

	void Undo() {
		if (undoStack->IsEmpty()) {
			std::cerr << "Nothing to undo" << std::endl;
			return;
		}
		TAction* action = undoStack->Pop();
		redoStack->Push(action);
	
		action->SwapType();
		action->PreformOnDocument(document);
#ifdef STACK_DEBUG
		undoStack->Dump();
#endif
		showDocument();
	}

	void Redo() {
		if (redoStack->IsEmpty()) {
			std::cerr << "Nothing to redo" << std::endl;
			return;
		}
		TAction* action = redoStack->Pop();
		undoStack->Push(action);
		
		action->SwapType();
		action->PreformOnDocument(document);
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
			a->PreformOnDocument(document);
			undoStack->Push(a);
			s = new string();
		}
		delete s;
		showDocument();
	};

	void loadTextFileCmd() {
		if (cmdStack->IsEmpty()) {
			std::cerr<<"Missing paramater!\n";
			return;
		}
		auto file = cmdStack->Pop();
		if (file->type != 's') {
			std::cerr<<"Not string!\n";
			return;
		}
		resetRedoStack();
		delete document;
		document = new TDLinkedList<std::string*>(true);
		LoadTextFile(std::string(*file->AsString));
	}

	void SpoolPrint() {
		TDLinkedListNode<std::string*>* a = document->GetHead();
		auto printDocumentCopy = new TDLinkedList<std::string*>;
		do {
			std::string* b = a->GetContent();
			if (b != nullptr) {
				printDocumentCopy->Append(new std::string(*b));
			}
			a = a->GetNextElement();
		} while (a != nullptr);
		printQueue->Enqueue(printDocumentCopy);
	}

	void PrintPrint() {
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

	void AddLine() {
		using namespace std;
		resetRedoStack();
		int lineno;
		auto textToAddToDocument = new string();

		if (cmdStack->IsEmpty()) {
			std::cerr<<"No parameter in stack!\n";
			return;
		}
		auto command = cmdStack->Pop();
		if (command->type != 'i') {
			std::cerr<<"Parameter in stack is wrong type ("<<command->type<<")!\n";
			return;
		}
		lineno = command->AsInt;

		getline(cin, *(std::string*)textToAddToDocument);

		auto action = new TAction(Etype::add, textToAddToDocument, lineno);
		action->PreformOnDocument(document);
		undoStack->Push(action);
	}

	void Append() {
		using namespace std;
		resetRedoStack();
		auto textToAddToDocument = new string();
		getline(cin, *(std::string*)textToAddToDocument);
		auto action = new TAction(Etype::add, textToAddToDocument, -1);
		action->PreformOnDocument(document);
		undoStack->Push(action);
	}

	void RemoveLine() {
		uint16_t lineno;
		resetRedoStack();
		TAction* action = nullptr;
		lineno = cmdStack->Pop()->AsInt;
		action = new TAction(Etype::remove, nullptr, --lineno);
		action->PreformOnDocument(document);
		undoStack->Push(action);
	}

	bool GetUserInput() {
		using namespace std;
		string token;
		string userInput;
		getline(cin, userInput);
		stringstream ss_userInput(userInput);
		const char* p_i = userInput.c_str();
		while (*p_i != 0) {
			if (*p_i == '"') {
				p_i++;
				const char* p_end = p_i;
				while (*p_end != '"') {
					p_end++;
					if (*p_end == 0) {
						cerr<<"Unterminated string!\n";
						return true;
					}
				}
				auto s = new string(p_i, p_end-p_i);
				auto e = new cmd_stack_t;
				e->type = 's';
				e->AsString = s;
				cmdStack->Push(e);
				p_i = p_end + 1;
			}

			else if ((INRANGE(*p_i, '0', '9')) || *p_i == '-') {
				const char* p_end = p_i + 1;
				while (INRANGE(*p_end, '0', '9')) {
					p_end++;
				}
				auto token = string(p_i, p_end - p_i);
				int number = std::stoi(token);
				auto e = new cmd_stack_t;
				e->type = 'i';
				e->AsInt = number;
				cmdStack->Push(e);
				p_i = p_end;
			}

			else if (*p_i <= ' ') {
				p_i++;
			}

			else {
				const char* p_end = strchr(p_i, ' ');
				if (p_end == nullptr) {
					p_end = strchr(p_i, 0);
				}
				token = string(p_i, p_end - p_i);
				// Got AI to fix this because it's a complicated one.
				(this->*(cmdParser->Get(token.c_str())))();
				p_i = p_end;
			}
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
