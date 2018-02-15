#pragma once

#include "Types.h"

#include <new> // for placement new
#include <utility> // for move

// Array: Simple dynamically-allocated array
template <typename T>
class Array {
public:
	Array() : items(nullptr), numItems(0), numAlloced(0) {};
	inline ~Array();

	inline T& Append();
	inline T& Append(const T& clone);
	template <typename ...ConstructorArgs>
	inline T& Append(ConstructorArgs... args);

	inline void RemoveByIndex(int index);

	inline int GetNum() const;

	inline void Clear();

	inline T* begin();
	inline T* end();
	inline const T* begin() const;
	inline const T* end() const;

	inline operator T*();

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

template<typename T>
template<typename ...ConstructorArgs>
inline T& Array<T>::Append(ConstructorArgs ...args) {
	// Resize list
	Realloc(numItems + 1);

	// Return placement new-initialised object
	return *new (&items[numItems++]) T(args...);
}
