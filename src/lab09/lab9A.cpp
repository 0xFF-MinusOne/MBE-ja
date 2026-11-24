/*
 * コンパイル方法: g++ -fstack-protector-all -z relro -z now -O1 -fPIE -pie ./lab9A.cpp -g -o lab9A
 * clark の改良版アイテム保管サーバーです!
 * 変更履歴:
 *   * HashSet を使って、即座にアクセスできるようにしました!
 *
*/

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include "utils.h"

ENABLE_TIMEOUT(300)

#define SIZE_HASHSET_VEC 8

void
print_menu(void)
{
    printf("+------------ clark の改良版アイテム保管 -------------+\n"
           "| [ -- アイテムへ即座にアクセス: HashSet を採用!|\n"
           "| 1. ロックボックスを開く                             |\n"
           "| 2. ロックボックスにアイテムを追加する               |\n"
           "| 3. ロックボックスからアイテムを取り出す             |\n"
           "| 4. ロックボックスと中のアイテムを破棄する           |\n"
           "| 5. 終了する                                         |\n"
           "+-----------------------------------------------------+\n");
}

/********* ストレージの実装 **********/

// ハッシュ関数オブジェクト (functor)
class hash_num {
    public:
        // 私は数学者ではないので
        unsigned int
        operator() (unsigned int const &key) const {
            return key;
        }
};

// ハッシュセット (HashSet)
template<class T, class HashFunc>
class HashSet {
    public:
        HashSet(unsigned int size) : m_size(size), set_data(new T[size]) {}
        virtual ~HashSet() { delete [] set_data; }
        virtual void add(T val);
        virtual unsigned int find(T val);
        virtual T get(unsigned int);
    private:
        unsigned int m_size;
        HashFunc m_hash;
        T *set_data;
};
typedef HashSet<int, hash_num> hashset_int;

template<class T, class HashFunc>
void
HashSet<T, HashFunc>::add(T val)
{
    int index = this->m_hash(val) % this->m_size;
    this->set_data[index] = val;
}

template<class T, class HashFunc>
unsigned int
HashSet<T, HashFunc>::find(T val)
{
    return this->m_hash(val) % this->m_size;
}

template<class T, class HashFunc>
T
HashSet<T, HashFunc>::get(unsigned int index)
{
    if (index >= m_size) {
        std::cout << "インデックスが不正です" << std::endl;
        return T();
    }
    return set_data[index];
}

/********* ストレージのインターフェースの実装 **********/
void
do_new_set(hashset_int **set_vec)
{
    int bin = -1;
    int choice = -1;
    std::cout << "どのロックボックスにしますか?: ";
    bin = get_unum();
    if (bin < 0) {
        std::cout << "セット ID が不正です!" << std::endl;
        return;
    } else if (bin >= SIZE_HASHSET_VEC) {
        std::cout << "空きがありません!" << std::endl;
        return;
    }
    std::cout << "いくつのアイテムを保存しますか?: ";
    choice = get_unum();
    set_vec[bin] = new hashset_int(choice);
}

void
do_add_item(hashset_int **set_vec)
{
    int set_id = -1;
    int item_val = -1;

    std::cout << "どのロックボックスですか?: ";
    set_id = get_unum();
    if (set_id < 0 || set_id >= SIZE_HASHSET_VEC) {
        std::cout << "セット ID が不正です!" << std::endl;
        return;
    }

    std::cout << "アイテムの値: ";
    item_val = get_unum();
    set_vec[set_id]->add(item_val);
}

void
do_find_item(hashset_int **set_vec)
{
    int set_id = -1;
    int item_val = -1;
    int item_index = -1;

    std::cout << "どのロックボックスですか?: ";
    set_id = get_unum();
    if (set_id < 0 || set_id >= SIZE_HASHSET_VEC) {
        std::cout << "セット ID が不正です!" << std::endl;
        return;
    }

    std::cout << "アイテムの値: ";
    item_val = get_unum();
    item_index = set_vec[set_id]->find(item_val);
    if (item_index == -1) {
        std::cout << "アイテムが見つかりません!" << std::endl;
    } else {
        std::cout << "アイテムが見つかりました" << std::endl;
        printf("lockbox[%d] = %d\n", item_index, set_vec[set_id]->get(item_index));
    }
}

void
do_del_set(hashset_int **set_vec)
{
    int del_id;
    std::cout << "どのセットですか?: ";
    del_id = get_unum();
    if (del_id < 0 || del_id >= SIZE_HASHSET_VEC) {
        std::cout << "ID が不正です!" << std::endl;
        return;
     }
    delete set_vec[del_id];
}

int
main(int argc, char *argv[])
{
    hashset_int **set_vec = new hashset_int*[SIZE_HASHSET_VEC];
    int choice = -1;
    bool done = false;
    disable_buffering(stdout);

    while (!done) {
        print_menu();
        std::cout << "選択してください: ";
        choice = get_unum();
        switch (choice) {
        case 1:
            do_new_set(set_vec);
            break;
        case 2:
            do_add_item(set_vec);
            break;
        case 3:
            do_find_item(set_vec);
            break;
        case 4:
            do_del_set(set_vec);
            break;
        case 5:
            done = true;
            break;
        default:
            puts("オプションが不正です!");
            break;
        }
    }

    delete [] set_vec;

    return EXIT_SUCCESS;
}
