#ifndef GRAPH
#define GRAPH

#include <vector>
#include <list>
#include <map>
#include "Edge.h"
#include "AdjListNode.h"

using namespace std;

/** An adjacency list representation of an undirected,
* weighted graph. */


template <class Type_V, class Type_W>
class Graph
{
private:
	int numVertices;   /** Number of vertices in the graph. */
	int numEdges;      /** Number of edges in the graph. */
	int numVerAdded;   /** Helper value to manage index values of vertices*/
	/** Adjacency list representation of the graph;
	* contains AdjListNodes which hold vertex and map pair
	* the map pair consists of the second vertex (key)
	* and the edge weight (value). */
	vector<AdjListNode<Type_V, Type_W> > adjList;
public:
	/** Constructor.
	* @pre The graph is empty.
	* @post The graph is initialized to hold n vertices. */
	Graph(int n);

	/** Determines the number of vertices in the graph.
	* @pre None.
	* @post None.
	* @return The number of vertices in the graph. */
	int getNumVertices() const;

	/** Determines the number of edges in the graph.
	* @pre None.
	* @post None.
	* @return The number of edges in the graph. */
	int getNumEdges() const;

	/** Determines the weight of an edge.
	* @pre The edge exists in the graph.
	* @post None.
	* @return The weight of the edge parameter. */
	Type_W getWeight(Edge<Type_V, Type_W> e) const;

	/** Creates an edge in the graph.
	* @pre The vertices exist in the graph.
	* @post Adds to both v and w's list. */
	void add(Edge<Type_V, Type_W> e);

	/** Removes an edge from the graph.
	* @pre The vertices exist in the graph.
	* @post Removes edges from both v and w's list. */
	void remove(Edge<Type_V, Type_W> e);

	/** Finds the edge connecting v and w.
	* @pre The edge exists.
	* @post None.
	* @return An iterator to map key w in vector[v]. */
	typename map<Type_V, Type_W>::iterator findEdge(Type_V v, Type_V w);

	/** Finds index of vertex ver
	* @returns the index of the vertex and -1 if the vertex does not exist*/
	int findIndexOfVertex(const Type_V & ver);

	/** BFS traversal starting at vertex v*/
	void BFS(const Type_V & v);

	/** Finds weight of Shortest Path between nodes start vertex s and destination vertex d*/
	Type_W shortestPath(const Type_V & s, const Type_V & d);
}; // end Graph
// End of header file

template <class Type_V, class Type_W>
Graph<Type_V, Type_W>::Graph(int n)
{
	AdjListNode<Type_V, Type_W> element;
	adjList.assign(n, element);
	numVertices = n;
	numEdges = 0;
	numVerAdded = 0;
}  // end constructor

template <class Type_V, class Type_W>
int Graph<Type_V, Type_W>::getNumVertices() const
{
	return numVertices;
}  // end getNumVertices

template <class Type_V, class Type_W>
int Graph<Type_V, Type_W>::getNumEdges() const
{
	return numEdges;
}  // end getNumEdges

template <class Type_V, class Type_W>
Type_W Graph<Type_V, Type_W>::getWeight(Edge<Type_V, Type_W> e) const
{
	return e.weight;
}  // end getWeight

template <class Type_V , class Type_W>
int Graph<Type_V, Type_W>::findIndexOfVertex(const Type_V & ver)
{
	for (int i = 0; i < numVerAdded; i++)
	{
		if (ver == adjList[i].startVertex)
			return i;
	}
	return -1;
}

template <class Type_V, class Type_W>
void Graph<Type_V, Type_W>::add(Edge<Type_V, Type_W> e)
{
	Type_V v = e.v,
		w = e.w;
	Type_W weight = e.weight;

	//find index values of v and w in the graph if they exist
	//if vertices have not yet been defined, add them into the vector nodes
	int indexV, indexW;
	int i = findIndexOfVertex(v);
	
	if (i != -1)
		indexV = i;
	else
	{
		indexV = numVerAdded;
		adjList[indexV].startVertex = v;
		numVerAdded++;
	}

	i = findIndexOfVertex(w);
	if (i != -1)
		indexW = i;
	else
	{
		indexW = numVerAdded;
		adjList[indexW].startVertex = w;
		numVerAdded++;
	}
	adjList[indexV].item.insert(make_pair(indexW, weight));
	adjList[indexW].item.insert(make_pair(indexV, weight));
	numEdges++;
}  // end add

template <class Type_V, class Type_W>
void Graph<Type_V, Type_W>::remove(Edge<Type_V, Type_W> e)
{
	Type_V v = e.v,
		w = e.w;
	Type_W	weight = e.weight;
	int indexV = findIndexOfVertex(v) ,
		indexW = findIndexOfVertex(w);

	adjList[indexV].item.erase(w);
	adjList[indexW].item.erase(v);
	numEdges--;
}  // end remove

template <class Type_V, class Type_W>
typename map<Type_V, Type_W>::iterator Graph<Type_V, Type_W>::findEdge(Type_V v, Type_V w)
{
	int indexV = findIndexOfVertex(v);
	int indexW = findIndexOfVertex(w);
	map<Type_V, Type_W> m = adjList[indexV].item;
	map<Type_V, Type_W>::iterator iter = m.find(indexW);

	return iter;
}  // end findEdge

template <class Type_V, class Type_W>
void Graph<Type_V, Type_W>::BFS(const Type_V & v)
{
	
	/** Used to mark vertices as visited. */
	int count = 0;

	/** Marked index of vertices. */
	vector<int> mark;
	mark.assign(numVertices, -1);

	/** index of parents of each vertex. */
	vector<int> parents;
	parents.assign(numVertices, 0);

	Edge e(v, v, 0);

	// create a queue to push edges
	queue<Edge> q;

	map<Type_V, Type_W> m;    // holds adjacency list of current vertex
	map<Type_V, Type_W>::iterator iter;

	q.push(e);
	while (!q.empty())
	{
		// get the edge at the front if the queue
		e = q.front();

		// pop the edge off the queue
		q.pop();
		int indexV = findIndexOfVertex(e.v);
		int indexW = findIndexOfVertex(e.w);

		// if the vertex w has not visited yet, visit it
		if (mark[indexW] == -1)
		{
			Type_V v = e.v,
				w = e.w;
			Type_W	weight = e.weight;
			mark[IndexW] = count++;  // mark w visited
			parents[indexW] = indexV;     // store w's parent

			// go through adjacency list of w
			m = adjList[indexW].item;
			for (iter = m.begin(); iter != m.end(); iter++)
			{	// if w's neighbor vertices have not been visited,
				// push the edge on the queue
				if (mark[iter->first] == -1)
				{
					Type_V u = adjList[iter->first].startVertex;
					q.push(Edge(w, u, iter->second));
				}

			}
		}  // end if
	}  // end while
}

template <class Type_V, class Type_W>
Type_W Graph<Type_V, Type_W>::shortestPath(const Type_V & s, const Type_V & d)
{
	int indexS = findIndexOfVertex(s);
	int indexD = findIndexOfVertex(d);
}

#endif