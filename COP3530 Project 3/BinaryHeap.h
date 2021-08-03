#pragma once
#include <vector>
#include <cmath>
#include <stdexcept>

template <typename K, typename T>
class BinaryHeap {
public:
	BinaryHeap(bool is_min_heap = true);
	BinaryHeap(std::vector<std::pair<K, T>>, bool is_min_heap = true);

	void Insert(K key, T item);
	T& Front();
	void Remove();

	static void HeapSort(std::vector<std::pair<K, T>>& vec, bool is_min_heap = true);
	unsigned int Size() const;
	bool isMinHeap() const;
	void Clear();

private:
	std::vector<std::pair<K, T>> heap;
	unsigned int size;
	bool is_min_heap;

	bool Compare(K& item1, K& item2) const;
	void HeapifyDown(unsigned int index);

	static bool Compare(K& item1, K& item2, bool is_min_heap);
	static void HeapifyDown(std::vector<std::pair<K, T>>& vec, unsigned int size, unsigned int index, bool is_min_heap);
};

template <typename K, typename T>
BinaryHeap<K, T>::BinaryHeap(bool is_min_heap) : is_min_heap(is_min_heap), size(0) {}

template <typename K, typename T>
BinaryHeap<K, T>::BinaryHeap(std::vector<std::pair<K, T>> vec, bool is_min_heap) : heap(vec), is_min_heap(is_min_heap), size(vec.size()) {
	for (int i = size / 2; i >= 0; i--) {
		HeapifyDown(i);
	}
}

template <typename K, typename T>
void BinaryHeap<K, T>::Insert(K key, T item) {
	if (heap.size() > size) heap[size] = std::make_pair(key, item);
	else heap.push_back(std::make_pair(key, item));
	size++;

	int child = size - 1;
	int parent = std::floor((child - 1) / 2);

	while (parent >= 0 && Compare(heap[child].first, heap[parent].first)) {
		std::swap(heap[parent], heap[child]);
		child = parent;
		parent = std::floor((child - 1) / 2);
	}
}

template <typename K, typename T>
T& BinaryHeap<K, T>::Front() {
	if (size == 0) throw std::out_of_range("Cannot call front on empty binary heap.");
	return heap[0].second;
}

template <typename K, typename T>
void BinaryHeap<K, T>::Remove() {
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

template <typename K, typename T>
void BinaryHeap<K, T>::HeapSort(std::vector<std::pair<K, T>>& vec, bool is_min_heap) {
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

template <typename K, typename T>
unsigned int BinaryHeap<K, T>::Size() const {
	return size;
}

template <typename K, typename T>
bool BinaryHeap<K, T>::isMinHeap() const {
	return is_min_heap;
}

template <typename K, typename T>
void BinaryHeap<K, T>::Clear() {
	heap.clear();
	size = 0;
}

template <typename K, typename T>
bool BinaryHeap<K, T>::Compare(K& item1, K& item2) const {
	if (is_min_heap) return item1 < item2;
	else return item1 > item2;
}

template <typename K, typename T>
void BinaryHeap<K, T>::HeapifyDown(unsigned int index) {
	int parent = index;
	int favored = parent;

	do {
		parent = favored;
		int left_child = 2 * parent + 1;
		int right_child = left_child + 1;

		if (left_child < size && Compare(heap[left_child].first, heap[favored].first)) favored = left_child;
		if (right_child < size && Compare(heap[right_child].first, heap[favored].first)) favored = right_child;

		std::swap(heap[parent], heap[favored]);

	} while (favored != parent);
}

template <typename K, typename T>
bool BinaryHeap<K, T>::Compare(K& item1, K& item2, bool is_min_heap) {
	if (is_min_heap) return item1 < item2;
	else return item1 > item2;
}

template <typename K, typename T>
void BinaryHeap<K, T>::HeapifyDown(std::vector<std::pair<K, T>>& vec, unsigned int size, unsigned int index, bool is_min_heap) {
	int parent = index;
	int favored = parent;

	do {
		parent = favored;
		int left_child = 2 * parent + 1;
		int right_child = left_child + 1;

		if (left_child < size && Compare(vec[left_child].first, vec[favored].first, is_min_heap)) favored = left_child;
		if (right_child < size && Compare(vec[right_child].first, vec[favored].first, is_min_heap)) favored = right_child;

		std::swap(vec[parent], vec[favored]);

	} while (favored != parent);
}