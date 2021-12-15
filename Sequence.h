//
// Created by adm on 01.11.2021.
//

#ifndef S3_LABORATORY_WORK_2_SEQUENCE_H
#define S3_LABORATORY_WORK_2_SEQUENCE_H

template <class T>
class Sequence {
public:
    //Декомпозиция
    virtual T GetFirst() = 0;//Получение первого элемента
    virtual T GetLast() = 0;//Получение последнего элемента
    virtual T& Get(int index) = 0;//Получение элемента по индексу
    virtual Sequence<T>* GetSubsequence(int startIndex, int endIndex) = 0;//Получене списка элемента по индексам
    virtual int GetLength() = 0;//Получение длины последовательности

    //Операции
    virtual T Pop() = 0;//Удаляет последний элемент в последовательности и возвращает его
    virtual void Remove(int, int) = 0;//Удаляет элементы начиная с первого до второго не включительно
    virtual void Append(T item) = 0; //Добавление элемента в конец последовательности
    virtual void Prepend(T item) = 0; //Добавление элемента в начало последовательности
    virtual void InsertAt(T item, int index) = 0; //Вставка элемента в заданную позицию последовательности
    virtual Sequence <T>* Concat(Sequence <T> *list) = 0; //Сцепление двух последовательностей
};


#endif //S3_LABORATORY_WORK_2_SEQUENCE_H
