//
// Created by adm on 01.11.2021.
//

#ifndef S3_LABORATORY_WORK_2_ARRAYSEQUENCE_H
#define S3_LABORATORY_WORK_2_ARRAYSEQUENCE_H
#include "Sequence/Sequence.h"
#include "Additional Stuctures/DynamicArray.cpp"

template <class T>
class ArraySequence : public Sequence<T>{
private:
    DynamicArray<T> dynamicArray;
public:
    class IndexOutOfRange : DynamicArray<T> :: IndexOutOfRange{};

    //Создание объекта
    ArraySequence (T* items, int count){
        dynamicArray = DynamicArray<T>(items, count);
    }//Создает последовательность с элементами из исходного массива
    ArraySequence (){
        dynamicArray = DynamicArray<T>(0);
    }//Создает пустую последовательность
    explicit ArraySequence (const DynamicArray<T> & dynamicArray){
        this->dynamicArray = dynamicArray;
    }//Копирующий конструктор
    explicit ArraySequence (const ArraySequence<T> & seq){
        dynamicArray = seq.dynamicArray;
    }//Копирующий конструктор(new)

    //Удаление объекта
    ~ArraySequence(){
        dynamicArray.Delete_DynamicArray();
    }//деструктор
    void Delete_ArraySequence(){
        dynamicArray.Delete_DynamicArray();
    }//функция удаления послдовательности

    //Декомпозиция
    T GetFirst(){
        return dynamicArray.Get(0);
    }//Возвращает первй элемент
    T GetLast(){
        return dynamicArray.Get(dynamicArray.GetLength() - 1);
    }//Возвращает последний элемент
    T& Get(int index){
        return dynamicArray.Get(index);
    }//Возвращает элемент по индексу
    ArraySequence<T>* GetSubsequence(int startIndex, int endIndex){
        if (startIndex >= dynamicArray.GetLength() || endIndex >= dynamicArray.GetLength() || startIndex < 0 || endIndex < 0){
            throw IndexOutOfRange();
        }

        auto *new_arraySequence = new ArraySequence<T>;
        int iter;
        if (startIndex < endIndex){
            iter = 1;
        }else{
            iter = -1;
        }

        for (int i = startIndex; i != endIndex; i += iter){
            new_arraySequence->Append(dynamicArray.Get(i));
        }

        return new_arraySequence;
    }//Получить список из всех элементов, начиная с startIndex и заканчивая endIndex
    int GetLength() {
        return dynamicArray.GetLength();
    }//Возвращает длину последовательности

    //Операции
    T Pop(){
        if (dynamicArray.GetLength() == 0)
            throw IndexOutOfRange();

        T item = dynamicArray[dynamicArray.GetLength() - 1];
        dynamicArray.Resize(dynamicArray.GetLength() - 1);
        return item;
    }//Удаляет последний элемент в последовательности и возвращает его
    void Remove(int index){
        if (index >= dynamicArray.GetLength() || index < 0) throw IndexOutOfRange();
        for (int i = index; i < dynamicArray.GetLength() - 1; i++){
            dynamicArray[i] = dynamicArray[i + 1];
        }
        dynamicArray.Resize(dynamicArray.GetLength() - 1);
    }
    void Remove(int from, int to){
        if (from < 0 || from >= dynamicArray.GetLength()) throw IndexOutOfRange();
        if (to < 0 || to >= dynamicArray.GetLength()) throw IndexOutOfRange();

        if (from >= to) return;

        for (int i = 0; i + to < dynamicArray.GetLength(); i++) {
            dynamicArray[i + from] = dynamicArray[i + to];
        }
        dynamicArray.Resize(dynamicArray.GetLength() - to + from);
    }//Удаляет с элемента с индексом from до элемента с индексом to не включительно
    void Append(T item){
        dynamicArray.Resize(dynamicArray.GetLength() + 1);
        dynamicArray[dynamicArray.GetLength() - 1] = item;
    }//дабавляет элемент в конец последовательности(при необходимости выделяет доп. ячейки)
    void Prepend(T item){
        dynamicArray.Resize(dynamicArray.GetLength() + 1);
        for (int i = dynamicArray.GetLength() - 1; i > 0; i++){
            dynamicArray[i] = dynamicArray[i - 1];
        }
        dynamicArray[0] = item;
    }//Добавляет элемент в начало строки
    void InsertAt(T item, int index){
        if (index < 0 || index > this->GetLength()){throw IndexOutOfRange();}

        dynamicArray.Resize(dynamicArray.GetLength() + 1);
        for (int i = dynamicArray.GetLength() - 1; i > index; i--){
            dynamicArray[i] = dynamicArray[i - 1];
        }
        dynamicArray[index] = item;
    }//Вставляет элемент по заданному индексу
    ArraySequence<T>* Concat(Sequence<T> *list){
        auto *new_arraySequence = new ArraySequence<T>;
        //new_arraySequence = ArraySequence<T>();

        for (int i = 0; i < dynamicArray.GetLength(); i++){
            new_arraySequence->Append(dynamicArray.Get(i));
        }
        for (int i = dynamicArray.GetLength(); i < list->GetLength() + dynamicArray.GetLength(); i++){
            new_arraySequence->Append(list->Get(i));
        }

        return new_arraySequence;
    }//Сцепляет две последовательности

    //Перегрузка операторов
    ArraySequence<T> &operator = (const ArraySequence<T>& n_dynamicArray){
        dynamicArray = n_dynamicArray.dynamicArray;
        return *this;
    }
    T &operator[](int index) {
        if (index < 0 || index >= GetLength())
            throw IndexOutOfRange();
        return dynamicArray[index];
    }
};

template<class T>
std::ostream &operator << (std::ostream &cout, ArraySequence<T>& arr) {
    cout << '{';
    for (int i = 0; i < arr.GetLength(); i++) {
        cout << arr.Get(i);
        if (i != arr.GetLength() - 1) {
            cout << ", ";
        }
    }
    cout << '}';
    return cout;
}



#endif //S3_LABORATORY_WORK_2_ARRAYSEQUENCE_H
