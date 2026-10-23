#include <iostream>
#include <cassert>
#include <string>
#include "ds/DoublyLinkedList.h"

void testBasicOperations() {
    std::cout << "[Test 1] Kiem tra Push, Pop, Front, Back, Size, Empty..." << std::endl;
    DoublyLinkedList<int> list;

    assert(list.empty() == true);
    assert(list.size() == 0);

    // Test pushBack: [10, 20, 30]
    list.pushBack(10);
    list.pushBack(20);
    list.pushBack(30);
    assert(list.size() == 3);
    assert(list.front() == 10);
    assert(list.back() == 30);

    // Test pushFront: [5, 10, 20, 30]
    list.pushFront(5);
    assert(list.size() == 4);
    assert(list.front() == 5);

    // Test popFront: [10, 20, 30]
    list.popFront();
    assert(list.size() == 3);
    assert(list.front() == 10);

    // Test popBack: [10, 20]
    list.popBack();
    assert(list.size() == 2);
    assert(list.back() == 20);

    std::cout << "=> [Test 1] Thanh cong!\n" << std::endl;
}

void testAccessAndRemove() {
    std::cout << "[Test 2] Kiem tra ham at() va remove()..." << std::endl;
    DoublyLinkedList<std::string> list;

    list.pushBack("A"); // index 0
    list.pushBack("B"); // index 1
    list.pushBack("C"); // index 2
    list.pushBack("D"); // index 3
    list.pushBack("E"); // index 4

    // Test at() duyet xuoi va nguoc
    assert(list.at(0) == "A");
    assert(list.at(1) == "B");
    assert(list.at(3) == "D");
    assert(list.at(4) == "E");

    // Test remove() phan tu giua: xoa "C" -> con [A, B, D, E]
    bool removed = list.remove("C");
    assert(removed == true);
    assert(list.size() == 4);
    assert(list.at(2) == "D");

    // Test remove() phan tu dau: xoa "A" -> con [B, D, E]
    list.remove("A");
    assert(list.front() == "B");

    // Test remove() phan tu cuoi: xoa "E" -> con [B, D]
    list.remove("E");
    assert(list.back() == "D");

    // Test remove() phan tu khong ton tai
    assert(list.remove("Z") == false);

    std::cout << "=> [Test 2] Thanh cong!\n" << std::endl;
}

void testCopyAndClear() {
    std::cout << "[Test 3] Kiem tra Copy Constructor, Operator= va Clear..." << std::endl;
    DoublyLinkedList<int> list1;
    list1.pushBack(1);
    list1.pushBack(2);
    list1.pushBack(3);

    // Test Copy Constructor (Deep copy)
    DoublyLinkedList<int> list2 = list1;
    assert(list2.size() == 3);
    assert(list2.front() == 1);
    assert(list2.back() == 3);

    // Thay doi list1 khong duoc anh huong den list2
    list1.popFront();
    assert(list1.size() == 2);
    assert(list2.size() == 3);
    assert(list2.front() == 1);

    // Test Operator=
    DoublyLinkedList<int> list3;
    list3.pushBack(99);
    list3 = list2;
    assert(list3.size() == 3);
    assert(list3.front() == 1);

    // Test clear()
    list2.clear();
    assert(list2.empty() == true);
    assert(list2.size() == 0);

    std::cout << "=> [Test 3] Thanh cong!\n" << std::endl;
}

int main() {
    std::cout << "================ BAT DAU TEST DOUBLYLINKEDLIST ================\n" << std::endl;
    
    testBasicOperations();
    testAccessAndRemove();
    testCopyAndClear();

    std::cout << "================ TAT CA TEST CASES DEU PASS! ================" << std::endl;
    return 0;
}