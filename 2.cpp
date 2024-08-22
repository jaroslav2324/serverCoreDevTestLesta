/*Вопрос №2

На языке Python или С++ написать минимум по 2 класса реализовывающих циклический буфер FIFO.
 Объяснить плюсы и минусы каждой реализации.

Оценивается:

Полнота и качество реализации
Оформление кода
Наличие сравнения и пояснения по быстродействию*/


#include <iostream>

// Реализация циклической очереди FIFO с использованием массива.
// В данной реализации используется динамический массив для определения размера очереди в процессе выполнения программы,
// при этом расширения размера очереди с перевыделением памяти не предусмотрено.
//
// Недостатки реализации:
// Фиксированный размер очереди
// В случае реализации расширения очереди могут возникнуть трудности с выделением большого непрерывного участка памяти,
// когда элементов очереди очень много. Также может быть затратным копирование элементов из старого массива в новый.
//
// Достоинства реализации:
// Несколько проще реализовать в сравнении со следующим подходом
// Малое количество выделений памяти и, соответственно, системных вызовов, что положительно сказывается на
// быстродействии


template <typename T>
class CyclicQueueAsArray{
public:
    CyclicQueueAsArray(int maxCountElements);
    ~CyclicQueueAsArray();
    int size();
    int maxSize();
    int push(const T& element);
    int pop(T& returnedElement);
    bool empty();
    void clear();

private:
    int countElements = 0;
    int maxCountElements = 0;
    int idxHead = 0;
    int idxTail = 0;
    T* arrayElements = nullptr;
};

template<typename T>
void CyclicQueueAsArray<T>::clear() {
    if (countElements == 0){
        return;
    }
    idxHead = 0;
    idxTail = 0;
    countElements = 0;
}

template<typename T>
bool CyclicQueueAsArray<T>::empty() {
    return countElements == 0;
}

// returns -1 if no elements to pop otherwise fills returnedElement and returns 0
template<typename T>
int CyclicQueueAsArray<T>::pop(T &returnedElement) {
    if (countElements == 0){
        return -1;
    }

    returnedElement = arrayElements[idxHead];
    idxHead = (idxHead + 1) % maxCountElements;
    countElements -= 1;
    return 0;
}

template<typename T>
int CyclicQueueAsArray<T>::push(const T &element) {
    if (countElements >= maxCountElements){
        return -1;
    }

    arrayElements[idxTail] = element;
    idxTail = (idxTail + 1) % maxCountElements;
    countElements += 1;
    return 0;
}

template<typename T>
int CyclicQueueAsArray<T>::maxSize() {
    return maxCountElements;
}

template<typename T>
int CyclicQueueAsArray<T>::size() {
    return countElements;
}

template<typename T>
CyclicQueueAsArray<T>::CyclicQueueAsArray(int maxCountElements) {
    this->maxCountElements = maxCountElements;
    arrayElements = new T[maxCountElements];
}

template<typename T>
CyclicQueueAsArray<T>::~CyclicQueueAsArray() {
    delete[] arrayElements;
    arrayElements = nullptr;
    maxCountElements = 0;
    countElements = 0;
    idxHead = 0;
    idxTail = 0;
}

// Реализация очереди FIFO на основании связного списка.

// Недостатки реализации:
// Отдельное динамическое выделение памяти под каждый элемент негативно сказывается на быстродействии, особенно при
// частом добавлении и удалении элементов
// Несколько сложнее реализовать в сравнении с предыдущим подходом
// Вероятно, находящиеся в разных частях памяти элементы будут реже попадать в кэш, что будет приводить к постоянной
// подгрузке страниц, а это негативно скажется на быстродействии
// Использование указателей - дополнительные затраты памяти на каждый элемент.

// Достоинства реализации:
// Потенциально неограниченное количество элементов
// Каждый элемент хранится в памяти отдельно от других, нет необходимости в использовании большого непрерывного участка
// памяти. Пямять будет использоваться более эффективно.
// В случае необходимости реализации вставки элемента в середину очереди это делается простой заменой указателей. В
// предыдущем подходе пришлось бы сдвигать элементы массива.

template <typename T>
struct QueueElement{
    QueueElement(T data);
    T data;
    QueueElement* next;
};

template<typename T>
QueueElement<T>::QueueElement(T data) {
    this->data = data;
    next = nullptr;
}

template <typename T>
class CyclicQueueAsList{
public:
    ~CyclicQueueAsList();
    int size();
    int push(const T& element);
    int pop(T& returnedElement);
    bool empty();
    void clear();

private:
    int countElements = 0;
    QueueElement<T>* head = nullptr;
    QueueElement<T>* tail = nullptr;
};

template<typename T>
CyclicQueueAsList<T>::~CyclicQueueAsList() {
    clear();
}

template<typename T>
void CyclicQueueAsList<T>::clear() {

    for (int i = 0; i < countElements; i++){
        QueueElement<T>* nextElement = head->next;
        delete head;
        head = nextElement;
    }
    countElements = 0;
    head = nullptr;
    tail = nullptr;
}

template<typename T>
int CyclicQueueAsList<T>::pop(T &returnedElement) {
    if (countElements == 0){
        return -1;
    }

    returnedElement = head->data;
    head = head->next;
    delete tail->next;
    tail->next = head;
    countElements -= 1;

    return 0;
}

template<typename T>
int CyclicQueueAsList<T>::push(const T &element) {
    QueueElement<T> *newElement = nullptr;
    try {
        newElement = new QueueElement<T>(element);
    }
    catch (std::bad_alloc& e){
        return -1;
    }

    if (head == nullptr){
        newElement->next = newElement;
        head = newElement;
        tail = newElement;
    }
    else{
        tail->next = newElement;
        newElement->next = head;
        tail = newElement;
    }
    countElements += 1;

    return 0;
}

template<typename T>
bool CyclicQueueAsList<T>::empty() {
    return countElements == 0;
}

template<typename T>
int CyclicQueueAsList<T>::size() {
    return countElements;
}

