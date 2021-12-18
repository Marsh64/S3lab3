//
// Created by adm on 06.12.2021.
//

#ifndef S3_LABORATORY_WORK_2_IDICTIONARY_H
#define S3_LABORATORY_WORK_2_IDICTIONARY_H
#include <iostream>
#include <string>
#include "DynamicArray.cpp"

static int hashstr(const std::string& key, size_t size)
{
    int asciisum = 7;
    for (char i : key)
    {
        asciisum = asciisum * 31 + i;
    }
    return int (asciisum % size);
}
static int hashint(const int& key, size_t size){
    return key % size;
}

template <class TKey, class TElement, int (*hashfunction)(const TKey&, size_t)>
class Dictionary {
private:
    struct Node{
        TKey key;
        TElement element;
        Node* next;
    };

    DynamicArray<Node*> table;

    //size_t capasity; //Сколько ячеек в массиве
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

            for (int index = 0; index < table.GetLength(); index++){
                for (auto temp = table[index]; temp != nullptr; temp = temp->next){
                    int newindex = hashfunction(temp->key, newsize);

                    if (newtable[newindex] == nullptr){
                        newtable.Set(newindex, new Node{temp->key, temp->element, nullptr});
                        //newtable[newindex] = new Node{table[index]->key, temp->element, nullptr};
                        //newtable.Relen(newtable.GetCountMemory() + 1);
                        continue;
                    }//появляется новый столбец(поэтому меням len)

                    auto tmp = newtable[newindex];
                    for (tmp; tmp->next != nullptr; tmp = tmp->next){}
                    tmp->next = new Node{temp->key, temp->element, nullptr};
                }
            }

            size = newtable.GetLength();

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

    Dictionary(){
        table.Resize(8);
        amount = 0;
        size = table.GetLength();
        for (int i = 0; i < size; i++) {
            table[i] = nullptr;
        }
    }// По-умолчанию делает словарь из 8 элементов
    explicit Dictionary(int newsize){
        table.Resize(newsize);
        size = newsize;
        amount = 0;
    }

    ~Dictionary(){
        for (int i = 0; i < table.GetLength(); i++) {
            auto temp = table[i];
            while (temp != nullptr) {
                auto del = temp;
                temp = temp->next;
                delete del;
            }
        }
    };
    //Все проверить, сделать копирующий конструктор, перегрузить операторы

    Dictionary(const Dictionary& dict){
        table = dict.table;
        size = dict.size;
        amount = dict.amount;

    }

    void Add(TKey key, const TElement& element){
        int index = hashfunction(key, size);
        amount+=1;

        if (table[index] == nullptr){
            table.Set(index, new Node{key, element, nullptr});
            //table[index] = new Node{key, element, nullptr};
            //table.Relen(table.GetCountMemory() + 1);
            return;
        }//появляется новый столбец(поэтому меням len)

        auto tmp = table[index];
        for (tmp; tmp->next != nullptr; tmp = tmp->next){}
        tmp->next = new Node{key, element, nullptr};

        Magnification();
    };//Добавить элемент с заданным ключом.

    void Remove(TKey key){
        int index = hashfunction(key, size);

        if (table[index] == nullptr)
            throw AbsenceOfIndex();

        auto temp = table[index];
        while (table[index]->key == key) {
            table[index] = temp->next;
            delete temp;
            temp = table[index];
            if (temp == nullptr)
                return;
        }

        Node* predtemp = table[index];
        temp = table[index]->next;
        while (temp != nullptr) {
            if (temp->key == key){
                auto del = temp;
                predtemp->next = temp->next;
                temp = temp->next;
                delete del;
                amount--;
                continue;
            }
            predtemp = temp;
            temp = temp->next;
        }
    };//Удаляет все элемент с заданным ключом.
    // Выбрасывает исключение, если заданный ключ отсутствует в таблице.

    DynamicArray<Node> Get_i(int index){
        if (table[index] == nullptr)
            throw AbsenceOfIndex();

        DynamicArray<Node> arr;

        for (auto temp = table[index]; temp!= nullptr; temp = temp->next){
            arr.Resize(arr.GetLength() + 1);
            //arr.Relen(arr.Relen() + 1);
            arr.Set(arr.GetLength() - 1, *temp);
        }

        return arr;
    }//дает массив всех элементов по индексу в массиве (то есть те, у кого одинаковый хэш)

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

    const TElement& GetFirstHash(TKey key){
        int index = hashfunction(key, size);

        for (auto temp = table[index]; temp != nullptr; temp = temp->next){
            if (temp->key == key){
                return temp->element;
            }
        }

        throw AbsenceOfIndex();
    }//

    const TElement& GetOne(TKey key){
        int index = hashfunction(key, size);

        if (table[index] == nullptr)
            throw AbsenceOfIndex();


        return table[index]->element;
    } //Дает первый элемент из столбца с хэшем соответствующим данному ключу

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

    Dictionary<TKey, TElement, hashfunction> &operator = (Dictionary<TKey, TElement, hashfunction> dict){
        std::swap(table, dict.table);
        size = dict.size;
        amount = dict.amount;
        return *this;
    }
};

template <class TKey, class TElement, int (*hashfunction)(const TKey&, size_t)>
std::ostream &operator << (std::ostream &cout, Dictionary<TKey, TElement, hashfunction>& dict) {
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


#endif //S3_LABORATORY_WORK_2_IDICTIONARY_H

