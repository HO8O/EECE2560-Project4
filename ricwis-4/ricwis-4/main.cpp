#include <iostream>
#include <limits.h>
#include <vector>
#include <list>
#include <fstream>
#include <queue>

#include <boost/graph/adjacency_list.hpp>
#include "d_except.h"
#include "d_matrix.h"
#include "maze.h"

//#include "heapV.h"

using namespace boost;
using namespace std;
   
int main()
{
   try
   {
      ifstream fin;

      // Read the maze from the file.
	  string fileName;
	  cout << "Enter path to puzzle : ";
	  cin >> fileName;
      
      fin.open(fileName.c_str());
      if (!fin)
      {
         cerr << "Cannot open " << fileName << endl;
         exit(1);
      }

      maze m(fin);
      fin.close();
      
      m.print(m.numRows()-1,m.numCols()-1,0,0);

      Graph g;
      m.mapMazeToGraph(g);

      //cout << g << endl;
   }
   catch (baseException b)
   {

   }
}
