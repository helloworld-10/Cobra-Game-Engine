#pragma once

template<typename T>
class Sparse_Set {
	Sparse_Set(int maxVal, int capacity);
	bool add(T x);
	bool remove();
	bool get();
	bool clear();
private:
	T[] sparse;
	T[] dense;
	int maxVal;
	int cap;
	int denseIndex;
	int sparseIndex;
};