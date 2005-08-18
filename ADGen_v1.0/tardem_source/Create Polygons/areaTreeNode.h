# include <stdio.h>
# include <stdlib.h>

# ifndef AREATREENODE_H
# define AREATREENODE_H

class areaTreeNode
{
	public:

		areaTreeNode( int p_areaID, double p_area = 0.0 )
			:left(NULL),right(NULL),area(p_area),areaID( p_areaID ){};
		areaTreeNode( const areaTreeNode & p_areaT )
		{	areaID = p_areaT.areaID;
			area = p_areaT.area;
			left = p_areaT.left;
			right = p_areaT.right;
		}
		~areaTreeNode()
		{	left = NULL;
			right = NULL;
		}
		int areaID;
		double area;	
		areaTreeNode * left;
		areaTreeNode * right;
};

# endif