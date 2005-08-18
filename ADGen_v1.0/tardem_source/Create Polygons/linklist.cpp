#include "linklist.h"


Node::~Node()
{

}

LinkList::~LinkList()
{
	Node* tmp;
	while(head) {
		tmp = head;
		head = head->Next;
		delete tmp;
	}

	head = NULL;
	tail = NULL;
	cur = NULL;
}

LinkList::LinkList(LinkList &list)
{
	if(list.head==NULL) {
		tail = NULL;
		head = NULL;
		cur = NULL;
	}
	else {
		head = new Node();
		head->data = list.head->data;
		Node* newPtr = head;
		for(Node* orig = list.head->Next; orig != NULL; orig = orig->Next) {
			newPtr->Next = new Node();
			newPtr = newPtr->Next;
			newPtr->data = orig->data;
		}
		tail = newPtr;
		cur = head;
	}
	
}

void LinkList::operator=(LinkList &list)
{
	if(list.head==NULL) {
		tail = NULL;
		head = NULL;
		cur = NULL;
	}
	else {
		head = new Node();
		head->data = list.head->data;
		Node* newPtr = head;
		for(Node* orig = list.head->Next; orig != NULL; orig = orig->Next) {
			newPtr->Next = new Node();
			newPtr = newPtr->Next;
			newPtr->data = orig->data;
		}
		tail = newPtr;
		cur = head;
	}
	
}

bool LinkList::EndOfList()
{
	if(cur == tail)
		return true;
	else
		return false;
}

bool LinkList::InsertItem(Node* & item)
{
	if(head == NULL) 
		head = item;	 
	else 
		tail->Next = item;

	tail = item;
	tail->Next=NULL;
	return true;
}

bool LinkList::IsEmpty()
{
	if(head == NULL)
		return true;
	return false;
}

void LinkList::DeleteHead(Node* &item)
{
	Node* tmp = head;
	if(head == tail) {
		head = NULL;
		tail = NULL;
	}
	else 
		head = head->Next;

	item->data = tmp->data;
	item->Next = NULL;

	delete tmp;
}

void LinkList::getHead(Node* & h)
{
	h = head;
}

void LinkList::getTail(Node* & t)
{
	t=tail;
}

void LinkList::Next(Node * &item)
{
	cur = cur->Next;
	item = cur;
}

void LinkList::First(Node * &head)
{
	cur = this->head;
	head = cur;
}

void LinkList::Last(Node * &tail)
{
	cur = this->tail;
	tail = cur;
}