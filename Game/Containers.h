#pragma once

#include "Types.h"

#include <new> // for placement new
#include <utility> // for move
#include <initializer_list> // For setting array contents

// Array: Simple dynamically-allocated array
template <typename T>
class Array {
public:
	Array() : items(nullptr), numItems(0), numAlloced(0) {};
	inline Array(const Array& other);
	inline Array(Array&& moved);
	inline ~Array();

	inline T& Append();
	inline T& Append(const T& clone);
	template <typename ...ConstructorArgs>
	inline T& Append(ConstructorArgs... args);

	inline T& Insert(int index, const T& clone);

	inline void Set(std::initializer_list<const T> list);

	inline void RemoveByIndex(int index);

	inline int GetNum() const;

	inline void Clear();

	inline bool IsIndexValid(int index) const;

	// Finds the first match of an item and returns its index, or -1 if not found
	inline int Find(const T& item) const;

	inline T* begin();
	inline T* end();
	inline const T* begin() const;
	inline const T* end() const;

	inline operator T*();
	inline operator const T*() const;

private:
	void Realloc(int32 numNeeded);

	union {
		T* items; // used for general purposes
		void* rawItems; // used for new and delete
	};

	int32 numItems;
	int32 numAlloced;
};

// StaticCircularArray: Statically-sized, circular array where 0 is the earliest item added and NumItems-1 is the last
template <typename ItemType, int NumItems>
class StaticCircularArray {
public:
	StaticCircularArray() : baseIndex(NumItems - 1), isFull(false) {};
	inline ~StaticCircularArray();

	inline ItemType& Append();
	inline ItemType& Append(const ItemType& clone);

	int GetNum() const;

	inline void Clear();

	// Negative indexes are allowed!
	inline ItemType& operator[](int index);
	inline const ItemType& operator[](int index) const;

private:
	ItemType items[NumItems];
	int32 baseIndex;
	bool8 isFull;
};

template <typename ItemType>
class Cache {
public:
	// Adds an item to the cache, fails if the item already exists (this should never happen!)
	// After an item is added to the cache, the caller must not delete it
	// Todo factory pattern?
	bool Bind(const char* tag, ItemType* itemPointer);

	// Retrieves an item from the cache, increasing its reference counter
	inline ItemType* Retrieve(const char* tag);

	// Releases an item in the cache, decrementing its reference counter. item is const
	inline void Release(const ItemType* itemPointer);

	// Returns the total number of items in the cache
	inline int GetNum() const;

	// Returns the number of references to items in the cache
	inline int CountReferences() const;

	// Cleans up unreferenced items in the cache
	inline void CleanupUnused();

private:
	// Generates a hash from an item tag
	inline uint32 GenerateHash(const char* tag);

	struct CacheItem {
		// Pointer to the image
		ItemType* item;

		// Number of referrers to this image. May be used in the future to cleanup the cache
		uint32 numReferers;

		// Hash of the image's name
		uint32 hash;
	};

	Array<CacheItem> items;
};

// ========== Cache functions ==========
template <typename ItemType> bool Cache<ItemType>::Bind(const char* tag, ItemType* itemPointer) {
	// Make sure it doesn't already exist
	if (Retrieve(tag)) {
		return false;
	}

	// Add the item and assign its info
	CacheItem& item = items.Append();

	item.item = itemPointer;
	item.hash = GenerateHash(tag);
	item.numReferers = 0;
	return true;
}

template <typename ItemType> inline ItemType* Cache<ItemType>::Retrieve(const char* tag) {
	// Try to find the item with the matching hash
	uint32 hash = GenerateHash(tag);

	for (CacheItem& item : items) {
		if (item.hash == hash) {
			++item.numReferers;
			return item.item;
		}
	}

	// None found, return nullptr
	return nullptr;
}

template<typename ItemType> inline void Cache<ItemType>::Release(const ItemType* itemPointer) {
	for (CacheItem& item : items) {
		if (item.item == itemPointer) {
			--item.numReferers;
		}
	}
}

template<typename ItemType> inline int Cache<ItemType>::GetNum() const {
	return items.GetNum();
}

template <typename ItemType> uint32 Cache<ItemType>::GenerateHash(const char* tag) {
	uint32 hash = 0x8DEFF5A1;
	
	for (int i = 0; tag[i]; ++i) {
		// I dunno tho
		hash += (hash >> (tag[i] & 7)) | ((tag[i] - i) << 24) * tag[i] >> 4;
	}

	return hash;
}

// ========== StaticCircularArray functions ==========
template <typename ItemType, int NumItems> StaticCircularArray<ItemType, NumItems>::~StaticCircularArray() {
	if (isFull) {
		for (int i = 0; i < NumItems; ++i) {
			items[i].~ItemType();
		}
	} else {
		for (int i = 0; i < baseIndex; ++i) {
			items[i].~ItemType();
		}
	}
}

template <typename ItemType, int NumItems> ItemType& StaticCircularArray<ItemType, NumItems>::Append() {
	ItemType& currentItem = items[baseIndex];

	if (isFull) {
		// Destroy the old item
		currentItem.~ItemType();
	}
		
	// Construct the new item
	new (currentItem) ItemType();

	// Increment the index
	++baseIndex;
	
	if (baseIndex == NumItems) {
		baseIndex = 0;
	} else if (baseIndex == NumItems - 1) {
		isFull = true;
	}

	return currentItem;
}

template <typename ItemType, int NumItems> ItemType& StaticCircularArray<ItemType, NumItems>::Append(const ItemType& clone) {
	ItemType& currentItem = items[baseIndex];

	if (isFull) {
		// Destroy the old item
		currentItem.~ItemType();
	}
		
	// Construct the new item
	new (&items[baseIndex]) ItemType(clone);

	// Increment the index
	++baseIndex;

	if (baseIndex == NumItems) {
		baseIndex = 0;
	} else if (baseIndex == NumItems - 1) {
		isFull = true;
	}

	return currentItem;
}

template <typename ItemType, int NumItems> int StaticCircularArray<ItemType, NumItems>::GetNum() const {
	return isFull ? NumItems : baseIndex;
}

template <typename ItemType, int NumItems> ItemType& StaticCircularArray<ItemType, NumItems>::operator[](int index) {
	if (index >= 0) {
		// Forward iteration
		return items[(baseIndex + index) % NumItems];
	} else {
		// Negative iteration (use fancy negative modulo)
		return items[baseIndex + index - ((baseIndex + index - (NumItems - 1)) / NumItems * NumItems)];
	}
}

template <typename ItemType, int NumItems> const ItemType& StaticCircularArray<ItemType, NumItems>::operator[](int index) const {
	if (index >= 0) {
		// Forward iteration
		return items[(baseIndex + index) % NumItems];
	} else {
		// Negative iteration (use fancy negative modulo)
		return items[baseIndex + index - ((baseIndex + index - (NumItems - 1)) / NumItems * NumItems)];
	}
}

// ========== Array<T> functions ==========
template<typename T> Array<T>::~Array() {
	delete[] rawItems;
}

template<typename T> Array<T>::Array(const Array& other) : rawItems(nullptr), numItems(0), numAlloced(0) {
	// Allocate a list of equal size
	Realloc(other.numItems);

	// Copy list items over
	for (int i = 0; i < numItems; ++i) {
		new (&items[i]) T(other.items[i]);
	}
	
	// We has lots of items now
	numItems = other.numItems;
}

template<typename T> inline Array<T>::Array(Array&& moved) {
	// Move items to this array
	items = moved.items;
	numItems = moved.numItems;
	numAlloced = moved.numAlloced;

	// Null out the source array
	moved.items = nullptr;
	moved.numItems = 0;
	moved.numAlloced = 0;
}

template<typename T> T& Array<T>::Append() {
	// Resize list
	Realloc(numItems + 1);
	
	// Return placement new-initialised object
	return *new (&items[numItems++]) T();
}

template<typename T> T& Array<T>::Append(const T& clone) {
	// Resize list
	Realloc(numItems + 1);

	// Return placement new-initialised object
	return *new (&items[numItems++]) T(clone);
}

template<typename T> inline T& Array<T>::Insert(int index, const T& clone) {
	// Resize list
	// Todo: Inefficient: two move calls are performed on some items
	Realloc(numItems + 1);

	// Move items above this item in the list
	for (int i = numItems; i > index; --i) {
		// Move next item into this slot
		new (&items[i]) T(std::move(items[i - 1]));
	}

	++numItems;
	return *new (&items[index]) T(clone);
}

template<typename T> inline void Array<T>::Set(std::initializer_list<const T> list) {
	// Clear current contents
	Clear();

	// Allocate enough room to refill contents
	Realloc(list.size());

	// Copy the initializer list items to the array
	numItems = list.size();

	for (int i = 0; i < numItems; ++i) {
		new (&items[i]) T(list.begin()[i]);
	}
}

template<typename T> void Array<T>::RemoveByIndex(int index) {
	// Decrement number of items
	numItems -= 1;

	// Shuffle tailing items back
	for (int i = index; i < numItems; ++i) {
		// Destruct item in slot
		items[i].~T();

		// Move next item into this slot
		new (&items[i]) T(std::move(items[i + 1]));
	}
	
	// Possibly shrink the list
	Realloc(numItems - 1);
}

template<typename T> inline int Array<T>::GetNum() const {
	return numItems;
}

template<typename T> void Array<T>::Realloc(int32 numNeeded) {
	if (numNeeded > numAlloced) {
		// Decide how much the list should allocated
		numAlloced = numNeeded * 2;
		
		// Allocate a new, bigger list. Allocate it as bytes so that the objects aren't initialised
		T* newItems = (T*)new uint8[numAlloced * sizeof (T)];
		
		// Move the old items to the new list
		for (int i = 0; i < numItems; ++i) {
			// Move to new
			new (&newItems[i]) T(std::move(items[i]));

			// Destroy old (this is necessary in some cases, particularly objects without move constructors!)
			items[i].~T();
		}

		// Delete the old items list
		delete[] rawItems;
		items = newItems;
	}
}

template<typename T> void Array<T>::Clear() {
	// Call destructors on items
	for (int i = 0; i < numItems; ++i) {
		items[i].~T();
	}

	// Clean up variables
	delete[] rawItems;
	
	items = nullptr;
	numItems = 0;
	numAlloced = 0;
}

template<typename T> inline bool Array<T>::IsIndexValid(int index) const {
	return (index >= 0 && index < numItems);
}

template<typename T> inline int Array<T>::Find(const T& item) const {
	for (int i = 0; i < numItems; ++i) {
		if (items[i] == item) {
			return i;
		}
	}

	return -1;
}

template<typename T> T* Array<T>::begin() {
	return items;
}

template<typename T> T* Array<T>::end() {
	return &items[numItems];
}

template<typename T> const T* Array<T>::begin() const {
	return items;
}

template<typename T> const T* Array<T>::end() const {
	return &items[numItems];
}

template<typename T> Array<T>::operator T*() {
	return items;
}

template<typename T> Array<T>::operator const T*() const {
	return items;
}

template<typename T>
template<typename ...ConstructorArgs>
inline T& Array<T>::Append(ConstructorArgs ...args) {
	// Resize list
	Realloc(numItems + 1);

	// Return placement new-initialised object
	return *new (&items[numItems++]) T(args...);
}
