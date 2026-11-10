/** @file */
#pragma once

/** Enum used for type in TAction */
enum class Etype {
	//! Add line
	add,
	//! Remove line
	remove
};

class TAction {
	Etype type;
	//! RefCounted* that has a std::string as its content.
	std::string* str;
	//! Line number, starts at 0
	int32_t lineno;

public:
	bool ownsData = false;
	/**
	* Constructor
	* @param type Etype::add or Etype::remove
	* @param str RefCounted* that has a std::string as its content. str will have its reference count incremented if it's not a nullptr.
	* @param lineno Linenumber or -1 to operate on the last line
	*/
	TAction(Etype type, std::string* str, int32_t lineno) {
		this->type = type;
		this->str = str;
		this->lineno = lineno;
	}

	//! Destructor, does decrease reference count for str
	~TAction() {
		if (ownsData) {
			delete str;
		}
	}

	auto GetType() { return type; }
	auto GetLine() { return lineno; }
	auto GetStr() { return str; }

	//! Turns type that is Etype::add into Etype::remove and vice versa.
	auto SwapType() {
		if (type == Etype::add) { this->type = Etype::remove; }
		else if (type == Etype::remove) { this->type = Etype::add; }
	}

	/**
	* TAction represents an action that is preformed on the document, so this function actualy goes ahead and preforms the action.
	* If str is nullptr this function will also grab it from document and increase the reference count so that remove operations can be undone.
	* @param document a TDLinkedList* that contains std::strings.
	*/
	auto PreformOnDocument(TDLinkedList<std::string*>* document) {
		if (type == Etype::add) {
			if (lineno <= -1) {
				document->Append(this->str);
			}
			else {
				document->InsertBefore(lineno, str);
			}
		}
		else if (type == Etype::remove) {
			if (lineno <= -1) {
				if (str == nullptr){
					str = document->GetTail()->GetPreviousElement()->GetContent();
				}
				document->RemoveLast();
			}
			else if (str == nullptr) {
				str = document->GetAndRemoveAtIndex(lineno);
			}
			else {
				document->Remove(lineno);
			}
		}
	}
};