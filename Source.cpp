#include <iostream>
#include <vector>
#include <string>
#include <cassert>


#define TABLE_SIZE 100
using namespace std; 

template<typename K, typename V>
class HashNode {
	K key;
	V value;
	HashNode *next;
public:
	HashNode(const K& key, const V& value) : key(key), value(value), next(NULL){}
	K getKey() const { return key; }
	V getValue() const { return value; }
	void setValue(V val) { HashNode::value = val; }
	HashNode *getNext() const { return next; }
	void setNext(HashNode *n) { HashNode::next = n; }
};

template <typename K>
struct KeyHash {
	unsigned long operator()(const K& key) const {
		return reinterpret_cast<unsigned long>(key) % TABLE_SIZE;
	}
};

template<typename K, typename V, typename F = KeyHash<K>>
class HashMap {
	HashNode<K, V> **table;
	F hashFunction;
public:
	//constructor fara parametri
	HashMap() { 
		table = new HashNode<K, V> *[TABLE_SIZE]();
	}

	//constructor de copiere
	HashMap(const HashMap& hm) : table(hm.table){}

	//destructor
	~HashMap() {
		for (int i = 0; i < TABLE_SIZE; ++i) {
			HashNode<K, V> *entry = table[i];
			while (entry != NULL) {
				HashNode<K, V> *prev = entry;
				entry = entry->getNext();
				delete prev;
			}
			table[i] = NULL;
		}
		delete[] table;
	}

	//metoda de cautare, care primeste ca parametru o cheie si o variabila, value
	//value primeste valoarea 'bucket'-ului in care se afla cheia
	//returneaza true daca cheia a fost gasita
	//false altfel
	bool search(const K &key, V &value) {
	unsigned long hashValue = hashFunction(key);
		HashNode<K, V> *entry = table[hashValue];
		
		while (entry != NULL) {
			if (entry->getKey() == key) {
				value = entry->getValue();
				return true;
			}
			entry = entry->getNext();
		}
		return false;
	}
	
	//metoda de adaugare a perechii key->value in tabel
	void add(const K &key, const V &value) {
		unsigned long hashValue = hashFunction(key);
		HashNode<K, V> *prev = NULL;
		HashNode<K, V> *entry = table[hashValue];

		while (entry != NULL && entry->getKey() != key) {
			prev = entry;
			entry = entry->getNext();
		}

		if (entry == NULL) {
			entry = new HashNode <K, V>(key, value);
			if (prev == NULL)
				table[hashValue] = entry;
			else prev->setNext(entry);
		}
		else entry->setValue(value);
	}

	//metoda de stergere un hashNode dupa cheie
	void remove(const K &key) {
		unsigned long HashValue = hashFunction(key);
		HashNode<K, V> *prev = NULL;
		HashNode<K, V> *entry = table[HashValue];

		while (entry != NULL && entry->getKey() != key) {
			prev = entry;
			entry = entry->getNext();
		}

		if (entry == NULL) {
			cout << "Elementul nu exista";
			return;
		}
		else {
			if (prev == NULL)
				table[HashValue] = entry->getNext();
			else prev->setNext(entry->getNext());
			delete entry;
		}
	}

	//metoda ce returneaza un vector cu valorile cunoscute pentru o anumita cheie
	vector<V> vectorVal(const K &key) {
		vector <V> valueVector;
		unsigned long HashValue = hashFunction(key);
		HashNode<K, V> *entry = table[HashValue];
		int nr = 0;
		string value;
		//cat timp nodul la care ne aflam e diferit de NULL, parcurgem hashMap-ul
		//ca sa vedem ce valori cu aceeasi cheie gasim
		while (entry != NULL) {
			if (search(entry->getKey(), value) == true) {
				valueVector.push_back(value);
				nr++;
			}
			entry = entry->getNext();
		}

		if (nr != 0)
			return valueVector;
		else {
			cout << "Cheia nu exista! \n";
		}

	}

	//metoda pentru obtinerea numarului de chei distincte din HashMap
	K difKeys() {
		K nr = 0;
		for (int i = 0; i < TABLE_SIZE; ++i) {
			HashNode <K, V> *entry = table[i];
			while (entry != NULL) {
				HashNode<K, V> *prev = entry;
				entry = prev->getNext();
				if (prev->getKey() != entry->getKey())
					nr++;
			}
		}

		return nr;
	}

	//supraincarcarea operatorului de afisare
	friend ostream& operator<<(ostream &out, const HashMap &h) {
		for (int i = 0; i < TABLE_SIZE; ++i)
		{
			HashNode <K, V> *entry = h.table[i];
			while (entry != NULL) {
				out << entry->getKey() << " -> " << entry->getValue() << endl;
				entry = entry->getNext();
			}
		}
		return out;
	}

	//supraincarcarea operatorului de atribuire
	HashNode<K, V> operator =(HashMap hash) {
		HashNode <K, V> **matrix;
		matrix = new HashNode <K, V> *[TABLE_SIZE];
		for (int i = 0; i < TABLE_SIZE; ++i) {
			HashNode<K, V> *entry = hash.table[i];
			while (entry != NULL) {
				matrix = entry;
				entry = entry->getNext();
			}
		}
		return matrix;
		
	}

	//atribuirea operatorului [], ce returneaza prima valoare cunoscuta pentru 
	//o anumita cheie
	V operator[](const K& key) {
		string value;
		for (unsigned i = 0; i < TABLE_SIZE; ++i){
			HashNode<K, V> *entry = table[i];
			while (entry != NULL) {
				if (search(entry->getKey(), value))
					return value;
			}
		}
	}
};

int main() {

	struct MyKeyHash {
		unsigned long operator()(const int& k)const {
			return k % 10;
		}
	};

	HashMap<int, string, MyKeyHash> hmap;
	hmap.add(1, "val1");
	hmap.add(2, "val2");
	hmap.add(3, "val3");
	hmap.add(4, "val4");
	hmap.add(3, "val5");
	hmap.add(5, "val6");

	string value;
	hmap.search(1, value);
	cout << value<< endl;

	assert(hmap.search(1, value));
	hmap.remove(1);
	assert(!hmap.search(1, value));

	vector<string> vec;
	vec = hmap.vectorVal(4);
	
	for (unsigned i = 0; i < vec.size(); ++i)
		cout << vec[i] << " ";
	cout << endl;

	system("pause");
	return 0;
}