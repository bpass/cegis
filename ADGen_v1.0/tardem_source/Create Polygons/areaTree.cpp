# include "areaTree.h"
# include "areaTreeNode.h"

AreaTree::AreaTree()
{	root = NULL;
}

AreaTree::AreaTree( const AreaTree & at )
{	root = recCopy( at.root );	
}

AreaTree::~AreaTree()
{	recDelete( root );
}

areaTreeNode * AreaTree::recCopy( const areaTreeNode * at )
{	if( at == NULL )
		return NULL;
	areaTreeNode * node = new areaTreeNode(at->areaID, at->area );
	node->left = recCopy( at->left );
	node->right = recCopy( at->right );
	return node;
}

void AreaTree::recDelete( areaTreeNode * at )
{	if( at == NULL )
		return;

	recDelete( at->left );
	recDelete( at->right );
	
	at = NULL;
}

double AreaTree::GetArea( int areaID )
{	return recGetArea( root, areaID );
}

void AreaTree::Area( int areaID, int changeAmount )
{	recArea( root, areaID, changeAmount );
}

double AreaTree::recGetArea( areaTreeNode * Root, int areaID )
{	//Base Case
	if( Root == NULL )
		return -1;
	//Found Case
	else if( areaID == Root->areaID )
		return Root->area;
	//Recursive Case
	else if( areaID < Root->areaID )
		return recGetArea( Root->left, areaID );
	else if( areaID > Root->areaID )
		return recGetArea( Root->right, areaID );
	//Default Case
	return -1;
}

void AreaTree::recArea( areaTreeNode *& Root, int areaID, int changeAmount )
{	//Base Case
	if( Root == NULL )
	{	areaTreeNode * atNode = new areaTreeNode( areaID, changeAmount );
		Root = atNode;
		return;
	}
	//Found Case
	else if( areaID == Root->areaID )
	{	Root->area += changeAmount;
		return;
	}
	//Recursive Case
	else if( areaID < Root->areaID )
		recArea(  Root->left, areaID, changeAmount );
	else if( areaID > Root->areaID )
		recArea( Root->right, areaID, changeAmount );	
}
