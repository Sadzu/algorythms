// #include "../headers/hash.h"
// #include <cmath>
//
// template <class Key, class Data>
// const unsigned int HashTable<Key, Data>::MERSENNE_PRIMES[11] = {
//     3,          // 2^2 - 1
//     7,          // 2^3 - 1
//     31,         // 2^5 - 1
//     127,        // 2^7 - 1
//     251,
//     509,
//     1021,
//     2039,
//     4093,
//     8191,
//     16381
// };
//
// template <class Key, class Data>
// HashTable<Key, Data>::HashTable(int cap) {
//     unsigned int min_size = cap * 2;
//
//     capacity = MERSENNE_PRIMES[0]; // Начинаем с первого числа
//     for (int i = 0; i < 15; ++i) {
//         if (MERSENNE_PRIMES[i] >= min_size) {
//             capacity = MERSENNE_PRIMES[i];
//             break;
//         }
//     }
//
//     if (capacity < min_size) {
//         for (int i = 14; i >= 0; --i) {
//             if (MERSENNE_PRIMES[i] != 0) {
//                 capacity = MERSENNE_PRIMES[i];
//                 break;
//             }
//         }
//     }
//
//     size = 0;
//     looked = 0;
//     lastIndex = -1;
//     lastKey = Key();
//     lastHashKey = 0;
//
//     keyArray = new Key[capacity];
//     dataArray = new Data[capacity];
//     stateArray = new State[capacity];
//
//     for (int i = 0; i < capacity; ++i)
//         stateArray[i] = FREE;
// }
//
// // Деструктор
// template <class Key, class Data>
// HashTable<Key, Data>::~HashTable() {
//     delete[] keyArray;
//     delete[] dataArray;
//     delete[] stateArray;
// }
//
// // Преобразование строки в целое число (k → k')
// template <class Key, class Data>
// unsigned long long HashTable<Key, Data>::stringToInt(const std::string& key) {
//     unsigned long long result = 0;
//     for (char c : key) {
//         result = (result << 8) | static_cast<unsigned char>(c);
//     }
//     return result;
// }
//
// // Основная хеш-функция (модульная)
// template <class Key, class Data>
// int HashTable<Key, Data>::hash(Key key) {
//     std::string str = keyToString(key); // преобразуем ключ в строку
//     lastKey = key;                      // сохраняем исходный ключ
//     lastHashKey = stringToInt(str);     // преобразуем строку в число
//     return static_cast<int>(lastHashKey % capacity);
// }
//
// // Вторая хеш-функция для двойного хеширования
// template <class Key, class Data>
// int HashTable<Key, Data>::hash2(Key key) {
//     std::string str = keyToString(key); // преобразуем ключ в строку
//     unsigned long long h = stringToInt(str);
//     return 1 + static_cast<int>(h % (capacity - 1));
// }
//
// // Вставка
// template <class Key, class Data>
// bool HashTable<Key, Data>::insertData(Key key, Data data) {
//     int h1 = hash(key);
//     int h2 = hash2(key);
//     looked = 0;
//
//     int i = 0;
//     int pos = -1;
//     int j;
//
//     do {
//         j = (h1 + i * h2) % capacity;
//         looked++;
//
//         if (stateArray[j] == BUSY && keyArray[j] == key) {
//             return false; // Ключ уже есть
//         }
//
//         if (stateArray[j] == DELETED && pos == -1) {
//             pos = j;
//         }
//
//         if (stateArray[j] == FREE && pos == -1) {
//             pos = j;
//         }
//
//         i++;
//     } while (i < capacity && stateArray[j] != FREE);
//
//     if (i == capacity && pos == -1) {
//         return false; // таблица полностью занята
//     }
//
//     keyArray[pos] = key;
//     dataArray[pos] = data;
//     stateArray[pos] = BUSY;
//     size++;
//     lastIndex = pos;
//     return true;
// }
//
//
// // Удаление
// template <class Key, class Data>
// bool HashTable<Key, Data>::deleteData(Key key) {
//     int h1 = hash(key);
//     int h2 = hash2(key);
//     looked = 0;
//
//     for (int i = 0; i < capacity; ++i) {
//         int index = (h1 + i * h2) % capacity;
//         looked++;
//
//         if (stateArray[index] == FREE)
//             return false;
//
//         if (stateArray[index] == BUSY && keyArray[index] == key) {
//             stateArray[index] = DELETED;
//             size--;
//             lastIndex = index;
//             return true;
//         }
//     }
//
//     return false;
// }
//
// // Поиск
// template <class Key, class Data>
// Data& HashTable<Key, Data>::getData(Key key) {
//     int h1 = hash(key);
//     int h2 = hash2(key);
//     looked = 0;
//
//     for (int i = 0; i < capacity; ++i) {
//         int index = (h1 + i * h2) % capacity;
//         looked++;
//
//         if (stateArray[index] == FREE)
//             break;
//
//         if (stateArray[index] == BUSY && keyArray[index] == key) {
//             lastIndex = index;
//             return dataArray[index];
//         }
//     }
//
//     throw std::runtime_error("Key not found");
// }
//
// // Очистка таблицы
// template <class Key, class Data>
// void HashTable<Key, Data>::clear() {
//     for (int i = 0; i < capacity; ++i)
//         stateArray[i] = FREE;
//     size = 0;
// }
//
// // Проверка на пустоту
// template <class Key, class Data>
// bool HashTable<Key, Data>::isEmpty() {
//     return size == 0;
// }
//
// // Размер
// template <class Key, class Data>
// int HashTable<Key, Data>::getSize() {
//     return size;
// }
//
// // Ёмкость
// template <class Key, class Data>
// int HashTable<Key, Data>::getCapacity() {
//     return capacity;
// }
//
// // Заполненность
// template <class Key, class Data>
// double HashTable<Key, Data>::getFilling() {
//     return (double)size / capacity;
// }
//
// // Количество зондирований
// template <class Key, class Data>
// int HashTable<Key, Data>::getLooked() {
//     return looked;
// }
//
// // Индекс последней операции
// template <class Key, class Data>
// int HashTable<Key, Data>::getLastIndex() {
//     return lastIndex;
// }
//
// // Ключ и его преобразование
// template <class Key, class Data>
// std::pair<Key, unsigned long long> HashTable<Key, Data>::getLastKeyInfo() {
//     return {lastKey, lastHashKey};
// }
//
// // Вывод таблицы
// template <class Key, class Data>
// void HashTable<Key, Data>::print() {
//     for (int i = 0; i < capacity; ++i) {
//         std::cout << i << " ";  // Индекс
//
//         if (stateArray[i] == FREE) {
//             std::cout << "FREE -\n";  // Если FREE, ключа нет (выводим "-")
//         }
//         else if (stateArray[i] == DELETED) {
//             std::cout << "DELETED -\n";  // Если DELETED, ключа тоже нет
//         }
//         else {  // BUSY — выводим ключ
//             std::cout << "BUSY " << keyArray[i] << "\n";
//         }
//     }
// }
//
// // ======================= ИТЕРАТОР ==========================
//
// template <class Key, class Data>
// HashTable<Key, Data>::Iterator::Iterator(HashTable<Key, Data>* t, int pos) {
//     table = t;
//     index = pos;
//
//     if (index == -1 || table->stateArray[index] != BUSY)
//         ++(*this);
// }
//
// template <class Key, class Data>
// Data& HashTable<Key, Data>::Iterator::operator*() {
//     if (index < 0 || index >= table->capacity || table->stateArray[index] != BUSY)
//         throw std::runtime_error("Iterator out of bounds");
//     return table->dataArray[index];
// }
//
// template <class Key, class Data>
// typename HashTable<Key, Data>::Iterator& HashTable<Key, Data>::Iterator::operator++() {
//     if (index >= table->capacity) {
//         return *this;
//     }
//
//     do {
//         index++;
//     } while (index < table->capacity && table->stateArray[index] != BUSY);
//
//     return *this;
// }
//
// template <class Key, class Data>
// bool HashTable<Key, Data>::Iterator::operator==(const Iterator& other) const {
//     return table == other.table && index == other.index;
// }
//
// template <class Key, class Data>
// bool HashTable<Key, Data>::Iterator::operator!=(const Iterator& other) const {
//     return !(*this == other);
// }
//
// template <class Key, class Data>
// typename HashTable<Key, Data>::Iterator HashTable<Key, Data>::begin() {
//     for (int i = 0; i < capacity; ++i) {
//         if (stateArray[i] == BUSY) {
//             return Iterator(this, i);
//         }
//     }
//     return end();
// }
//
// template <class Key, class Data>
// typename HashTable<Key, Data>::Iterator HashTable<Key, Data>::end() {
//     return Iterator(this, capacity);
// }
