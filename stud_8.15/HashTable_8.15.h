#pragma once

#include <iostream>
#include <vector>
#include <utility>
#include <string>

using namespace std;

template<class K>
struct HashFunc
{
    size_t operator()(const K& key)
    {
        return key;
    }
};

// 特化
template<>
struct HashFunc<string>
{
    // BKDR
    size_t operator()(const string& s)
    {
        size_t hash = 0;
        for (auto ch : s)
        {
            hash += ch;
            hash *= 31;
        }
        return hash;
    }
};

namespace HashBucket
{
    template<class T>
    struct HashNode
    {
        HashNode<T>* _next;
        T _data;

        HashNode(const T& data)
            : _next(nullptr), _data(data) {}
    };

    template<class K, class T, class KeyOfT, class Hash>
    class HashTable;

    template<class K, class T, class Ref, class Ptr, class KeyOfT, class Hash>
    struct __HashIterator
    {
        typedef HashNode<T> Node;
        typedef HashTable<K, T, KeyOfT, Hash> HT;
        typedef __HashIterator<K, T, Ref, Ptr, KeyOfT, Hash> Self;

        Node* _node;
        const HT* _ht;

        __HashIterator(Node* node, const HT* ht)
            : _node(node), _ht(ht) {}

        Ref operator*() { return _node->_data; }
        Ptr operator->() { return &_node->_data; }

        bool operator!=(const Self& s) { return _node != s._node; }

        Self& operator++()
        {
            if (_node->_next != nullptr)
            {
                _node = _node->_next;
            }
            else
            {
                KeyOfT kot;
                Hash hash;
                size_t hashi = hash(kot(_node->_data)) % _ht->_tables.size();
                ++hashi;
                while (hashi < _ht->_tables.size())
                {
                    if (_ht->_tables[hashi])
                    {
                        _node = _ht->_tables[hashi];
                        break;
                    }
                    ++hashi;
                }
                if (hashi == _ht->_tables.size())
                {
                    _node = nullptr;
                }
            }
            return *this;
        }
    };

    template<class K, class T, class KeyOfT, class Hash>
    class HashTable
    {
        typedef HashNode<T> Node;
    public:
        typedef __HashIterator<K, T, T&, T*, KeyOfT, Hash> iterator;
        typedef __HashIterator<K, T, const T&, const T*, KeyOfT, Hash> const_iterator;

        // 友元声明
        friend class luow::unordered_map<K, T>;

        iterator begin()
        {
            Node* cur = nullptr;
            for (size_t i = 0; i < _tables.size(); ++i)
            {
                cur = _tables[i];
                if (cur) break;
            }
            return iterator(cur, this);
        }

        iterator end() { return iterator(nullptr, this); }

        const_iterator begin() const
        {
            Node* cur = nullptr;
            for (size_t i = 0; i < _tables.size(); ++i)
            {
                cur = _tables[i];
                if (cur) break;
            }
            return const_iterator(cur, this);
        }

        const_iterator end() const { return const_iterator(nullptr, this); }

        ~HashTable()
        {
            for (auto& cur : _tables)
            {
                while (cur)
                {
                    Node* next = cur->_next;
                    delete cur;
                    cur = next;
                }
                cur = nullptr;
            }
        }

        pair<iterator, bool> Insert(const T& value)
        {
            KeyOfT kot;
            Hash hash;
            size_t hashi = hash(kot(value)) % _tables.size();
            Node* cur = _tables[hashi];

            while (cur)
            {
                if (kot(cur->_data) == kot(value))
                {
                    return { iterator(cur, this), false }; // 已存在
                }
                cur = cur->_next;
            }

            Node* newNode = new Node(value);
            newNode->_next = _tables[hashi];
            _tables[hashi] = newNode;
            return { iterator(newNode, this), true }; // 插入成功
        }

        iterator Find(const K& key)
        {
            if (_tables.size() == 0) return end();

            KeyOfT kot;
            Hash hash;
            size_t hashi = hash(key) % _tables.size();
            Node* cur = _tables[hashi];
            while (cur)
            {
                if (kot(cur->_data) == key)
                {
                    return iterator(cur, this);
                }
                cur = cur->_next;
            }
            return end();
        }

        bool Erase(const K& key)
        {
            Hash hash; KeyOfT kot;
            size_t hashi = hash(key) % _tables.size();
            Node* prev = nullptr;
            Node* cur = _tables[hashi];
            while (cur)
            {
                if (kot(cur->_data) == key)
                {
                    if (prev == nullptr)
                    {
                        _tables[hashi] = cur->_next;
                    }
                    else
                    {
                        prev->_next = cur->_next;
                    }
                    delete cur;
                    return true;
                }
                else
                {
                    prev = cur;
                    cur = cur->_next;
                }
            }
            return false;
        }

    private:
        vector<Node*> _tables; // 指针数组
        size_t _n = 0; // 存储有效数据个数
    };
}