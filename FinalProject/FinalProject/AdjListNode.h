#include <map>
using namespace std;

#ifndef ADJ_LIST_NODE
#define ADJ_LIST_NODE

template <class Type_V, class Type_W>
class AdjListNode
{
public:
	Type_V startVertex;
	// maps to second vertex and weight of edge
	map<Type_V, Type_W> item;
};



#endif