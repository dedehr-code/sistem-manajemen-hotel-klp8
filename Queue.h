#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include <string>
using namespace std;

// ============================================================================
// QUEUE - UNTUK ANTRIAN BOOKING
// ============================================================================
// Digunakan untuk: Antrian booking kamar, Waiting list kamar premium
// Prinsip: FIFO (First In First Out)
// ============================================================================

template<typename T>
struct QueueNode {
    T data;
    QueueNode<T>* next;
    
    QueueNode(const T& nilai) : data(nilai), next(nullptr) {}
};

template<typename T>
class Queue {
private:
    QueueNode<T>* front; // Depan antrian (untuk dequeue)
    QueueNode<T>* rear;  // Belakang antrian (untuk enqueue)
    int jumlahItem;
    int maksimalKapasitas;

public:
    // Constructor
    Queue(int maksKapasitas = 50) 
        : front(nullptr), rear(nullptr), jumlahItem(0), 
          maksimalKapasitas(maksKapasitas) {}
    
    // Destructor
    ~Queue() {
        while (!kosong()) {
            dequeue();
        }
    }
    
    // Cek apakah queue kosong
    bool kosong() const {
        return front == nullptr;
    }
    
    // Cek apakah queue penuh
    bool penuh() const {
        return jumlahItem >= maksimalKapasitas;
    }
    
    // Dapatkan ukuran queue
    int ukuran() const {
        return jumlahItem;
    }
    
    // Enqueue - Tambah data ke belakang antrian
    bool enqueue(const T& data) {
        if (penuh()) {
            cout << "[QUEUE PENUH] Tidak dapat menambah ke antrian. Max: " 
                 << maksimalKapasitas << endl;
            return false;
        }
        
        QueueNode<T>* nodeBaru = new QueueNode<T>(data);
        
        if (kosong()) {
            // Queue kosong, front dan rear sama
            front = rear = nodeBaru;
        } else {
            // Tambahkan di belakang
            rear->next = nodeBaru;
            rear = nodeBaru;
        }
        
        jumlahItem++;
        return true;
    }
    
    // Dequeue - Ambil dan hapus data dari depan antrian
    bool dequeue(T& dataKeluar) {
        if (kosong()) {
            return false;
        }
        
        QueueNode<T>* temp = front;
        dataKeluar = front->data;
        
        front = front->next;
        
        // Jika queue jadi kosong, reset rear juga
        if (front == nullptr) {
            rear = nullptr;
        }
        
        delete temp;
        jumlahItem--;
        
        return true;
    }
    
    // Dequeue tanpa return data
    bool dequeue() {
        if (kosong()) {
            return false;
        }
        
        QueueNode<T>* temp = front;
        front = front->next;
        
        if (front == nullptr) {
            rear = nullptr;
        }
        
        delete temp;
        jumlahItem--;
        
        return true;
    }
    
    // Peek - Lihat data terdepan tanpa menghapus
    bool peek(T& dataDepan) const {
        if (kosong()) {
            return false;
        }
        
        dataDepan = front->data;
        return true;
    }
    
    // Peek tanpa parameter (return pointer)
    T* peek() {
        if (kosong()) {
            return nullptr;
        }
        
        return &(front->data);
    }
    
    // Peek rear - Lihat data terbelakang
    T* peekRear() {
        if (kosong()) {
            return nullptr;
        }
        
        return &(rear->data);
    }
    
    // Kosongkan queue
    void clear() {
        while (!kosong()) {
            dequeue();
        }
    }
    
    // Tampilkan semua isi queue
    void tampilkan() const {
        if (kosong()) {
            cout << "[Queue Kosong]" << endl;
            return;
        }
        
        cout << "\n=== Queue (Front to Rear) ===" << endl;
        cout << "Jumlah Antrian: " << jumlahItem << "/" << maksimalKapasitas << endl;
        cout << "----------------------------" << endl;
        
        QueueNode<T>* current = front;
        int nomor = 1;
        
        while (current != nullptr) {
            cout << nomor++ << ". " << current->data << endl;
            current = current->next;
        }
    }
    
    // Iterasi melalui semua elemen (front ke rear)
    template<typename Function>
    void iterasi(Function func) const {
        QueueNode<T>* current = front;
        
        while (current != nullptr) {
            func(current->data);
            current = current->next;
        }
    }
    
    // Cari data dalam queue
    template<typename Predicate>
    T* cari(Predicate kondisi) {
        QueueNode<T>* current = front;
        
        while (current != nullptr) {
            if (kondisi(current->data)) {
                return &(current->data);
            }
            current = current->next;
        }
        
        return nullptr;
    }
    
    // Hapus item tertentu dari queue (TIDAK FIFO, untuk kasus khusus)
    // Misalnya: customer cancel dari waiting list
    template<typename Predicate>
    bool hapusKhusus(Predicate kondisi) {
        if (kosong()) return false;
        
        QueueNode<T>* current = front;
        QueueNode<T>* previous = nullptr;
        
        while (current != nullptr) {
            if (kondisi(current->data)) {
                // Hapus front
                if (current == front) {
                    front = front->next;
                    if (front == nullptr) {
                        rear = nullptr;
                    }
                }
                // Hapus rear
                else if (current == rear) {
                    rear = previous;
                    if (rear != nullptr) {
                        rear->next = nullptr;
                    }
                }
                // Hapus tengah
                else {
                    previous->next = current->next;
                }
                
                delete current;
                jumlahItem--;
                return true;
            }
            
            previous = current;
            current = current->next;
        }
        
        return false;
    }
    
    // Dapatkan posisi dalam antrian (1-based index)
    template<typename Predicate>
    int dapatkanPosisi(Predicate kondisi) const {
        QueueNode<T>* current = front;
        int posisi = 1;
        
        while (current != nullptr) {
            if (kondisi(current->data)) {
                return posisi;
            }
            posisi++;
            current = current->next;
        }
        
        return -1; // Tidak ditemukan
    }
};

#endif
