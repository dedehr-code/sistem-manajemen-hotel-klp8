#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include <iostream>
#include <string>
#include "Autentikasi.h"
#include "ManajemenKamar.h"
#include "ManajemenLayanan.h"
#include "SistemPembayaran.h"
#include "PengelolaFile.h"
using namespace std;

// ============================================================================
// DATA MANAGER - CENTRAL DATA MANAGEMENT
// ============================================================================
// Mengelola: Load/save semua data, Inisialisasi data default 20+
// ============================================================================

class DataManager {
private:
    SistemAutentikasi* autentikasi;
    ManajemenKamar* manajemenKamar;
    ManajemenLayanan* manajemenLayanan;
    SistemPembayaran* sistemPembayaran;

public:
    // Constructor
    DataManager(SistemAutentikasi* auth, ManajemenKamar* kamar,
                ManajemenLayanan* layanan, SistemPembayaran* pembayaran)
        : autentikasi(auth), manajemenKamar(kamar), 
          manajemenLayanan(layanan), sistemPembayaran(pembayaran) {}
    
    // Load semua data dari file
    bool muatSemuaData() {
        // Silent load - no messages
        bool authLoaded = autentikasi->muatDataPengguna();
        bool kamarLoaded = manajemenKamar->muatDataKamar();
        bool layananLoaded = manajemenLayanan->muatDataLayanan();
        bool transaksiLoaded = sistemPembayaran->muatDataTransaksi();
        
        // Jika data kosong, inisialisasi default (silent)
        if (!kamarLoaded || manajemenKamar->getJumlahKamar() == 0) {
            inisialisasiDataKamarDefault();
        }
        
        if (!layananLoaded || manajemenLayanan->getJumlahLayanan() == 0) {
            inisialisasiDataLayananDefault();
        }
        
        return true;
    }
    
    // Inisialisasi data kamar default (20+ kamar)
    void inisialisasiDataKamarDefault() {
        // Lantai 1 - Standard (10 kamar)
        for (int i = 1; i <= 10; i++) {
            string nomor = string("K1") + (i < 10 ? "0" : "") + to_string(i);
            manajemenKamar->tambahKamar(TipeKamar::STANDARD, nomor, 1, false, false);
        }
        
        // Lantai 2 - Deluxe (8 kamar)
        for (int i = 1; i <= 8; i++) {
            string nomor = string("K2") + (i < 10 ? "0" : "") + to_string(i);
            bool balkon = (i % 2 == 0); // Balkon untuk nomor genap
            manajemenKamar->tambahKamar(TipeKamar::DELUXE, nomor, 2, balkon, false);
        }
        
        // Lantai 3 - Suite (5 kamar)
        for (int i = 1; i <= 5; i++) {
            string nomor = string("K3") + (i < 10 ? "0" : "") + to_string(i);
            bool pemandangan = (i % 2 == 1); // Pemandangan laut untuk nomor ganjil
            manajemenKamar->tambahKamar(TipeKamar::SUITE, nomor, 3, true, pemandangan);
        }
        
        // Lantai 5 - Presidential (2 kamar)
        manajemenKamar->tambahKamar(TipeKamar::PRESIDENTIAL, "K501", 5, true, true);
        manajemenKamar->tambahKamar(TipeKamar::PRESIDENTIAL, "K502", 5, true, true);
        
        // Silent init
    }
    
    // Inisialisasi data layanan default (10+ layanan)
    void inisialisasiDataLayananDefault() {
        // Ballroom (2 jenis)
        manajemenLayanan->tambahLayanan(KategoriLayanan::BALLROOM, 
            "Grand Ballroom", 5000000, 1, 
            "Kapasitas 500 orang - Cocok untuk wedding & seminar besar");
        
        manajemenLayanan->tambahLayanan(KategoriLayanan::BALLROOM, 
            "Meeting Room Premium", 2000000, 1,
            "Kapasitas 100 orang - Untuk meeting & workshop");
        
        // Restaurant (2 jenis)
        manajemenLayanan->tambahLayanan(KategoriLayanan::RESTAURANT, 
            "Fine Dining Prasmanan", 200000, 1,
            "Menu prasmanan internasional - Breakfast, lunch, dinner");
        
        manajemenLayanan->tambahLayanan(KategoriLayanan::RESTAURANT, 
            "A La Carte Premium", 150000, 1,
            "Menu pilihan chef - Makanan kelas dunia");
        
        // Laundry
        manajemenLayanan->tambahLayanan(KategoriLayanan::LAUNDRY, 
            "Laundry Express", 15000, 1,
            "Cuci + setrika + pewangi - Selesai 1 hari");
        
        manajemenLayanan->tambahLayanan(KategoriLayanan::LAUNDRY, 
            "Laundry Regular", 10000, 1,
            "Cuci + setrika - Selesai 2-3 hari");
        
        // Kolam Renang
        manajemenLayanan->tambahLayanan(KategoriLayanan::KOLAM_RENANG, 
            "Pool Access Full Day", 50000, 1,
            "Akses kolam renang seharian - Jam 06:00-22:00");
        
        // Gym & Fitness
        manajemenLayanan->tambahLayanan(KategoriLayanan::GYM, 
            "Gym + Personal Trainer", 150000, 1,
            "1 sesi gym dengan personal trainer - 1.5 jam");
        
        manajemenLayanan->tambahLayanan(KategoriLayanan::GYM, 
            "Gym Access Only", 75000, 1,
            "Akses gym tanpa trainer - Unlimited time");
        
        // Transfer Bandara
        manajemenLayanan->tambahLayanan(KategoriLayanan::TRANSFER_BANDARA, 
            "Airport Shuttle - Sedan", 300000, 1,
            "Antar-jemput bandara dengan sedan - Max 4 pax");
        
        manajemenLayanan->tambahLayanan(KategoriLayanan::TRANSFER_BANDARA, 
            "Airport Shuttle - Van", 500000, 1,
            "Antar-jemput bandara dengan van - Max 8 pax");
        
        // Catering
        manajemenLayanan->tambahLayanan(KategoriLayanan::CATERING, 
            "Catering Premium Package", 150000, 20,
            "Paket catering premium untuk event - Min 20 pax");
        
        manajemenLayanan->tambahLayanan(KategoriLayanan::CATERING, 
            "Catering VIP Package", 250000, 50,
            "Paket catering VIP untuk event besar - Min 50 pax");
        
        // Silent init
    }
    
    // Simpan semua data
    bool simpanSemuaData() {
        cout << "\n[INFO] Menyimpan semua data..." << endl;
        
        bool authSaved = autentikasi->simpanDataPengguna();
        bool kamarSaved = manajemenKamar->simpanDataKamar();
        bool layananSaved = manajemenLayanan->simpanDataLayanan();
        
        if (authSaved && kamarSaved && layananSaved) {
            cout << "[SUKSES] Semua data berhasil disimpan!" << endl;
            return true;
        }
        
        cout << "[ERROR] Gagal menyimpan beberapa data!" << endl;
        return false;
    }
    
    // Tampilkan summary data
    void tampilkanSummaryData() const {
        cout << "\n+------------------------------------------------------------+" << endl;
        cout << "¦          SUMMARY DATA SISTEM                               ¦" << endl;
        cout << "¦------------------------------------------------------------¦" << endl;
        cout << "¦  Total Pengguna  : " << autentikasi->getDaftarPengguna().ukuran() 
             << " user" << string(40, ' ') << "¦" << endl;
        cout << "¦  Total Kamar     : " << manajemenKamar->getJumlahKamar() 
             << " kamar" << string(39, ' ') << "¦" << endl;
        cout << "¦  Total Layanan   : " << manajemenLayanan->getJumlahLayanan() 
             << " layanan" << string(37, ' ') << "¦" << endl;
        cout << "¦  Total Transaksi : " << sistemPembayaran->getDaftarTransaksi().ukuran() 
             << " transaksi" << string(35, ' ') << "¦" << endl;
        cout << "+------------------------------------------------------------+" << endl;
    }
    
    // Reset semua data (DANGER!)
    void resetSemuaData() {
        cout << "\n[WARNING] Anda yakin ingin mereset semua data? (y/n): ";
        string konfirmasi;
        getline(cin, konfirmasi);
        
        if (konfirmasi != "y" && konfirmasi != "Y") {
            cout << "[INFO] Reset dibatalkan." << endl;
            return;
        }
        
        // Hapus semua file
        PengelolaFile::hapusFile("pengguna.txt");
        PengelolaFile::hapusFile("kamar.txt");
        PengelolaFile::hapusFile("layanan.txt");
        PengelolaFile::hapusFile("transaksi.txt");
        
        // Reinisialisasi
        PengelolaFile::buatFileJikaBelumAda("pengguna.txt");
        PengelolaFile::buatFileJikaBelumAda("kamar.txt");
        PengelolaFile::buatFileJikaBelumAda("layanan.txt");
        PengelolaFile::buatFileJikaBelumAda("transaksi.txt");
        
        // Reload default
        autentikasi->buatAkunPemilikDefault();
        inisialisasiDataKamarDefault();
        inisialisasiDataLayananDefault();
        
        cout << "\n[SUKSES] Semua data telah direset ke default!" << endl;
    }
    
    // Backup data (simple copy)
    bool backupData() {
        cout << "\n[INFO] Membuat backup data..." << endl;
        
        // Baca semua file
        string penggunaBackup[1000], kamarBackup[1000], layananBackup[1000];
        int jPengguna = PengelolaFile::bacaSemuaBaris("pengguna.txt", penggunaBackup, 1000);
        int jKamar = PengelolaFile::bacaSemuaBaris("kamar.txt", kamarBackup, 1000);
        int jLayanan = PengelolaFile::bacaSemuaBaris("layanan.txt", layananBackup, 1000);
        
        // Simpan ke file backup
        bool b1 = PengelolaFile::tulisSemuaBaris("pengguna_backup.txt", penggunaBackup, jPengguna);
        bool b2 = PengelolaFile::tulisSemuaBaris("kamar_backup.txt", kamarBackup, jKamar);
        bool b3 = PengelolaFile::tulisSemuaBaris("layanan_backup.txt", layananBackup, jLayanan);
        
        if (b1 && b2 && b3) {
            cout << "[SUKSES] Backup berhasil dibuat!" << endl;
            cout << "  - pengguna_backup.txt" << endl;
            cout << "  - kamar_backup.txt" << endl;
            cout << "  - layanan_backup.txt" << endl;
            return true;
        }
        
        cout << "[ERROR] Gagal membuat backup!" << endl;
        return false;
    }
};

#endif
