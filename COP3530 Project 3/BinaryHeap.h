#pragma once
#include <vector>
#include <cmath>
#include <stdexcept>

template <typename T>
class BinaryHeap {
public:
	BinaryHeap(bool is_min_heap = true);
	BinaryHeap(std::vector<T> vec, bool is_min_heap = true);

	void Insert(T item);
	T& Front();
	void Remove();

	static void HeapSort(std::vector<T>& vec, bool is_min_heap = true);
	unsigned int Size() const;
	bool isMinHeap() const;
	void Clear();

private:
	std::vector<T> heap;
	unsigned int size;
	bool is_min_heap;

	bool Compare(T& item1, T& item2) const;
	void HeapifyDown(unsigned int index);

	static bool Compare(T& item1, T& item2, bool is_min_heap);
	static void HeapifyDown(std::vector<T>& vec, unsigned int size, unsigned int index, bool is_min_heap);
};

template <typename T>
BinaryHeap<T>::BinaryHeap(bool is_min_heap) : is_min_heap(is_min_heap), size(0) {}

template <typename T>
BinaryHeap<T>::BinaryHeap(std::vector<T> vec, bool is_min_heap) : heap(vec), is_min_heap(is_min_heap), size(vec.size()) {
	for (int i = size / 2; i >= 0; i--) {
		HeapifyDown(i);
	}
}

template <typename T>
void BinaryHeap<T>::Insert(T item) {
	if (heap.size() > size) heap[size] = item;
	else heap.push_back(item);
	size++;

	int child = size - 1;
	int parent = std::floor((child - 1) / 2);

	while (parent >= 0 && Compare(heap[child], heap[parent])) {
		std::swap(heap[parent], heap[child]);
		child = parent;
		parent = std::floor((child - 1) / 2);
	}
}

template <typename T>
T& BinaryHeap<T>::Front() {
	if (size == 0) throw std::out_of_range("Cannot call front on empty binary heap.");
	return heap[0];
}

template <typename T>
void BinaryHeap<T>::Remove() {
	if (size == 0) return;
	if (size == 1) {
		heap.clear();
		size--;
		return;
	}

	heap[0] = heap[size - 1];
	size--;

	HeapifyDown(0);
}

template <typename T>
void BinaryHeap<T>::HeapSort(std::vector<T>& vec, bool is_min_heap) {
	//The process of sorting results in a reversed array, so is_min_heap is negated.
	for (int i = vec.size() / 2; i >= 0; i--) {
		HeapifyDown(vec, vec.size(), i, !is_min_heap);
	}

	int size = vec.size();

	for (int i = 0; i < vec.size(); i++) {
		std::swap(vec[0], vec[size - 1]);
		size--;
		HeapifyDown(vec, size, 0, !is_min_heap);
	}
}

template <typename T>
unsigned int BinaryHeap<T>::Size() const {
	return size;
}

template <typename T>
bool BinaryHeap<T>::isMinHeap() const {
	return is_min_heap;
}

template <typename T>
void BinaryHeap<T>::Clear() {
	heap.clear();
	size = 0;
}

template <typename T>
bool BinaryHeap<T>::Compare(T& item1, T& item2) const {
	if (is_min_heap) return item1 < item2;
	else return item1 > item2;
}

template <typename T>
void BinaryHeap<T>::HeapifyDown(unsigned int index) {
	int parent = index;
	int favored = parent;

	do {
		parent = favored;
		int left_child = 2 * parent + 1;
		int right_child = left_child + 1;

		if (left_child < size && Compare(heap[left_child], heap[favored])) favored = left_child;
		if (right_child < size && Compare(heap[right_child], heap[favored])) favored = right_child;

		std::swap(heap[parent], heap[favored]);

	} while (favored != parent);
}

template <typename T>
bool BinaryHeap<T>::Compare(T& item1, T& item2, bool is_min_heap) {
	if (is_min_heap) return item1 < item2;
	else return item1 > item2;
}

template <typename T>
void BinaryHeap<T>::HeapifyDown(std::vector<T>& vec, unsigned int size, unsigned int index, bool is_min_heap) {
	int parent = index;
	int favored = parent;

	do {
		parent = favored;
		int left_child = 2 * parent + 1;
		int right_child = left_child + 1;

		if (left_child < size && Compare(vec[left_child], vec[favored], is_min_heap)) favored = left_child;
		if (right_child < size && Compare(vec[right_child], vec[favored], is_min_heap)) favored = right_child;

		std::swap(vec[parent], vec[favored]);

	} while (favored != parent);
}