#include <iostream>
#include <string>
#include "../headers/hash.h"
#include "../headers/benchmark.h"

using namespace std;

typedef string KeyType;  // Define the KeyType as string
typedef int DataType;    // Define the DataType as int

void printMenu() {
    cout << "\n0) Get capacity" << endl;
    cout << "1) Get size" << endl;
    cout << "2) Is empty" << endl;
    cout << "3) Clear" << endl;
    cout << "4) Get value by key" << endl;
    cout << "5) Insert key-value" << endl;
    cout << "6) Delete by key" << endl;
    cout << "7) Print table" << endl;
    cout << "8) Get last key info (original & converted)" << endl;
    cout << "9) Get last index" << endl;
    cout << "10) Get last probe count" << endl;

    cout << "Iterator:" << endl;
    cout << "\t11) Set to begin()" << endl;
    cout << "\t12) ++" << endl;
    cout << "\t13) == end() ?" << endl;
    cout << "\t14) Get value of current" << endl;
    cout << "\t15) Set value of current" << endl;
    cout << "\t16) Run benchmark" << endl;

    cout << "99) Exit" << endl;
}

int main() {
    int capacity;
    cout << "Enter hash table capacity: ";
    cin >> capacity;

    // Instantiate the HashTable with both KeyType and DataType
    HashTable<KeyType, DataType> table(capacity);
    auto it = table.end();
    string key;
    DataType value;
    bool isExit = false;

    while (!isExit) {
        printMenu();

        int cmd;
        cout << "Enter command: ";
        cin >> cmd;

        switch (cmd) {
            case 0:
                cout << table.getCapacity() << endl;
                break;

            case 1:
                cout << table.getSize() << endl;
                break;

            case 2:
                cout << table.isEmpty() << endl;
                break;

            case 3:
                table.clear();
                break;

            case 4:
                cout << "Enter the key: ";
                cin >> key;
                try {
                    cout << table.getData(key) << endl;
                } catch (...) {
                    cout << "Exception" << endl;
                }
                break;

            case 5:
                cout << "Enter the key: ";
                cin >> key;
                cout << "Enter the value: ";
                cin >> value;
                cout << table.insertData(key, value) << endl;
                break;

            case 6:
                cout << "Enter the key: ";
                cin >> key;
                cout << table.deleteData(key) << endl;
                break;

            case 7:
                table.print();
                break;

            case 8: {
                auto [original, converted] = table.getLastKeyInfo();
                cout << original << " " << converted << endl;
                break;
            }

            case 9:
                cout << table.getLastIndex() << endl;
                break;

            case 10:
                cout << table.getLooked() << endl;
                break;

            case 11:
                it = table.begin();
                break;

            case 12:
                try {
                    ++it;
                } catch (...) {
                    cout << "Exception" << endl;
                }
                break;

            case 13:
                cout << (it == table.end()) << endl;
                break;

            case 14:
                try {
                    cout << *it << endl;
                } catch (...) {
                    cout << "Exception" << endl;
                }
                break;

            case 15:
                try {
                    cout << "Enter the value: ";
                    cin >> *it;
                } catch (...) {
                    cout << "Exception" << endl;
                }
                break;
            case 16:
                run();

            case 99:
                isExit = true;
                break;

            default:
                break;
        }
    }

    return 0;
}
