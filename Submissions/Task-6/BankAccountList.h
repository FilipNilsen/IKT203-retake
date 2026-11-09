#pragma once
#ifndef BANKACCOUNTLIST_H
#define BANKACCOUNTLIST_H
#include "BankAccount.h"
#include <string>
#include <functional>

/** @brief Function that is used by TLinkedList::Find() and TLinkedList::Every(). */
typedef bool (*FCompareAccount)(TBankAccount* account, void* searchKey);

/** @brief Function that is used by TLinkedList::forEach(). */
typedef void (*FForEachAccount)(TBankAccount* account, int index);

/** @brief Function that is used by TLinkedList::Every(). */
typedef bool (*FEveryAccount)(TBankAccount*, int);

// Node class for linked list
class TLinkedListNode
{
public:
	TBankAccount* data;
	TLinkedListNode* next;
	TLinkedListNode(TBankAccount* aData) : data(aData), next(nullptr) {}
	~TLinkedListNode()
	{
		// Destructor logic if needed
	}
};

// Use dummy head node for simplicity
class TLinkedList
{
private:
	TLinkedListNode* head;
	bool ownsData;
	int size;
public:
	TLinkedList(bool);
	~TLinkedList();
	int getSize() const;
	TLinkedListNode* getHead() const;

	void Add(TBankAccount*);

	/**
	@brief Finds a TBankAccount in the list.
	@param An implementation of FCompareAccount.
	@returns The first TBankAccount* that matches the criteria.
	*/
	TBankAccount* Find(FCompareAccount, void*);

	TLinkedList* Every(FCompareAccount, void*);
	TBankAccount* Every(FEveryAccount aEveryFunc);
	TBankAccount** ToArray();
	void forEach(FForEachAccount);

	void Append(TBankAccount* account);
	void Remove(TBankAccount* account);
};

#endif// BANKACCOUNTLIST_H