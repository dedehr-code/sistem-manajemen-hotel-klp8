#include <iostream>
#include <string>
using namespace std;

// Fase 1 - Struktur Data & Entitas
#include "CircularLinkedList.h"
#include "Stack.h"
#include "Queue.h"
#include "BinarySearchTree.h"
#include "PengelolaFile.h"
#include "Pengguna.h"
#include "Kamar.h"
#include "Layanan.h"
#include "Transaksi.h"

// Fase 2 - Core Systems
#include "Autentikasi.h"
#include "KeranjangBelanja.h"
#include "ManajemenKamar.h"
#include "ManajemenLayanan.h"
#include "SistemPembayaran.h"
#include "DataManager.h"

// Fase 3 - Advanced Features
#include "LaporanKeuangan.h"
#include "EditProfil.h"

// Fase 2-3 - UI Menus
#include "MenuPelanggan.h"
#include "MenuPegawai.h"
#include "MenuPemilik.h"

void tampilkanHeaderUtama() {
    cout << "\n";
    cout << "¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦\n";
    cout << "¦                                                            ¦\n";
    cout << "¦            SISTEM MANAJEMEN HOTEL PARADISE                 ¦\n";
    cout << "¦                    Hotel Bintang 5                         ¦\n";
    cout << "¦                                                            ¦\n";
    cout << "¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦\n";
}

int main() {
    // Inisialisasi semua sistem
    SistemAutentikasi autentikasi;
    ManajemenKamar manajemenKamar;
    ManajemenLayanan manajemenLayanan;
    SistemPembayaran sistemPembayaran;
    DataManager dataManager(&autentikasi, &manajemenKamar, &manajemenLayanan, &sistemPembayaran);
    
    tampilkanHeaderUtama();
    
    // Silent load semua data
    dataManager.muatSemuaData();
    
    int pilihanUtama;
    
    do {
        cout << "\n+------------------------------------------------------------+\n";
        cout << "¦                         MENU UTAMA                         ¦\n";
        cout << "¦------------------------------------------------------------¦\n";
        cout << "¦  1. Login                                                  ¦\n";
        cout << "¦  2. Registrasi (Customer)                                  ¦\n";
        cout << "¦  3. Info Hotel                                             ¦\n";
        cout << "¦  0. Keluar                                                 ¦\n";
        cout << "+------------------------------------------------------------+\n";
        cout << "Pilihan: "; cin >> pilihanUtama; cin.ignore();
        
        if (pilihanUtama == 1) {
            // LOGIN
            cout << "\n+------------------------------------------------------------+\n";
            cout << "¦          LOGIN                                             ¦\n";
            cout << "+------------------------------------------------------------+\n";
            cout << "Email/ID: "; string email; getline(cin, email);
            cout << "Password: "; string pass; getline(cin, pass);
            
            if (autentikasi.login(email, pass)) {
                Pengguna* user = autentikasi.getPenggunaAktif();
                RolePengguna role = user->getRole();
                
                if (role == RolePengguna::PELANGGAN) {
                    Pelanggan* pelanggan = dynamic_cast<Pelanggan*>(user);
                    MenuPelanggan menu(pelanggan, &manajemenKamar, &manajemenLayanan, &sistemPembayaran, &autentikasi);
                    menu.tampilkanMenu();
                    
                } else if (role == RolePengguna::PEGAWAI) {
                    Pegawai* pegawai = dynamic_cast<Pegawai*>(user);
                    MenuPegawai menu(pegawai, &manajemenKamar, &sistemPembayaran, &autentikasi);
                    menu.tampilkanMenu();
                    
                } else if (role == RolePengguna::PEMILIK) {
                    Pemilik* pemilik = dynamic_cast<Pemilik*>(user);
                    MenuPemilik menu(pemilik, &autentikasi, &manajemenKamar, &manajemenLayanan, 
                                     &sistemPembayaran, &dataManager);
                    menu.tampilkanMenu();
                }
                
                autentikasi.logout();
            }
            
        } else if (pilihanUtama == 2) {
            // REGISTRASI
            cout << "\n+------------------------------------------------------------+\n";
            cout << "¦          REGISTRASI CUSTOMER BARU                          ¦\n";
            cout << "+------------------------------------------------------------+\n";
            
            string nama, email, telp, pass, alamat;
            cout << "Nama Lengkap   : "; getline(cin, nama);
            cout << "Email          : "; getline(cin, email);
            cout << "No. Telepon    : "; getline(cin, telp);
            cout << "Password       : "; getline(cin, pass);
            cout << "Alamat         : "; getline(cin, alamat);
            
            autentikasi.registerPelanggan(nama, email, telp, pass, alamat);
            
        } else if (pilihanUtama == 3) {
            // INFO HOTEL
            cout << "\n+------------------------------------------------------------+\n";
            cout << "¦          HOTEL PARADISE - Info                             ¦\n";
            cout << "¦------------------------------------------------------------¦\n";
            cout << "¦  Hotel Bintang 5 dengan fasilitas lengkap                 ¦\n";
            cout << "¦                                                            ¦\n";
            dataManager.tampilkanSummaryData();
            cout << "\n¦  Alamat: Jl. Paradise No. 1, Jakarta                      ¦\n";
            cout << "¦  Telepon: +62 21 1234 5678                                 ¦\n";
            cout << "¦  Email: info@hotelparadise.com                             ¦\n";
            cout << "+------------------------------------------------------------+\n";
            
        } else if (pilihanUtama == 0) {
            // KELUAR
            cout << "\n[SISTEM] Menyimpan data...\n";
            dataManager.simpanSemuaData();
            
            cout << "\n+------------------------------------------------------------+\n";
            cout << "¦          Terima kasih telah menggunakan sistem kami!      ¦\n";
            cout << "¦          Hotel Paradise - RDF Development | @2026      ¦\n";
            cout << "+------------------------------------------------------------+\n";
            
        } else {
            cout << "[ERROR] Pilihan tidak valid!\n";
        }
        
    } while (pilihanUtama != 0);
    
    return 0;
}
