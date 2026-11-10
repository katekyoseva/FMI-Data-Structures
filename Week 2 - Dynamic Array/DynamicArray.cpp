#include <iostream>
#include <utility>
#include <stdexcept>

template <typename T>
class DynamicArray {
public:
    DynamicArray()
        : DynamicArray(4) {
    }

    explicit DynamicArray(int capacity)
    {
        if (!capacity) throw std::invalid_argument("Cannot be 0");
        arrData = new T[capacity];
        cap = capacity;
        currentSize = 0;
    }

    DynamicArray(const DynamicArray& other)
    {
        cap = other.cap;
        currentSize = other.currentSize;
        arrData = new T[cap];
        try
        {
            for (int i = 0; i < other.currentSize; i++)
            {
                arrData[i] = other.arrData[i];
            }
        }
        catch (...)
        {
            delete[] arrData;
            throw;
        }

    }

    DynamicArray& operator=(const DynamicArray& other) {
        if (this != &other) 
        {
            DynamicArray temp(other);
            swap(temp);
        }

        return *this;
    }

    ~DynamicArray() {
        delete[] arrData;
    }

    void clear() {
        currentSize = 0;
    }

    void insert(int index, const T& elem) {
        if (index < 0 || index > currentSize) {
            throw std::out_of_range("Invalid index");
        }

        if (currentSize == cap) resize();

        for (int i = currentSize; i > index; i--) {
            arrData[i] = arrData[i - 1];
        }

        arrData[index] = elem;
        currentSize++;
    }

    void erase(int index) {
        if (index < 0 || index >= currentSize) {
            throw std::out_of_range("Invalid index");
        }

        for (int i = index; i < currentSize - 1; i++) {
            arrData[i] = arrData[i + 1];
        }

        currentSize--;
    }

    void erase(int first, int last)
    {
        if (first < 0 || first > last || last > currentSize)
        {
            throw std::out_of_range("Invalid range");
        }

        int count = last - first;

        for (int i = first; i < currentSize - count; i++)
        {
            arrData[i] = arrData[i + count];
        }

        currentSize -= count;
    }

    void pop_back() {
        if (currentSize == 0) {
            throw std::out_of_range("Array is empty");
        }

        currentSize--;
    }

    void push_back(const T& elem) {
        if (currentSize == cap) resize();

        arrData[currentSize] = elem;
        currentSize++;
    }

    int size() const {
        return currentSize;
    }

    bool empty() const {
        return currentSize == 0;
    }

    T& at(int index) {
        if (index < 0 || index >= currentSize) {
            throw std::out_of_range("Invalid index");
        }

        return arrData[index];
    }

    const T& at(int index) const {
        if (index < 0 || index >= currentSize) {
            throw std::out_of_range("Invalid index");
        }

        return arrData[index];
    }

    T& operator[](int index) {
        return arrData[index];
    }

    const T& operator[](int index) const {
        return arrData[index];
    }

    T* data() {
        return arrData;
    }

    const T* data() const {
        return arrData;
    }

    void print() const {
        for (int i = 0; i < currentSize; i++) {
            std::cout << arrData[i] << " ";
        }

        std::cout << '\n';
    }
private:
    T* arrData = nullptr;
    int currentSize;
    int cap;

    void resize() 
    {
        int newCapacity = cap * 2;
        T* newElements = new T[newCapacity];

        try {
            for (int i = 0; i < currentSize; i++) {
                newElements[i] = arrData[i];
            }
        }
        catch (...) {
            delete[] newElements;
            throw;
        }

        delete[] arrData;
        arrData = newElements;
        cap = newCapacity;
    }

    void swap(DynamicArray& other)
    {
        std::swap(arrData, other.arrData);
        std::swap(currentSize, other.currentSize);
        std::swap(cap, other.cap);
    }
};

int main() {
    DynamicArray<int> arr;

    arr.push_back(10);
    arr.push_back(20);
    arr.push_back(30);

    arr.print();

    DynamicArray<int> copy(arr);
    copy.push_back(40);

    copy.print();

    std::cout << arr.size() << '\n';
    std::cout << arr[1] << '\n';

    return 0;
}