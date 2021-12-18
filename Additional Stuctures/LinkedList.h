#ifndef LinkedList_h
#define LinkedList_h

template <class T>
class LinkedList {
private:
    struct element{
        T value;
        struct element* next_element;
    };
    int len = 0;
    element* head_element = nullptr;
    element* end_element = nullptr;

public:
    class IndexOutOfRange{};

    class iterator {
        element* item;
    public:
        iterator(element *item): item(item) {}

        T& operator *() {
            if (!item)
                throw IndexOutOfRange();
            return item->value;
        }

        bool operator == (const iterator& iter) {
            return item == iter.item;
        }

        bool operator != (const iterator& iter) {
            return item != iter.item;
        }

        T* operator ->() {
            if (!item)
                throw IndexOutOfRange();
            return &item->value;
        }

        iterator& operator++ () {
            if (item)
                item = item->next_element;
            return *this;
        }

        iterator operator++(int) {
            if (!item)
                return iterator(nullptr);
            iterator res(item);
            item = item->next_element;
            return res;
        }
    };

    //Создание объекта
    LinkedList (T* items, int count); //Копирует элементы из переданного списка
    LinkedList (); //Создает пустой список
    LinkedList (const LinkedList <T> & list); //Копирующий конструктор

    //удаление объекта
    ~LinkedList();//деструктор
    void Delete_LinkedList(); //удаление списка
    void Remove(int index);
    void Remove(int from, int to);//удаляет элементы c from до to не включительно
    void RemoveLast();//удаляет последний элемент

    //декомпозиция
    T GetFirst();//Возвращает первый элемент
    T GetLast();//Возвращает последний элемент
    T& Get(int index);//Возвращает элемент по индексу
    LinkedList<T> GetSubList(int startIndex, int endIndex);//Возваращет список элементов в исходных пределах
    int GetLength();//Возвращает длину

    //Операции
    void Append(T item); //Добавление элемента в конец списка
    void Prepend(T item); //Добавление элемента в начало списка
    void InsertAt(T item, int index); //Вставка элемента в заданную позицию
    LinkedList<T> Concat(LinkedList<T> *list); //Сцепляет два списка

    //Перегрузка операторов
    LinkedList<T> &operator = (LinkedList<T> linkedlist) {
        Delete_LinkedList();
        struct element *cell = linkedlist.head_element;
        for (int i = 0; i < linkedlist.len; i++, cell = cell->next_element){
            Append(cell->value);
        }
        return *this;
    }

    iterator begin() {
        return iterator(head_element);
    }

    iterator end() {
        return iterator(nullptr);
    }
};


#endif