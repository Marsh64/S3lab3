//
// Created by adm on 12.12.2021.
//

#ifndef S3_LABORATORY_WORK_3_IGRAPH_H
#define S3_LABORATORY_WORK_3_IGRAPH_H
#include "IUnorderedMap.h"
#include "ArraySequence.h"
#include "SortedSequence.h"

template<class TWeight, class TName, int (*hashfunction)(const TName&, size_t) = umhashint>
class Graph {
private:
    struct Node {
        TWeight weight;
        size_t vertex;

        friend std::ostream &operator<<(std::ostream &out, const Node &nd) {
            return std::cout << "[w: " << nd.weight << "; vrtx: " << nd.vertex << "]";
        }
    };



    struct Trio{
        TName name;
        int vertex;
        int degree;

        friend bool operator > (const Trio& p1, const Trio& p2) {
            return p1.degree > p2.degree;
        }
        friend std::ostream& operator<< (std::ostream& out, const Trio& pr){
            return std::cout << "[vrtx: " << pr.vertex <<"; name: "<< pr.name << "; st: " << pr.degree << "]";
        }
    };

    ArraySequence<UnorderedMap<TName, Node, hashfunction> *> adjlist;
    UnorderedMap<TName, size_t, hashfunction> connection;
public:
    class ErrorInMissingVertex {
    };
    class ErrorInMissingEdge {
    };
    class Error {
    };

    Graph() {
        //adjlist = ArraySequence<UnorderedMap<TName, Node, hashfunction>>();
        //connection = UnorderedMap<int, int, hashint>();
    }

    void AddVertex(const TName &name) {
        if (connection.ContainsKey(name))
            throw Error();

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
            auto *map = new UnorderedMap<TName, Node, hashfunction>;
            map->Add(name2, {weight, v2});
            adjlist[v1] = map;
        } else {
            if (adjlist[v1]->ContainsKey(name2))
                adjlist[v1]->Remove(name2);
            //возникает повторная дуга
            adjlist[v1]->Add(name2, {weight, v2});
        }

        if (adjlist.Get(v2) == nullptr) {
            auto *map = new UnorderedMap<TName, Node, hashfunction>;
            map->Add(name1, {weight, v1});
            adjlist[v2] = map;
        } else {
            if (adjlist[v2]->ContainsKey(name1))
                adjlist[v2]->Remove(name1);
            //возникает повторная дуга
            adjlist[v2]->Add(name1, {weight, v1});
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

        if (adjlist[v1]->ContainsKey(name2))
            adjlist[v1]->Remove(name2);
        else
            throw ErrorInMissingEdge();
        if (adjlist[v2]->ContainsKey(name1))
            adjlist[v2]->Remove(name1);
        else
            throw ErrorInMissingEdge();
    }//если не будет связи, то вылезет ошибка

    void RemoveVertex(const TName &name) {
        if (!(connection.ContainsKey(name)))
            throw ErrorInMissingVertex();//указатели указывают не на вершины
        if (connection.Get(name) >= adjlist.GetLength() || connection.Get(name) < 0)
            throw ErrorInMissingVertex();//какая-то левая вершина

        size_t vrtx = connection.Get(name);

        for (int i = 0; i < adjlist.GetLength(); i++) {
            if (!adjlist[i])
                continue;

            if (i == vrtx)
                continue;//пропустим столбец который нужно удалить

            if (adjlist[i]->ContainsKey(name))
                adjlist[i]->Remove(name);
        }

        adjlist.Remove(vrtx);
        connection.Remove(name);

        for (auto val : connection) {
            if (val.element > vrtx)
                val.element--;
        }
    }
    /*
    friend std::ostream &operator << (std::ostream &cout, Graph<TWeight, TName, hashfunction>& graph) {
        cout << graph.adjlist;
        return cout;
    }
     */

    friend std::ostream &operator << (std::ostream &cout, Graph<TWeight, TName, hashfunction>& graph) {
        cout << '(';
        for (int i = 0; i < graph.adjlist.GetLength(); i++) {
            cout << '{';
            if (!graph.adjlist[i]) {
                cout << "}, ";
                continue;
            }
            for (const auto &j : *graph.adjlist[i]) {
                cout << j.element << ", ";
            }
            cout << "\b\b}, ";
        }
        return cout << "\b\b)";
    }

    bool isAdjacent(size_t v1, size_t v2){
        if (v1 >= adjlist.GetLength() || v1 < 0 || v2 >= adjlist.GetLength() || v2 < 0)
            throw ErrorInMissingVertex();

        if (adjlist[v1] == nullptr)
            return 0;


        for (auto val : *adjlist[v1]) {
            if (val.element.vertex == v2)//////////////////////////////////////////////////////////////////////////////
                return true;
        }
        return false;
    }//показывает смежны ли вершины(c ссылками позже сделаю)

    int VertexDegree(size_t v){
        if (v >= adjlist.GetLength() || v < 0)
            throw ErrorInMissingVertex();

        if (adjlist[v] == nullptr)
            return 0;
        return adjlist[v]->GetCapasity();
    }//выдает степень вершины(с сылками позже сделаю)

    TWeight getWeight(const TName& v1, const TName& v2) {
        if (!(connection.ContainsKey(v1)) || !(connection.ContainsKey(v2)))
            throw ErrorInMissingVertex();//указатели указывают не на вершины

        int v_1 = connection.Get(v1);
        int v_2 = connection.Get(v2);

        if (!adjlist[v_1] || !adjlist[v_1]->ContainsKey(v2))
            throw ErrorInMissingEdge();

        return adjlist[v_1]->Get(v2).weight;
    }

    ArraySequence<std::pair<TName, int>>* Colouring(){
        ArraySequence<std::pair<TName, int>>* colours = new ArraySequence<std::pair<TName, int>>;
        if (adjlist.GetLength() == 0)
            return colours;

        SortedSequence<Trio> degree;
        for (auto u : connection)
            degree.Add({u.key, (int)u.element, VertexDegree(u.element)});

        //for (auto u : connection) {
        //    colours->Append({u.key, 0});
        //}
        TName nm = connection.begin()->key;
        for (int i = 0; i < adjlist.GetLength(); i++)
            colours->Append({nm, 0});

        int clr = 0;//цвет
        std::cout << degree << std::endl;
        //std::cout << connection << std::endl;

        for (int h = 0; h < degree.GetLength(); h++){
            clr++;

            for (int j = 0; j < degree.GetLength(); j++) {
                int i = degree[j].vertex;
                //int i = u.element;
                if (colours->Get(degree[i].vertex).second != 0)
                    continue;
                bool log = true;//стоит ли красить
                for (int m = 0; m < colours->GetLength(); m++){
                    if (colours->Get(m).second == clr){
                        if ((isAdjacent(m, degree[i].vertex)))
                            log = false;
                    }
                }
                if (log){
                    (*colours)[degree[i].vertex].first = degree[i].name;
                    (*colours)[degree[i].vertex].second = clr;
                }
            }
        }

        /*
        for (int h = 0; h < degree.GetLength(); h++){
            if (colours->Get(h) == 0){
                //закрашиваем одним цветов

                for (int i = 0; i < degree.GetLength(); i++){
                    if (colours->Get(degree[i].vertex) != 0)
                        continue;

                    bool log = true;//стоит ли красить
                    for (int m = 0; m < colours->GetLength(); m++){
                        if (colours->Get(m) == clr){
                            if ((isAdjacent(m, degree[i].vertex)))
                                log = false;
                        }
                    }
                    if (log)
                        colours->Get(degree[i].vertex) = clr;
                }
                clr++;
            }
        }*/

        return colours;
    }

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

template<class T1, class T2>
std::ostream &operator<<(std::ostream &out, const std::pair<T1, T2> &nd) {
    return std::cout << "[name: " << nd.first << "; colour: " << nd.second << "]";
}

#endif //S3_LABORATORY_WORK_3_IGRAPH_H
