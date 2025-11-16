#pragma once
#include <iostream>
#include <stdexcept>
#include <utility>

template<typename T>
class ListContainer {
private:
    struct Node {
        T data;
        Node* prev;
        Node* next;
        explicit Node(const T& val) : data(val), prev(nullptr), next(nullptr) {}
        explicit Node(T&& val) : data(std::move(val)), prev(nullptr), next(nullptr) {}
    };

    Node* head = nullptr;
    Node* tail = nullptr;
    size_t m_size = 0;

    // 清空函数
    void clearAll() noexcept {
        Node* current = head;
        while (current) {
            Node* tmp = current->next;
            delete current;
            current = tmp;
        }
        head = tail = nullptr;
        m_size = 0;
    }

    void clearAfterMove() noexcept {
        head = tail = nullptr;
        m_size = 0;
    }

public:
    ListContainer() = default;

    ~ListContainer() { clear_all(); }

    ListContainer(const ListContainer&) = delete;
    ListContainer& operator=(const ListContainer&) = delete;

    // 移动构造
    ListContainer(ListContainer&& other) noexcept
        : head(std::move(other.head)), tail(std::move(other.tail)), m_size(std::move(other.m_size))
    {
        other.clearAfterMove();
    }

    // 移动赋值
    ListContainer& operator=(ListContainer&& other) noexcept {
        if (this == &other) return *this;

        clearAll();
        head = std::move(other.head);
        tail = std::move(other.tail);
        m_size = std::move(other.m_size);

        other.clearAfterMove();
        return *this;
    }

    // push_back
    void push_back(const T& value) {
        Node* newNode = new Node(value);
        if (!head) {
            head = tail = newNode;
            ++m_size;
            return;
        }
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
        ++m_size;
    }

    void push_back(T&& value) {
        Node* newNode = new Node(std::move(value));
        if (!head) {
            head = tail = newNode;
            ++m_size;
            return;
        }
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
        ++m_size;
    }

    // insert
    void insert(size_t pos, const T& value) {
        if (pos > m_size) throw std::out_of_range("pos is outside of current list size");

        if (pos == m_size) { push_back(value); return; }
        if (pos == 0) {
            Node* newNode = new Node(value);
            newNode->next = head;
            if (head) head->prev = newNode;
            else tail = newNode;
            head = newNode;
            ++m_size;
            return;
        }

        Node* current = head;
        for (size_t i = 0; i < pos; ++i) current = current->next;
        Node* newNode = new Node(value);
        newNode->next = current;
        newNode->prev = current->prev;
        current->prev->next = newNode;
        current->prev = newNode;
        ++m_size;
    }

    void insert(size_t pos, T&& value) {
        if (pos > m_size) throw std::out_of_range("pos is outside of current list size");

        if (pos == m_size) { push_back(std::move(value)); return; }
        if (pos == 0) {
            Node* newNode = new Node(std::move(value));
            newNode->next = head;
            if (head) head->prev = newNode;
            else tail = newNode;
            head = newNode;
            ++m_size;
            return;
        }

        Node* current = head;
        for (size_t i = 0; i < pos; ++i) current = current->next;
        Node* newNode = new Node(std::move(value));
        newNode->next = current;
        newNode->prev = current->prev;
        current->prev->next = newNode;
        current->prev = newNode;
        ++m_size;
    }

    // erase
    void erase(size_t pos) {
        if (pos >= m_size) throw std::out_of_range("pos is outside of current list size");

        if (pos == 0) {
            Node* temp = head;
            head = head->next;
            if (head) head->prev = nullptr;
            else tail = nullptr;
            delete temp;
            --m_size;
            return;
        }

        if (pos == m_size - 1) {
            Node* temp = tail;
            tail = tail->prev;
            tail->next = nullptr;
            delete temp;
            --m_size;
            return;
        }

        Node* current = head;
        for (size_t i = 0; i < pos; ++i) current = current->next;
        current->prev->next = current->next;
        current->next->prev = current->prev;
        delete current;
        --m_size;
    }

    // 访问元素
    T& get(size_t index) {
        if (index >= m_size) throw std::out_of_range("Index out of range");

        Node* cur;
        if (index < m_size / 2) {
            cur = head;
            for (size_t i = 0; i < index; ++i) cur = cur->next;
        } else {
            cur = tail;
            for (size_t i = m_size - 1; i > index; --i) cur = cur->prev;
        }
        return cur->data;
    }

    const T& get(size_t index) const {
        if (index >= m_size) throw std::out_of_range("Index out of range");

        Node* cur;
        if (index < m_size / 2) {
            cur = head;
            for (size_t i = 0; i < index; ++i) cur = cur->next;
        } else {
            cur = tail;
            for (size_t i = m_size - 1; i > index; --i) cur = cur->prev;
        }
        return cur->data;
    }

    T& operator[](size_t index) { return get(index); }
    const T& operator[](size_t index) const { return get(index); }

    size_t size() const noexcept { return m_size; }

    void print() const {
        Node* current = head;
        while (current) {
            std::cout << current->data;
            if (current->next) std::cout << ",";
            current = current->next;
        }
        std::cout << '\n';
    }

    // 迭代器
    class Iterator {
        Node* node;
    public:
        explicit Iterator(Node* n) : node(n) {}
        T& operator*() const { return node->data; }
        Iterator& operator++() { node = node->next; return *this; }
        bool operator!=(const Iterator& other) const { return node != other.node; }
    };

    Iterator begin() const { return Iterator(head); }
    Iterator end() const { return Iterator(nullptr); }
};
