//
// Created by adm on 20.12.2021.
//

#include "TestGraph.h"
#include <iostream>
#include <cassert>

void testAdding(){
    srand(679);
    Graph<int, int> graph;

    int len = rand()%100 + 1;
    for (int i = 0; i < len; i++)
        graph.AddVertex(i);

    assert(graph.Colouring()->GetLength() == len);
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

void testRemoving(){
    srand(679);
    Graph<int, int> graph1;
    Graph<int, int> graph2;
    Graph<int, int> graph3;

    int len = rand()%100;
    for (int i = 0; i < len; i++){
        graph1.AddVertex(i);
        graph2.AddVertex(i);
        graph3.AddVertex(i);
    }

    for (int i = 0; i < len%25; i++){
        graph1.RemoveVertex(i);
        assert(graph1.Colouring()->GetLength() == len - i - 1);
    }
    //Проверялось удаление вершин из графа содержащего только веришны

    for (int i = 0; i < len; i++){
        for (int j = 0; j < len; j++){
            if (i == j)
                continue;

            int w = rand()%25;
            graph2.AddEdge(i, j, w);
            graph3.AddEdge(i, j, w);
        }
    }//сделали графы graph2 и graph3 полными

    for (int i = 0; i < len; i++){
        for (int j = 0; j < len; j++){
            if (i == j)
                continue;

            graph2.RemoveEdge(i, j);
            assert(!(graph2.isAdjacent(i, j)));
            assert(!(graph2.isAdjacent(j, i)));
        }
        assert(graph2.VertexDegree(i) == 0);
    }
    //проверили удаление ребер из графа

    for (int i = 0; i < len%50; i++){
        graph3.RemoveVertex(i);
        assert(graph1.Colouring()->GetLength() == len - i - 1);
        for (int j = 0; j < len - i - 1; j++){
            assert(graph3.VertexDegree(j) == len - i - 2);
        }
    }
    //проверили удаление вершин из полного графа

}