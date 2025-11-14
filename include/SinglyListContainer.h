#pragma once
#include <iostream>
#include <stdexcept>

class SinglyListContainer {
private:
	struct Node {
		int data;
		Node* next;
		explicit Node(int val) : data(val), next(nullptr) {}
	};

	Node* head = nullptr;
	size_t m_size = 0;

public:
	SinglyListContainer() = default;
	~SinglyListContainer() {
		while (head) {
			Node* temp = head;
			head = head->next;
			delete temp;
		}
	}

	SinglyListContainer(const SinglyListContainer&) = delete;
	SinglyListContainer& operator=(const SinglyListContainer&) = delete;

	SinglyListContainer(SinglyListContainer&& other) noexcept
		: head(other.head), m_size(other.m_size) {
		other.head = nullptr;
		other.m_size = 0;
	}

	SinglyListContainer& operator=(SinglyListContainer&& other) noexcept {
		if (this == &other)
			return *this;

		while (head) {
			Node* temp = head;
			head = head->next;
			delete temp;
		}
		head = other.head;
		m_size = other.m_size;
		other.head = nullptr;
		other.m_size = 0;
		return *this;
	}

	void insert(size_t pos, int value) {
		if (pos > m_size)
			throw std::out_of_range("position out of range");

		if (pos == 0) {
			Node* node = new Node(value);
			node->next = head;
			head = node;
			++m_size;
			return;
		}

		Node* current = head;
		for (size_t i = 0; i < pos - 1; ++i)
			current = current->next;
		Node* node = new Node(value);
		node->next = current->next;
		current->next = node;
		++m_size;
	}

	void push_back(int value) {
		Node* node = new Node(value);

		if (head == nullptr) {
			head = node;
			++m_size;
			return;
		}

		Node* current = head;
		while (current->next)
			current = current->next;
		current->next = node;
		++m_size;
	}

	void erase(size_t pos) {
		if (pos >= m_size)
			throw std::out_of_range("position out of range");

		if (pos == 0) {
			Node* temp = head;
			head = head->next;
			delete temp;
			--m_size;
			return;
		}

		Node* current = head;
		for (size_t i = 0; i < pos - 1; ++i)
			current = current->next;
		Node* target = current->next;
		current->next = target->next;
		delete target;
		--m_size;
	}

	int& get(size_t index) {
    	if (index >= m_size)
        	throw std::out_of_range("Index out of range");

    	Node* cur = head;
    	for (size_t i = 0; i < index; i++)
        	cur = cur->next;

    	return cur->data;
	}

	const int& get(size_t index) const {
    	if (index >= m_size)
        	throw std::out_of_range("Index out of range");

    	Node* cur = head;
    	for (size_t i = 0; i < index; i++)
        	cur = cur->next;

    	return cur->data;
	}

	int& operator[](size_t index) {
    	return get(index);
	}

	const int& operator[](size_t index) const {
		return get(index);
	}


	size_t size() const noexcept { return m_size; }

	void print() const {
		Node* p = head;
		while (p) {
			std::cout << p->data;
			if (p->next) std::cout << ", ";
			p = p->next;
		}
		std::cout << '\n';
	}

	class Iterator {
		Node* node;
	public:
		explicit Iterator(Node* n) : node(n) {}
		int& operator*() const { return node->data; }
		Iterator& operator++() { node = node->next; return *this; }
		bool operator!=(const Iterator& other) const { return node != other.node; }
	};

	Iterator begin() const { return Iterator(head); }
	Iterator end() const { return Iterator(nullptr); }
};
