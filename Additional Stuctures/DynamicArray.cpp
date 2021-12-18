//
// Created by adm on 06.12.2021.
//

#ifndef S3_LABORATORY_WORK_2_DYNAMICARRAY_CPP
#define S3_LABORATORY_WORK_2_DYNAMICARRAY_CPP

#include "DynamicArray.h"
template <class T>
void DynamicArray<T>::Resize(int newLenght /* подразумевается место для пользователя*/){
    int ConstMemory = int(0.25 * newLenght); //будем увеличивать память пачками

    if (newLenght < 0){
        throw IndexOutOfRange();
    }// неправильный размер массива

    if (newLenght == 0){
        delete[] array;
        array = nullptr;
        size = 0;
        lenght = 0;
        return;
    }// удаление массива

    if (newLenght == lenght){return;}// длина не изменится

    if (newLenght < lenght){
        lenght = newLenght;
        return;
    }// укорачивание массива

    if (newLenght > 0 && lenght == 0){
        T *new_array = new T[newLenght + ConstMemory];
        delete[] array;
        lenght = newLenght;
        size = newLenght + ConstMemory;
        array = new_array;
        return;
    }//увеличение размера массива 0 длины

    if (newLenght > lenght){
        if (newLenght >= size){
            T *new_array = new T[newLenght + ConstMemory];
            for (int i = 0; i < lenght; i++){
                new_array[i] = array[i];
            }
            delete[] array;
            lenght = newLenght;
            size = newLenght + ConstMemory;
            array = new_array;
        }
        else{
            lenght = newLenght;
        }
        return;
    }// увеличение длины
}

template <class T>
DynamicArray<T>::DynamicArray() {
    array = nullptr;
    lenght = 0;
    size = 0;
}

template <class T>
DynamicArray<T>::DynamicArray(T *items, int count) {
    array = nullptr;
    size = 0;
    lenght = 0;

    Resize(count);
    for (int i = 0; i < count; i++){
        array[i] = items[i];
    }
}

template <class T>
DynamicArray<T>::DynamicArray(int newSize){
    if (newSize < 0){newSize = 0;}

    array = nullptr;
    size = 0;
    lenght = 0;
    Resize(newSize);
}

template<class T>
DynamicArray<T>::DynamicArray(int newLenght, int needSize){
    if (newLenght < 0 || needSize < newLenght){
        throw IndexOutOfRange();
    }

    array = new T[needSize];
    lenght = newLenght;
    size = needSize;
}

template <class T>
T& DynamicArray<T>::Get(int index) {
    if (index < 0 || index >= lenght){
        throw IndexOutOfRange();
    }

    return array[index];
}

template <class T>
void DynamicArray<T>::Set(int index, T value) {
    if (index < 0 || index >= lenght){
        throw IndexOutOfRange();
    }

    array[index] = value;
}

template <class T>
int DynamicArray<T>::GetCountMemory() {
    return size;
}

template <class T>
int DynamicArray<T>::GetLength() const {
    return lenght;
}

template <class T>
void DynamicArray<T> :: Delete_DynamicArray(){
    delete[] array;
    array = nullptr;
    size = 0;
    lenght = 0;
}

template <class T>
DynamicArray<T> :: ~DynamicArray(){
    delete[] array;
    array = nullptr;
    size = 0;
    lenght = 0;
}

#endif
