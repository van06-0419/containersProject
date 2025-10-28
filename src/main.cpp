#include <iostream>
#include "SeqContainer.h"
#include "ListContainer.h"
using namespace std;

int main() {
    cout << "=== Тест последовательного контейнера ===" << endl;
    SeqContainer<int> seq;
    for (int i = 0; i < 10; ++i) seq.push_back(i);
    seq.print();
    cout << "Размер: " << seq.size() << endl;

    seq.erase(2);
    seq.erase(3);
    seq.erase(4);
    seq.print();

    seq.insert(0, 10);
    seq.print();

    seq.insert(seq.size() / 2, 20);
    seq.print();

    seq.push_back(30);
    seq.print();

    cout << endl << "=== Тест спискового контейнера ===" << endl;
    ListContainer<int> list;
    for (int i = 0; i < 10; ++i) list.push_back(i);
    list.print();
    cout << "Размер: " << list.size() << endl;

    list.erase(2);
    list.erase(3);
    list.erase(4);
    list.print();

    list.insert(0, 10);
    list.print();

    list.insert(list.size() / 2, 20);
    list.print();

    list.push_back(30);
    list.print();

    return 0;
}
