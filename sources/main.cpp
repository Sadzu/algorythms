#include "../headers/OneLinkedList.h"

#include <iostream>

int main() {
    int code = 0;
    int data = 0;
    size_t id = 0;
    OneLinkedList<int> list = OneLinkedList<int>();
    OneLinkedList<int> list2;
    OneLinkedList<int>::ListIterator iterator = list.begin();
    OneLinkedList<int>::ListIterator iter2 = list2.begin();
    while (true) {
        std::cout << "1 - Copy list\n";
        std::cout << "2 - Print list size\n";
        std::cout << "3 - Clear list\n";
        std::cout << "4 - Check list on isEmpty\n";
        std::cout << "5 - Check list in contains\n";
        std::cout << "6 - Print element by id\n";
        std::cout << "7 - Change element by id\n";
        std::cout << "8 - Get id by value\n";
        std::cout << "9 - Push back new element\n";
        std::cout << "10 - Push front new element\n";
        std::cout << "11 - Push element to position\n";
        std::cout << "12 - Delete by value\n";
        std::cout << "13 - Delete by id\n";
        std::cout << "14 - Pop back\n";
        std::cout << "15 - Pop front\n";
        std::cout << "16 - Get begin\n";
        std::cout << "17 - Get end\n";
        std::cout << "18 - Print list\n";
        std::cout << "19 - Begin read\n";
        std::cout << "20 - Begin write\n";
        std::cout << "21 - Begin++\n";
        std::cout << "22 - == with begin\n";
        std::cout << "23 - != with begin\n";
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
                list2 = OneLinkedList<int>(list);
            std::cout << "----------------------------------------------\n";
                list2.print();
            std::cout << "----------------------------------------------\n";
                break;
            case 2:
                std::cout << "----------------------------------------------\n";
                std::cout << list.getSize() << std::endl;
            std::cout << "----------------------------------------------\n";
                break;
            case 3:
                list.clear();
                break;
            case 4:
                std::cout << "----------------------------------------------\n";
                std::cout << list.isEmpty() << std::endl;
            std::cout << "----------------------------------------------\n";
                break;
            case 5:
                std::cout << "Enter element: ";
                std::cin >> data;
            std::cout << "----------------------------------------------\n";
                std::cout << list.contains(data) << std::endl;
            std::cout << "----------------------------------------------\n";
                break;
            case 6:
                std::cout << "Enter id: ";
                std::cin >> id;
            std::cout << "----------------------------------------------\n";
                try {
                    std::cout << list.getById(id) << std::endl;
                } catch (std::out_of_range &e) {
                    std::cout << e.what() << std::endl;
                }
            std::cout << "----------------------------------------------\n";
                break;
            case 7:
                std::cout << "Enter id: ";
                std::cin >> id;
                std::cout << "Enter value: ";
                std::cin >> data;
                std::cout << list.changeById(id, data) << std::endl;
                break;
            case 8:
                std::cout << "Enter value: ";
                std::cin >> data;
            std::cout << "----------------------------------------------\n";
                std::cout << list.getId(data) << std::endl;
            std::cout << "----------------------------------------------\n";
                break;
            case 9:
                std::cout << "Enter value: ";
                std::cin >> data;
                list.pushBack(data);
                iterator = list.begin();
                break;
            case 10:
                std::cout << "Enter value: ";
                std::cin >> data;
                list.pushFront(data);
                break;
            case 11:
                std::cout << "Enter value: ";
                std::cin >> data;
                std::cout << "Enter position: ";
                std::cin >> id;
                try {
                    list.addToPosition(id, data);
                } catch (std::out_of_range &e) {
                    std::cout << e.what() << std::endl;
                }
                break;
            case 12:
                std::cout << "Enter value: ";
                std::cin >> data;
                try {
                    list.removeByValue(data);
                } catch (std::invalid_argument &e) {
                    std::cout << e.what() << std::endl;
                }
                break;
            case 13:
                std::cout << "Enter id: ";
                std::cin >> id;
                try {
                    list.removeFromPosition(id);
                } catch (std::out_of_range &e) {
                    std::cout << e.what() << std::endl;
                }
                break;
            case 14:
                list.popBack();
                break;
            case 15:
                list.popFront();
                break;
            case 16:
                std::cout << "----------------------------------------------\n";
                std::cout << list.begin().getCurrent()->getData() << std::endl;
            std::cout << "----------------------------------------------\n";
                break;
            case 17:
                std::cout << "----------------------------------------------\n";
                std::cout << list.tail().getCurrent()->getData() << std::endl;
            std::cout << "----------------------------------------------\n";
                break;
            case 18:
                std::cout << "----------------------------------------------\n";
                list.print();
            std::cout << "----------------------------------------------\n";
                break;
            case 19:
                std::cout << "----------------------------------------------\n";
                try {
                    std::cout << *iterator << std::endl;
                } catch (std::runtime_error &e) {
                    std::cout << e.what() << std::endl;
                }
            std::cout << "----------------------------------------------\n";
                break;
            case 20:
                std::cout << "Enter value: ";
                try {
                    std::cin >> *iterator;
                } catch (std::runtime_error &er) {
                    std::cout << er.what() << std::endl;
                }
                break;
            case 21:
                iterator++;
            case 22:
                iter2 = list.begin();
            std::cout << "----------------------------------------------\n";
                std::cout << (iter2 == iterator) << std::endl;
            std::cout << "----------------------------------------------\n";
                break;
            case 23:
                iter2 = list.begin();
            std::cout << "----------------------------------------------\n";
                std::cout << (iter2 != iterator) << std::endl;
            std::cout << "----------------------------------------------\n";
                break;

        }
    }

    return 0;
}