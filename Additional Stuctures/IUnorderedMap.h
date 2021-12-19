//
// Created by adm on 09.12.2021.
//

#ifndef S3_LABORATORY_WORK_3_IUNORDEREDMAP_H
#define S3_LABORATORY_WORK_3_IUNORDEREDMAP_H
#include <iostream>
#include <string>
#include "DynamicArray.cpp"

static int umhashstr(const std::string& key, size_t size)
{
    int asciisum = 7;
    for (char i : key)
    {
        asciisum = asciisum * 31 + i;
    }
    return int (asciisum % size);
}
static int umhashint(const int& key, size_t size){
    return (int)(key % size);
}

template <class TKey, class TElement, int (*hashfunction)(const TKey&, size_t)>
class UnorderedMap {
private:
    struct Node{
        TKey key;
        TElement element;
        Node* next;
        Node* ifol; //следующий в итерации
        Node* iprev; //предыдущий в итерации
    };

    Node* start = nullptr; //указатель на самый первый элемент
    DynamicArray<Node*> table;

    size_t size = table.GetLength(); //Кол-во ячеек в массиве
    size_t amount; //Кол-во элементов во всей таблице

    friend std::ostream &operator << (std::ostream &cout, Node& nd) {
        std::cout << '(';
        std::cout << nd.key << ": " << nd.element;
        std::cout << ')';
        return cout;
    }


    void Magnification(){
        double occupacity = double (amount)/ size;
        if (occupacity >= 0.75){
            int newsize = table.GetLength() * 2;

            int CountMemory;
            if (newsize >= table.GetCountMemory()){
                CountMemory = int(newsize * 1.5);
            }else{
                CountMemory = table.GetCountMemory();
            }
            DynamicArray<Node*> newtable (newsize, CountMemory);

            for (int i = 0; i < newtable.GetLength(); i++) {
                newtable[i] = nullptr;
            }

            int oldamount = amount;
            amount = 0;

            for (int index = 0; index < table.GetLength(); index++){
                for (auto temp = table[index]; temp != nullptr; temp = temp->next){
                    int newindex = hashfunction(temp->key, newsize);
                    amount++;

                    if (amount == 1){
                        start = new Node{temp->key, temp->element, nullptr, nullptr, nullptr};
                        newtable.Set(newindex, start);
                    }else{
                        if (newtable[newindex] == nullptr){
                            newtable.Set(newindex, new Node{temp->key, temp->element, nullptr, start, nullptr});
                            start->iprev = newtable[newindex];
                            start = newtable[newindex];
                            continue;
                        }//появляется новый столбец(поэтому меням len)

                        auto tmp = newtable[newindex];
                        for (tmp; tmp->next != nullptr; tmp = tmp->next){}
                        tmp->next = new Node{temp->key, temp->element, nullptr, tmp->ifol, tmp};
                        if (tmp->ifol)
                            tmp->ifol->iprev = tmp->next;
                        tmp->ifol = tmp->next;
                    }
                }
            }

            size = newsize; //newtable.GetLength();

            //Удаляем предыдущую
            for (int i = 0; i < table.GetLength(); i++) {
                auto temp = table[i];
                while (temp != nullptr) {
                    auto del = temp;
                    temp = temp->next;
                    delete del;
                }
            }

            table = newtable;
        }
    }//Увеличивает таблицу, пересобирает

public:
    class AbsenceOfIndex{};

    struct Pair {
        TKey& key;
        TElement& element;
    };

    friend std::ostream& operator << (std::ostream& cout, const Pair& pair) {
        return cout << '(' << pair.key << ": " << pair.element << ')';
    }

    UnorderedMap(){
        table.Resize(8);
        amount = 0;
        size = table.GetLength();
        for (int i = 0; i < size; i++) {
            table[i] = nullptr;
        }
        start = nullptr;
    }// По-умолчанию делает словарь из 8 элементов

    explicit UnorderedMap(int newsize){
        table.Resize(newsize);
        size = newsize;
        amount = 0;
        start = nullptr;
    }

    ~UnorderedMap(){
        for (int i = 0; i < table.GetLength(); i++) {
            auto temp = table[i];
            while (temp != nullptr) {
                auto del = temp;
                temp = temp->next;
                delete del;
            }
        }
    };

    void Remove() {
        for (int i = 0; i < table.GetLength(); i++) {
            auto temp = table[i];
            while (temp != nullptr) {
                auto del = temp;
                temp = temp->next;
                delete del;
            }
        }

        start = nullptr; //указатель на самый первый элемент
        table.Resize(0);
        table.Resize(8);
        for (int i = 0; i < table.GetLength(); i++)
            table[i] = nullptr;
        size = table.GetLength(); //Кол-во ячеек в массиве
        amount = 0; //Кол-во элементов во всей таблице
    }

   //UnorderedMap(const Dictionary& dict){
   //    table = dict.table;
   //    size = dict.size;
   //    amount = dict.amount;
   //}

    void Add(TKey key, const TElement& element){
        int index = hashfunction(key, size);
        amount++;

        if (amount == 1){
            start = new Node{key, element, nullptr, nullptr, nullptr};
            table.Set(index, start);
        }else{
            if (table[index] == nullptr){
                table.Set(index, new Node{key, element, nullptr, start, nullptr});
                start->iprev = table[index];
                start = table[index]; //сделал новое начало
                return;
            }//появляется новый столбец(поэтому меням len)

            auto tmp = table[index];
            for (tmp; tmp->next != nullptr; tmp = tmp->next){}
            tmp->next = new Node{key, element, nullptr, tmp->ifol, tmp};
            if (tmp->ifol) {
                tmp->ifol->iprev = tmp->next;
            }
            tmp->ifol = tmp->next;

            Magnification();
        }
    };//Добавить элемент с заданным ключом.

    void Remove(TKey key){
        int index = hashfunction(key, size);

        if (table[index] == nullptr)
            throw AbsenceOfIndex();

        Node* dtemp = nullptr;
        auto temp = table[index];
        while(temp->key == key) {
            table[index] = temp->next;
            if (start == temp)
                start = temp->ifol;
            if (temp->iprev) {
                temp->iprev->ifol = temp->ifol;
            }
            if (temp->ifol) {
                temp->ifol->iprev = temp->iprev;
            }
            delete temp;
            amount--;
            temp = table[index];
            if (!temp)
                return;
        }

        while (temp && index == hashfunction(temp->key, size)){
            if (temp->key == key){
                if (temp->iprev) {
                    temp->iprev->ifol = temp->ifol;
                    temp->iprev->next = temp->next;
                }
                if (temp->ifol)
                    temp->ifol->iprev = temp->iprev;

                dtemp = temp;
                temp = temp->ifol;
                delete dtemp;
                amount--;
            }else{
                temp = temp->ifol; //учитывается, что последний nullptr
            }
        }

    };//Удаляет все элемент с заданным ключом.
    // Выбрасывает исключение, если заданный ключ отсутствует в таблице.

    TElement Get(TKey key){
        int index = hashfunction(key, size);

        if (table[index] == nullptr)
            throw AbsenceOfIndex();

        TElement element;

        for (auto temp = table[index]; temp != nullptr; temp = temp->next){
            if (temp->key == key)
                return temp->element;
        }

        return element;
    }//Дает значение лежащее по этому ключу

    void Swap(TKey key, TElement newelement){
        int index = hashfunction(key, size);

        if (table[index] == nullptr)
            throw AbsenceOfIndex();

        for (auto temp = table[index]; temp != nullptr; temp = temp->next){
            if (temp->key == key){
                temp->element = newelement;
                break;
            }
        }

    }//Меняет значение элемента с этим ключом

    bool ContainsKey(TKey key){
        int index = hashfunction(key, size);

        if (table[index] == nullptr)
            return false;

        for (auto temp = table[index]; temp != nullptr; temp = temp->next){
            if (temp->key == key){
                return true;
            }
        }

        return false;
    };//Проверка, что в таблице уже есть элемент с заданным ключом.

    int GetCollumn(){
        return size;
    }//Дает кол-во столбцов в словаре
    int GetCapasity(){
        return amount;
    }//Дает кол-во элементов в словаре

    UnorderedMap<TKey, TElement, hashfunction> &operator = (UnorderedMap<TKey, TElement, hashfunction> map){
        std::swap(table, map.table);
        size = map.size;
        amount = map.amount;
        start = map.start;
        return *this;
    }

    class iterator {
        Node* temp;
    public:
        iterator(Node* temp): temp(temp) {}

        Pair operator *() {
            if (!temp)
                throw AbsenceOfIndex();
            return Pair{temp->key, temp->element};
        }

        bool operator == (const iterator& iter) {
            return temp == iter.temp;
        }

        bool operator != (const iterator& iter) {
            return temp != iter.temp;
        }

        Pair* operator ->() {
            if (!temp)
                throw AbsenceOfIndex();
            return new Pair{temp->key, temp->element};
        }

        iterator& operator++ () {
            if (temp)
                temp = temp->ifol;
            return *this;
        }

        iterator operator++(int) {
            if (!temp)
                return iterator(nullptr);
            iterator res(temp);
            temp = temp->ifol;
            return res;
        }
    };

    TElement& operator [] (const TKey& key) {
        int index = hashfunction(key, size);

        Node* temp = table[index];
        while(temp){
            if (temp->key == key)
                return temp->element;
            temp = temp->next;
        }
        throw AbsenceOfIndex();
    }

    iterator begin() {
        return iterator(start);
    }

    iterator end() {
        return iterator(nullptr);
    }
};
/*
template <class TKey, class TElement, int (*hashfunction)(const TKey&, size_t)>
std::ostream& operator << (std::ostream& cout, typename UnorderedMap<TKey, TElement, hashfunction>::Pair pair) {
    return cout << '(' << pair.key << ": " << pair.element << ')';
}
*/
template <class TKey, class TElement, int (*hashfunction)(const TKey&, size_t)>
std::ostream& operator << (std::ostream& cout, UnorderedMap<TKey, TElement, hashfunction>& dict) {
    cout << '{';
    for (auto && i : dict) {
        //UnorderedMap<TKey, TElement, hashfunction>::Pair::
        cout << i << ", ";
    }
    return cout << "\b\b}";
}

/*
template <class TKey, class TElement, int (*hashfunction)(const TKey&, size_t)>
std::ostream &operator << (std::ostream &cout, UnorderedMap<TKey, TElement, hashfunction>& dict) {
    std::cout << '{';
    for (int i = 0; i < dict.GetCollumn(); i++) {
        try {
            std::cout << dict.Get_i(i);
            if (i != dict.GetCollumn() - 1) {
                cout << ", ";
            }
        }
        catch(typename Dictionary<TKey, TElement, hashfunction>::AbsenceOfIndex& err) {
        }
    }
    cout << '}';
    return cout;
}
*/
#endif //S3_LABORATORY_WORK_3_IUNORDEREDMAP_H
