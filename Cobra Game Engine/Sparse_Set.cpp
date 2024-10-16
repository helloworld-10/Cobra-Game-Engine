#include "Sparse_Set.h"
template<typename T>
Sparse_Set<T>::Sparse_Set(int maximumValue, int capacity)
{
	maxVal = maximumVal;
	cap = capacity;
	sparse = new T[maxVal + 1];
	dense = new T[cap];
}

template<typename T>
bool Sparse_Set<T>::add(T element)
{
	if (x > n || x > maxVal) {
		return false;
	}
	dense[sparseIndex] = element;
	sparse
	
	
}



template<typename T>
bool Sparse_Set<T>::remove()
{
	return false;
}

template<typename T>
bool Sparse_Set<T>::get()
{
	return false;
}

template<typename T>
bool Sparse_Set<T>::clear()
{
	return false;
}
