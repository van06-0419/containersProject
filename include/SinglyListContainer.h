#pragma once
#include <iostream>
#include <stdexcept>

class SinglyListContainer {
private:
	struct Node {
		int data;
		Node* next;
		Node(int val) : data(val), next(nullptr) {}
	};
	Node* head;
	size_t m_size;

public:
	SinglyListContainer() {
		head = nullptr;
		m_size = 0;
	}

	~SinglyListContainer() {
		while (head) {
			Node* temp = head;
			head = head->next;
			delete temp;
		}
	}

	// добавить в указанное место.
	void insert(size_t pos, int value) {
		if (pos > m_size)
			throw std::out_of_range("pos is outside current list size"); 
			// Позиция вне списка

		Node* newNode = new Node(value);

		if (pos == 0) {
			newNode->next = head; // новый узел указывает на старый головной узел
			head = newNode; // теперь новый узел становится первым
		}
		else {
			Node* current = head;
			for (size_t i = 0; i < pos - 1; i++) {
				current = current->next;
			}
			newNode->next = current->next;
			current->next = newNode;
		}
		++m_size;
	}

	// добавить в конец
	void push_back(int value) {
		Node* newNode = new Node(value);

		if (head == nullptr) head = newNode; 
		// Если список пуст, новый узел становится головным узлом

		else {
			Node* current = head;
			while (current->next != nullptr) {
				current = current->next;
			}
			current->next = newNode;
		}
		++m_size;
	}

	// удалить элемент в указанной позиции
	void erase(size_t pos) {
		if (pos > m_size)
			throw std::out_of_range("pos is outside of current list size");

		if (pos == 0) {
			Node* temp = head;
			head = head->next;
			delete temp;
		}
		else {
			Node* current = head;
			for (size_t i = 0; i < pos - 1; i++) {
				current = current->next;
			}
			Node* temp = current->next;
			current->next = temp->next;
			delete temp;
		}
		--m_size;
	}

	size_t size() { return m_size; }

	void print() const {
		Node* current = head;
		while (current) {
			std::cout << current->data;
			if (current->next) std::cout << ", ";
			current = current->next;
		}
		std::cout << std::endl;
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

