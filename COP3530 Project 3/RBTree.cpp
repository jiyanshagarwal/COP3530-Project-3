#include <iostream>
#include <string>
#include <vector>
#include "CardManager.h"
using namespace std;

enum Color {RED, BLACK};

struct Node {
    CarData carInfo;
    bool color;
    Node* left, * right, * parent;

    Node(CarData car)
    {
        this->carInfo = car;
        left = right = parent = nullptr;
        this->color = RED;
    }
};

class RBTree
{
private:
    Node* root;
protected:
    void rotateLeft(Node*&, Node*&);
    void rotateRight(Node*&, Node*&);
    void fixViolation(Node*&, Node*&);
public:
    RBTree() { root = nullptr; }
    void insert(const CarData& car);
};

Node* BSTinsert(Node* root, Node* car) {
    if (root == nullptr) {
        return car;
    }

    if (car->carInfo.car_price < root->carInfo.car_price) {
        root->left = BSTinsert(root->left, car);
        root->left->parent = root;
    }
    else if (car->carInfo.car_price > root->carInfo.car_price) {
        root->right = BSTinsert(root->right, car);
        root->right->parent = root;
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

void RBTree::rotateLeft(Node*& root, Node*& car)
{
    Node* carRight = car->right;
    car->right = carRight->left;

    if (car->right != nullptr)
        car->right->parent = car;

    carRight->parent = car->parent;

    if (car->parent == nullptr)
        root = carRight;

    else if (car == car->parent->left)
        car->parent->left = carRight;

    else
        car->parent->right = carRight;

    carRight->left = car;
    car->parent = carRight;
}

void RBTree::rotateRight(Node*& root, Node*& car)
{
    Node* carLeft = car->left;
    car->left = carLeft->right;

    if (car->left != nullptr)
        car->left->parent = car;

    carLeft->parent = car->parent;

    if (car->parent == nullptr)
        root = carLeft;

    else if (car == car->parent->left)
        car->parent->left = carLeft;

    else
        car->parent->right = carLeft;

    carLeft->right = car;
    car->parent = carLeft;
}

void RBTree::fixViolation(Node*& root, Node*& car)
{
    Node* parentCar = nullptr;
    Node* grandCar = nullptr;

    while ((car != root) && (car->color != BLACK) && (car->parent->color == RED))
    {
        parentCar = car->parent;
        grandCar = car->parent->parent;

        if (parentCar == grandCar->left)
        {
            Node* uncleCar = grandCar->right;

            if (uncleCar != nullptr && uncleCar->color == RED)
            {
                grandCar->color = RED;
                parentCar->color = BLACK;
                uncleCar->color = BLACK;
                car = grandCar;
            }

            else
            {
                if (car == parentCar->right)
                {
                    rotateLeft(root, parentCar);
                    car = parentCar;
                    parentCar = car->parent;
                }

                rotateRight(root, grandCar);
                swap(parentCar->color, grandCar->color);
                car = parentCar;
            }
        }

        else
        {
            Node* uncleCar = grandCar->left;

            if (uncleCar != nullptr && uncleCar->color == RED)
            {
                grandCar->color = RED;
                parentCar->color = BLACK;
                uncleCar->color = BLACK;
                car = grandCar;
            }

            else
            {
                if (car == parentCar->left)
                {
                    rotateRight(root, parentCar);
                    car = parentCar;
                    parentCar = car->parent;
                }

                rotateLeft(root, grandCar);
                swap(parentCar->color, grandCar->color);
                car = parentCar;
            }
        }
    }

    root->color = BLACK;
}

void RBTree::insert(const CarData& data)
{
    Node* car = new Node(data);
    root = BSTinsert(root, car);
    fixViolation(root, car);
}
