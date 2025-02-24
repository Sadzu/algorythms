#include "../headers/OneLinkedList.h"

#include <iostream>

int main() {
    int code = 0;
    int data = 0;
    size_t id = 0;
    OneLinkedList<int> list = OneLinkedList<int>();
    OneLinkedList<int> list2;
    while (true) {
        std::cout << "1 - Make new list\n";
        std::cout << "2 - Copy list\n";
        std::cout << "3 - Print list size\n";
        std::cout << "4 - Clear list\n";
        std::cout << "5 - Check list on isEmpty\n";
        std::cout << "6 - Check list in contains\n";
        std::cout << "7 - Print element by id\n";
        std::cout << "8 - Change element by id\n";
        std::cout << "9 - Get id by value\n";
        std::cout << "10 - Push back new element\n";
        std::cout << "11 - Push front new element\n";
        std::cout << "12 - Push element to position\n";
        std::cout << "13 - Delete by value\n";
        std::cout << "14 - Delete by id\n";
        std::cout << "15 - Pop back\n";
        std::cout << "16 - Pop front\n";
        std::cout << "17 - Get begin\n";
        std::cout << "18 - Get end\n";
        std::cout << "19 - Print list\n";
        std::cout << "0 - Exit\n";
        std::cout << "Enter the operation's number: ";
        std::cin >> code;
        if (code <= 0) {
            break;
        }
        switch (code) {
            default:
                break;
            case 1:
                list = OneLinkedList<int>();
            std::cout << "----------------------------------------------\n";
                std::cout << "Created\n" << std::endl;
            std::cout << "----------------------------------------------\n";
                break;
            case 2:
                list2 = OneLinkedList<int>(list);
            std::cout << "----------------------------------------------\n";
                std::cout << "Copied\n" << std::endl;
                list2.print();
            std::cout << "----------------------------------------------\n";
                break;
            case 3:
                std::cout << "----------------------------------------------\n";
                std::cout << "Size: " << list.getSize() << std::endl;
            std::cout << "----------------------------------------------\n";
                break;
            case 4:
                list.clear();
            std::cout << "----------------------------------------------\n";
                std::cout << "Cleared\n";
            std::cout << "----------------------------------------------\n";
                break;
            case 5:
                std::cout << "----------------------------------------------\n";
                std::cout << list.isEmpty() << std::endl;
            std::cout << "----------------------------------------------\n";
                break;
            case 6:
                std::cout << "Enter element: ";
                std::cin >> data;
            std::cout << "----------------------------------------------\n";
                std::cout << list.contains(data) << std::endl;
            std::cout << "----------------------------------------------\n";
                break;
            case 7:
                std::cout << "Enter id: ";
                std::cin >> id;
            std::cout << "----------------------------------------------\n";
                std::cout << list.getById(id) << std::endl;
            std::cout << "----------------------------------------------\n";
            case 8:
                std::cout << "Enter id: ";
                std::cin >> id;
                std::cout << "Enter value: ";
                std::cin >> data;
                list.changeById(id, data);
            std::cout << "----------------------------------------------\n";
                std::cout << "Changed\n";
                list.print();
            std::cout << "----------------------------------------------\n";
                break;
            case 9:
                std::cout << "Enter value: ";
                std::cin >> data;
            std::cout << "----------------------------------------------\n";
                std::cout << list.getId(data) << std::endl;
            std::cout << "----------------------------------------------\n";
                break;
            case 10:
                std::cout << "Enter value: ";
                std::cin >> data;
                list.pushBack(data);
            std::cout << "----------------------------------------------\n";
                std::cout << "Pushed\n";
                list.print();
            std::cout << "----------------------------------------------\n";
                break;
            case 11:
                std::cout << "Enter value: ";
                std::cin >> data;
                list.pushFront(data);
            std::cout << "----------------------------------------------\n";
                std::cout << "Pushed\n";
                list.print();
            std::cout << "----------------------------------------------\n";
                break;
            case 12:
                std::cout << "Enter value: ";
                std::cin >> data;
                std::cout << "Enter position: ";
                std::cin >> id;
                list.addToPosition(id, data);
            std::cout << "----------------------------------------------\n";
                std::cout << "Added\n";
                list.print();
            std::cout << "----------------------------------------------\n";
                break;
            case 13:
                std::cout << "Enter value: ";
                std::cin >> data;
                list.removeByValue(data);
            std::cout << "----------------------------------------------\n";
                std::cout << "Removed\n";
                list.print();
            std::cout << "----------------------------------------------\n";
                break;
            case 14:
                std::cout << "Enter id: ";
                std::cin >> id;
                list.removeFromPosition(id);
            std::cout << "----------------------------------------------\n";
                std::cout << "Removed\n";
                list.print();
            std::cout << "----------------------------------------------\n";
                break;
            case 15:
                std::cout << "----------------------------------------------\n";
                list.popBack();
                std::cout << "Popped\n";
                list.print();
                std::cout << "----------------------------------------------\n";
                break;
            case 16:
                std::cout << "----------------------------------------------\n";
                list.popFront();
                std::cout << "Popped\n";
                list.print();
                std::cout << "----------------------------------------------\n";
                break;
            case 17:
                std::cout << "----------------------------------------------\n";
                list.print();
                std::cout << "Element on begin: " << (*list.begin())->getData() << std::endl;
                std::cout << "----------------------------------------------\n";
                break;
            case 18:
                std::cout << "----------------------------------------------\n";
                list.print();
                std::cout << "Element on end: " << (*list.end())->getData() << std::endl;
                std::cout << "----------------------------------------------\n";
                break;
            case 19:
                list.print();
                break;
        }
    }

    return 0;
}