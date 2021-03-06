// Sample solution for project #5

#include <boost/graph/adjacency_list.hpp>
#include <iostream>
#include <limits.h>
#include <list>
#include <fstream>
#include <queue>
#include <vector>
#include <stack>

#include "d_except.h"
#include "d_matrix.h"

#define LargeValue 99999999

using namespace boost;
using namespace std;

struct VertexProperties;
struct EdgeProperties;

typedef adjacency_list<vecS, vecS, bidirectionalS, VertexProperties, EdgeProperties> Graph;

struct VertexProperties
{
	pair<int, int> cell; // maze cell (x,y) value
	Graph::vertex_descriptor pred;
	bool visited;
	bool marked;
	int weight;
};

// Create a struct to hold properties for each edge
struct EdgeProperties
{
	int weight;
	bool visited;
	bool marked;
};

typedef adjacency_list<vecS, vecS, bidirectionalS, VertexProperties, EdgeProperties> Graph;

// typedef property<edge_weight_t, int> EdgeProperty;

class maze
{
public:
   maze(ifstream &fin);
   void print(int,int,int,int);
   bool isLegal(int i, int j);
   void mapMazeToGraph(Graph &g);
   void printPath(Graph::vertex_descriptor end,
                        stack<Graph::vertex_descriptor> &s,
                        Graph g);
   int numRows(){return rows;};
   int numCols(){return cols;};

private:
   int rows; // number of rows in the maze
   int cols; // number of columns in the maze12 a
   
   matrix<bool> value;
   matrix<Graph::vertex_descriptor> vMap;
};

maze::maze(ifstream &fin)
// Initializes a maze by reading values from fin.  Assumes that the
// number of rows and columns indicated in the file are correct.
{
   fin >> rows;
   fin >> cols;
   
   char x;
   
   value.resize(rows,cols);
   for (int i = 0; i <= rows-1; i++)
      for (int j = 0; j <= cols-1; j++)
      {
         fin >> x;
         if (x == 'O')
            value[i][j] = true;
         else
            value[i][j] = false;
      }
   
   vMap.resize(rows,cols);
}

void maze::print(int goalI, int goalJ, int currI, int currJ)
// Print out a maze, with the goal and current cells marked on the
// board.
{
   cout << endl;

   if (goalI < 0 || goalI > rows || goalJ < 0 || goalJ > cols)
      throw rangeError("Bad value in maze::print");

   if (currI < 0 || currI > rows || currJ < 0 || currJ > cols)
      throw rangeError("Bad value in maze::print");

   for (int i = 0; i <= rows-1; i++)
   {
      for (int j = 0; j <= cols-1; j++)
      {
	 if (i == goalI && j == goalJ)
	    cout << "*";
	 else
	    if (i == currI && j == currJ)
	       cout << "+";
	    else
	       if (value[i][j])
		  cout << " ";
	       else
		  cout << "X";	  
      }
      cout << endl;
   }
   cout << endl;
}

bool maze::isLegal(int i, int j)
// Return the value stored at the (i,j) entry in the maze, indicating
// whether it is legal to go to cell (i,j).
{
   if (i < 0 || i > rows || j < 0 || j > cols)
      throw rangeError("Bad value in maze::isLegal");

   return value[i][j];
}

void maze::mapMazeToGraph(Graph &g)
// Create a graph g that represents the legal moves in the maze m.
{
	for (int i = 0; i < rows; i++)
	{
		for (int  j = 0; j < cols; j++)
		{
			vMap[i][j] = add_vertex(g);
			g[vMap[i][j]].cell = make_pair(i, j);
			g[vMap[i][j]].visited = false;
			g[vMap[i][j]].pred = vMap[i][j];
			g[vMap[i][j]].weight = 0;
			g[vMap[i][j]].marked = value[i][j];
		}
	}
	
	//create edges
}

// Prints the path represented by the vertices in stack s. Repeatedly
// calls print() to show each step of the path.
void maze::printPath(Graph::vertex_descriptor end,
	stack<Graph::vertex_descriptor> &s,
	Graph g)
{
	while (!s.empty())
	{
		print((rows - 1), (cols - 1), g[s.top()].cell.first, g[s.top()].cell.second);
		s.pop();
	}
}

void clearVisited(Graph &g)
// Mark all nodes in g as not visited.
{
	// Get a pair containing iterators pointing the beginning and end of the
	// list of nodes
	pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);

	// Loop over all nodes in the graph
	for (Graph::vertex_iterator vItr = vItrRange.first; vItr != vItrRange.second; ++vItr)
	{
		g[*vItr].visited = false;
	}
}

void setNodeWeights(Graph &g, int w)
// Set all node weights to w.
{

	// Get a pair containing iterators pointing the beginning and end of the
	// list of nodes
	pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);

	// Loop over all nodes in the graph
	for (Graph::vertex_iterator vItr = vItrRange.first; vItr != vItrRange.second; ++vItr)
	{
		g[*vItr].weight = w;
	}
}

void clearMarked(Graph &g)
// Unmark all nodes.
{
	// Get a pair containing iterators pointing the beginning and end of the
	// list of nodes
	pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);

	// Loop over all nodes in the graph
	for (Graph::vertex_iterator vItr = vItrRange.first; vItr != vItrRange.second; ++vItr)
	{
		g[*vItr].marked = false;
	}
}

ostream &operator<<(ostream &ostr, const Graph &g)
// Output operator for the Graph class. Prints out all nodes and their
// properties, and all edges and their properties.
{
	pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);

	for (Graph::vertex_iterator vItr = vItrRange.first; vItr != vItrRange.second; ++vItr)
	{
		cout << "Cell: " << g[*vItr].cell.first << ", " << g[*vItr].cell.second <<
			"     Visited: " << g[*vItr].visited << "     Marked: " << g[*vItr].marked <<
			"     Weight: " << g[*vItr].weight << "\n";
	}

	pair<Graph::edge_iterator, Graph::edge_iterator> eItrRange = edges(g);

	for (Graph::edge_iterator eItr = eItrRange.first; eItr != eItrRange.second; ++eItr)
	{
		cout << "Visited: " << g[*eItr].visited << "     Marked: " << g[*eItr].marked <<
			"     Weight: " << g[*eItr].weight << "\n";
	}
	return ostr;
}

