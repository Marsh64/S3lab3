#include <iostream>
#include "Additional Stuctures/IDictionary.h"
#include "Sequence/ArraySequence.h"
#include "Sequence/LinkedListSequence.h"
#include "Bin/IDirectedGraphLL.h"
#include "Additional Stuctures/IUnorderedMap.h"
#include "Bin/IDirectedGraphLLCon.h"
#include "Bin/IGraphLLCon.h"
#include "Bin/IGraphUMCon.h"
#include "IGraph.h"
#include "TestGraph.h"

int main() {

    std::cout << "Hello, World!" << std::endl;
    testAdding();
    std::cout << "Hello, World!" << std::endl;
    testRemoving();
    //ArraySequence<int> seq;
    //seq.Append(5);
    //seq.Append(6);
    //std::cout << seq.GetFirst() << std::endl;
    //std::cout << seq.GetLength() << std::endl;
    //std::cout << seq.Get(1) << std::endl;
    /*
    Graph<int, int> graph;
    int n1 = 1111111;
    int n2 = 2222222;
    int n3 = 3333333;
    int n4 = 4444444;
    int n5 = 5555555;

    graph.AddVertex(n1);
    graph.AddVertex(n2);
    graph.AddVertex(n3);
    graph.AddVertex(n4);
    //graph.AddVertex(n5);

    graph.AddEdge(n1, n2, 5);
    graph.AddEdge(n2, n4, 5);
    graph.AddEdge(n2, n3, 5);
    graph.AddEdge(n3, n1, 5);
    graph.AddEdge(n1, n4, 5);
    //graph.AddEdge(n2, n4, 6);
    //graph.AddEdge(n3, n1, 10);
    //graph.AddEdge(n3, n2, 66);
    //graph.AddEdge(n3, n4, 15);
    //graph.AddEdge(n4, n1, 15);
    //graph.AddEdge(n4, n2, 15);
    //graph.AddEdge(n4, n3, 15);
    //std::cout << graph << std::endl;
    //graph.RemoveEdge(n1, n2);
    //graph.AddEdge(n1, n2, 5);
    //graph.RemoveEdge(n1, n3);
    //graph.RemoveVertex(n1);
    std::cout << graph << std::endl;
    std::cout << *graph.Colouring() << std::endl;

    //std::cout << graph.isAdjacent(0, 1) << std::endl;
    //std::cout << graph.VertexDegree(0) << std::endl;
    //std::cout << graph.VertexDegree(1) << std::endl;

    //std::cout << graph. << std::endl;
    //graph.AddEdge(1, 2, 5);
    //graph.AddEdge(1, 1, 6);
    //graph.AddEdge(1, 3, 6);
    //graph.RemoveVertex(2);


    std::cout << "Hello, World!" << std::endl;

    /*
    UnorderedMap<std::string, int, umhashstr> map;

    for (int i = 0; i < 10; i++) {
        map.Add(std::to_string(i*10+31), i);
    }

    std::cout << map << '\n';

    for (int i = 3; i < 7; i++) {
        map.Remove(std::to_string(i*10+31));
    }

    std::cout << map << '\n';

    std::cout << map.GetCapasity() << std::endl;
    std::cout << map.ContainsKey("101") << std::endl;
    std::cout << map.Get("41") << std::endl;
    map.Swap("41", 1000);
    std::cout << map.Get("41") << std::endl;
    std::cout << map["41"] << std::endl;
    map["41"] = 58;
    std::cout << map["41"] << std::endl;
    std::cout << map.ContainsKey("61") << std::endl;

    */
    //std::cout << map;
    return 228;
}
