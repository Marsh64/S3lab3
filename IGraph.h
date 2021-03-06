//
// Created by adm on 12.12.2021.
//

#ifndef S3_LABORATORY_WORK_3_IGRAPH1_H
#define S3_LABORATORY_WORK_3_IGRAPH1_H
#include "Additional Stuctures/IUnorderedMap.h"
#include "Sequence/ArraySequence.h"
#include "Sequence/SortedSequence.h"
#include "Additional Stuctures/LinkedList.cpp"

template<class TWeight>
class Graph;

template<class TWeight>
class Path {
private:
    ArraySequence<size_t>* path;
    Graph<TWeight>* graph;
public:
    class Error{};

    Path(){
        path = new ArraySequence<size_t>;
        graph = new Graph<TWeight>();
    }

    explicit Path(ArraySequence<size_t>* pa, Graph<TWeight>* gr){
        path = pa;
        graph = gr;
        if (pa->GetLength() < 2)
            throw Error();
    }

    ~Path() {
        delete path;
    }

    TWeight GetLength(){
        TWeight weight = graph->GetWeight(path->Get(0), path->Get(1));

        for (int i = 1; i < path->GetLength() - 1; i++){
            weight += graph->GetWeight(path->Get(i), path->Get(i + 1));
        }

        return weight;
    }

    ArraySequence<size_t>* GetPath(){
        return path;
    }

    Path<TWeight> operator + (Path<TWeight>& other) {
        if (graph != other.graph)
            throw Error();
        if (path->operator[](path->GetLength() - 1) != other.path->operator[](0))
            throw Error();
        Path<TWeight> res(path, graph);

        for (int i = 1; i < other.path->GetLength(); i++) {
            res.path->Append(other.path->operator[](i));
        }

        return res;
    }
};

template<class TWeight>
class Graph {
private:
    static int hashfunc(const size_t& v1, size_t length) {
        return v1 % length;
    }

    static bool dijkstra_cmp(const std::pair<size_t, int>& el1, const std::pair<size_t, int>& el2) {
        return el1.second < el2.second;
    }

    static bool colouring_cmp(const std::pair<size_t, int>& p1, const std::pair<size_t, int>& p2) {
        return p1.second > p2.second;
    }

    ArraySequence<UnorderedMap<size_t, TWeight, hashfunc> *> adjlist;
//    UnorderedMap<TName, size_t, hashfunction> connection;
public:
    class ErrorInMissingVertex {

    };
    class ErrorInWrongVertex {

    };
    class ErrorInMissingEdge {

    };
    class Error {

    };

    Graph() {
    }

    void Remove() {
        for (int i = 0; i < adjlist.GetLength(); i++) {
            delete adjlist[i];
        }
        adjlist = ArraySequence<UnorderedMap<size_t, TWeight, hashfunc> *>();
    }

    void AddVertex() {
        adjlist.Append(nullptr);
    }//?????????????????? ???????? ??????????????

    void AddEdge(const size_t &name1, const size_t &name2, TWeight weight) {
        if (name1 == name2)
            throw Error();

        if (name1 > adjlist.GetLength() || name2 > adjlist.GetLength() || name1 < 0 || name2 < 0)
            throw ErrorInWrongVertex();

        if (adjlist.Get(name1) == nullptr) {
            auto *map = new UnorderedMap<size_t , TWeight, hashfunc>;
            map->Add(name2, weight);
            adjlist[name1] = map;
        } else {
            if (adjlist[name1]->ContainsKey(name2))
                adjlist[name1]->Remove(name2);
            //?????????????????? ?????????????????? ????????
            adjlist[name1]->Add(name2, weight);
        }

        if (adjlist.Get(name2) == nullptr) {
            auto *map = new UnorderedMap<size_t, TWeight, hashfunc>;
            map->Add(name1, weight);
            adjlist[name2] = map;
        } else {
            if (adjlist[name2]->ContainsKey(name1))
                adjlist[name2]->Remove(name1);
            //?????????????????? ?????????????????? ????????
            adjlist[name2]->Add(name1, weight);
        }
    }

    void RemoveEdge(const size_t &name1, const size_t &name2) {
        if (name1 == name2)
            return;

        if (name1 > adjlist.GetLength() || name2 > adjlist.GetLength() || name1 < 0 || name2 < 0)
            throw ErrorInWrongVertex();

        if (adjlist[name1]->ContainsKey(name2))
            adjlist[name1]->Remove(name2);
        else
            throw ErrorInMissingEdge();

        if (adjlist[name2]->ContainsKey(name1))
            adjlist[name2]->Remove(name1);
        else
            throw ErrorInMissingEdge();
    }//???????? ???? ?????????? ??????????, ???? ?????????????? ????????????

    void RemoveVertex(const size_t &name) {
        if (name > adjlist.GetLength() || name < 0)
            throw ErrorInWrongVertex();

        for (int i = 0; i < adjlist.GetLength(); i++) {
            if (!adjlist[i])
                continue;

            if (i == name)
                continue;//?????????????????? ?????????????? ?????????????? ?????????? ??????????????

            if (adjlist[i]->ContainsKey(name))
                adjlist[i]->Remove(name);
        }

        adjlist.Remove(name);

        for (int i = 0; i < adjlist.GetLength(); i++){
            if (!adjlist[i])
                continue;

            auto newMap = new UnorderedMap<size_t, TWeight, hashfunc>;

            for (auto val: *adjlist[i]){
                if (val.key > name){
                    newMap->Add(val.key - 1, val.element);
                }
                else{
                    newMap->Add(val.key, val.element);
                }
            }
            delete adjlist[i];
            adjlist[i] = newMap;
        }
    }

    TWeight GetWeight(const size_t& name1, const size_t& name2) {
        if (name1 == name2)
            throw Error();

        if (name1 >= adjlist.GetLength() || name2 >= adjlist.GetLength() || name1 < 0 || name2 < 0)
            throw ErrorInWrongVertex();

        if (!adjlist[name1] || !adjlist[name1]->ContainsKey(name2))
            throw ErrorInMissingEdge();

        return adjlist[name1]->Get(name2);
    }
    /*
    friend std::ostream &operator << (std::ostream &cout, Graph<TWeight, TName, hashfunction>& graph) {
        cout << graph.adjlist;
        return cout;
    }
     */

    friend std::ostream &operator << (std::ostream &cout, Graph<TWeight>& graph) {
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

    bool isAdjacent(const size_t &name1, const size_t &name2){
        if (name1 == name2)
            return false;

        if (name1 > adjlist.GetLength() || name2 > adjlist.GetLength() || name1 < 0 || name2 < 0)
            throw ErrorInWrongVertex();

        auto *list = adjlist[name1];

        if (list == nullptr)
            return false;

        for (auto val : *list) {
            if (val.key == name2)
                return true;
        }
        return false;
    }//???????????????????? ???????????? ???? ??????????????, ?????????????? ???????? ???????? ????????????

    int VertexDegree(const size_t &name){
        if (name >= adjlist.GetLength() || name < 0)
            throw ErrorInMissingVertex();

        if (adjlist[name] == nullptr)
            return 0;
        return adjlist[name]->GetCapasity();
    }//???????????? ?????????????? ??????????????

    ArraySequence<int>* Colouring() {
        ArraySequence<int>* colours = new ArraySequence<int>;
        if (adjlist.GetLength() == 0)
            return colours;

        SortedSequence<std::pair<size_t, int>, colouring_cmp> degree;
        for (int i = 0; i < adjlist.GetLength(); i++){
            degree.Add({i, VertexDegree(i)});//{??????????????, ?????????????? ??????????????}
            colours->Append(-1);
        }

        int clr = 0;//????????

        for (int h = 0; h < degree.GetLength(); h++) {
            if (colours->Get(h) != -1)
                continue;

            //?????????????????????? ?????????? ????????????
            for (int i = 0; i < degree.GetLength(); i++) {
                if (colours->Get(degree[i].first) != -1)
                    continue;
                bool log = true;//?????????? ???? ??????????????
                for (int m = 0; m < colours->GetLength(); m++) {
                    if (colours->Get(m) == clr) {
                        if ((isAdjacent(m, degree[i].first)))
                            log = false;
                    }
                }
                if (log)
                    colours->operator[](degree[i].first) = clr;
            }
            clr++;
        }

        return colours;
    }

    UnorderedMap<int, ArraySequence<size_t>*, umhashint>* Connectivity() {
        auto map = new UnorderedMap<int, ArraySequence<size_t>*, umhashint>;
        if (adjlist.GetLength() == 0)
            return map;

        UnorderedMap<size_t, int, hashfunc> colours;

        for (size_t i = 0; i < adjlist.GetLength(); i++) {
            colours.Add(i, 0);
        }

        int clr = 0;
        for (size_t i = 0; i < adjlist.GetLength(); i++) {
            if (colours.Get(i) != 0)
                continue;

            clr++;
            LinkedList<size_t> stack;
            stack.Append(i);
            colours.Swap(i, clr);
            //result->Append({i, clr});
            while (stack.GetLength()) {
                size_t& element = stack.Get(0);
                stack.Remove(0);

                auto arr = adjlist[element];
                if (!arr) {
                    continue;
                }
                for (auto j : *arr) {
                    //std::cout ?? j.key ?? ", ";
                    if (colours.Get(j.key))
                        continue;
                    colours.Swap(j.key, clr);
                    stack.Append(j.key);
                    //result->Append({j.key, clr});
                }
            }
        }

        for (auto val : colours){
            if (!map->ContainsKey(val.element)) {
                map->Add(val.element, new ArraySequence<size_t>);
            }

            map->operator[](val.element)->Append(val.key);

        }

        return map;
    }

    Path<TWeight> Dijkstra(const size_t& v1, const size_t& v2) {
        if (v1 >= adjlist.GetLength() || v2 >= adjlist.GetLength())
            throw ErrorInMissingVertex();

        auto* result = new ArraySequence<size_t>;
        if (v1 == v2) {
            result->Append(v1);
            return Path<TWeight>(result, this);
        }

        UnorderedMap<size_t, TWeight, hashfunc> distances;
        UnorderedMap<size_t, size_t, hashfunc> visits;

        SortedSequence<std::pair<size_t, int>, dijkstra_cmp> list;
        list.Add({v1, 0});
        distances.Add(v1, 0);
        visits.Add(v1, v1);

        while(list.GetLength()) {
            auto el = list.Get(0);
            list.Remove(0);
            //std::cout ?? list.GetLength() ?? std::endl;
            //td::cout ?? visits ?? std::endl ?? distances ?? "\n\n";

            auto *arr = adjlist[el.first];
            if (arr == nullptr)
                continue;

            for (auto vertex : *arr) {
                size_t &u = vertex.key;
                TWeight w_new = el.second + GetWeight(el.first, u);
                try {
                    int &w = distances[u];
                    if (w > w_new) {
                        w = w_new;
                        visits[u] = el.first;
                        for (int i = 0; i < list.GetLength(); i++) {
                            if (list[i].first == u) {
                                list.Remove(i);
                                break;
                            }
                        }
                        list.Add({u, w});
                    }
                }
                catch (typename UnorderedMap<size_t, int, hashfunc>::AbsenceOfIndex &err) {
                    distances.Add(u, w_new);
                    visits.Add(u, el.first);
                    list.Add({u, w_new});
                }
            }
        }

        if (!distances.ContainsKey(v2))
        return Path<TWeight>(result, this);

        auto v = v2;
        ArraySequence<size_t> tmp;
        while (v != v1) {
            tmp.Append(v);
            v = visits[v];
        }
        result->Append(v1);
        for (int i = tmp.GetLength() - 1; i >= 0; i--) {
            result->Append(tmp[i]);
        }

        return Path<TWeight>(result, this);
    }
};

#endif //S3_LABORATORY_WORK_3_IGRAPH1_H
