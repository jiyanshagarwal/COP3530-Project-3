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

	unsigned int Size() const;
	bool isMinHeap() const;
	void Clear();

private:
	std::vector<T> heap;
	unsigned int size;
	bool is_min_heap;

	bool Compare(T& item1, T& item2) const;
	void HeapifyDown(unsigned int index);
};

template <typename T>
BinaryHeap<T>::BinaryHeap(bool is_min_heap) : is_min_heap(is_min_heap) {
	size = 0;
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

	unsigned int parent = 0;
	unsigned int left_child = 1;
	unsigned int right_child = 2;

	while (left_child < size) {
		int favored_child = Compare(heap[left_child], heap[right_child]) ? left_child : right_child;

		//Note that the order matters here. By putting parent second, we are effectively asking the OPPOSITE of what the heap type is.
		//E.g. if its a min heap, then we are asking when parent is GREATER THAN favored_child
		if (Compare(heap[favored_child], heap[parent])) {
			std::swap(heap[parent], heap[favored_child]);
			parent = favored_child;
			left_child = 2 * parent + 1;
			right_child = left_child + 1;
		}
		else {
			break;
		}
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