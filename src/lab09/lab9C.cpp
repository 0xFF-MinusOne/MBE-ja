/*
 *  コンパイル方法: g++ -fstack-protector-all -z relro -z now ./lab9C.cpp -o lab9C
 *
 *  DSVector - std::vector の基本的な宿題レベルの実装
 *  これは、それをテストするためのラッパープログラムです!
 */

#include <iostream>
#include <limits>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include "utils.h"

ENABLE_TIMEOUT(60)

void
print_menu(void)
{
    printf("+----- DSVector テストメニュー ------+\n"
           "| 1. アイテムを追加する            |\n"
           "| 2. アイテムを読み出す            |\n"
           "| 3. 終了する                      |\n"
           "+----------------------------------+\n");
}

template <class T>
class DSVector {
    public:
                     // 0 から始まる添字は好きじゃない。最初に VB.NET を学んだので。
        DSVector() : len(1), alloc_len(len+256) {}
        unsigned int size() { return len; }
        void append(T item);
                                            // 情報リークもなし!
        T get(unsigned int index) { return (index < alloc_len ? vector_data[index] : -1); };
    private:
        unsigned int alloc_len;
        unsigned int len;
        // 動的サイズ変更の授業は寝ていたけれど、少なくともオーバーフローはしない!
        T vector_data[1+256];
};

template <class T>
void
DSVector<T>::append(T item)
{
    // オーバーフローはさせません!
    if (len >= alloc_len) {
        std::cout << "ベクトルが満杯です!" << std::endl;
        return;
    }
    vector_data[this->len++] = item;
}

int
main(int argc, char *argv[])
{
    DSVector<int> test1;
    unsigned int choice = 0;
    bool done = false;
    disable_buffering(stdout);

    while (!done) {
        print_menu();
        std::cout << "選択してください: ";
        choice = get_unum();

        /* メニューの選択を処理する */
        switch (choice) {
        case 1:
            std::cout << "数値を入力してください: ";
            choice = get_unum();
            test1.append(choice);
            break;
        case 2:
            std::cout << "インデックスを選んでください: ";
            choice = get_unum();
            printf("DSVector[%d] = %d\n", choice, test1.get(choice));
            break;
        case 3:
            done = true;
            break;
        default:
            puts("選択肢が不正です!");
            break;
        }
    }

    return EXIT_SUCCESS;
}
