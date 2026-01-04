#ifndef CIRCULAR_LINKED_LIST_H
#define CIRCULAR_LINKED_LIST_H

#include <iostream>
#include <string>
using namespace std;

// ============================================================================
// CIRCULAR LINKED LIST - TEMPLATE GENERIC
// ============================================================================
// Digunakan untuk menyimpan: Pengguna, Kamar, Layanan, Transaksi
// Keunggulan: Akses melingkar, efisien untuk iterasi berulang
// ============================================================================

template<typename T>
struct Node {
    T data;
    Node<T>* next;
    
    Node(const T& nilai) : data(nilai), next(nullptr) {}
};

template<typename T>
class CircularLinkedList {
private:
    Node<T>* head;
    int jumlahNode;

public:
    // Constructor
    CircularLinkedList() : head(nullptr), jumlahNode(0) {}
    
    // Destructor - Mencegah memory leak
    ~CircularLinkedList() {
        if (head == nullptr) return;
        
        Node<T>* current = head;
        Node<T>* temp;
        
        // Putus circular sebelum delete
        Node<T>* tail = head;
        while (tail->next != head) {
            tail = tail->next;
        }
        tail->next = nullptr;
        
        // Delete semua node
        while (current != nullptr) {
            temp = current;
            current = current->next;
            delete temp;
        }
        
        head = nullptr;
        jumlahNode = 0;
    }
    
    // Cek apakah list kosong
    bool kosong() const {
        return head == nullptr;
    }
    
    // Dapatkan jumlah elemen
    int ukuran() const {
        return jumlahNode;
    }
    
    // Tambah data di akhir list
    void tambah(const T& data) {
        Node<T>* nodeBaru = new Node<T>(data);
        
        if (head == nullptr) {
            // List kosong, buat circular ke diri sendiri
            head = nodeBaru;
            nodeBaru->next = head;
        } else {
            // Cari node terakhir
            Node<T>* tail = head;
            while (tail->next != head) {
                tail = tail->next;
            }
            
            // Tambahkan di akhir
            tail->next = nodeBaru;
            nodeBaru->next = head;
        }
        
        jumlahNode++;
    }
    
    // Tambah data di awal list
    void tambahDiAwal(const T& data) {
        Node<T>* nodeBaru = new Node<T>(data);
        
        if (head == nullptr) {
            head = nodeBaru;
            nodeBaru->next = head;
        } else {
            // Cari tail
            Node<T>* tail = head;
            while (tail->next != head) {
                tail = tail->next;
            }
            
            nodeBaru->next = head;
            tail->next = nodeBaru;
            head = nodeBaru;
        }
        
        jumlahNode++;
    }
    
    // Hapus data berdasarkan kondisi (menggunakan function pointer)
    // Contoh: hapus([](const T& item) { return item.getId() == "P001"; })
    template<typename Predicate>
    bool hapus(Predicate kondisi) {
        if (head == nullptr) return false;
        
        Node<T>* current = head;
        Node<T>* previous = nullptr;
        
        // Cari tail untuk handle circular
        Node<T>* tail = head;
        while (tail->next != head) {
            tail = tail->next;
        }
        
        // Cari node yang memenuhi kondisi
        do {
            if (kondisi(current->data)) {
                // Kasus 1: Hanya 1 node
                if (current == head && current->next == head) {
                    delete current;
                    head = nullptr;
                    jumlahNode--;
                    return true;
                }
                
                // Kasus 2: Hapus head
                if (current == head) {
                    tail->next = head->next;
                    Node<T>* temp = head;
                    head = head->next;
                    delete temp;
                    jumlahNode--;
                    return true;
                }
                
                // Kasus 3: Hapus di tengah/akhir
                previous->next = current->next;
                delete current;
                jumlahNode--;
                return true;
            }
            
            previous = current;
            current = current->next;
            
        } while (current != head);
        
        return false; // Tidak ditemukan
    }
    
    // Cari data berdasarkan kondisi, return pointer ke data (atau nullptr)
    template<typename Predicate>
    T* cari(Predicate kondisi) {
        if (head == nullptr) return nullptr;
        
        Node<T>* current = head;
        
        do {
            if (kondisi(current->data)) {
                return &(current->data);
            }
            current = current->next;
        } while (current != head);
        
        return nullptr;
    }
    
    // Cari data berdasarkan kondisi (const version)
    template<typename Predicate>
    const T* cari(Predicate kondisi) const {
        if (head == nullptr) return nullptr;
        
        Node<T>* current = head;
        
        do {
            if (kondisi(current->data)) {
                return &(current->data);
            }
            current = current->next;
        } while (current != head);
        
        return nullptr;
    }
    
    // Dapatkan semua data yang memenuhi kondisi
    template<typename Predicate>
    CircularLinkedList<T> filter(Predicate kondisi) const {
        CircularLinkedList<T> hasil;
        
        if (head == nullptr) return hasil;
        
        Node<T>* current = head;
        
        do {
            if (kondisi(current->data)) {
                hasil.tambah(current->data);
            }
            current = current->next;
        } while (current != head);
        
        return hasil;
    }
    
    // Iterasi melalui semua elemen dengan function
    template<typename Function>
    void iterasi(Function func) {
        if (head == nullptr) return;
        
        Node<T>* current = head;
        
        do {
            func(current->data);
            current = current->next;
        } while (current != head);
    }
    
    // Iterasi const
    template<typename Function>
    void iterasi(Function func) const {
        if (head == nullptr) return;
        
        Node<T>* current = head;
        
        do {
            func(current->data);
            current = current->next;
        } while (current != head);
    }
    
    // Tampilkan semua data (untuk debugging)
    void tampilkan() const {
        if (head == nullptr) {
            cout << "[List Kosong]" << endl;
            return;
        }
        
        Node<T>* current = head;
        int nomor = 1;
        
        cout << "\n=== Daftar Data (Total: " << jumlahNode << ") ===" << endl;
        do {
            cout << nomor++ << ". " << current->data << endl;
            current = current->next;
        } while (current != head);
    }
    
    // Akses data by index (untuk keperluan tertentu)
    T* dapatkanByIndex(int index) {
        if (head == nullptr || index < 0 || index >= jumlahNode) {
            return nullptr;
        }
        
        Node<T>* current = head;
        for (int i = 0; i < index; i++) {
            current = current->next;
        }
        
        return &(current->data);
    }
};

#endif
