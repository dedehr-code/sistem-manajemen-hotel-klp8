#ifndef EDIT_PROFIL_H
#define EDIT_PROFIL_H

#include <iostream>
#include <string>
#include "Pengguna.h"
#include "Autentikasi.h"
#include "PengelolaFile.h"
using namespace std;

// ============================================================================
// EDIT PROFIL - UPDATE USER INFORMATION
// ============================================================================
// Allow users to update their profile: nama, email, telepon, password, dll
// ============================================================================

class EditProfil {
private:
    SistemAutentikasi* autentikasi;
    
    // Helper: Konfirmasi password lama
    bool konfirmasiPasswordLama(Pengguna* user) {
        cout << "\n[SECURITY] Masukkan password lama untuk konfirmasi: ";
        string passLama;
        getline(cin, passLama);
        
        if (!user->verifikasiPassword(passLama)) {
            cout << "[ERROR] Password lama salah!" << endl;
            return false;
        }
        
        return true;
    }
    
    // Helper: Validasi password baru
    bool validasiPasswordBaru(const string& passBaru, const string& konfirmasi) {
        if (passBaru.length() < 6) {
            cout << "[ERROR] Password minimal 6 karakter!" << endl;
            return false;
        }
        
        if (passBaru != konfirmasi) {
            cout << "[ERROR] Password dan konfirmasi tidak cocok!" << endl;
            return false;
        }
        
        return true;
    }

public:
    EditProfil(SistemAutentikasi* auth) : autentikasi(auth) {}
    
    // Edit nama
    bool editNama(Pengguna* user) {
        cout << "\n+------------------------------------------------------------+" << endl;
        cout << "¦          EDIT NAMA                                         ¦" << endl;
        cout << "+------------------------------------------------------------+" << endl;
        
        cout << "\nNama sekarang: " << user->getNama() << endl;
        cout << "Nama baru: ";
        string namaBaru;
        getline(cin, namaBaru);
        
        if (namaBaru.empty()) {
            cout << "[INFO] Perubahan dibatalkan." << endl;
            return false;
        }
        
        // Konfirmasi
        cout << "\nUbah nama dari '" << user->getNama() << "' menjadi '" << namaBaru << "'? (y/n): ";
        string konfirmasi;
        getline(cin, konfirmasi);
        
        if (konfirmasi != "y" && konfirmasi != "Y") {
            cout << "[INFO] Perubahan dibatalkan." << endl;
            return false;
        }
        
        user->setNama(namaBaru);
        autentikasi->simpanDataPengguna();
        
        cout << "\n[SUKSES] Nama berhasil diubah!" << endl;
        return true;
    }
    
    // Edit email
    bool editEmail(Pengguna* user) {
        cout << "\n+------------------------------------------------------------+" << endl;
        cout << "¦          EDIT EMAIL                                        ¦" << endl;
        cout << "+------------------------------------------------------------+" << endl;
        
        cout << "\nEmail sekarang: " << user->getEmail() << endl;
        cout << "Email baru: ";
        string emailBaru;
        getline(cin, emailBaru);
        
        // Validasi format
        if (!PengelolaFile::validasiEmail(emailBaru)) {
            cout << "[ERROR] Format email tidak valid!" << endl;
            return false;
        }
        
        // Cek apakah email sudah digunakan user lain
        bool emailSudahAda = false;
        autentikasi->getDaftarPengguna().iterasi([&](Pengguna* p) {
            if (p != user && p->getEmail() == emailBaru) {
                emailSudahAda = true;
            }
        });
        
        if (emailSudahAda) {
            cout << "[ERROR] Email sudah digunakan oleh user lain!" << endl;
            return false;
        }
        
        // Konfirmasi password
        if (!konfirmasiPasswordLama(user)) {
            return false;
        }
        
        user->setEmail(emailBaru);
        autentikasi->simpanDataPengguna();
        
        cout << "\n[SUKSES] Email berhasil diubah!" << endl;
        return true;
    }
    
    // Edit telepon
    bool editTelepon(Pengguna* user) {
        cout << "\n+------------------------------------------------------------+" << endl;
        cout << "¦          EDIT NOMOR TELEPON                                ¦" << endl;
        cout << "+------------------------------------------------------------+" << endl;
        
        cout << "\nTelepon sekarang: " << user->getNomorTelepon() << endl;
        cout << "Telepon baru: ";
        string telpBaru;
        getline(cin, telpBaru);
        
        // Validasi format
        if (!PengelolaFile::validasiNomorTelepon(telpBaru)) {
            cout << "[ERROR] Format nomor telepon tidak valid!" << endl;
            return false;
        }
        
        user->setNomorTelepon(telpBaru);
        autentikasi->simpanDataPengguna();
        
        cout << "\n[SUKSES] Nomor telepon berhasil diubah!" << endl;
        return true;
    }
    
    // Edit password
    bool editPassword(Pengguna* user) {
        cout << "\n+------------------------------------------------------------+" << endl;
        cout << "¦          GANTI PASSWORD                                    ¦" << endl;
        cout << "+------------------------------------------------------------+" << endl;
        
        // Konfirmasi password lama
        if (!konfirmasiPasswordLama(user)) {
            return false;
        }
        
        // Input password baru
        cout << "\nPassword baru (min 6 karakter): ";
        string passBaru;
        getline(cin, passBaru);
        
        cout << "Konfirmasi password baru: ";
        string konfirmasi;
        getline(cin, konfirmasi);
        
        // Validasi
        if (!validasiPasswordBaru(passBaru, konfirmasi)) {
            return false;
        }
        
        user->setPassword(passBaru);
        autentikasi->simpanDataPengguna();
        
        cout << "\n[SUKSES] Password berhasil diubah!" << endl;
        cout << "[INFO] Silakan login ulang dengan password baru." << endl;
        
        return true;
    }
    
    // Edit alamat (khusus Pelanggan)
    bool editAlamat(Pelanggan* pelanggan) {
        cout << "\n+------------------------------------------------------------+" << endl;
        cout << "¦          EDIT ALAMAT                                       ¦" << endl;
        cout << "+------------------------------------------------------------+" << endl;
        
        cout << "\nAlamat sekarang: " << pelanggan->getAlamat() << endl;
        cout << "Alamat baru: ";
        string alamatBaru;
        getline(cin, alamatBaru);
        
        if (alamatBaru.empty()) {
            cout << "[INFO] Perubahan dibatalkan." << endl;
            return false;
        }
        
        pelanggan->setAlamat(alamatBaru);
        autentikasi->simpanDataPengguna();
        
        cout << "\n[SUKSES] Alamat berhasil diubah!" << endl;
        return true;
    }
    
    // Edit posisi (khusus Pegawai)
    bool editPosisi(Pegawai* pegawai) {
        cout << "\n+------------------------------------------------------------+" << endl;
        cout << "¦          EDIT POSISI KERJA                                 ¦" << endl;
        cout << "+------------------------------------------------------------+" << endl;
        
        cout << "\nPosisi sekarang: " << pegawai->getPosisi() << endl;
        cout << "Posisi baru: ";
        string posisiBaru;
        getline(cin, posisiBaru);
        
        if (posisiBaru.empty()) {
            cout << "[INFO] Perubahan dibatalkan." << endl;
            return false;
        }
        
        pegawai->setPosisi(posisiBaru);
        autentikasi->simpanDataPengguna();
        
        cout << "\n[SUKSES] Posisi berhasil diubah!" << endl;
        return true;
    }
    
    // Menu edit profil untuk Pelanggan
    void menuEditProfilPelanggan(Pelanggan* pelanggan) {
        int pilihan;
        bool logout = false;
        
        do {
            cout << "\n+------------------------------------------------------------+" << endl;
            cout << "¦          EDIT PROFIL - " << pelanggan->getNama().substr(0, 38) 
                 << string(38 - min(38, (int)pelanggan->getNama().length()), ' ') << "¦" << endl;
            cout << "¦------------------------------------------------------------¦" << endl;
            cout << "¦  1. Edit Nama                                              ¦" << endl;
            cout << "¦  2. Edit Email                                             ¦" << endl;
            cout << "¦  3. Edit Nomor Telepon                                     ¦" << endl;
            cout << "¦  4. Edit Alamat                                            ¦" << endl;
            cout << "¦  5. Ganti Password                                         ¦" << endl;
            cout << "¦  0. Kembali                                                ¦" << endl;
            cout << "+------------------------------------------------------------+" << endl;
            cout << "Pilihan: "; cin >> pilihan; cin.ignore();
            
            switch(pilihan) {
                case 1: editNama(pelanggan); break;
                case 2: editEmail(pelanggan); break;
                case 3: editTelepon(pelanggan); break;
                case 4: editAlamat(pelanggan); break;
                case 5: 
                    if (editPassword(pelanggan)) {
                        logout = true; // Force logout setelah ganti password
                    }
                    break;
                case 0: cout << "\n[INFO] Kembali ke menu utama.\n"; break;
                default: cout << "[ERROR] Pilihan tidak valid!\n";
            }
            
            if (logout) {
                cout << "\n[INFO] Silakan login ulang dengan password baru.\n";
                pilihan = 0; // Exit loop
            }
            
        } while(pilihan != 0);
    }
    
    // Menu edit profil untuk Pegawai
    void menuEditProfilPegawai(Pegawai* pegawai) {
        int pilihan;
        bool logout = false;
        
        do {
            cout << "\n+------------------------------------------------------------+" << endl;
            cout << "¦          EDIT PROFIL - " << pegawai->getNama().substr(0, 38) 
                 << string(38 - min(38, (int)pegawai->getNama().length()), ' ') << "¦" << endl;
            cout << "¦------------------------------------------------------------¦" << endl;
            cout << "¦  1. Edit Nama                                              ¦" << endl;
            cout << "¦  2. Edit Email                                             ¦" << endl;
            cout << "¦  3. Edit Nomor Telepon                                     ¦" << endl;
            cout << "¦  4. Edit Posisi                                            ¦" << endl;
            cout << "¦  5. Ganti Password                                         ¦" << endl;
            cout << "¦  0. Kembali                                                ¦" << endl;
            cout << "+------------------------------------------------------------+" << endl;
            cout << "Pilihan: "; cin >> pilihan; cin.ignore();
            
            switch(pilihan) {
                case 1: editNama(pegawai); break;
                case 2: editEmail(pegawai); break;
                case 3: editTelepon(pegawai); break;
                case 4: editPosisi(pegawai); break;
                case 5: 
                    if (editPassword(pegawai)) {
                        logout = true;
                    }
                    break;
                case 0: cout << "\n[INFO] Kembali ke menu utama.\n"; break;
                default: cout << "[ERROR] Pilihan tidak valid!\n";
            }
            
            if (logout) {
                cout << "\n[INFO] Silakan login ulang dengan password baru.\n";
                pilihan = 0;
            }
            
        } while(pilihan != 0);
    }
    
    // Menu edit profil untuk Pemilik
    void menuEditProfilPemilik(Pemilik* pemilik) {
        int pilihan;
        bool logout = false;
        
        do {
            cout << "\n+------------------------------------------------------------+" << endl;
            cout << "¦          EDIT PROFIL - OWNER                               ¦" << endl;
            cout << "¦------------------------------------------------------------¦" << endl;
            cout << "¦  1. Edit Nama                                              ¦" << endl;
            cout << "¦  2. Edit Email                                             ¦" << endl;
            cout << "¦  3. Edit Nomor Telepon                                     ¦" << endl;
            cout << "¦  4. Ganti Password                                         ¦" << endl;
            cout << "¦  0. Kembali                                                ¦" << endl;
            cout << "+------------------------------------------------------------+" << endl;
            cout << "Pilihan: "; cin >> pilihan; cin.ignore();
            
            switch(pilihan) {
                case 1: editNama(pemilik); break;
                case 2: editEmail(pemilik); break;
                case 3: editTelepon(pemilik); break;
                case 4: 
                    if (editPassword(pemilik)) {
                        logout = true;
                    }
                    break;
                case 0: cout << "\n[INFO] Kembali ke menu utama.\n"; break;
                default: cout << "[ERROR] Pilihan tidak valid!\n";
            }
            
            if (logout) {
                cout << "\n[INFO] Silakan login ulang dengan password baru.\n";
                pilihan = 0;
            }
            
        } while(pilihan != 0);
    }
};

#endif
