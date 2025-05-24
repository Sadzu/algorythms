#pragma once

#include "hash.h"

#include <iostream>
#include <random>
#include <vector>
#include <string>

using namespace std;

string generate_random_string(int length = 10) {
    static const char alphanum[] = "abcdefghijklmnopqrstuvwxyz";
    static mt19937 rng(random_device{}());
    static uniform_int_distribution<int> dist(0, sizeof(alphanum) - 2);

    string s;
    s.reserve(length);
    for (int i = 0; i < length; ++i) {
        s += alphanum[dist(rng)];
    }
    return s;
}

void test_hash_quality(int requested_capacity, int key_count) {
    static const unsigned int MERSENNE_PRIMES[11] = {
        3, 7, 31, 127, 251, 509, 1021, 2039, 4093, 8191, 16381
    };

    // Выбираем реальную capacity как в HashTable
    unsigned int min_size = requested_capacity * 2;
    unsigned int table_size = MERSENNE_PRIMES[0];
    for (int i = 0; i < 11; ++i) {
        if (MERSENNE_PRIMES[i] >= min_size) {
            table_size = MERSENNE_PRIMES[i];
            break;
        }
    }

    vector<int> slot_counts(table_size, 0); // f_i

    auto hash_func = [](const string& key, int m) -> int {
        unsigned long long hash = 0;
        for (char c : key) {
            hash = (hash << 8) | static_cast<unsigned char>(c);
        }
        return static_cast<int>(hash % m);
    };

    for (int i = 0; i < key_count; ++i) {
        string key = generate_random_string(10);
        int index = hash_func(key, table_size);
        slot_counts[index]++;
    }

    double expected = static_cast<double>(key_count) / table_size;
    double chi_squared = 0.0;

    for (int count : slot_counts) {
        chi_squared += ((count - expected) * (count - expected)) / expected;
    }

    double lower_bound = table_size - sqrt(table_size);
    double upper_bound = table_size + sqrt(table_size);

    cout << "Table size after adjustment: " << table_size << endl;
    cout << "Actual Chi-squared:    " << chi_squared << endl;
    cout << "Expected Chi-squared [ " << lower_bound << " ; " << upper_bound << " ]" << endl;
}

void testHashTableOpenMode(int N, double ALPHA) {
    HashTable<string, int>* table = new HashTable<string, int>(N);
    int capacity = table->getCapacity();
    int K = capacity * 20;

    cout << "Created table with capacity: " << capacity << '\n';
    cout << "Created table with size: " << K << '\n';

    vector<string> keys(K);
    for (int i = 0; i < K; ++i) {
        keys[i] = generate_random_string();
        table->insertData(keys[i], 1);
    }

    int insertCounter = 0, deleteCounter = 0, findCounter = 0;

    int operationsCount = K / 2;

    cout << "Running " << operationsCount << " operations for testing\n";

    for (int i = 0; i < operationsCount; ++i) {
        if (i % 10 == 0) {
            // 10% unsuccessful operations
            table->deleteData(generate_random_string());
            deleteCounter += table->getLooked();

            table->insertData(keys[rand() % K], 1);
            insertCounter += table->getLooked();

            try {
                table->getData(generate_random_string());
            } catch (...) {
                // miss
            }
            findCounter += table->getLooked();
        } else {
            // 90% successful operations
            int idx = rand() % K;
            table->deleteData(keys[idx]);
            deleteCounter += table->getLooked();

            string newKey = generate_random_string();
            table->insertData(newKey, 1);
            insertCounter += table->getLooked();

            keys[idx] = newKey;

            try {
                table->getData(keys[rand() % K]);
            } catch (...) {
                // should not happen
            }
            findCounter += table->getLooked();
        }
    }

    //cout << "  Search:    " << 0.9*((1.0 / ALPHA) * log(1.0 / (1.0 - ALPHA))) + 0.1*(1.0 / (1.0 - ALPHA)) << endl;
    //cout << "  Insert:    " << 0.9*((1.0 / ALPHA) * log(1.0 / (1.0 - ALPHA))) + 0.1*(1.0 / (1.0 - ALPHA)) << endl;

    cout << "Number of elements after testing: " << table->getSize() << endl;
    cout << "Final load factor: " << double(table->getSize()) / table->getCapacity() << endl;

    cout << "  Insert theory:    " << 0.1*((1.0 / 2.0) * (1.0 + 1.0 * (1.0 / (1.0 - ALPHA))) ) + 0.9*((1.0 / 2.0) * (1.0 + 1.0 * ((1.0 / (1.0 - ALPHA)) * (1.0 / (1.0 - ALPHA)))) ) << endl;
    cout << "Insert average cost: " << double(insertCounter) / operationsCount << '\n';

    cout << "  Delete theory:    " << 0.9*((1.0 / 2.0) * (1.0 + 1.0 * (1.0 / (1.0 - ALPHA))) ) + 0.1*((1.0 / 2.0) * (1.0 + 1.0 * ((1.0 / (1.0 - ALPHA)) * (1.0 / (1.0 - ALPHA)))) ) << endl;
    cout << "Delete average cost: " << double(deleteCounter) / operationsCount << '\n';

    cout << "  Search theory:    " << 0.9*((1.0 / 2.0) * (1.0 + 1.0 * (1.0 / (1.0 - ALPHA))) ) + 0.1*((1.0 / 2.0) * (1.0 + 1.0 * ((1.0 / (1.0 - ALPHA)) * (1.0 / (1.0 - ALPHA)))) ) << endl;
    cout << "Search average cost: " << double(findCounter) / operationsCount << '\n';

    delete table;
}

int run() {
    int N;
    double ALPHA;
    cout << "\nInitial capacity: ";
    cin >> N;
    cout << "Load factor (alpha): ";
    cin >> ALPHA;

    cout << "\n=== Hash Quality Test ===" << endl;
    test_hash_quality(N, N * ALPHA);

    testHashTableOpenMode(N, ALPHA);
    return 0;
}
