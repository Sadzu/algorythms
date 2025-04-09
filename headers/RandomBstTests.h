#pragma once

#include <iostream>
#include <tgmath.h>

#include "RandomizedBinarySearchTree.h"

using namespace std;
typedef unsigned long long INT_64;

// Переменная и константы генератора LineRand()
static INT_64 RRand = 15750;
const INT_64 mRand = (1 << 63) - 1;
const INT_64 aRand = 6364136223846793005;
const INT_64 cRand = 1442695040888963407;

// Функция установки первого случайного числа от часов компьютера
void sRand() {
    srand(time(0));
    RRand = (INT_64) rand();
}

// Функция генерации случайного числа
// Линейный конгруэнтный генератор Xi+1=(a*Xi+c)%m
INT_64 LineRand() {
    INT_64 y1, y2;
    y1 = (aRand * RRand + cRand) % mRand;
    y2 = (aRand * y1 + cRand) % mRand;
    RRand = y1 & 0xFFFFFFFF00000000LL ^ (y2 & 0xFFFFFFFF00000000LL) >> 32;
    return RRand;
}

void test_rand(int n) {
    RandomizedBinarySearchTree<INT_64, int> tree;
    INT_64* m = new INT_64[n];
    sRand();

    // Заполнение дерева
    for (int i = 0; i < n; i++) {
        m[i] = LineRand();
        tree.insert(m[i], 1);
    }

    cout << "items count before test: " << tree.getSize() << endl;

    double I = 0, D = 0, S = 0;
    int operations = n / 2;

    for (int i = 0; i < operations; i++) {
        // Всегда сначала удаляем, потом вставляем
        int ind = rand() % n;
        tree.remove(m[ind]);
        D += tree.getNodesVisited();

        INT_64 key = LineRand();
        tree.insert(key, 1);
        I += tree.getNodesVisited();
        m[ind] = key;

        // Поиск
        try {
            tree[m[rand() % n]];
            S += tree.getNodesVisited();
        } catch (...) {
            S += tree.getNodesVisited();
        }
    }

    cout << "items count after test: " << tree.getSize() << endl;
    cout << "1.39*log2(n)=" << 1.39 * (log((double)n) / log(2.0)) << endl;
    cout << "Count insert: " << I / operations << endl;
    cout << "Count delete: " << D / operations << endl;
    cout << "Count search: " << S / operations << endl;

    delete[] m;
}