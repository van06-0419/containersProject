#pragma once
#include <iostream>
#include <stdexcept>

class ListContainer {
private:
	struct Node {
		int data;
		Node* prev;
		Node* next;
		Node(const int& val) : data(val), prev(nullptr), next(nullptr) {}
	};

	Node* head;
	Node* tail;
	size_t m_size;

public:
	ListContainer() : head(nullptr), tail(nullptr), m_size(0) {}

	~ListContainer() {
		Node* current = head;
		while (current) {
			Node* temp = current->next;
			delete current;
			current = temp;
		}
	}

	ListContainer(ListContainer&& other) noexcept
		: head(other.head), tail(other.tail), m_size(other.m_size) {
		other.head = other.tail = nullptr;
		other.m_size = 0;
	}

	ListContainer& operator=(ListContainer&& other) noexcept {
		if (this == &other)
			return *this;

		this->~ListContainer();
		head = other.head;
		tail = other.tail;
		m_size = other.m_size;
		other.head = other.tail = nullptr;
		other.m_size = 0;
		return *this;
	}

	void push_back(const int& value) {
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

	void insert(size_t pos, const int& value) {
		if (pos > m_size)
			throw std::out_of_range("pos is outside of current list size");

		if (pos == m_size) {
			push_back(value);
			return;
		}

		if (pos == 0) {
			Node* newNode = new Node(value);
			newNode->next = head;
			if (head)
				head->prev = newNode;
			else
				tail = newNode;
			head = newNode;
			++m_size;
			return;
		}

		Node* current = head;
		for (size_t i = 0; i < pos; ++i)
			current = current->next;
		Node* newNode = new Node(value);
		newNode->next = current;
		newNode->prev = current->prev;
		current->prev->next = newNode;
		current->prev = newNode;
		++m_size;
	}

	void erase(size_t pos) {
		if (pos >= m_size)
			throw std::out_of_range("pos is outside of current list size");

		if (pos == 0) {
			Node* temp = head;
			head = head->next;
			if (head)
				head->prev = nullptr;
			else
				tail = nullptr;
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
		for (size_t i = 0; i < pos; ++i)
			current = current->next;
		current->prev->next = current->next;
		current->next->prev = current->prev;
		delete current;
		--m_size;
	}

	int& get(size_t index) {
    	if (index >= m_size)
        	throw std::out_of_range("Index out of range");

    	Node* cur;
    	if (index < m_size / 2) {
        	cur = head;
        	for (size_t i = 0; i < index; ++i)
            	cur = cur->next;
	        return cur->data;
    	}

    	cur = tail;
    	for (size_t i = m_size - 1; i > index; --i)
    	    cur = cur->prev;
    	return cur->data;      
	}

	const int& get(size_t index) const {
    	if (index >= m_size)
        	throw std::out_of_range("Index out of range");

    	Node* cur;
    	if (index < m_size / 2) {
        	cur = head;
        	for (size_t i = 0; i < index; ++i)
            	cur = cur->next;
        	return cur->data;
    	}

    	cur = tail;
    	for (size_t i = m_size - 1; i > index; --i)
        	cur = cur->prev;
    	return cur->data;
	}

	int& operator[](size_t index) {
    	return get(index);
	}

	const int& operator[](size_t index) const {
    	return get(index);
	}

	size_t size() const { return m_size; }

	void print() const {
		Node* current = head;
		while (current) {
			std::cout << current->data;
			if (current->next)
				std::cout << ",";
			current = current->next;
		}
		std::cout << '\n';
	}

	class Iterator {
		Node* node;
	public:
		Iterator(Node* n) : node(n) {}
		int& operator*() const { return node->data; }
		Iterator& operator++() { node = node->next; return *this; }
		bool operator!=(const Iterator& other) const { return node != other.node; }
	};

	Iterator begin() const { return Iterator(head); }
	Iterator end() const { return Iterator(nullptr); }
};
