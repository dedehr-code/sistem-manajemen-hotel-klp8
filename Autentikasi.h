#ifndef AUTENTIKASI_H
#define AUTENTIKASI_H

#include <iostream>
#include <string>
#include "CircularLinkedList.h"
#include "PengelolaFile.h"
#include "Pengguna.h"
using namespace std;

// ============================================================================
// SISTEM AUTENTIKASI - LOGIN & REGISTER
// ============================================================================
// Mengelola: Login, Register, Session management
// ============================================================================

class SistemAutentikasi {
private:
    CircularLinkedList<Pengguna*> daftarPengguna;
    Pengguna* penggunaAktif;
    string namaFilePengguna;
    int counterIdPelanggan;
    int counterIdPegawai;
    
    // Helper: Cari pengguna by username
    Pengguna* cariPenggunaByUsername(const string& username) {
        Pengguna* hasil = nullptr;
        
        daftarPengguna.iterasi([&](Pengguna* p) {
            if (p->getId() == username || 
                PengelolaFile::toLower(p->getNama()) == PengelolaFile::toLower(username)) {
                hasil = p;
            }
        });
        
        return hasil;
    }
    
    // Helper: Validasi password strength
    bool validasiPasswordStrength(const string& password) {
        if (password.length() < 6) {
            cout << "[ERROR] Password minimal 6 karakter!" << endl;
            return false;
        }
        
        // Password harus ada huruf dan angka
        bool adaHuruf = false;
        bool adaAngka = false;
        
        for (char c : password) {
            if (isalpha(c)) adaHuruf = true;
            if (isdigit(c)) adaAngka = true;
        }
        
        if (!adaHuruf || !adaAngka) {
            cout << "[INFO] Password sebaiknya kombinasi huruf dan angka" << endl;
        }
        
        return true;
    }

public:
    // Constructor
    SistemAutentikasi(const string& namaFile = "pengguna.txt") 
        : penggunaAktif(nullptr), namaFilePengguna(namaFile),
          counterIdPelanggan(1), counterIdPegawai(1) {
        
        // Pastikan file ada
        PengelolaFile::buatFileJikaBelumAda(namaFilePengguna);
    }
    
    // Destructor
    ~SistemAutentikasi() {
        // Hapus semua pointer pengguna
        daftarPengguna.iterasi([](Pengguna* p) {
            delete p;
        });
    }
    
    // Load data pengguna dari file
    bool muatDataPengguna() {
        string baris[1000];
        int jumlah = PengelolaFile::bacaSemuaBaris(namaFilePengguna, baris, 1000);
        
        if (jumlah == 0) {
            cout << "[INFO] File pengguna kosong. Membuat akun pemilik default..." << endl;
            buatAkunPemilikDefault();
            return true;
        }
        
        int maxIdPelanggan = 0;
        int maxIdPegawai = 0;
        
        for (int i = 0; i < jumlah; i++) {
            string fields[15];
            int fieldCount;
            PengelolaFile::split(baris[i], '|', fields, fieldCount);
            
            if (fieldCount < 7) continue;
            
            string role = fields[0];
            string id = fields[1];
            string nama = fields[2];
            string email = fields[3];
            string telp = fields[4];
            string password = fields[5];
            bool aktif = (fields[6] == "1");
            
            Pengguna* pengguna = nullptr;
            
            if (role == "PELANGGAN" && fieldCount >= 10) {
                string alamat = fields[7];
                int totalTransaksi = (fieldCount > 8) ? stoi(fields[8]) : 0;
                long long totalBelanja = (fieldCount > 9) ? stoll(fields[9]) : 0;
                
                Pelanggan* p = new Pelanggan(id, nama, email, telp, password, alamat);
                p->setAktif(aktif);
                // Set stats (tidak ada setter publik, jadi skip untuk sekarang)
                pengguna = p;
                
                // Track max ID
                if (id[0] == 'P' && id.length() > 1) {
                    int num = stoi(id.substr(1));
                    if (num > maxIdPelanggan) maxIdPelanggan = num;
                }
                
            } else if (role == "PEGAWAI" && fieldCount >= 11) {
                string posisi = fields[7];
                string shift = fields[8];
                long long gaji = (fieldCount > 9) ? stoll(fields[9]) : 5000000;
                
                Pegawai* p = new Pegawai(id, nama, email, telp, password, posisi, shift, gaji);
                p->setAktif(aktif);
                pengguna = p;
                
                // Track max ID
                if (id.substr(0, 2) == "PG" && id.length() > 2) {
                    int num = stoi(id.substr(2));
                    if (num > maxIdPegawai) maxIdPegawai = num;
                }
                
            } else if (role == "PEMILIK") {
                pengguna = new Pemilik(id, nama, email, telp, password);
                pengguna->setAktif(aktif);
            }
            
            if (pengguna != nullptr) {
                daftarPengguna.tambah(pengguna);
            }
        }
        
        // Update counter
        counterIdPelanggan = maxIdPelanggan + 1;
        counterIdPegawai = maxIdPegawai + 1;
        
        cout << "[SUKSES] Berhasil memuat " << daftarPengguna.ukuran() << " pengguna" << endl;
        return true;
    }
    
    // Simpan semua data pengguna ke file
    bool simpanDataPengguna() {
        string baris[1000];
        int jumlah = 0;
        
        daftarPengguna.iterasi([&](Pengguna* p) {
            if (jumlah < 1000) {
                baris[jumlah++] = p->toFileFormat();
            }
        });
        
        return PengelolaFile::tulisSemuaBaris(namaFilePengguna, baris, jumlah);
    }
    
    // Buat akun pemilik default jika belum ada
    void buatAkunPemilikDefault() {
        Pemilik* owner = new Pemilik("OWNER", "Administrator", 
                                     "admin@hotelparadise.com", 
                                     "081234567890", "admin123");
        daftarPengguna.tambah(owner);
        simpanDataPengguna();
        
        cout << "\n+------------------------------------------------------------+" << endl;
        cout << "¦          AKUN PEMILIK DEFAULT TELAH DIBUAT                 ¦" << endl;
        cout << "¦------------------------------------------------------------¦" << endl;
        cout << "¦  Username : OWNER                                          ¦" << endl;
        cout << "¦  Password : admin123                                       ¦" << endl;
        cout << "¦  Role     : Pemilik (Full Access)                          ¦" << endl;
        cout << "+------------------------------------------------------------+" << endl;
    }
    
    // Register pelanggan baru
    bool registerPelanggan(const string& nama, const string& email, 
                          const string& telp, const string& password,
                          const string& alamat) {
        
        // Validasi email
        if (!PengelolaFile::validasiEmail(email)) {
            cout << "[ERROR] Format email tidak valid!" << endl;
            return false;
        }
        
        // Validasi telepon
        if (!PengelolaFile::validasiNomorTelepon(telp)) {
            cout << "[ERROR] Format nomor telepon tidak valid!" << endl;
            return false;
        }
        
        // Validasi password
        if (!validasiPasswordStrength(password)) {
            return false;
        }
        
        // Cek email sudah terdaftar
        bool emailSudahAda = false;
        daftarPengguna.iterasi([&](Pengguna* p) {
            if (p->getEmail() == email) {
                emailSudahAda = true;
            }
        });
        
        if (emailSudahAda) {
            cout << "[ERROR] Email sudah terdaftar!" << endl;
            return false;
        }
        
        // Generate ID
        string id = PengelolaFile::generateID("P", counterIdPelanggan++);
        
        // Buat pelanggan baru
        Pelanggan* pelangganBaru = new Pelanggan(id, nama, email, telp, password, alamat);
        daftarPengguna.tambah(pelangganBaru);
        
        // Simpan ke file
        simpanDataPengguna();
        
        cout << "\n+------------------------------------------------------------+" << endl;
        cout << "¦          REGISTRASI BERHASIL!                              ¦" << endl;
        cout << "¦------------------------------------------------------------¦" << endl;
        cout << "¦  ID User  : " << id << string(46 - id.length(), ' ') << "¦" << endl;
        cout << "¦  Nama     : " << nama.substr(0, 46) << string(46 - min((int)nama.length(), 46), ' ') << "¦" << endl;
        cout << "¦  Email    : " << email.substr(0, 46) << string(46 - min((int)email.length(), 46), ' ') << "¦" << endl;
        cout << "+------------------------------------------------------------+" << endl;
        cout << "\nSilakan login dengan email dan password Anda!" << endl;
        
        return true;
    }
    
    // Login
    bool login(const string& emailOrId, const string& password) {
        Pengguna* pengguna = nullptr;
        
        // Cari by email atau ID
        daftarPengguna.iterasi([&](Pengguna* p) {
            if (p->getEmail() == emailOrId || p->getId() == emailOrId) {
                pengguna = p;
            }
        });
        
        if (pengguna == nullptr) {
            cout << "[ERROR] Email/ID tidak ditemukan!" << endl;
            return false;
        }
        
        // Cek aktif
        if (!pengguna->isAktif()) {
            cout << "[ERROR] Akun Anda telah dinonaktifkan!" << endl;
            return false;
        }
        
        // Verifikasi password
        if (!pengguna->verifikasiPassword(password)) {
            cout << "[ERROR] Password salah!" << endl;
            return false;
        }
        
        // Set sebagai pengguna aktif
        penggunaAktif = pengguna;
        
        cout << "\n+------------------------------------------------------------+" << endl;
        cout << "¦          LOGIN BERHASIL!                                   ¦" << endl;
        cout << "¦------------------------------------------------------------¦" << endl;
        cout << "¦  Selamat Datang, " << pengguna->getNama().substr(0, 40) 
             << string(40 - min((int)pengguna->getNama().length(), 40), ' ') << "¦" << endl;
        cout << "¦  Role: " << pengguna->getRoleString() << string(50 - pengguna->getRoleString().length(), ' ') << "¦" << endl;
        cout << "+------------------------------------------------------------+" << endl;
        
        return true;
    }
    
    // Logout
    void logout() {
        if (penggunaAktif != nullptr) {
            cout << "\n[INFO] " << penggunaAktif->getNama() << " telah logout." << endl;
            penggunaAktif = nullptr;
        }
    }
    
    // Getter
    Pengguna* getPenggunaAktif() const { return penggunaAktif; }
    
    bool isLogin() const { return penggunaAktif != nullptr; }
    
    RolePengguna getRoleAktif() const {
        if (penggunaAktif != nullptr) {
            return penggunaAktif->getRole();
        }
        return RolePengguna::PELANGGAN; // Default
    }
    
    // Untuk owner: Tambah pegawai
    bool tambahPegawai(const string& nama, const string& email,
                      const string& telp, const string& password,
                      const string& posisi, const string& shift, double gaji) {
        
        // Validasi
        if (!PengelolaFile::validasiEmail(email)) {
            cout << "[ERROR] Format email tidak valid!" << endl;
            return false;
        }
        
        if (!PengelolaFile::validasiNomorTelepon(telp)) {
            cout << "[ERROR] Format nomor telepon tidak valid!" << endl;
            return false;
        }
        
        // Cek email sudah ada
        bool emailAda = false;
        daftarPengguna.iterasi([&](Pengguna* p) {
            if (p->getEmail() == email) emailAda = true;
        });
        
        if (emailAda) {
            cout << "[ERROR] Email sudah terdaftar!" << endl;
            return false;
        }
        
        // Generate ID
        string id = "PG" + PengelolaFile::generateID("", counterIdPegawai++);
        
        Pegawai* pegawaiBaru = new Pegawai(id, nama, email, telp, password,
                                           posisi, shift, gaji);
        daftarPengguna.tambah(pegawaiBaru);
        simpanDataPengguna();
        
        cout << "[SUKSES] Pegawai berhasil ditambahkan dengan ID: " << id << endl;
        return true;
    }
    
    // Lihat semua pengguna (untuk owner)
    void tampilkanSemuaPengguna() {
        if (daftarPengguna.kosong()) {
            cout << "[INFO] Belum ada pengguna terdaftar." << endl;
            return;
        }
        
        cout << "\n+------------------------------------------------------------+" << endl;
        cout << "¦          DAFTAR SEMUA PENGGUNA                             ¦" << endl;
        cout << "+------------------------------------------------------------+" << endl;
        
        int nomor = 1;
        daftarPengguna.iterasi([&](Pengguna* p) {
            cout << "\n[" << nomor++ << "] " << p->getRoleString() << endl;
            cout << "ID       : " << p->getId() << endl;
            cout << "Nama     : " << p->getNama() << endl;
            cout << "Email    : " << p->getEmail() << endl;
            cout << "Telepon  : " << p->getNomorTelepon() << endl;
            cout << "Status   : " << (p->isAktif() ? "Aktif" : "Non-aktif") << endl;
            cout << "-----------------------------------------------------------" << endl;
        });
    }
    
    // Nonaktifkan pengguna (untuk owner)
    bool nonaktifkanPengguna(const string& id) {
        Pengguna* target = nullptr;
        
        daftarPengguna.iterasi([&](Pengguna* p) {
            if (p->getId() == id) target = p;
        });
        
        if (target == nullptr) {
            cout << "[ERROR] Pengguna dengan ID " << id << " tidak ditemukan!" << endl;
            return false;
        }
        
        target->setAktif(false);
        simpanDataPengguna();
        
        cout << "[SUKSES] Pengguna " << target->getNama() << " telah dinonaktifkan." << endl;
        return true;
    }
    
    // Aktifkan kembali pengguna
    bool aktifkanPengguna(const string& id) {
        Pengguna* target = nullptr;
        
        daftarPengguna.iterasi([&](Pengguna* p) {
            if (p->getId() == id) target = p;
        });
        
        if (target == nullptr) {
            cout << "[ERROR] Pengguna dengan ID " << id << " tidak ditemukan!" << endl;
            return false;
        }
        
        target->setAktif(true);
        simpanDataPengguna();
        
        cout << "[SUKSES] Pengguna " << target->getNama() << " telah diaktifkan kembali." << endl;
        return true;
    }
    
    // Get daftar pengguna (untuk keperluan lain)
    CircularLinkedList<Pengguna*>& getDaftarPengguna() {
        return daftarPengguna;
    }
};

#endif
