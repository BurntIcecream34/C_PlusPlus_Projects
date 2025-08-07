#ifndef DL_HASHTABLE_H
#define DL_HASHTABLE_H
#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <fstream>
#include <string>
#include <unistd.h>
#include <sstream>

using namespace std;

namespace cop4530 {

    template <typename K, typename V>
        class HashTable {
	    public:
		HashTable(size_t s = 101);
		~HashTable();
		bool contains(const K & k) const;
		bool match(const std::pair<K, V> &kv) const;
		bool insert(const std::pair<K, V> & kv);
		bool insert (std::pair<K,  V> && kv);
		bool remove(const K & k);
		void clear();
		bool load(const char *filename);
		void dump() const;
		size_t size() const;
		bool write_to_file(const char *filename) const;

	    private:
		vector<list<pair<K, V>>> container;
		size_t curSize;
		static const unsigned int max_prime = 1301081;
		static const unsigned int default_capacity = 101;

		void makeEmpty();
		void rehash();
		size_t myhash(const K &k) const;
		unsigned long prime_below (unsigned long n);
		void setPrimes(vector<unsigned long> & vprimes);
        };


#include "hashtable.hpp"

};

#endif

