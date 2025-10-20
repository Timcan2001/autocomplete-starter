// Yusuf Pisan pisan@uw.edu
// 17 Jan 2021

// BST class
// Creates a BST to store values
// Uses Node which holds the data

#include "bstmap.h"
#include <cassert>

using namespace std;

// copy constructor
BSTMap::BSTMap(const BSTMap &bst) { root = copyTree(bst.root); }

// given an array of length n
// create a tree to have all items in that array
// with the minimum height (uses same helper as rebalance)
BSTMap::BSTMap(const vector<value_type> &v) {
  root = buildFromSorted(v, 0, v.size());
}

BSTMap::Node *BSTMap::buildFromSorted(const vector<value_type> &v, size_t start,
                                      size_t end) {
  if (start >= end) {
    return nullptr;
  }

  size_t mid = start + (end - start) / 2;

  Node *newNode = new Node;
  newNode->data = v[mid];
  newNode->left = nullptr;
  newNode->right = nullptr;
  newNode->left = buildFromSorted(v, start, mid);
  newNode->right = buildFromSorted(v, mid + 1, end);

  return newNode;
}

// destructor
BSTMap::~BSTMap() { clear(); }

// delete all nodes in tree
void BSTMap::clear() {
  deleteTree(root);
  root = nullptr;
}

// true if no nodes in BST
bool BSTMap::empty() const { return root == nullptr; }

// Number of nodes in BST
int BSTMap::size() const {
  if (root == nullptr) {
    return 0;
  }
  return sizeH(root);
}

int BSTMap::sizeH(Node *node) const {
  if (node == nullptr) {
    return 0;
  }
  return 1 + sizeH(node->left) + sizeH(node->right);
}

// true if item is in BST
bool BSTMap::contains(const key_type &key) const {
  if (root == nullptr) {
    return false;
  }
  Node *curr = root;
  while (curr != nullptr) {
    if (key == curr->data.first) {
      return true;
    }
    if (key < curr->data.first) {
      curr = curr->left;
    } else {
      curr = curr->right;
    }
  }
  return false;
}

// If k matches the key returns a reference to its value
// If k does not match any key, inserts a new element
// with that key and returns a reference to its mapped value.
BSTMap::mapped_type &BSTMap::operator[](const key_type &k) {
  return bracketHelper(root, k);
}

BSTMap::mapped_type &BSTMap::bracketHelper(Node *&curr, const key_type &k) {
  if (curr == nullptr) {
    Node *newNode = new Node;
    newNode->data.first = k;
    newNode->data.second = 0;
    curr = newNode;
    curr->left = nullptr;
    curr->right = nullptr;
    return curr->data.second;
  }

  if (k < curr->data.first) {
    return bracketHelper(curr->left, k);
  }
  if (k > curr->data.first) {
    return bracketHelper(curr->right, k);
  }
  if (curr->data.first == k) {
    return curr->data.second;
  }
  return curr->data.second;
}

// returns a vector of key-value pairs that partially match the key
// Main function used by autocomplete
// Might traverse both left and right subbranches of a node
// Example: getall("hel")
// Return: { (hello, 10), (help, 20)}
vector<BSTMap::value_type> BSTMap::getAll(const key_type &k) const {
  vector<value_type> v;
  getAllH(root, k, v);
  return v;
}

void BSTMap::getAllH(const Node *curr, const key_type &prefix,
                     vector<value_type> &out) const {
  if (curr == nullptr) {
    return;
  }
  if (curr->data.first.rfind(prefix, 0) == 0) {
    getAllH(curr->left, prefix, out);
    out.push_back(curr->data);
    getAllH(curr->right, prefix, out);
  } else if (curr->data.first < prefix) {
    getAllH(curr->right, prefix, out);
  } else {
    getAllH(curr->left, prefix, out);
  }
}

// 0 if empty, 1 if only root, otherwise
// height of root is max height of subtrees + 1
int BSTMap::height() const { return getHeight(root); }

// height of a Node, nullptr is 0, root is 1, static, no access to 'this'
// helper function to height(), used by printVertical
int BSTMap::getHeight(const Node *n) {
  if (n == nullptr) {
    return 0;
  }
  return 1 + max(getHeight(n->right), getHeight(n->left));
}

BSTMap::Node *BSTMap::copyTree(Node *curr) {
  if (curr == nullptr) {
    return nullptr;
  }
  Node *newCurr = new Node;
  newCurr->data = curr->data;
  newCurr->left = copyTree(curr->left);
  newCurr->right = copyTree(curr->right);

  return newCurr;
}

void BSTMap::deleteTree(Node *curr) {
  if (curr == nullptr) {
    return;
  }
  deleteTree(curr->left);
  deleteTree(curr->right);
  delete curr;
}

// same as contains, but returns 1 or 0
// compatibility with std::map
size_t BSTMap::count(const key_type &k) const {
  if (root == nullptr) {
    return 0;
  }
  Node *curr = root;
  while (curr != nullptr) {
    if (k == curr->data.first) {
      return 1;
    }
    if (k < curr->data.first) {
      curr = curr->left;
    } else {
      curr = curr->right;
    }
  }
  return 0;
}

// inorder traversal: left-root-right
// takes a function that takes a single parameter of type T
void BSTMap::inorder(void visit(const value_type &item)) const {
  if (root == nullptr) {
    return;
  }
  inorderH(visit, root);
}

void BSTMap::inorderH(function<void(const value_type &item)> visit,
                      const Node *node) const {
  if (node == nullptr) {
    return;
  }
  inorderH(visit, node->left);
  visit(node->data);
  inorderH((visit), node->right);
}

// preorder traversal: root-left-right
void BSTMap::preorder(void visit(const value_type &item)) const {
  if (root == nullptr) {
    return;
  }
  preorderH(visit, root);
}

void BSTMap::preorderH(void visit(const value_type &item),
                       const Node *node) const {
  if (node == nullptr) {
    return;
  }
  visit(node->data);
  preorderH(visit, node->left);
  preorderH(visit, node->right);
}

// postorder traversal: left-right-root
void BSTMap::postorder(void visit(const value_type &item)) const {
  if (root == nullptr) {
    return;
  }
  postorderH(visit, root);
}

void BSTMap::postorderH(void visit(const value_type &item),
                        const Node *node) const {
  if (node == nullptr) {
    return;
  }
  postorderH(visit, node->left);
  postorderH(visit, node->right);
  visit(node->data);
}

// balance the BST by saving all nodes to a vector inorder
// and then recreating the BST from the vector
void BSTMap::rebalance() {
  if (root == nullptr) {
    return;
  }
  vector<value_type> nodes;
  auto visit = [&nodes](const value_type &item) { nodes.push_back(item); };
  inorderH(visit, root);
  clear();
  root = buildFromSorted(nodes, 0, nodes.size());
}

// trees are equal if they have the same structure
// AND the same item values at all the nodes
bool BSTMap::operator==(const BSTMap &other) const {
  return equalsH(root, other.root);
}

bool BSTMap::equalsH(const Node *t1, const Node *t2) const {
  if (t1 == nullptr && t2 == nullptr) {
    return true;
  }

  if (t1 == nullptr || t2 == nullptr || t1->data.first != t2->data.first ||
      t1->data.second != t2->data.second) {
    return false;
  }

  return equalsH(t1->left, t2->left) && equalsH(t1->right, t2->right);
}

// not == to each other
bool BSTMap::operator!=(const BSTMap &other) const {
  return !(equalsH(root, other.root));
}
