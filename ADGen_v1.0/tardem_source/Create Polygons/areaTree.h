# include "areaTreeNode.h"

#ifndef AREATREE_H
#define AREATREE_H

class AreaTree
{	
	public:

		AreaTree();
		AreaTree( const AreaTree & at );
		~AreaTree();
		areaTreeNode * recCopy( const areaTreeNode * at );
		void recDelete( areaTreeNode * at );
		double GetArea( int areaID );
		void Area( int areaID, int changeAmount );
	
	private:
		double recGetArea( areaTreeNode * Root, int areaID );
		void recArea( areaTreeNode *& Root, int areaID, int changeAmount );
		areaTreeNode * root;
};

#endif