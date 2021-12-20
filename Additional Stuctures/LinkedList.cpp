#include "LinkedList.h"

#ifndef LinkedList_cpp
#define LinkedList_cpp
template <class T>
void LinkedList<T>::Append(T item) {
    auto *new_cell = new struct element;
    new_cell->value = item;
    new_cell->next_element = nullptr;

    if (head_element == nullptr){ //случай, когда список пустой
        head_element = new_cell;
        end_element = new_cell;
        len = 1;
        return;
    }

    end_element->next_element = new_cell;
    end_element = new_cell;
    len++;
}

template <class T>
LinkedList<T>::LinkedList() {
    len = 0;
    head_element = nullptr;
    end_element = nullptr;
}

template <class T>
LinkedList<T>::LinkedList(T *items, int count) {
    len = 0;
    head_element = nullptr;
    end_element = nullptr;

    for (int i = 0; i < count; i++){
        Append(items[i]);
    }
}

template <class T>
LinkedList<T>::LinkedList(const LinkedList<T> &list) {
    len = 0;
    head_element = nullptr;
    end_element = nullptr;
    struct element *cell = list.head_element;
    for (int i = 0; i < list.len; i++, cell = cell->next_element){
        Append(cell->value);
    }
}

template <class T>
T LinkedList<T>::GetFirst() {
    if (head_element == nullptr){
        throw IndexOutOfRange();
    }

    return head_element->value;
}

template <class T>
T LinkedList<T>::GetLast() {
    if (end_element == nullptr){
        throw IndexOutOfRange();
    }

    return end_element->value;
}

template <class T>
T& LinkedList<T>::Get(int index) {
    if (index < 0 || index >= len){
        throw IndexOutOfRange();
    }

    struct element *cell = head_element;
    for (int i = 0; i < index; i++, cell = cell->next_element);

    return cell->value;
}

template<class T>
void LinkedList<T>::Remove(int from, int to) {
    if (from < 0 || from >= len) throw IndexOutOfRange();
    if (to < 0 || to + 1 >= len) throw IndexOutOfRange();

    if (to == len){
        auto *precell = new struct element;
        precell = head_element;
        for (int i = 0; i < from - 1; i++, precell = precell->next_element); //ячейка перед from
        end_element = precell;
    }
    else{
        auto *precell = new struct element;
        auto *cell= new struct element;
        precell = head_element;
        for (int i = 0; i < from - 1; i++, precell = precell->next_element); //ячейка перед from
        cell = precell; //это ячейка from
        for (int i = 0; i < from - to; i++, cell = cell->next_element);//TODO нужно ли удалять указатель внутри element
        precell->next_element = cell;
    }
    //TODO изменить длину
}

template<class T>
void LinkedList<T>::Remove(int index) {
    //if (index == len - 1){
    //    auto cell = new struct element;
    //    cell = head_element;
    //    for (int i = 0; i < index - 1; i++, cell = cell->next_element);
    //    cell->next_element = nullptr;
    //    end_element = cell;
    //}else if (index == 0){
    //    head_element = head_element->next_element;
    //}else {
    //    auto cell = new struct element;
    //    cell = head_element;
    //    for (int i = 0; i < index - 1; i++, cell = cell->next_element);
    //    cell->next_element = cell->next_element->next_element;
    //}
    //len--;
    if (index < 0 || index >= len) throw IndexOutOfRange();

    element *el = head_element;
    element *pref = nullptr;

    T value;

    for (int i = 0; i < index; i++, pref = el, el = el->next_element);

    value = el->value;

    if (pref == nullptr) {
        head_element = el->next_element;
    }
    else {
        pref->next_element = el->next_element;
    }
    delete el;
    len--;
}

template<class T>
void LinkedList<T>::RemoveLast() {
    auto *cell= new struct element;
    cell = head_element;
    for (int i = 0; i < len - 1; i++, cell = cell->next_element);//cell - предпоследняя ячейка
    end_element = cell;
    len--;
}

template <class T>
LinkedList<T> LinkedList<T>::GetSubList(int startIndex, int endIndex) {
    if (startIndex >= len || endIndex >= len || startIndex < 0 || endIndex < 0){
        throw IndexOutOfRange();
    }

    LinkedList<T> new_list = LinkedList<T>();
    auto *cell = new struct element;
    cell = head_element;
    for (int i = 0; i < startIndex; i++, cell = cell->next_element);
    for (int i = startIndex; i <= endIndex; i++, cell = cell->next_element){
        new_list.Append(cell->value);
    }

    return new_list;
}

template <class T>
int LinkedList<T>::GetLength() {
    return len;
}

template <class T>
void LinkedList<T>::Prepend(T item) {
    auto *new_cell = new struct element;
    new_cell->value = item;

    if (head_element == nullptr){
        end_element = new_cell;
    }

    new_cell->next_element = head_element;
    head_element = new_cell;
    len++;
}

template <class T>
void LinkedList<T>::InsertAt(T item, int index) {
    if (index >= len || index < 0){
        throw IndexOutOfRange();
    }

    if (index == 0){Append(item); return;}
    else if (index == len - 1){Prepend(item); return;}
    else{
        auto *new_cell = new struct element;
        new_cell->value = item;
        struct element *previous_cell = head_element;
        for (int i = 0; i < index - 1; i++, previous_cell = previous_cell->next_element);
        new_cell->next_element = previous_cell->next_element;
        previous_cell->next_element = new_cell;
        len++;
    }
}

template<class T>
LinkedList<T> LinkedList<T>::Concat(LinkedList<T> *list) {
    LinkedList<T> new_list = LinkedList<T>();

    struct element *new_cell = head_element;
    for (int i = 0; i < len; i++, new_cell = new_cell->next_element){
        new_list.Append(new_cell->value);
    }

    new_cell = list->head_element;
    for (int i = 0; i < list->len; i++, new_cell = new_cell->next_element){
        new_list.Append(new_cell->value);
    }

    return new_list;
}

template<class T>
LinkedList<T> :: ~LinkedList(){
    Delete_LinkedList();
}

template <class T>
void LinkedList<T> :: Delete_LinkedList() {
    struct element* this_cell;
    while(head_element != nullptr){
        this_cell = head_element;
        head_element = head_element->next_element;
        delete this_cell;
    }
    end_element = nullptr;
    len = 0;
}

#endif