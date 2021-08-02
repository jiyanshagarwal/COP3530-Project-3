#pragma once
#include <string>
#include <vector>
#include <stdexcept>

template <typename K, typename T>
class RBTree {
private:
	enum Color { RED, BLACK };
	struct Node {
		K key;
		T data;
		bool color;
		Node* left, * right, * parent;

		Node(K key, T data);
		~Node();
	};

	Node* root;

	Node* BSTinsert(Node*& root, Node* parent, Node* data);

	void rotateLeft(Node*&, Node*&);
	void rotateRight(Node*&, Node*&);
	void fixViolation(Node*&, Node*&);

	T search(Node* root, K key);
	void inOrder(Node* root, std::vector<T>& data);

public:
	RBTree() { root = nullptr; }
	~RBTree() { delete root; }
	void insert(K key, T data);
	bool search(K key);
	void inOrder(std::vector<T>& data);
};

template <typename K, typename T>
RBTree<K, T>::Node::Node(K key, T data) {
	this->key = key;
	this->data = data;
	left = right = parent = nullptr;
	this->color = Color::RED;
}

template <typename K, typename T>
RBTree<K, T>::Node::~Node() {
	delete left;
	delete right;
}

template <typename K, typename T>
typename RBTree<K, T>::Node* RBTree<K, T>::BSTinsert(Node*& root, Node* parent, Node* data) {
	if (root == nullptr) {
		root = data;
		root->parent = parent;
		return root;
	}

	if (data->key < root->key) return BSTinsert(root->left, root, data);
	return BSTinsert(root->right, root, data);
}

template <typename K, typename T>
T RBTree<K, T>::search(Node* root, K key) {
	if (root == nullptr) throw std::out_of_range("Root is nullptr");

	if (root->key == key) return root->data;

	if (root->left != nullptr) return search(root->left, key);
	if (root->right != nullptr) return search(root->right, key);

	throw std::out_of_range("Key not found");
}

template <typename K, typename T>
bool RBTree<K, T>::search(K key) {
	return search(root, key);
}

template <typename K, typename T>
void RBTree<K, T>::rotateLeft(Node*& root, Node*& data) {
	Node* dataRight = data->right;
	data->right = dataRight->left;

	if (data->right != nullptr) {
		data->right->parent = data;
	}

	dataRight->parent = data->parent;

	if (data->parent == nullptr)
		root = dataRight;

	else if (data == data->parent->left)
		data->parent->left = dataRight;

	else
		data->parent->right = dataRight;

	dataRight->left = data;
	data->parent = dataRight;
}

template <typename K, typename T>
void RBTree<K, T>::rotateRight(Node*& root, Node*& data) {
	Node* dataLeft = data->left;
	data->left = dataLeft->right;

	if (data->left != nullptr)
		data->left->parent = data;

	dataLeft->parent = data->parent;

	if (data->parent == nullptr)
		root = dataLeft;

	else if (data == data->parent->left)
		data->parent->left = dataLeft;

	else
		data->parent->right = dataLeft;

	dataLeft->right = data;
	data->parent = dataLeft;
}

template <typename K, typename T>
void RBTree<K, T>::fixViolation(Node*& root, Node*& car) {
	if (root == nullptr || car == nullptr) return;

	Node* parentCar = nullptr;
	Node* grandCar = nullptr;

	while ((car != root) && (car->color != Color::BLACK) && (car->parent->color == Color::RED)) {
		parentCar = car->parent;
		grandCar = car->parent->parent;

		if (parentCar == grandCar->left) {
			Node* uncleCar = grandCar->right;

			if (uncleCar != nullptr && uncleCar->color == Color::RED) {
				grandCar->color = Color::RED;
				parentCar->color = Color::BLACK;
				uncleCar->color = Color::BLACK;
				car = grandCar;
			}
			else {
				if (car == parentCar->right) {
					rotateLeft(root, parentCar);
					car = parentCar;
					parentCar = car->parent;
				}

				rotateRight(root, grandCar);
				std::swap(parentCar->color, grandCar->color);
				car = parentCar;
			}
		}
		else {
			Node* uncleCar = grandCar->left;

			if (uncleCar != nullptr && uncleCar->color == Color::RED) {
				grandCar->color = Color::RED;
				parentCar->color = Color::BLACK;
				uncleCar->color = Color::BLACK;
				car = grandCar;
			}
			else {
				if (car == parentCar->left) {
					rotateRight(root, parentCar);
					car = parentCar;
					parentCar = car->parent;
				}

				rotateLeft(root, grandCar);
				std::swap(parentCar->color, grandCar->color);
				car = parentCar;
			}
		}
	}

	root->color = Color::BLACK;
}

template <typename K, typename T>
void RBTree<K, T>::insert(K key, T data) {
	Node* node = BSTinsert(root, nullptr, new Node(key, data));
	fixViolation(root, node);
}

template <typename K, typename T>
void RBTree<K, T>::inOrder(std::vector<T>& data) {
	data.clear();
	inOrder(root, data);
}

template <typename K, typename T>
void RBTree<K, T>::inOrder(Node* root, std::vector<T>& data) {
	if (root == nullptr) return;
	inOrder(root->left, data);
	data.push_back(root->data);
	inOrder(root->right, data);
}
