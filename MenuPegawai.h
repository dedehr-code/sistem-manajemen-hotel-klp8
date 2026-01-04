#ifndef MENU_PEGAWAI_H
#define MENU_PEGAWAI_H

#include "ManajemenKamar.h"
#include "SistemPembayaran.h"
#include "Pengguna.h"
#include "EditProfil.h"
#include "Autentikasi.h"

class MenuPegawai {
private:
    Pegawai* pegawai;
    ManajemenKamar* manajemenKamar;
    SistemPembayaran* sistemPembayaran;
    EditProfil* editProfil;

public:
    MenuPegawai(Pegawai* p, ManajemenKamar* mk, SistemPembayaran* sp, SistemAutentikasi* auth)
        : pegawai(p), manajemenKamar(mk), sistemPembayaran(sp) {
        editProfil = new EditProfil(auth);
    }
    
    ~MenuPegawai() {
        delete editProfil;
    }
    
    void tampilkanMenu() {
        int pilihan;
        do {
            cout << "\n+------------------------------------------------------------+\n";
            cout << "¦  MENU PEGAWAI - " << pegawai->getNama().substr(0, 42) << string(42-min(42,(int)pegawai->getNama().length()), ' ') << "¦\n";
            cout << "¦  Posisi: " << pegawai->getPosisi().substr(0, 48) << string(48-min(48,(int)pegawai->getPosisi().length()), ' ') << "¦\n";
            cout << "¦------------------------------------------------------------¦\n";
            cout << "¦  1. Lihat Daftar Kamar                                     ¦\n";
            cout << "¦  2. Update Status Kamar                                    ¦\n";
            cout << "¦  3. Lihat Semua Transaksi                                  ¦\n";
            cout << "¦  4. Update Status Transaksi                                ¦\n";
            cout << "¦  5. Statistik Kamar                                        ¦\n";
            cout << "¦  6. Edit Profil Saya                                       ¦\n";
            cout << "¦  0. Logout                                                 ¦\n";
            cout << "+------------------------------------------------------------+\n";
            cout << "Pilihan: "; cin >> pilihan; cin.ignore();
            
            switch(pilihan) {
                case 1: manajemenKamar->tampilkanSemuaKamar(); break;
                case 2: updateStatusKamar(); break;
                case 3: sistemPembayaran->tampilkanSemuaTransaksi(); break;
                case 4: updateStatusTransaksi(); break;
                case 5: manajemenKamar->tampilkanStatistik(); break;
                case 6: editProfil->menuEditProfilPegawai(pegawai); break;
                case 0: cout << "\n[INFO] Logout berhasil!\n"; break;
                default: cout << "[ERROR] Pilihan tidak valid!\n";
            }
        } while(pilihan != 0);
    }

private:
    void updateStatusKamar() {
        cout << "\nNomor kamar: "; string nomor; getline(cin, nomor);
        cout << "Status baru:\n1. TERSEDIA  2. TERISI  3. PERBAIKAN  4. DIBERSIHKAN\nPilih: ";
        int s; cin >> s; cin.ignore();
        
        StatusKamar status = StatusKamar::TERSEDIA;
        if(s==2) status = StatusKamar::TERISI;
        else if(s==3) status = StatusKamar::PERBAIKAN;
        else if(s==4) status = StatusKamar::DIBERSIHKAN;
        
        manajemenKamar->updateStatusKamar(nomor, status);
    }
    
    void updateStatusTransaksi() {
        cout << "\nID Transaksi: "; string id; getline(cin, id);
        cout << "Status baru:\n1. PENDING  2. CONFIRMED  3. COMPLETED  4. CANCELLED\nPilih: ";
        int s; cin >> s; cin.ignore();
        
        StatusTransaksi status = StatusTransaksi::PENDING;
        if(s==2) status = StatusTransaksi::CONFIRMED;
        else if(s==3) status = StatusTransaksi::COMPLETED;
        else if(s==4) status = StatusTransaksi::CANCELLED;
        
        sistemPembayaran->updateStatusTransaksi(id, status);
    }
};

#endif
