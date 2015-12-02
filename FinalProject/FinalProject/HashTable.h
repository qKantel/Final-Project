#include "LinkedHashedEntry.h"
#include "NotFoundException.h"
#include <vector>

#ifndef _HASH_TABLE
#define _HASH_TABLE

const int TABLE_SIZE = 41;

template<class KeyType, class ItemType>
class HashTable
{
private:
	LinkedHashedEntry<KeyType, ItemType>** table;
	int itemCount;

	// Compute hashed index into the array using ( (prime * key + prime) % LARGE_PRIME ) % TABLE_SIZE
	int getHashIndex(const KeyType& key) const;

	/** Copies entries over from existing hash table.
	@post  Duplicate Hash Table; since copied items have the same key, they will have the same hashed index. */
	void copyHashTable(const HashTable<KeyType, ItemType>& aHashTable);

public:
	HashTable();
	HashTable(const HashTable& aHashTable);
	~HashTable();

	HashTable<KeyType, ItemType>& operator=(const HashTable<KeyType, ItemType>& aHashTable);

	/** Retrieves an item with a given search key.
	@post  If the retrieval is successful, the item is returned.
	@param key  The search key of the item to be retrieved.
	@return  The item associated with the search key.
	@throw NotFoundException if the item does not exist. */
	ItemType find(const KeyType& key) const;

	/** Retrieves all items in the hash table. */
	vector<ItemType> getAll() const;

	/** Returns the number of entries in the Hash Table. */
	int getTotalEntries() const { return itemCount; }

	/** Inserts an item into the HashTable according to the item's search key. */
	void add(const KeyType& key, const ItemType& value);
	
	/** Removes an item with the given search key from the hash table. 
	@return true if item is successfully removed, false otherwise. */
	bool remove(const KeyType& key);

	/** Deallocates all dynamic memory and sets itemCount to 0.  */
	void clear();

};

//==============================
//		Implementation
//==============================


//==============================
//		Private Methods
//==============================

template<class KeyType, class ItemType>
int HashTable<KeyType, ItemType>::getHashIndex(const KeyType& key) const
{
	return ( ( ( 17 * key + 5 ) % 997) % TABLE_SIZE );
}  // end getHashCode

template<class KeyType, class ItemType>
void HashTable<KeyType, ItemType>::copyHashTable(const HashTable<KeyType, ItemType>& aHashTable)
{
	for (int i = 0; i < TABLE_SIZE; i++)
	{
		if (aHashTable.table[i] != nullptr)
		{
			LinkedHashedEntry<KeyType, ItemType>* copiedEntry = new LinkedHashedEntry<KeyType, ItemType>( aHashTable.table[i]->getKey(), aHashTable.table[i]->getValue(), aHashTable.table[i]->getNext() );
			table[i] = copiedEntry;
		}
		else
			table[i] = nullptr;
	}
}  // end copyHashTable


//==============================
//		Public Methods
//==============================

template<class KeyType, class ItemType>
HashTable<KeyType, ItemType>::HashTable()
{
	table = new LinkedHashedEntry<KeyType, ItemType>*[TABLE_SIZE];
	itemCount = 0;
	for (size_t i = 0; i < TABLE_SIZE; i++)
		table[i] = nullptr;
}  // end constructor

template<class KeyType, class ItemType>
HashTable<KeyType, ItemType>::HashTable(const HashTable& aHashTable) : itemCount(0)
{
	*this = aHashTable;
}  // end copy constructor

template<class KeyType, class ItemType>
HashTable<KeyType, ItemType>& HashTable<KeyType, ItemType>::operator=(const HashTable<KeyType, ItemType>& aHashTable)
{
	if (this == &aHashTable)
		return *this;

	if (itemCount != 0)
	{
		clear();
		delete[] table;
		table = nullptr;
	}

	if (aHashTable.getTotalEntries() == 0)
	{
		table = new LinkedHashedEntry<KeyType, ItemType>*[TABLE_SIZE];
		for (size_t i = 0; i < TABLE_SIZE; i++)
			table[i] = nullptr;
		itemCount = 0;
	}
	else
	{
		table = new LinkedHashedEntry<KeyType, ItemType>*[TABLE_SIZE];
		itemCount = aHashTable.getTotalEntries();
		copyHashTable(aHashTable);
}  // end overloaded operator =

return *this;
}  // end overloaded = operator

template<class KeyType, class ItemType>
ItemType HashTable<KeyType, ItemType>::find(const KeyType& key) const
{
	int hashIndex = getHashIndex(key);
	if (table[hashIndex] == nullptr)
		throw NotFoundException();
	else
	{
		LinkedHashedEntry<KeyType, ItemType>* entry = table[hashIndex];
		return entry->getValue();
	}
}  // end get

template<class KeyType, class ItemType>
vector<ItemType> HashTable<KeyType, ItemType>::getAll() const
{
	vector<ItemType> entries;

	for (size_t i = 0; i < TABLE_SIZE; i++)
	{
		if (table[i] != nullptr)
		{
			LinkedHashedEntry<KeyType, ItemType>* entry = table[i];
			entries.push_back( entry->getValue() );
		}
	}

	return entries;
}  // end getAll

template<class KeyType, class ItemType>
void HashTable<KeyType, ItemType>::add(const KeyType& key, const ItemType& value)
{
	// Create entry to add to hash table
	LinkedHashedEntry<KeyType, ItemType>* entryToAddPtr = new LinkedHashedEntry<KeyType, ItemType>(key, value);

	// Compute the hashed index into the array
	int hashIndex = getHashIndex(key);

	// Add the entry to the chain at hashIndex
	if (table[hashIndex] == nullptr)
		table[hashIndex] = entryToAddPtr;  // No item at hashed index
	else
	{
		entryToAddPtr->setNext(table[hashIndex]);  // New entry points to current item at hashed index
		table[hashIndex] = entryToAddPtr;  // Add entry to top of chain at index
	}

	itemCount++;
}  // end put

template<class KeyType, class ItemType>
bool HashTable<KeyType, ItemType>::remove(const KeyType& key)
{
	bool itemFound = false;
	// Compute the hashed index into the array
	int hashIndex = getHashIndex(key);
	if (table[hashIndex] != nullptr)
	{
		// First node has target
		if ( key == table[hashIndex]->getKey() )
		{
			LinkedHashedEntry<KeyType, ItemType>* entryToRemovePtr = table[hashIndex];
			table[hashIndex] = table[hashIndex]->getNext();
			delete entryToRemovePtr;
			entryToRemovePtr = nullptr;
			itemFound = true;
		}
		else  // Search the rest of the chain
		{
			LinkedHashedEntry<KeyType, ItemType>* prevPtr = table[hashIndex];
			LinkedHashedEntry<KeyType, ItemType>* curPtr = prevPtr->getNext();
			while ((curPtr != nullptr) && !itemFound)
			{
				// Found item in chain so remove that node
				if ( key == curPtr->getKey() )
				{
					prevPtr->setNext(curPtr->getNext());
					delete curPtr;
					curPtr = nullptr;
					itemFound = true;
				}
				else  // Look at next entry in chain
				{
					prevPtr = curPtr;
					curPtr = curPtr->getNext();
				}  // end if
			}  // end while
		}  // end if
	}  // end if

	return itemFound;
}  // end remove

template<class KeyType, class ItemType>
void HashTable<KeyType, ItemType>::clear()
{
	itemCount = 0;

	for (size_t i = 0; i < TABLE_SIZE; i++)
	{
		if (table[i] != nullptr)
		{
			LinkedHashedEntry<KeyType, ItemType>* prevEntry = nullptr;
			LinkedHashedEntry<KeyType, ItemType>* entry = table[i];
			while (entry != nullptr)
			{
				prevEntry = entry;
				entry = entry->getNext();
				delete prevEntry;
				prevEntry = nullptr;
			}
		}  // end if
	}  // end for
}  // end clear

template<class KeyType, class ItemType>
HashTable<KeyType, ItemType>::~HashTable()
{
	clear();
	delete [] table;
	table = nullptr;
}  // end destructor

#endif