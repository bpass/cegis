#include "linklist.h"
#include <fstream>

using namespace std;

Node::~Node()
{	if( data.point != NULL )
	{	delete data.point;
		data.point = NULL;
	}
	else if( data.polyline != NULL )
	{	delete data.polyline;
		data.polyline = NULL;
	}					
	else if( data.polygon != NULL )
	{	delete data.polygon;
		data.polygon = NULL;
	}		
}

LinkList::~LinkList()
{
	Node* tmp;
	while(head)
	{	tmp = head;
		head = head->Next;
		if( tmp->data.point != NULL )
		{	delete tmp->data.point;
			tmp->data.point = NULL;
		}
		else if( tmp->data.polyline != NULL )
		{	delete tmp->data.polyline;
			tmp->data.polyline = NULL;
		}					
		else if( tmp->data.polygon != NULL )
		{	delete tmp->data.polygon;
			tmp->data.polygon = NULL;
		}		
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
	if(head == tail)
	{
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

void LinkList::First(Node * &p_head)
{
	cur = head;
	p_head = cur;
}

void LinkList::Last(Node * &p_tail)
{
	cur = tail;
	p_tail = cur;
}