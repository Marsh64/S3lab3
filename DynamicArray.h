//
// Created by adm on 06.12.2021.
//

#ifndef S3_LABORATORY_WORK_2_DYNAMICARRAY_H
#define S3_LABORATORY_WORK_2_DYNAMICARRAY_H

template <class T>
class DynamicArray {
private:
    T* array;
    int lenght; //на сколько элементов у пользователя есть доступ
    int size; //на сколько элементов выделена память
public:
    class IndexOutOfRange{};

    // Создание объект
    DynamicArray(); //создание пустого массива
    DynamicArray(T* items, int count); //создает пустой массив и копирует элементы из исходного
    explicit DynamicArray(int newSize); //создание пустого массива заданной длины
    explicit DynamicArray(int newLenght, int needSize);//Создание массива заданной длины со свободным местом needSize - newLenght
    DynamicArray(const DynamicArray& dynArr) {
        size = dynArr.size;
        lenght = dynArr.lenght;

        array = new T[size];
        for (int i = 0; i < lenght; i++) {
            array[i] = dynArr.array[i];
        }
    }

    //Удаление объекта
    ~DynamicArray(); //деструктор
    void Delete_DynamicArray(); //удаление массива

    //Декомпозиция
    T& Get(int index); //возвращает элемент по индексу
    int GetCountMemory();//Общее число ячеек, под которые выделили память
    int GetLength() const;//Число ячеек, которые может использовать пользователь


    //Операции над параметрами массива
    void Set(int index, T value); //Задает элемент по индексу
    void Resize(int newLenght); //Изменяет длину массива доступную пользователю

    DynamicArray<T> &operator = (DynamicArray<T> dynamicArray) {
        std::swap(array, dynamicArray.array);
        size = dynamicArray.size;
        lenght = dynamicArray.lenght;
        return *this;
    }

    T  &operator [](int index){
        if (index < 0 || index >= lenght){
            throw IndexOutOfRange();
        }

        return array[index];
    }


};


template<class T>
std::ostream &operator << (std::ostream &cout, DynamicArray<T> arr) {
    std::cout << '{';
    for (int i = 0; i < arr.GetLength(); i++) {
        std::cout << arr.Get(i);
        if (i != arr.GetLength() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << '}';
    return cout;
}

#endif //S3_LABORATORY_WORK_2_DYNAMICARRAY_H

