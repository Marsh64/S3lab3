#ifndef LinkedListSequence_h
#define LinkedListSequence_h

#include "Sequence.h"
#include "../Additional Stuctures/LinkedList.cpp"

template <class T>
class LinkedListSequence : Sequence<T>{
private:
    LinkedList<T> listSequence;
public:

    class IndexOutOfRange : LinkedList<T>::IndexOutOfRange{};

    //Создание объекта
    LinkedListSequence (T* items, int count){
        listSequence = LinkedList<T>(items, count);
    }//Копирует элемнеты из переданного массива
    LinkedListSequence (){
        listSequence = LinkedList<T>();
    }//Создает пустую последовательность
    explicit LinkedListSequence (const LinkedList <T> &list){
        listSequence = LinkedList<T>(list);
    }//Копирующий конструктор

    //Удаление объекта
    ~LinkedListSequence(){
        listSequence.Delete_LinkedList();
    }//деструктор
    void Delete_LinkedListSequene(){
        listSequence.Delete_LinkedList();
    }//функция удаление последовательности

    //Декомпозиция
    T GetFirst(){
        return listSequence.GetFirst();
    }//Возвращает первый элемент
    T GetLast(){
        return listSequence.GetLast();
    }//Возвращает последний элемент
    T& Get(int index){
        return listSequence.Get(index);
    }//Возвращает элемент по индексу
    LinkedListSequence<T>* GetSubsequence(int startIndex, int endIndex){
        //auto *new_LinkedList = new LinkedList<T>;
        auto new_LinkedList = listSequence.GetSubList(startIndex, endIndex);
        auto *new_LinkedListSequence = new LinkedListSequence<T>(new_LinkedList);
        return new_LinkedListSequence;
    }//Получить список из всех элементов, начиная с startIndex и заканчивая endIndex
    int GetLength() {
        return listSequence.GetLength();
    }

    //Операции
    void Append(T item){
        listSequence.Append(item);
    }//Добавляет элемент в конец списка
    void Prepend(T item){
        listSequence.Prepend(item);
    }//Добавляет элемент в начало списка
    void InsertAt(T item, int index){
        listSequence.InsertAt(item, index);
    }//Вставляет элемент в заданную позицию
    void Remove(int from, int to){
        listSequence.Remove(from, to);
    }
    void Remove(int index){
        listSequence.Remove(index);
    }
    T Pop(){
        T element = listSequence.GetLast();
        listSequence.RemoveLast();
        return element;
    }

    LinkedListSequence<T>* Concat(Sequence<T> *list){
        auto *new_LinkedList = new LinkedListSequence<T> (*this);
        for (int i = 0; i < list->GetLength(); i++){
            new_LinkedList->Append(list->Get(i));
        }

        return new_LinkedList;
    }//Сцепляет два списка

    //Перегрузка операторов
    LinkedListSequence<T>& operator = (const LinkedListSequence<T>& seq) {
        listSequence = seq.listSequence;
        return *this;
    }

    typename LinkedList<T>::iterator begin() {
        return listSequence.begin();
    }

    typename LinkedList<T>::iterator end() {
        return listSequence.end();
    }
};

template<class T>
std::ostream &operator << (std::ostream &cout, LinkedListSequence<T>& list) {
    cout << '{';
    for (int i = 0; i < list.GetLength(); i++) {
        cout << list.Get(i);
        if (i != list.GetLength() - 1) {
            cout << ", ";
        }
    }
    cout << '}';
    return cout;
}
template<class T>
std::ostream &operator << (std::ostream &cout, LinkedListSequence<T>* list) {
    if (list == nullptr){
        cout << "{}";
        return cout;
    }

    cout << '{';
    for (int i = 0; i < list->GetLength(); i++) {
        cout << list->Get(i);
        if (i != list->GetLength() - 1) {
            cout << ", ";
        }
    }
    cout << '}';
    return cout;
}

#endif