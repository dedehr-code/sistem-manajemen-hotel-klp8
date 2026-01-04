#ifndef MENU_PEMILIK_H
#define MENU_PEMILIK_H

#include "Autentikasi.h"
#include "ManajemenKamar.h"
#include "ManajemenLayanan.h"
#include "SistemPembayaran.h"
#include "DataManager.h"
#include "Pengguna.h"
#include "LaporanKeuangan.h"
#include "EditProfil.h"

class MenuPemilik {
private:
    Pemilik* pemilik;
    SistemAutentikasi* autentikasi;
    ManajemenKamar* manajemenKamar;
    ManajemenLayanan* manajemenLayanan;
    SistemPembayaran* sistemPembayaran;
    DataManager* dataManager;
    LaporanKeuangan* laporanKeuangan;
    EditProfil* editProfil;

public:
    MenuPemilik(Pemilik* p, SistemAutentikasi* auth, ManajemenKamar* mk, 
                ManajemenLayanan* ml, SistemPembayaran* sp, DataManager* dm)
        : pemilik(p), autentikasi(auth), manajemenKamar(mk), 
          manajemenLayanan(ml), sistemPembayaran(sp), dataManager(dm) {
        laporanKeuangan = new LaporanKeuangan(sp);
        editProfil = new EditProfil(auth);
    }
    
    ~MenuPemilik() {
        delete laporanKeuangan;
        delete editProfil;
    }
    
    void tampilkanMenu() {
        int pilihan;
        do {
            cout << "\n+------------------------------------------------------------+\n";
            cout << "¦  MENU PEMILIK (OWNER) - Full Access                       ¦\n";
            cout << "¦------------------------------------------------------------¦\n";
            cout << "¦  1. Manajemen Kamar        7. Lihat Semua Transaksi       ¦\n";
            cout << "¦  2. Manajemen Layanan      8. Summary Data                ¦\n";
            cout << "¦  3. Manajemen Pengguna     9. Backup Data                 ¦\n";
            cout << "¦  4. Tambah Pegawai         10. Laporan Keuangan Detail    ¦\n";
            cout << "¦  5. Statistik Kamar        11. Edit Profil Saya           ¦\n";
            cout << "¦  6. Statistik Keuangan     0. Logout                      ¦\n";
            cout << "+------------------------------------------------------------+\n";
            cout << "Pilihan: "; cin >> pilihan; cin.ignore();
            
            switch(pilihan) {
                case 1: menuManajemenKamar(); break;
                case 2: menuManajemenLayanan(); break;
                case 3: autentikasi->tampilkanSemuaPengguna(); break;
                case 4: tambahPegawai(); break;
                case 5: manajemenKamar->tampilkanStatistik(); break;
                case 6: sistemPembayaran->tampilkanStatistikKeuangan(); break;
                case 7: sistemPembayaran->tampilkanSemuaTransaksi(); break;
                case 8: dataManager->tampilkanSummaryData(); break;
                case 9: dataManager->backupData(); break;
                case 10: laporanKeuangan->menuLaporan(); break;
                case 11: editProfil->menuEditProfilPemilik(pemilik); break;
                case 0: cout << "\n[INFO] Logout berhasil!\n"; break;
                default: cout << "[ERROR] Pilihan tidak valid!\n";
            }
        } while(pilihan != 0);
    }

private:
    void menuManajemenKamar() {
        int p;
        cout << "\n=== MANAJEMEN KAMAR ===\n";
        cout << "1. Lihat Semua  2. Tambah  3. Hapus  4. Update Harga  0. Kembali\nPilih: ";
        cin >> p; cin.ignore();
        
        if(p==1) manajemenKamar->tampilkanSemuaKamar();
        else if(p==2) {
            cout << "Nomor: "; string no; getline(cin, no);
            cout << "Tipe (1.Standard 2.Deluxe 3.Suite 4.Presidential): "; int t; cin >> t; cin.ignore();
            cout << "Lantai: "; int l; cin >> l; cin.ignore();
            TipeKamar tipe = (TipeKamar)(t-1);
            manajemenKamar->tambahKamar(tipe, no, l);
        } else if(p==3) {
            cout << "Nomor kamar: "; string no; getline(cin, no);
            manajemenKamar->hapusKamar(no);
        } else if(p==4) {
            cout << "Nomor: "; string no; getline(cin, no);
            cout << "Harga baru: "; double h; cin >> h; cin.ignore();
            manajemenKamar->updateHargaKamar(no, h);
        }
    }
    
    void menuManajemenLayanan() {
        int p;
        cout << "\n=== MANAJEMEN LAYANAN ===\n";
        cout << "1. Lihat Semua  2. Tambah  3. Hapus  4. Update Harga  0. Kembali\nPilih: ";
        cin >> p; cin.ignore();
        
        if(p==1) manajemenLayanan->tampilkanSemuaLayanan();
        else if(p==2) {
            cout << "Nama: "; string nama; getline(cin, nama);
            cout << "Kategori (0-6): "; int k; cin >> k; cin.ignore();
            cout << "Harga: "; double h; cin >> h; cin.ignore();
            manajemenLayanan->tambahLayanan((KategoriLayanan)k, nama, h);
        } else if(p==3) {
            cout << "ID Layanan: "; string id; getline(cin, id);
            manajemenLayanan->hapusLayanan(id);
        } else if(p==4) {
            cout << "ID: "; string id; getline(cin, id);
            cout << "Harga baru: "; double h; cin >> h; cin.ignore();
            manajemenLayanan->updateHargaLayanan(id, h);
        }
    }
    
    void tambahPegawai() {
        cout << "\n=== TAMBAH PEGAWAI ===\n";
        cout << "Nama: "; string nama; getline(cin, nama);
        cout << "Email: "; string email; getline(cin, email);
        cout << "Telepon: "; string telp; getline(cin, telp);
        cout << "Password: "; string pass; getline(cin, pass);
        cout << "Posisi: "; string posisi; getline(cin, posisi);
        cout << "Shift (Pagi/Siang/Malam): "; string shift; getline(cin, shift);
        cout << "Gaji: "; double gaji; cin >> gaji; cin.ignore();
        
        autentikasi->tambahPegawai(nama, email, telp, pass, posisi, shift, gaji);
    }
};

#endif
