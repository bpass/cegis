#ifndef COMPARE_H
#define COMPARE_H

#include <math.h>
#include <list>
using namespace std;

template <class T>
int compareDataAbs( const list<T> &list1, const list<T> &list2, list<T> &diffList )
{
    int errorCount = 0,                              //initalize error count to 0
        difference = 0;                              //temp to hold difference

    diffList.clear();                                //empty diffList

    list<T>::const_iterator list1Iter;               //Iterators for list1 and list2
    list<T>::const_iterator list2Iter;

    list1Iter = list1.begin();                       //Set iterators to beginning
    list2Iter = list2.begin();                       //of lists

    for( ; (list1Iter!=list1.end()) && (list2Iter!=list2.end());  )
    {
    	difference = 1; //abs( (*list1Iter) - (*list2Iter) );  //compute absolute val. diff.

    	if( difference ) errorCount++;                    //if different increment
                                                          //error counter
        diffList.push_back( difference );                 //put the value in diff list
    	list1Iter++;                                      //move iterator to next
    	list2Iter++;                                      //element in lists
    }

    if( list1.size() > list2.size() )                     //if one list is bigger
    {
        for( ; list1Iter!=list1.end(); list1Iter++ )      //insert all remaining
        {                                                 //elements and increment
        	errorCount++;                                 //error counter
//            diffList.push_back( std::abs(((T)(*list1Iter)))+1 );
        }
    }
    else if( list1.size() < list2.size() )                //save as above
    {
        for( ; list2Iter!=list2.end(); list2Iter++ )
        {
        	errorCount++;
 //           diffList.push_back( std::abs(((T)(*list2Iter)))+1 );
        }
    }

    return errorCount;
}

//U MUST be SIGNED
template <class T, class U>
int compareData( const list<T> &list1, const list<T> &list2, list<U> &diffList )
{
    int errorCount = 0,                              //initalize error count to 0
        difference = 0;                              //temp to hold difference

    diffList.clear();                                //empty diffList

    list<T>::const_iterator list1Iter;               //Iterators for list1 and list2
    list<T>::const_iterator list2Iter;

    list1Iter = list1.begin();                       //Set iterators to beginning
    list2Iter = list2.begin();                       //of lists

    for( ; (list1Iter!=list1.end()) && (list2Iter!=list2.end());  )
    {
    	difference = (*list1Iter) - (*list2Iter);          //compute difference

    	if( difference!= 0 ) errorCount++;                //if different increment
                                                          //error counter
        diffList.push_back( difference );                 //put the value in diff list
    	list1Iter++;                                      //move iterator to next
    	list2Iter++;                                      //element in lists
    }

    if( list1.size() > list2.size() )                     //if one list is bigger
    {
        for( ; list1Iter!=list1.end(); list1Iter++ )      //insert all remaining
        {                                                 //elements and increment
        	errorCount++;                                 //error counter
            diffList.push_back( (*list1Iter)+1 );
        }
    }
    else if( list1.size() < list2.size() )                //save as above
    {
        for( ; list2Iter!=list2.end(); list2Iter++ )
        {
        	errorCount++;
            diffList.push_back( (*list2Iter)+1 );
        }
    }

    return errorCount;
}

#endif
