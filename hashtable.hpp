#include "hashtable.h"

using namespace std;
using namespace cop4530;

// public functions
template <typename K, typename V>
HashTable<K, V>::HashTable(size_t s) {
    container.resize(prime_below(s));
    curSize = 0;
    cout << prime_below(s) << '\n';
}

template <typename K, typename V>
HashTable<K, V>::~HashTable() {
    clear();
}

template <typename K, typename V>
bool HashTable<K, V>::contains(const K & k) const {
    auto & curList = container[myhash(k)];
    for (auto i = curList.begin(); i != curList.end(); i++) {
	if (i->first == k) {
	    return true;
	}
    }
    return false;
}

template <typename K, typename V>
bool HashTable<K, V>::match(const std::pair<K, V> &kv) const {
    auto & curList = container[myhash(kv.first)];
    return find(curList.begin(), curList.end(), kv) != curList.end();
}

template <typename K, typename V>
bool HashTable<K, V>::insert(const std::pair<K, V> & kv) {
    if (match(kv))  
        return false;        

    auto & curList = container[myhash(kv.first)];
    if (contains(kv.first)) {
	for (auto i = curList.begin(); i != curList.end(); i++) {
            if (i->first == kv.first) {    
                i->second = kv.second;
                return true;
            }
        }
    }

    curList.push_back(kv);   
    if (++curSize > container.size())             
        rehash();                
    return true;
}

template <typename K, typename V>
bool HashTable<K, V>::insert (std::pair<K,  V> && kv) {
    if (match(kv))  
        return false;        

    auto & curList = container[myhash(kv.first)];
    if (contains(kv.first)) {
	for (auto i = curList.begin(); i != curList.end(); i++) {
	    if (i->first == kv.first) {    
 	        i->second = kv.second;
	        return true;
	    }
	}    
    }

    curList.push_back(std::move(kv));     
    if (++curSize > container.size())          
        rehash();        
    return true;
}

template <typename K, typename V>
bool HashTable<K, V>::remove(const K & k) {
    if (contains(k)) {
	auto & curList = container[myhash(k)];
	for (auto i = curList.begin(); i != curList.end(); i++) {
	    if (i->first == k) {
		i = curList.erase(i);
		curSize--;
		return true;
	    }
	}
    }
    return false;
}

template <typename K, typename V>
void HashTable<K, V>::clear() {
    makeEmpty();
}

template <typename K, typename V>
bool HashTable<K, V>::load(const char *filename) {
    ifstream file(filename);
    if (file.is_open()) {
	string line;
	while (getline(file, line)) {
	    size_t splitPos = line.find(' ');
	    string keyS;
	    string valueS;
	    keyS = line.substr(0, splitPos);
	    valueS = line.substr(splitPos + 1);
	    
	    // Convert both items in pair
	    K key;
	    stringstream tmp(keyS);
	    tmp >> key;
	    V value;
	    stringstream tmp2(valueS);
	    tmp2 >> value;

	    pair<K, V> temp(key, value);
	    insert(temp);
	}
	file.close();
	return true;
    }
    return false;    
}

template <typename K, typename V>
void HashTable<K, V>::dump() const {
    int index = 0;
    for (auto & list : container) {
	cout << "v[" << index << "]: ";

	for (auto i = list.begin(); i != list.end(); i++) {
	    if (i != list.begin()) {
		cout << " : ";
	    }
	    cout << i->first << ' ' << i->second;
	}
	cout << '\n';
	index++;
    }
}

template <typename K, typename V>
size_t HashTable<K, V>::size() const {
    return curSize;
}

template <typename K, typename V>
bool HashTable<K, V>::write_to_file(const char *filename) const {
    ofstream file(filename);
    if (file.is_open() && curSize != 0) {
	for (auto & list : container) {
	    for (auto i = list.begin(); i != list.end(); i++) {
		file << i->first << ' ' << i->second << '\n';
	    }    
	}
	return true;
    }
    return false;
}

// private functions
// vector<list<K, V>> container;
template <typename K, typename V>
void HashTable<K, V>::makeEmpty() {
    for (auto & l : container) {
	l.clear();	
    }
    curSize = 0;
}

template <typename K, typename V>
void HashTable<K, V>::rehash() {
    vector<list<pair<K, V>>> oldCon = container;
    container.resize(2 * prime_below(2 * container.size()));
    makeEmpty();

    for (auto & list : oldCon) {
	for (auto & p : list) {
	    insert(move(p));
	}
    }
}

template <typename K, typename V>
size_t HashTable<K, V>::myhash(const K &k) const {
    static hash<K> hf;
    return hf(k) % container.size();
}

template <typename K, typename V>
unsigned long HashTable<K, V>::prime_below(unsigned long n) {
    if (n > max_prime)
    {
        std::cerr << "** input too large for prime_below()\n";
        return 0;
    }
    if (n == max_prime)
    {
        return max_prime;
    }
    if (n <= 1)
    {
        std::cerr << "** input too small \n";
        return 0;
    }

    // now: 2 <= n < max_prime
    std::vector <unsigned long> v (n+1);
    setPrimes(v);
    while (n > 2)
    {
        if (v[n] == 1)
            return n;
        --n;
    }

    return 2;
}

template <typename K, typename V>
void HashTable<K, V>::setPrimes(vector<unsigned long> & vprimes) {
    int i = 0;
    int j = 0;

    vprimes[0] = 0;
    vprimes[1] = 0;
    int n = vprimes.capacity();

    for (i = 2; i < n; ++i)
        vprimes[i] = 1;

    for( i = 2; i*i < n; ++i)
    {
        if (vprimes[i] == 1)
            for(j = i + i ; j < n; j += i)
                vprimes[j] = 0;
    }
}
