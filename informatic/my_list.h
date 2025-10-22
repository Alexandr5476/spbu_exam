//
// Created by Alexander on 22.10.2025.
//

#ifndef EXAM_MY_LIST_H
#define EXAM_MY_LIST_H

#include <list>
#include <stdexcept>
#include <string>
#include <iostream>

template<typename T>
class my_list {
private:
    size_t list_size;
    void *list_data; // нужно хранить то указатель на данные, то на массив данных, то на список данных

public:
    /// Конструктор
    my_list() : list_size(0), list_data(nullptr) {
    }


    /// Конструктор копирования
    my_list(const my_list &orig_list) : list_size(orig_list.list_size) {
        if (list_size == 0) {
            list_data = nullptr;
        } else if (list_size == 1) {
            list_data = new T(*static_cast<T *>(orig_list.list_data));
        } else if (list_size <= 5) {
            auto *src = static_cast<T *>(orig_list.list_data);
            auto *arr = new T[5];
            std::copy(src, src + list_size, arr);
            list_data = arr;
        } else {
            list_data = new std::list<T>(*static_cast<std::list<T> *>(orig_list.list_data));
        }
    }


    /// Конструктор по перемещению
    my_list(my_list &&orig_list) noexcept : list_size(orig_list.list_size), list_data(orig_list.list_data) {
        orig_list.list_data = nullptr;
        orig_list.list_size = 0;
    }


    /// Присваивание
    my_list &operator=(const my_list &orig) {
        if (this == &orig) return *this;
        clear_data();

        list_size = orig.list_size;
        if (list_size == 0) list_data = nullptr;
        else if (list_size == 1)
            list_data = new T(*static_cast<T *>(orig.list_data));
        else if (list_size <= 5) {
            auto *src = static_cast<T *>(orig.list_data);
            auto *arr = new T[5];
            std::copy(src, src + list_size, arr);
            list_data = arr;
        } else {
            list_data = new std::list<T>(*static_cast<std::list<T> *>(orig.list_data));
        }
        return *this;
    }


    /// Присваивание по перемещению
    my_list &operator=(my_list &&orig) noexcept {
        if (this == &orig) return *this;
        if (list_size == 1) delete static_cast<T *>(list_data);
        else if (list_size <= 5 && list_size > 1) delete[] static_cast<T *>(list_data);
        else if (list_size > 5) delete static_cast<std::list<T> *>(list_data);

        list_size = orig.list_size;
        list_data = orig.list_data;
        orig.list_size = 0;
        orig.list_data = nullptr;
        return *this;
    }


    /// Дестроуктор
    ~my_list() { clear_data(); }


    /// Сравнение двух спсиокв (поэлементное)
    bool operator==(const my_list& other) const {
        if (list_size != other.list_size)
            return false;

        if (list_size == 0)
            return true;

        if (list_size == 1)
            return *static_cast<T*>(list_data) == *static_cast<T*>(other.list_data);

        if (list_size <= 5) {
            T* arr1 = static_cast<T*>(list_data);
            T* arr2 = static_cast<T*>(other.list_data);
            for (size_t i = 0; i < list_size; ++i) {
                if (arr1[i] != arr2[i]) return false;
            }
            return true;
        }

        auto* list1 = static_cast<std::list<T>*>(list_data);
        auto* list2 = static_cast<std::list<T>*>(other.list_data);
        return *list1 == *list2;
    }


    /// Количество элементов в списке
    [[nodiscard]] size_t get_size() const { return list_size; }


    /// Получение элемента по индексу index
    T &get_element(size_t index) {
        if (index >= list_size)
            throw std::out_of_range(
                std::string("index ") + std::to_string(index) + " is out of range" + std::to_string(list_size));

        if (list_size == 1)
            return *static_cast<T *>(list_data);

        if (list_size <= 5)
            return static_cast<T *>(list_data)[index];

        auto &lst = *static_cast<std::list<T> *>(list_data);
        auto it = lst.begin();
        std::advance(it, index);
        return *it;
    }


    /// Константное получение элемента
    const T &get_element(size_t index) const { return const_cast<my_list *>(this)->get_element(index); }


    /// Устновка значения для элемента
    void set(size_t index, const T &value) { get_element(index) = value; }


    /// Добавление элемента в конец
    void push_back(const T &value) {
        if (list_size == 0) {
            list_data = new T(value);
            list_size = 1;
        } else if (list_size == 1) {
            // переход к массиву
            T *arr = new T[5];
            arr[0] = *static_cast<T *>(list_data);
            arr[1] = value;
            delete static_cast<T *>(list_data);
            list_data = arr;
            list_size = 2;
        } else if (list_size < 5) {
            static_cast<T *>(list_data)[list_size] = value;
            ++list_size;
        } else if (list_size == 5) {
            // переход к std::list
            auto* lst = new std::list<T>();
            T *arr = static_cast<T *>(list_data);
            for (int i = 0; i < 5; ++i) lst->push_back(arr[i]);
            lst->push_back(value);
            delete[] arr;
            list_data = lst;
            ++list_size;
        } else {
            // >5
            static_cast<std::list<T> *>(list_data)->push_back(value);
            ++list_size;
        }
    }

    // Удаление элемента из конца списка
    void pop_back() {
        if (list_size == 0) throw std::out_of_range("empty list");
        if (list_size == 1) {
            delete static_cast<T *>(list_data);
            list_data = nullptr;
            list_size = 0;
        } else if (list_size <= 5) {
            --list_size;
        } else if (list_size == 6) {
            // возвращаемся к массиву
            auto lst = static_cast<std::list<T> *>(list_data);
            T *arr = new T[5];
            int i = 0;
            for (auto it = lst->begin(); i < 5 && it != lst->end(); ++it, ++i)
                arr[i] = *it;
            delete lst;
            list_data = arr;
            list_size = 5;
        } else {
            // >6
            static_cast<std::list<T> *>(list_data)->pop_back();
            --list_size;
        }
    }

private:
    void clear_data() {
        if (list_size == 0) return;
        if (list_size == 1) delete static_cast<T *>(list_data);
        else if (list_size <= 5) delete[] static_cast<T *>(list_data);
        else delete static_cast<std::list<T> *>(list_data);
        list_data = nullptr;
        list_size = 0;
    }
};

#endif //EXAM_MY_LIST_H
