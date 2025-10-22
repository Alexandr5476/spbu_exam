#include "my_list.h"
#include <iostream>

using namespace std;

int main() {
    my_list<int> lst;

    // Проверка добавления
    for (int i = 1; i <= 7; ++i)
        lst.push_back(i);
    cout << "Size after push: " << lst.get_size() << endl; // 7

    // Проверка получения элементов
    cout << "List elements: ";
    for (size_t i = 0; i < lst.get_size(); ++i)
        cout << lst.get_element(i) << ' ';
    cout << endl;

    // Проверка удаления элементов
    lst.pop_back();
    lst.pop_back();
    cout << "Size after pop: " << lst.get_size() << endl; // 5
    cout << "List elements: ";
    for (size_t i = 0; i < lst.get_size(); ++i)
        cout << lst.get_element(i) << ' ';
    cout << endl;

    // Проверка копирования копирование
    my_list<int> copy = lst;
    cout << "Copy equal: " << (copy == lst ? "OK" : "FAIL") << endl;

    cout << "Copy elements: ";
    for (size_t i = 0; i < copy.get_size(); ++i)
        cout << copy.get_element(i) << ' ';
    cout << endl;

    // Проверка удаления элементов пока не останется один
    lst.pop_back();
    lst.pop_back();
    lst.pop_back();
    lst.pop_back();
    cout << "Size after pop: " << lst.get_size() << endl; // 5
    cout << "List elements: ";
    for (size_t i = 0; i < lst.get_size(); ++i)
        cout << lst.get_element(i) << ' ';
    cout << endl;
}