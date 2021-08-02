#include <iostream>
#include <string>
#include <vector>
#include "CardManager.h"
using namespace std;

struct Node {
    CarData carInfo;
    Node *left, *right;
};

Node* newNode(CarData car) {
    Node* node = new Node;
    node->carInfo = car;
    node->left = node->right = nullptr;
    return node;
}

Node* insert(Node* root, CarData car) {
    if (root == nullptr) {
        return newNode(car);
    }

    if (car.car_price < root->carInfo.car_price) {
        root->left = insert(root->left, car);
    } else {
        root->right = insert(root->right, car);
    }

    return root;
}

Node* searchbyPrice(Node* root, int desiredPrice) {
    if (root == NULL || stoi(root->carInfo.car_price) == desiredPrice) {
        return root;
    }

    if (stoi(root->carInfo.car_price) < desiredPrice) {
        return searchbyPrice(root->right, desiredPrice);
    }

    return searchbyPrice(root->left, desiredPrice);
}

Node* searchbyBrand(Node* root, string desiredBrand) {
    if (root == NULL || root->carInfo.car_name == desiredBrand) {
        return root;
    }

    if (root->carInfo.car_price < desiredBrand) {
        return searchbyBrand(root->right, desiredBrand);
    }

    return searchbyBrand(root->left, desiredBrand);
}