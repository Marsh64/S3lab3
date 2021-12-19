//
// Created by adm on 09.12.2021.
//

#ifndef S3_LABORATORY_WORK_3_IDIRECTEDGRAPHUM_H
#define S3_LABORATORY_WORK_3_IDIRECTEDGRAPHUM_H
#include "Additional Stuctures/IUnorderedMap.h"
#include "Sequence/ArraySequence.h"
#include "Sequence/LinkedListSequence.h"

template<class TWeight, class TName, int (*hashfunction)(const TName&, size_t) = umhashint>
class DirectedGraphUM{
private:
    struct Node{
        TWeight weight;
        size_t vertex;

        friend std::ostream& operator<< (std::ostream &out, const  Node &nd){
            return std::cout << "[w: " << nd.weight<< "; vrtx: " << nd.vertex << "]";
        }
    };

    ArraySequence<LinkedListSequence<Node>*> adjlist;
    UnorderedMap<TName, size_t, hashfunction> connection;

    void RemoveEdgePrivate(size_t v1, size_t v2) {
        //if (v1 == v2)
        //    throw Error();//указатели указывают на одну вершину

        if (adjlist[v1]->GetLength() == 0)
            throw ErrorInMissingEdge();

        for (int i = 0; i < adjlist[v1]->GetLength(); i++){
            if (adjlist[v1]->Get(i).vertex == v2){
                adjlist[v1]->Remove(i);
                break;
            }
        }
    }
public:
    class ErrorInMissingVertex{};
    class ErrorInMissingEdge{};
    class Error{};

    DirectedGraphUM(){
        adjlist = ArraySequence<LinkedListSequence<Node>*>();
        //connection = UnorderedMap<int, int, hashint>();
    }

    void AddVertex(const TName& name) {
        adjlist.Append(nullptr);
        connection.Add(name, adjlist.GetLength() - 1);
    }//Добавляет одну вершину

    void AddEdge(const TName& name1, const TName& name2, TWeight weight){
        if (!(connection.ContainsKey(name1) && connection.ContainsKey(name2)))
            throw ErrorInMissingVertex();

        if (connection.Get(name1) >= adjlist.GetLength() || connection.Get(name2) >= adjlist.GetLength()
                                                         || connection.Get(name1) < 0 || connection.Get(name2) < 0)
            throw ErrorInMissingVertex();

        size_t v1 = connection.Get(name1);
        size_t v2 = connection.Get(name2);
        if (v1 == v2)
            throw Error();

        if (adjlist.Get(v1) == nullptr){
            auto* list = new LinkedListSequence<Node>;
            list->Append({weight, v2});
            adjlist[v1] = list;
        }
        else{
            for (auto &val : *adjlist[v1]) {
                if (val.vertex == v2)
                    throw Error();//повторная дуга
            }
            adjlist[v1]->Append({weight, v2});
        }
    }

    void RemoveEdge(const TName& name1, const TName& name2){
        if (!(connection.ContainsKey(name1) && connection.ContainsKey(name2)))
            throw ErrorInMissingVertex();//указатели указывают не на вершины

        if (connection.Get(name1) >= adjlist.GetLength() || connection.Get(name2) >= adjlist.GetLength()
            || connection.Get(name1) < 0 || connection.Get(name2) < 0)
            throw ErrorInMissingVertex();//какая-то левая вершина

        int v1 = connection.Get(name1);
        int v2 = connection.Get(name2);
        RemoveEdgePrivate(v1, v2);
    }

    void RemoveVertex(const TName& name){
        if (!(connection.ContainsKey(name)))
            throw ErrorInMissingVertex();//указатели указывают не на вершины
        if (connection.Get(name) >= adjlist.GetLength() || connection.Get(name) < 0)
            throw ErrorInMissingVertex();//какая-то левая вершина

        size_t vrtx = connection.Get(name);
        adjlist.Remove(vrtx);

        for (auto val : connection){
            if (val.element > vrtx)
                val.element--;
        }

        for (int i = 0; i < adjlist.GetLength(); i++){
            if (!adjlist[i])
                continue;

            for (auto &val : *adjlist[i]) {
                if (val.vertex == vrtx)
                    RemoveEdgePrivate(i, val.vertex);
            }
        }

        for (int i = 0; i < adjlist.GetLength(); i++){
            if (!adjlist[i])
                continue;

            for (auto &val : *adjlist[i]) {
                if (val.vertex > vrtx)
                    val.vertex--;
            }
        }
    }// по индексу в массиве

    friend std::ostream &operator << (std::ostream &cout, DirectedGraphUM<TWeight, TName, hashfunction>& graph) {
        cout << graph.adjlist;
        return cout;
    }

};


#endif //S3_LABORATORY_WORK_3_IDIRECTEDGRAPHUM_H
