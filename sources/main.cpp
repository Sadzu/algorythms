#include "../headers/BinarySearchTree.h"
#include "../headers/bst_tests.h"

int main() {
    BinarySearchTree<int, int> tree = BinarySearchTree<int, int>();
    int key = 0, value = 0;
    int operation = 0;
    int nForTests = 0;
    BinarySearchTree<int, int>::iterator it = tree.begin();
    BinarySearchTree<int, int>::reverse_iterator rIt = tree.rbegin();
    bool compare;
    while (true) {
        std::cout << "1 - Print tree size\n";
        std::cout << "2 - Clear tree\n";
        std::cout << "3 - Is empty\n";
        std::cout << "4 - Read by the key\n";
        std::cout << "5 - Write by the key\n";
        std::cout << "6 - Insert\n";
        std::cout << "7 - Delete by key\n";
        std::cout << "8 - Keys list\n";
        std::cout << "9 - Count of keys bigger than value\n";
        std::cout << "10 - Begin (minimum key)\n";
        std::cout << "11 - rBegin (maximum key)\n";
        std::cout << "12 - End\n";
        std::cout << "13 - rEnd\n";
        std::cout << "14 - Iterator*\n";
        std::cout << "15 - Write iterator*\n";
        std::cout << "16 - Iterator++\n";
        std::cout << "17 - Iterator--\n";
        std::cout << "18 - rIterator*\n";
        std::cout << "19 - Write rIterator*\n";
        std::cout << "20 - rIterator++\n";
        std::cout << "21 - rIterator--\n";
        std::cout << "22 - Iterator==begin\n";
        std::cout << "23 - Iterator!=begin\n";
        std::cout << "24 - rIterator==rbegin\n";
        std::cout << "25 - rIterator!=rbegin\n";
        std::cout << "26 - Print tree\n";
        std::cout << "27 - Get nodes visited\n";
        std::cout << "28 - Test rand\n";
        std::cout << "29 - Test ord\n";
        std::cout << "Enter a number (0 to terminate): ";
        std::cin >> operation;
        switch (operation) {
            case 0:
                return 0;
            case 1:
                std::cout << "------------------\n";
                std::cout << tree.getSize() << std::endl;
                std::cout << "------------------\n";
                break;
            case 2:
                std::cout << "------------------\n";
                tree.clear();
                std::cout << "------------------\n";
                break;
            case 3:
                std::cout << "------------------\n";
                std::cout << tree.isEmpty() << std::endl;
                std::cout << "------------------\n";
                break;
            case 4:
                std::cout << "Enter a key: ";
                std::cin >> key;
                try {
                    std::cout << "------------------\n";
                    std::cout << tree[key] << std::endl;
                    std::cout << "------------------\n";
                } catch (const std::runtime_error& e) {
                    std::cout << "Exception: " << e.what() << std::endl;
                }
                break;
            case 5:
                std::cout << "Enter a key: ";
                std::cin >> key;
                std::cout << "Enter a value: ";
                try {
                    std::cin >> tree[key];
                } catch (std::runtime_error& e) {
                    std::cout << "Exception: " << e.what() << std::endl;
                }
                break;
            case 6:
                std::cout << "Enter a key: ";
                std::cin >> key;
                std::cout << "Enter a value: ";
                std::cin >> value;
                std::cout << "------------------\n";
                std::cout << tree.insert(key, value) << '\n';
                std::cout << "------------------\n";
                break;
            case 7:
                std::cout << "Enter a key: ";
                std::cin >> key;
                std::cout << "------------------\n";
                std::cout << tree.remove(key) << std::endl;
                std::cout << "------------------\n";
                break;
            case 8:
                std::cout << "------------------\n";
                for (int key : tree.getKeys()) {
                    std::cout << key << " ";
                }
                std::cout << std::endl;
                std::cout << "------------------\n";
                break;
            case 9:
                std::cout << "Enter a key: ";
                std::cin >> key;
                std::cout << "------------------\n";
                std::cout << tree.countGreaterThan(key) << '\n';
                std::cout << "------------------\n";
                break;
            case 10:
                it = tree.begin();
                break;
            case 11:
                rIt = tree.rbegin();
                break;
            case 12:
                it = tree.end();
                break;
            case 13:
                rIt = tree.rend();
                break;
            case 14:
                std::cout << "------------------\n";
                try {
                    std::cout << *it << std::endl;
                } catch (std::runtime_error& e) {
                    std::cout << "Exception: " << e.what() << std::endl;
                }
                std::cout << "------------------\n";
                break;
            case 15:
                std::cout << "Enter a value: ";
                try {
                    std::cin >> *it;
                } catch (std::runtime_error& e) {
                    std::cout << "Exception: " << e.what() << std::endl;
                }
                break;
            case 16:
                ++it;
                break;
            case 17:
                --it;
                break;
            case 18:
                std::cout << "------------------\n";
                try {
                    std::cout << *rIt << std::endl;
                } catch (std::runtime_error& e) {
                    std::cout << "Exception: " << e.what() << std::endl;
                }
                std::cout << "------------------\n";
                break;
            case 19:
                std::cout << "Enter a value: ";
                try {
                    std::cin >> *rIt;
                } catch (std::runtime_error& e) {
                    std::cout << "Exception: " << e.what() << std::endl;
                }
                break;
            case 20:
                ++rIt;
                break;
            case 21:
                --rIt;
            case 22:
                compare = it == tree.begin();
                std::cout << "------------------\n";
                std::cout << compare << std::endl;
                std::cout << "------------------\n";
                break;
            case 23:
                 compare = it != tree.begin();
                std::cout << "------------------\n";
                std::cout << compare << std::endl;
                std::cout << "------------------\n";
                break;
            case 24:
                compare = rIt == tree.rbegin();
                std::cout << "------------------\n";
                std::cout << compare << std::endl;
                std::cout << "------------------\n";
                break;
            case 25:
                compare = rIt != tree.rbegin();
                std::cout << "------------------\n";
                std::cout << compare << std::endl;
                std::cout << "------------------\n";
                break;
            case 26:
                std::cout << "------------------\n";
                tree.print();
                std::cout << "------------------\n";
                break;
            case 27:
                std::cout << "------------------\n";
                std::cout << tree.getNodesVisited() << std::endl;
                std::cout << "------------------\n";
                break;
            case 28:
                std::cout << "Enter n: ";
                std::cin >> nForTests;
                std::cout << "--------------\n";
                test_rand(nForTests);
                std::cout << "---------------\n";
                break;
            case 29:
                std::cout << "Enter n: ";
                std::cin >> nForTests;
                std::cout << "---------------\n";
                test_ord(nForTests);
                std::cout << "---------------\n";
                break;
        }
    }

    return 0;
}