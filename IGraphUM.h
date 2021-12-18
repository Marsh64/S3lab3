//
// Created by adm on 09.12.2021.
//

#ifndef S3_LABORATORY_WORK_3_IGRAPHUM_H
#define S3_LABORATORY_WORK_3_IGRAPHUM_H
#include "Additional Stuctures/IUnorderedMap.h"
#include "ArraySequence.h"
#include "Sequence/LinkedListSequence.h"
#include "Sequence/SortedSequence.h"

template<class TWeight, class TName, int (*hashfunction)(const TName&, size_t) = umhashint>
class GraphUM {
private:
    struct Node {
        TWeight weight;
        size_t vertex;

        friend std::ostream &operator<<(std::ostream &out, const Node &nd) {
            return std::cout << "[w: " << nd.weight << "; vrtx: " << nd.vertex << "]";
        }
    };

    /*
    struct Pair{
        size_t vertex;
        size_t degree;

        friend std::ostream &operator > (Pair& p1, Pair& p2) {
            return p1.degree > p2.degree;
        }
    };
     */

    ArraySequence<LinkedListSequence<Node> *> adjlist;
    UnorderedMap<TName, size_t, hashfunction> connection;

    void RemoveEdgePrivate(size_t v1, size_t v2) {
        //if (v1 == v2)
        //    throw Error();//указатели указывают на одну вершину

        if (adjlist[v1] == nullptr)
            throw ErrorInMissingEdge(); //если у вершины нет смежных

        if (adjlist[v1]->GetLength() == 0)
            throw ErrorInMissingEdge();

        for (int i = 0; i < adjlist[v1]->GetLength(); i++) {
            if (adjlist[v1]->Get(i).vertex == v2) {
                adjlist[v1]->Remove(i);
                break;
            }
        }
        for (int i = 0; i < adjlist[v2]->GetLength(); i++) {
            if (adjlist[v2]->Get(i).vertex == v1) {
                adjlist[v2]->Remove(i);
                break;
            }
        }
    }

public:
    class ErrorInMissingVertex {
    };

    class ErrorInMissingEdge {
    };

    class Error {
    };

    GraphUM() {
        adjlist = ArraySequence<LinkedListSequence<Node> *>();
        //connection = UnorderedMap<int, int, hashint>();
    }

    void AddVertex(const TName &name) {
        adjlist.Append(nullptr);
        connection.Add(name, adjlist.GetLength() - 1);
    }//Добавляет одну вершину

    void AddEdge(const TName &name1, const TName &name2, TWeight weight) {
        if (!(connection.ContainsKey(name1) && connection.ContainsKey(name2)))
            throw ErrorInMissingVertex();

        if (connection.Get(name1) >= adjlist.GetLength() || connection.Get(name2) >= adjlist.GetLength()
            || connection.Get(name1) < 0 || connection.Get(name2) < 0)
            throw ErrorInMissingVertex();

        size_t v1 = connection.Get(name1);
        size_t v2 = connection.Get(name2);
        if (v1 == v2)
            throw Error();

        if (adjlist.Get(v1) == nullptr) {
            auto *list = new LinkedListSequence<Node>;
            list->Append({weight, v2});
            adjlist[v1] = list;
        } else {
            bool log = false;
            for (auto &val : *adjlist[v1]) {
                if (val.vertex == v2){
                    val.weight = weight;//повторная дуга - меняем вес на новый
                    log = true;
                }
            }
            if (!log)
                adjlist[v1]->Append({weight, v2});
        }

        if (adjlist.Get(v2) == nullptr) {
            auto *list = new LinkedListSequence<Node>;
            list->Append({weight, v1});
            adjlist[v2] = list;
        } else {
            bool log = false;
            for (auto &val : *adjlist[v2]) {
                if (val.vertex == v1){
                    val.weight = weight;//повторная дуга - меняем вес на новый
                    log = true;
                }
            }
            if (!log)
                adjlist[v2]->Append({weight, v1});
        }
    }

    void RemoveEdge(const TName &name1, const TName &name2) {
        if (!(connection.ContainsKey(name1) && connection.ContainsKey(name2)))
            throw ErrorInMissingVertex();//указатели указывают не на вершины

        if (connection.Get(name1) >= adjlist.GetLength() || connection.Get(name2) >= adjlist.GetLength()
            || connection.Get(name1) < 0 || connection.Get(name2) < 0)
            throw ErrorInMissingVertex();//какая-то левая вершина

        int v1 = connection.Get(name1);
        int v2 = connection.Get(name2);
        RemoveEdgePrivate(v1, v2);
    }

    void RemoveVertex(const TName &name) {
        if (!(connection.ContainsKey(name)))
            throw ErrorInMissingVertex();//указатели указывают не на вершины
        if (connection.Get(name) >= adjlist.GetLength() || connection.Get(name) < 0)
            throw ErrorInMissingVertex();//какая-то левая вершина

        size_t vrtx = connection.Get(name);

        for (int i = 0; i < adjlist.GetLength(); i++) {
            if (!adjlist[i])
                continue;

            if ( i == vrtx)
                continue;//пропустим столбец который нужно удалить

            for (auto &val : *adjlist[i]) {
                if (val.vertex == vrtx)
                    RemoveEdgePrivate(i, val.vertex);
            }
        }

        adjlist.Remove(vrtx);

        connection.Remove(name);
        for (auto val : connection) {
            if (val.element > vrtx)
                val.element--;
        }

        /*
        for (int i = 0; i < adjlist.GetLength(); i++) {
            if (!adjlist[i])
                continue;

            for (auto &val : *adjlist[i]) {
                if (val.vertex > vrtx)
                    val.vertex--;
            }
        }
         */
    }// по индексу в массиве

    friend std::ostream &operator << (std::ostream &cout, GraphUM<TWeight, TName, hashfunction>& graph) {
        cout << graph.adjlist;
        return cout;
    }

    /*
    bool isAdjacent(size_t v1, size_t v2){
        if (v1 >= adjlist.GetLength() || v1 < 0 || v2 >= adjlist.GetLength() || v2 < 0)
            throw ErrorInMissingVertex();

        for (auto &val : *adjlist[v1]) {
            if (val.vertex == v2)
                return true;
        }
        return false;
    }//показывает смежны ли вершины

    size_t VertexDegree(size_t v){
        if (v >= adjlist.GetLength() || v < 0)
            throw ErrorInMissingVertex();

        size_t deg = 0;

        return adjlist[v]->GetLength();
    }//выдает степень вершины
    */
    /*
    ArraySequence<int> Colourize(){
        SortedSequence<Pair> degree;
        for (int i = 0; i < this->adjlist.GetLength(); i++){
            degree.Add({i, VertexDegree(i)});
        }//сортированная последовательность с степенями вершин

        //цвета обозначены цифрами
        ArraySequence<int> colors (adjlist.GetLength());
        colors[degree[0].vertex] = 1;
        //degree.Remove(0);
        int clr = 1;//цвет
        int j = 1;
        while (degree.GetLength() != 0){
            for (int k = 0; k < degree.GetLength(); k ++){


                for (int i = j; i < degree.GetLength(); i++){
                    if (!(isAdjacent(degree[i].vertex, j - 1)){
                        colors[degree[i].vertex] = clr;
                        j++;
                        break;
                    }
                }


            }
            //окрашиваем все несмежные вершины
            clr++;
            //удаляем элементы из degree, которые добавили
        }


    }
    */
};
/*
bool isAdjacent(size_t v1, size_t v2){
    if (v1 >= adjlist.GetLength() || v1 < 0 || v2 >= adjlist.GetLength() || v2 < 0)
        throw ErrorInMissingVertex();

    bool log = false;
    for (auto &val : *adjlist[v1]) {
        if (val.vertex == v2)
            return true;
    }

    for (int i = 0; i < adjlist.GetLength(); i++){
        if (i == v1)
            continue;
        if (!adjlist[i])
            continue;
        for (auto &val : *adjlist[i]) {
            if (val.vertex == v1)
                RemoveEdgePrivate(i, val.vertex);
        }
    }
}//раскрашиваем как обычный граф

size_t VertexDegree(size_t v){
    if (v >= adjlist.GetLength() || v < 0)
        throw ErrorInMissingVertex();

    size_t deg = 0;

    if (table[])
    for (auto &val : *adjlist[v1]) {
        if (val.vertex == v2)
            return true;
    }

    for (int i = 0; i < adjlist.GetLength(); i++){
        if (i == v1)
            continue;
        if (!adjlist[i])
            continue;
        for (auto &val : *adjlist[i]) {
            if (val.vertex == v1)
                RemoveEdgePrivate(i, val.vertex);
        }
    }
}//считаем как в обычном графе
 */



#endif //S3_LABORATORY_WORK_3_IGRAPHUM_H
