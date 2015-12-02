#ifndef _LINKED_HASHED_ENTRY
#define _LINKED_HASHED_ENTRY

template<class KeyType, class ItemType>
class LinkedHashedEntry
{
private:
	KeyType key;  // Search key for Entry
	ItemType value;  // The entry itself
	LinkedHashedEntry<KeyType, ItemType>* next;  // Point to the hashed entry

public:
	LinkedHashedEntry() : next(nullptr) {}
	LinkedHashedEntry(const KeyType& newKey, const ItemType& newEntry)
	{
		key = newKey;
		value = newEntry;
		next = nullptr;
	}
	LinkedHashedEntry(const KeyType& newKey, const ItemType& newEntry, LinkedHashedEntry<KeyType, ItemType>* nextPtr)
	{
		key = newKey;
		value = newEntry;
		next = nextPtr;
	}

	KeyType		       getKey()   const	{ return key;   }
	ItemType		   getValue() const { return value; }
	
	LinkedHashedEntry<KeyType, ItemType>* getNext()  const { return next;  }

	void setNext(LinkedHashedEntry<KeyType, ItemType>* nextEntryPtr) { next = nextEntryPtr; }
};

#endif