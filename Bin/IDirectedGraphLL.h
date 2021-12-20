//
// Created by adm on 06.12.2021.
//

#ifndef S3_LABORATORY_WORK_3_IDIRECTEDGRAPHLL_H
#define S3_LABORATORY_WORK_3_IDIRECTEDGRAPHLL_H
#include "../Sequence/ArraySequence.h"
#include "../Sequence/LinkedListSequence.h"

template<class TWeight>
class DirectedGraphLL{
private:
    struct Node{
        TWeight weight;
        size_t vertex;

        friend std::ostream& operator<< (std::ostream &out, const  Node &nd){
            return std::cout << "[w: " << nd.weight<< "; vrtx: " << nd.vertex << "]";
        }
    };

    ArraySequence<LinkedListSequence<Node>*> adjlist;
public:
    class ErrorInMissingVertex{};
    class ErrorInMissingEdge{};
    class Error{};


    DirectedGraphLL(){
        adjlist = ArraySequence<LinkedListSequence<Node>*>();
    }
    explicit DirectedGraphLL(size_t count){
        ArraySequence<LinkedListSequence<Node>*> newadj((DynamicArray<LinkedListSequence<Node>*>(count)));
        adjlist = ArraySequence<LinkedListSequence<Node>*>((DynamicArray<LinkedListSequence<Node>*>(count)));
        for (int i = 0; i < adjlist.GetLength(); i++)
            adjlist[i] = nullptr;
    }//сразу создает count кол-во вершин

    void AddVertex(size_t count){
        for(int i = 0 ; i < count; i++){
            adjlist.Append(nullptr);
        }
    }//Добавляет count кол-во вершин
    void AddVertex(){
        adjlist.Append(nullptr);
    }//Добавляет одну вершину
    void AddEdge(size_t v1, size_t v2, TWeight weight){
        if (v1 > adjlist.GetLength() || v2 > adjlist.GetLength())
            throw ErrorInMissingVertex();

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
    void RemoveEdge(size_t v1, size_t v2){
        if (v1 > adjlist.GetLength() || v2 > adjlist.GetLength())
            throw ErrorInMissingVertex();

        if (adjlist[v1]->GetLength() == 0)
            throw ErrorInMissingEdge();

        for (int i = 0; i < adjlist[v1]->GetLength(); i++){
            if (adjlist[v1]->Get(i).vertex == v2){
                adjlist[v1]->Remove(i);
                break;
            }
        }
    }
    void RemoveVertex(size_t vrtx){
        adjlist.Remove(vrtx);

        for (int i = 0; i < adjlist.GetLength(); i++){
            if (!adjlist[i])
                continue;

            for (auto &val : *adjlist[i]) {
                if (val.vertex == vrtx)
                    RemoveEdge(i, val.vertex);
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
    }


    friend std::ostream &operator << (std::ostream &cout, DirectedGraphLL<TWeight>& graph) {
        cout << graph.adjlist;
        return cout;
    }

};


#endif //S3_LABORATORY_WORK_3_IDIRECTEDGRAPHLL_H
