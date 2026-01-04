#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include <iostream>
#include <string>
using namespace std;

// ============================================================================
// BINARY SEARCH TREE (BST)
// ============================================================================
// Digunakan untuk pencarian cepat: O(log n) average case
// Contoh penggunaan: Cari kamar by nomor, cari user by ID
// ============================================================================

template<typename T, typename KeyType>
struct BSTNode {
    T data;
    KeyType key;
    BSTNode<T, KeyType>* left;
    BSTNode<T, KeyType>* right;
    
    BSTNode(const T& nilai, const KeyType& kunci) 
        : data(nilai), key(kunci), left(nullptr), right(nullptr) {}
};

template<typename T, typename KeyType>
class BinarySearchTree {
private:
    BSTNode<T, KeyType>* root;
    int jumlahNode;
    
    // Helper function untuk insert rekursif
    BSTNode<T, KeyType>* insertRekursif(BSTNode<T, KeyType>* node, 
                                         const T& data, const KeyType& key) {
        // Base case: tempat kosong ditemukan
        if (node == nullptr) {
            jumlahNode++;
            return new BSTNode<T, KeyType>(data, key);
        }
        
        // Rekursif ke kiri atau kanan
        if (key < node->key) {
            node->left = insertRekursif(node->left, data, key);
        } else if (key > node->key) {
            node->right = insertRekursif(node->right, data, key);
        } else {
            // Key sudah ada, update data
            node->data = data;
        }
        
        return node;
    }
    
    // Helper function untuk search rekursif
    BSTNode<T, KeyType>* searchRekursif(BSTNode<T, KeyType>* node, 
                                         const KeyType& key) const {
        // Base case: tidak ditemukan atau ditemukan
        if (node == nullptr || node->key == key) {
            return node;
        }
        
        // Rekursif ke kiri atau kanan
        if (key < node->key) {
            return searchRekursif(node->left, key);
        } else {
            return searchRekursif(node->right, key);
        }
    }
    
    // Helper function untuk cari node minimum
    BSTNode<T, KeyType>* findMin(BSTNode<T, KeyType>* node) const {
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }
    
    // Helper function untuk delete rekursif
    BSTNode<T, KeyType>* deleteRekursif(BSTNode<T, KeyType>* node, 
                                         const KeyType& key, bool& berhasil) {
        if (node == nullptr) {
            berhasil = false;
            return nullptr;
        }
        
        if (key < node->key) {
            node->left = deleteRekursif(node->left, key, berhasil);
        } 
        else if (key > node->key) {
            node->right = deleteRekursif(node->right, key, berhasil);
        } 
        else {
            // Node ditemukan, hapus!
            berhasil = true;
            
            // Case 1: Leaf node (tidak punya anak)
            if (node->left == nullptr && node->right == nullptr) {
                delete node;
                jumlahNode--;
                return nullptr;
            }
            
            // Case 2: Punya 1 anak (kiri)
            if (node->right == nullptr) {
                BSTNode<T, KeyType>* temp = node->left;
                delete node;
                jumlahNode--;
                return temp;
            }
            
            // Case 3: Punya 1 anak (kanan)
            if (node->left == nullptr) {
                BSTNode<T, KeyType>* temp = node->right;
                delete node;
                jumlahNode--;
                return temp;
            }
            
            // Case 4: Punya 2 anak
            // Cari successor (node terkecil di subtree kanan)
            BSTNode<T, KeyType>* successor = findMin(node->right);
            
            // Copy data successor ke node ini
            node->data = successor->data;
            node->key = successor->key;
            
            // Hapus successor
            node->right = deleteRekursif(node->right, successor->key, berhasil);
        }
        
        return node;
    }
    
    // Helper untuk inorder traversal
    template<typename Function>
    void inorderRekursif(BSTNode<T, KeyType>* node, Function func) const {
        if (node != nullptr) {
            inorderRekursif(node->left, func);
            func(node->data);
            inorderRekursif(node->right, func);
        }
    }
    
    // Helper untuk destroy tree
    void destroyRekursif(BSTNode<T, KeyType>* node) {
        if (node != nullptr) {
            destroyRekursif(node->left);
            destroyRekursif(node->right);
            delete node;
        }
    }

public:
    // Constructor
    BinarySearchTree() : root(nullptr), jumlahNode(0) {}
    
    // Destructor
    ~BinarySearchTree() {
        clear();
    }
    
    // Cek apakah tree kosong
    bool kosong() const {
        return root == nullptr;
    }
    
    // Dapatkan jumlah node
    int ukuran() const {
        return jumlahNode;
    }
    
    // Insert data dengan key
    void insert(const T& data, const KeyType& key) {
        root = insertRekursif(root, data, key);
    }
    
    // Search data berdasarkan key
    T* search(const KeyType& key) {
        BSTNode<T, KeyType>* node = searchRekursif(root, key);
        
        if (node != nullptr) {
            return &(node->data);
        }
        
        return nullptr;
    }
    
    // Search const version
    const T* search(const KeyType& key) const {
        BSTNode<T, KeyType>* node = searchRekursif(root, key);
        
        if (node != nullptr) {
            return &(node->data);
        }
        
        return nullptr;
    }
    
    // Hapus data berdasarkan key
    bool hapus(const KeyType& key) {
        bool berhasil = false;
        root = deleteRekursif(root, key, berhasil);
        return berhasil;
    }
    
    // Clear semua data
    void clear() {
        destroyRekursif(root);
        root = nullptr;
        jumlahNode = 0;
    }
    
    // Inorder traversal (terurut ascending)
    template<typename Function>
    void inorder(Function func) const {
        inorderRekursif(root, func);
    }
    
    // Tampilkan tree (inorder - terurut)
    void tampilkan() const {
        if (kosong()) {
            cout << "[Tree Kosong]" << endl;
            return;
        }
        
        cout << "\n=== Binary Search Tree (Inorder) ===" << endl;
        cout << "Jumlah Node: " << jumlahNode << endl;
        cout << "-----------------------------------" << endl;
        
        int nomor = 1;
        inorder([&nomor](const T& data) {
            cout << nomor++ << ". " << data << endl;
        });
    }
    
    // Cari data dalam range [keyMin, keyMax]
    template<typename Function>
    void searchRange(const KeyType& keyMin, const KeyType& keyMax, 
                     Function func) const {
        searchRangeRekursif(root, keyMin, keyMax, func);
    }
    
private:
    template<typename Function>
    void searchRangeRekursif(BSTNode<T, KeyType>* node, 
                             const KeyType& keyMin, const KeyType& keyMax,
                             Function func) const {
        if (node == nullptr) return;
        
        // Jika current key lebih besar dari min, cek left subtree
        if (node->key > keyMin) {
            searchRangeRekursif(node->left, keyMin, keyMax, func);
        }
        
        // Jika current key dalam range, proses
        if (node->key >= keyMin && node->key <= keyMax) {
            func(node->data);
        }
        
        // Jika current key lebih kecil dari max, cek right subtree
        if (node->key < keyMax) {
            searchRangeRekursif(node->right, keyMin, keyMax, func);
        }
    }
};

#endif
