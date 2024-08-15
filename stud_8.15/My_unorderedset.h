#pragma once
#include "HashTable_8.15.h"

namespace luow
{
	template<class K, class Hash = HashFunc<K>>
	class unordered_set
	{
	public:
		struct SetKeyOfT
		{
			const K& operator()(const K& key)
			{
				return key;
			}
		};
	public:
		typedef typename HashBucket::HashTable<K, K, SetKeyOfT, Hash>::const_iterator iterator;
		typedef typename HashBucket::HashTable<K, K, SetKeyOfT, Hash>::const_iterator const_iterator;


		iterator begin()
		{
			return _ht.begin();
		}

		iterator end()
		{
			return _ht.end();
		}

		const_iterator begin() const
		{
			return _ht.begin();
		}

		const_iterator end() const
		{
			return _ht.end();
		}

		pair<iterator, bool> insert(const K& key)
		{
			return _ht.Insert(key);
		}

		iterator find(const K& key)
		{
			return _ht.Find(key);
		}

		bool erase(const K& key)
		{
			return _ht.Erase(key);
		}

	private:
		HashBucket::HashTable<K, K, SetKeyOfT, Hash> _ht;
	};

	void print(const unordered_set<int>& s)
	{
		unordered_set<int>::const_iterator it = s.begin();
		while (it != s.end())
		{
			//*it = 1;
			cout << *it << " ";
			++it;
		}
		cout << endl;
	}

	void test_unordered_set1()
	{
		int a[] = { 3, 33, 2, 13, 5, 12, 1002 };
		unordered_set<int> s;
		for (auto e : a)
		{
			s.insert(e);
		}

		s.insert(54);
		s.insert(107);


		unordered_set<int>::iterator it = s.begin();
		while (it != s.end())
		{
			//*it = 1;
			cout << *it << " ";
			++it;
		}
		cout << endl;

		for (auto e : s)
		{
			cout << e << " ";
		}
		cout << endl;

		print(s);
	}
}
