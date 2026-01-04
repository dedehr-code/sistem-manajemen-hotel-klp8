#ifndef STACK_H
#define STACK_H

#include <iostream>
#include <string>
using namespace std;

// ============================================================================
// STACK - UNTUK HISTORY TRANSAKSI
// ============================================================================
// Digunakan untuk: History pesanan pelanggan, Undo operations
// Prinsip: LIFO (Last In First Out)
// ============================================================================

template<typename T>
struct StackNode {
    T data;
    StackNode<T>* next;
    
    StackNode(const T& nilai) : data(nilai), next(nullptr) {}
};

template<typename T>
class Stack {
private:
    StackNode<T>* top;
    int jumlahItem;
    int maksimalKapasitas; // Batas maksimal untuk prevent memory overflow

public:
    // Constructor
    Stack(int maksKapasitas = 100) 
        : top(nullptr), jumlahItem(0), maksimalKapasitas(maksKapasitas) {}
    
    // Destructor
    ~Stack() {
        while (!kosong()) {
            pop();
        }
    }
    
    // Cek apakah stack kosong
    bool kosong() const {
        return top == nullptr;
    }
    
    // Cek apakah stack penuh
    bool penuh() const {
        return jumlahItem >= maksimalKapasitas;
    }
    
    // Dapatkan ukuran stack
    int ukuran() const {
        return jumlahItem;
    }
    
    // Push - Tambah data ke top stack
    bool push(const T& data) {
        if (penuh()) {
            cout << "[STACK PENUH] Tidak dapat menambah data. Max: " 
                 << maksimalKapasitas << endl;
            return false;
        }
        
        StackNode<T>* nodeBaru = new StackNode<T>(data);
        nodeBaru->next = top;
        top = nodeBaru;
        jumlahItem++;
        
        return true;
    }
    
    // Pop - Ambil dan hapus data dari top
    bool pop(T& dataKeluar) {
        if (kosong()) {
            return false;
        }
        
        StackNode<T>* temp = top;
        dataKeluar = top->data;
        top = top->next;
        
        delete temp;
        jumlahItem--;
        
        return true;
    }
    
    // Pop tanpa return data
    bool pop() {
        if (kosong()) {
            return false;
        }
        
        StackNode<T>* temp = top;
        top = top->next;
        
        delete temp;
        jumlahItem--;
        
        return true;
    }
    
    // Peek - Lihat data teratas tanpa menghapus
    bool peek(T& dataTop) const {
        if (kosong()) {
            return false;
        }
        
        dataTop = top->data;
        return true;
    }
    
    // Peek tanpa parameter (return pointer)
    T* peek() {
        if (kosong()) {
            return nullptr;
        }
        
        return &(top->data);
    }
    
    // Kosongkan semua stack
    void clear() {
        while (!kosong()) {
            pop();
        }
    }
    
    // Tampilkan semua isi stack (untuk debugging)
    void tampilkan() const {
        if (kosong()) {
            cout << "[Stack Kosong]" << endl;
            return;
        }
        
        cout << "\n=== Stack (Top to Bottom) ===" << endl;
        cout << "Jumlah Item: " << jumlahItem << "/" << maksimalKapasitas << endl;
        cout << "----------------------------" << endl;
        
        StackNode<T>* current = top;
        int nomor = 1;
        
        while (current != nullptr) {
            cout << nomor++ << ". " << current->data << endl;
            current = current->next;
        }
    }
    
    // Iterasi melalui semua elemen (top ke bottom)
    template<typename Function>
    void iterasi(Function func) const {
        StackNode<T>* current = top;
        
        while (current != nullptr) {
            func(current->data);
            current = current->next;
        }
    }
    
    // Cari data dalam stack (linear search dari top)
    template<typename Predicate>
    T* cari(Predicate kondisi) {
        StackNode<T>* current = top;
        
        while (current != nullptr) {
            if (kondisi(current->data)) {
                return &(current->data);
            }
            current = current->next;
        }
        
        return nullptr;
    }
    
    // Copy semua data ke array (untuk export/save)
    void keArray(T* array, int& panjang) const {
        panjang = 0;
        StackNode<T>* current = top;
        
        while (current != nullptr && panjang < jumlahItem) {
            array[panjang++] = current->data;
            current = current->next;
        }
    }
};

#endif
