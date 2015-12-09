/*
*	file	FlightMap.cpp
*	status	Complete
*/

#include "FlightMap.h"
#include <set>
#include <queue>

FlightMap::FlightMap()
{
	//Create the possible flight locations
	FlightLocation	A('A'),
					B('B'), 
					C('C'),
					D('D'),
					E('E'),
					F('F'),
					G('G'),
					I('I'),
					J('J'),
					L('L'),
					M('M'),
					N('N'),
					O('O'),
					P('P');

	//Assemble the map with distances
	B.addAdjacentLocation(A,142)
		.addAdjacentLocation(C,170)
		.addAdjacentLocation(D,114)
		.addAdjacentLocation(M,209)
		.addAdjacentLocation(N,208)
		.addAdjacentLocation(P,134)
		.addAdjacentLocation(O,193);
	D.addAdjacentLocation(E,93)
		.addAdjacentLocation(F,155)
		.addAdjacentLocation(I,160);
	F.addAdjacentLocation(G,184)
		.addAdjacentLocation(I,83);
	I.addAdjacentLocation(L,88);
	I.addAdjacentLocation(J,73);

	//Add them all to the vector
	locations.insert(std::pair<char,FlightLocation>(A.getIdentifier(),A));
	locations.insert(std::pair<char,FlightLocation>(B.getIdentifier(),B));
	locations.insert(std::pair<char,FlightLocation>(C.getIdentifier(),C));
	locations.insert(std::pair<char,FlightLocation>(D.getIdentifier(),D));
	locations.insert(std::pair<char,FlightLocation>(E.getIdentifier(),E));
	locations.insert(std::pair<char,FlightLocation>(F.getIdentifier(),F));
	locations.insert(std::pair<char,FlightLocation>(G.getIdentifier(),G));
	locations.insert(std::pair<char,FlightLocation>(I.getIdentifier(),I));
	locations.insert(std::pair<char,FlightLocation>(J.getIdentifier(),J));
	locations.insert(std::pair<char,FlightLocation>(L.getIdentifier(),L));
	locations.insert(std::pair<char,FlightLocation>(M.getIdentifier(),M));
	locations.insert(std::pair<char,FlightLocation>(N.getIdentifier(),N));
	locations.insert(std::pair<char,FlightLocation>(O.getIdentifier(),O));
	locations.insert(std::pair<char,FlightLocation>(P.getIdentifier(),P));
}

//Class for doing shortest path calculations using Dijikstra's algorithm
class PathNode
{
public:
	PathNode(char newIdentifier, int initialDistanceScore) : identifier(newIdentifier), distanceScore(initialDistanceScore)
	{
		
	}
	char getIdentifier() const
	{
		return identifier;
	}
	int getFScore() const
	{
		return distanceScore;
	}
	void setFScore(int newScore)
	{
		distanceScore = newScore;
	}

	bool operator==(const PathNode& other) const
	{
		return other.identifier == identifier;
	}

	//The operators are inverted to work with the priority queue
	bool operator<(const PathNode& other) const
	{
		return distanceScore > other.distanceScore;
	}

	bool operator>(const PathNode& other) const
	{
		return distanceScore < other.distanceScore;
	}

private:
	char identifier;
	int distanceScore;
};

//compares node pointers for use with a priority queue of pointers
struct NodeComparer
{
	bool operator() (PathNode* node1,  PathNode* node2) const
	{
		return node1->getFScore() > node2->getFScore();
	}
};

//get the distance between two nodes with the given identifiers
int FlightMap::getMinDistance(char startIdentifier, char endIdentifier)
{
	//get the starting location
	FlightLocation current = locations[startIdentifier];
	int currentDistance = 0;

	//A priority queue of PathNodes that sorts them according to their distance Score.
	//That means the lowest distance is the highest priority
	std::priority_queue<PathNode*, std::vector<PathNode*>, NodeComparer> unVisitedNodes;
	std::map<char,PathNode*> nodeMap;
	std::set<char> visitedNodes;

	//add all the nodes to the unvisited set with a starting distance of infinity (or zero for starting node)
	for(std::map<char,FlightLocation>::iterator itt = locations.begin(); itt != locations.end(); ++itt)
	{	
		PathNode* nodePtr;
		if(itt->first == startIdentifier)
			nodePtr = new PathNode(itt->first,0);
		else
			nodePtr = new PathNode(itt->first,999999);

		unVisitedNodes.push(nodePtr);
		nodeMap[itt->first] = nodePtr;
	}

	int returnDistance = 0;

	//implementation of Dijkstra's shortest path algorithm
	while(true)
	{
		//If weve found the target node, we break out with the current distance (which will be the shortest)
		if(current.getIdentifier() == endIdentifier)
		{
			returnDistance = currentDistance;
			break;
		}

		//pop off the node with the lowest distance score (it will have already been set as the current node)
		unVisitedNodes.pop();

		//iterate through all the adjacent UNVISITED nodes of the current node
		for(std::vector<char>::iterator itt = current.getAdjacencyIterator(); itt != current.getIteratorEnd(); ++itt)
		{
			//Get the node corresonding to the identifer (we iterate through identifiers not actual nodes)
			FlightLocation node = locations[*itt];

			//Only do something if the node is NOT visited
			if(visitedNodes.find(*itt) == visitedNodes.end())
			{
				//calculate the new distance to the node were checking
				int newDistance = currentDistance + current.getDistanceToLocation(*itt);

				//get the old score for this node
				std::map<char,PathNode*>::iterator oldScore = nodeMap.find(*itt);

				//If the old score for this node is infinity (not in the map) or is greater than the new distance, then set the new distance
				if(oldScore == nodeMap.end() || oldScore->second->getFScore() > newDistance)
				{
					//Get the node pointer and set its distance score to the new distance
					nodeMap[*itt]->setFScore(newDistance);

					//Solution from stackoverflow for refreshing the priority queue after we change the Fscore of the node pointers
					//tl;dr--This refreshes the priority queue
					std::make_heap(const_cast<PathNode**>(&(unVisitedNodes.top())),const_cast<PathNode**>(&(unVisitedNodes.top())) + unVisitedNodes.size(),NodeComparer());
				}
			}
		}

		//add the current node to the visited list
		visitedNodes.insert(current.getIdentifier());

		//get the next node, which is the unvisited node with the shortest distance
		//AKK the top node of the priority queue
		PathNode* next = unVisitedNodes.top();

		//set the current node to the next node
		current = locations[next->getIdentifier()];
		currentDistance = next->getFScore();
	}

	for(std::map<char,PathNode*>::iterator delItt = nodeMap.begin(); delItt != nodeMap.end(); ++delItt)
	{
		//Delete all the dynamically allocated memory for all the node pointers
		delete (delItt->second);
	}

	//return the shortest distance that we calculated
	return returnDistance;
}


