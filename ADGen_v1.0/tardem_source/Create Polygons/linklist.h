#ifndef LINKLIST_H
#define LINKLIST_H

#include <stdio.h>
#include <iostream.h>
#include "types.h"
#include "point1.h"
#include "polygon.h"
#include "PolyLine.h"


union GeoData {
	Point1*	   point;
	PolyGon*   polygon;
	PolyLine*  polyline;
	
};

class Node 
{
public:
//constructors and destructor
	Node():Next(NULL){};
	~Node();
	
//data types
	GeoData		data;
	Node*		Next;
};



class LinkList
{
//constructors and destructor
public:
	LinkList():head(NULL),tail(NULL),cur(NULL),cur_index(0){};
	~LinkList();
	LinkList(LinkList &list);

	void operator=(LinkList &list);

//data types
private:
	Node* head;
	Node* tail;
	Node* cur;
	int   cur_index;


//member functions
public:
	bool	InsertItem(Node* &item);
	bool	IsEmpty();
	void	DeleteHead(Node* &item);
	void	Next(Node * &item);
	void	First(Node * &head);
	void	Last(Node * &tail);
	bool	EndOfList();

	void	getHead(Node* & h);
	void	getTail(Node* & t);

};

#endif