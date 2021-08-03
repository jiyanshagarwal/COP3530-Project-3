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

	void rotateLeft(Node*& node);
	void rotateRight(Node*& node);
	void fixViolation(Node* node);

	T search(Node* root, K key) const;
	void inOrder(Node* root, std::vector<T>& data) const;

	Node* getUncle(Node* node) const;

public:
	RBTree() { root = nullptr; }
	~RBTree() { delete root; }
	void insert(K key, T data);
	bool search(K key) const;
	void inOrder(std::vector<T>& data) const;
	void clear();
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
	if (data->key > root->key) return BSTinsert(root->right, root, data);
	return root;	//if keys are equal
}

template <typename K, typename T>
T RBTree<K, T>::search(Node* root, K key) const {
	if (root == nullptr) throw std::out_of_range("Root is nullptr");

	if (root->key == key) return root->data;

	if (root->left != nullptr) return search(root->left, key);
	if (root->right != nullptr) return search(root->right, key);

	throw std::out_of_range("Key not found");
}

template <typename K, typename T>
bool RBTree<K, T>::search(K key) const {
	return search(root, key);
}

template <typename K, typename T>
void RBTree<K, T>::rotateLeft(Node*& node) {
	//Referred to AVL lecture 4c
	if (node == nullptr || node->right == nullptr) return;
	Node* oldParent = node;
	node = node->right;				//Changing the node reference itself.
	oldParent->right = node->left;
	node->left = oldParent;
}

template <typename K, typename T>
void RBTree<K, T>::rotateRight(Node*& node) {
	//Referred to AVL lecture 4c
	if (node == nullptr || node->left == nullptr) return;
	Node* oldParent = node;
	node = node->left;				//Changing the node reference itself.
	oldParent->left = node->right;
	node->right = oldParent;
}

template <typename K, typename T>
void RBTree<K, T>::fixViolation(Node* node) {
	//Used Cheryl Red Black Tree lecture for help with pseudocode
	//Also used: https://www.youtube.com/watch?v=qA02XWRTBdw
	if (node->parent == nullptr) {
		node->color = Color::BLACK;
		return;
	}
	if (node->parent->color == Color::BLACK) return;

	Node* parent = node->parent;
	Node* grandparent = node->parent->parent;
	Node* uncle = getUncle(node);

	if (uncle != nullptr && uncle->color == Color::RED) {
		parent->color = uncle->color = Color::BLACK;
		grandparent->color = Color::RED;
		fixViolation(grandparent);
		return;
	}
	
	if (parent == grandparent->left) {
		if (node == parent->left) {				//Left-Left case
			rotateRight(grandparent);
			parent->color = Color::BLACK;
			parent->right->color = Color::RED;
		}
		else {									//Left-Right case
			rotateLeft(parent);
			rotateLeft(grandparent);
			parent->color = Color::BLACK;
			parent->right->color = Color::RED;
		}
	}
	else {
		if (node == parent->left) {				//Right-Left case
			rotateRight(parent);
			rotateLeft(grandparent);
			parent->color = Color::BLACK;
			parent->left->color = Color::RED;
		}
		else {									//Right-Right case
			rotateLeft(grandparent);
			parent->color = Color::BLACK;
			parent->left->color = Color::RED;
		}
	}

}

template <typename K, typename T>
void RBTree<K, T>::insert(K key, T data) {
	Node* node = BSTinsert(root, nullptr, new Node(key, data));
	//fixViolation(node);
}

template <typename K, typename T>
void RBTree<K, T>::inOrder(std::vector<T>& data) const {
	data.clear();
	inOrder(root, data);
}

template <typename K, typename T>
void RBTree<K, T>::clear() {
	delete root;
}

template <typename K, typename T>
void RBTree<K, T>::inOrder(Node* root, std::vector<T>& data) const {
	if (root == nullptr) return;
	inOrder(root->left, data);
	data.push_back(root->data);
	inOrder(root->right, data);
}

template <typename K, typename T>
typename RBTree<K, T>::Node* RBTree<K, T>::getUncle(Node* node) const {
	if (node == nullptr || node->parent == nullptr || node->parent->parent == nullptr) return nullptr;

	Node* grandparent = node->parent->parent;
	if (grandparent->left == node->parent) return grandparent->right;
	else return grandparent->left;
}