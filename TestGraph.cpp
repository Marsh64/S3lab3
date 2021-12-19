//
// Created by adm on 20.12.2021.
//

#include "TestGraph.h"
#include <iostream>
#include <cassert>

void testAdding(){
    srand(679);
    Graph<int, int> graph;

    int len = rand()%100;
    for (int i = 0; i < len; i++)
        graph.AddVertex(i);

    assert(graph.Connectivity()->GetLength() == len);
    //Кол-во вершин проверяется с помощью связности

    for (int i = 0; i < len; i++){
        for (int j = 0; j < len; j++){
            if (i == j)
                continue;

            int w = rand()%25;
            graph.AddEdge(i, j, w);

            assert(graph.isAdjacent(i, j));
            assert(graph.isAdjacent(j, i));
            assert(graph.getWeight(i, j) == w);
            assert(graph.getWeight(j, i) == w);
        }
        assert(graph.VertexDegree(i) == len - 1);
    }
    //Добавляет ребро и сразу проверяет на смежность с обоих сторон
    //Также проверяет подстановку необходимого веса вершине
    //Также проверяет степень вершины с помощью добавления ребер
}//Проверяется добавление вершин и добавление ребер