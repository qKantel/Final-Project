/** @file Edge.h */

/** @class Edge
* An Edge class for graph implementations. */

#ifndef EDGE
#define EDGE

template <class Type_V, class Type_W>
class Edge
{
public:
	Type_V v, w;
	Type_W	weight;
	Edge(Type_V firstVertex, Type_V secondVertex, Type_W edgeWeight)
	{
		v = firstVertex;
		w = secondVertex;
		weight = edgeWeight;
	}  // end constructor
}; // end Edge
// End of header file

#endif
