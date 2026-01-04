#ifndef MANAJEMEN_KAMAR_H
#define MANAJEMEN_KAMAR_H

#include <iostream>
#include <string>
#include "CircularLinkedList.h"
#include "BinarySearchTree.h"
#include "PengelolaFile.h"
#include "Kamar.h"
using namespace std;

// ============================================================================
// MANAJEMEN KAMAR - CRUD & SEARCH
// ============================================================================
// Mengelola semua operasi kamar: Create, Read, Update, Delete, Search
// Menggunakan BST untuk pencarian cepat berdasarkan nomor kamar
// ============================================================================

class ManajemenKamar {
private:
    CircularLinkedList<Kamar*> daftarKamar;
    BinarySearchTree<Kamar*, string> indexKamar; // BST untuk search by nomor
    string namaFileKamar;
    
    // Helper: Rebuild BST index
    void rebuildIndex() {
        indexKamar.clear();
        daftarKamar.iterasi([&](Kamar* k) {
            indexKamar.insert(k, k->getNomorKamar());
        });
    }

public:
    // Constructor
    ManajemenKamar(const string& namaFile = "kamar.txt") 
        : namaFileKamar(namaFile) {
        PengelolaFile::buatFileJikaBelumAda(namaFileKamar);
    }
    
    // Destructor
    ~ManajemenKamar() {
        daftarKamar.iterasi([](Kamar* k) {
            delete k;
        });
    }
    
    // Load data kamar dari file
    bool muatDataKamar() {
        string baris[1000];
        int jumlah = PengelolaFile::bacaSemuaBaris(namaFileKamar, baris, 1000);
        
        if (jumlah == 0) {
            cout << "[INFO] File kamar kosong. Membuat data default..." << endl;
            return false;
        }
        
        for (int i = 0; i < jumlah; i++) {
            string fields[15];
            int fieldCount;
            PengelolaFile::split(baris[i], '|', fields, fieldCount);
            
            if (fieldCount < 9) continue;
            
            string tipeStr = fields[0];
            string nomor = fields[1];
            string statusStr = fields[2];
            double harga = stod(fields[3]);
            int lantai = stoi(fields[4]);
            int kapasitas = stoi(fields[5]);
            bool balkon = (fields[6] == "1");
            bool pemandangan = (fields[7] == "1");
            
            TipeKamar tipe = Kamar::stringKeTipe(tipeStr);
            StatusKamar status = Kamar::stringKeStatus(statusStr);
            
            Kamar* kamar = nullptr;
            
            // Buat kamar sesuai tipe
            switch (tipe) {
                case TipeKamar::STANDARD:
                    kamar = new KamarStandard(nomor, lantai);
                    break;
                case TipeKamar::DELUXE:
                    kamar = new KamarDeluxe(nomor, lantai, balkon);
                    break;
                case TipeKamar::SUITE:
                    kamar = new KamarSuite(nomor, lantai, pemandangan);
                    break;
                case TipeKamar::PRESIDENTIAL:
                    kamar = new KamarPresidential(nomor, lantai);
                    break;
            }
            
            if (kamar != nullptr) {
                kamar->setStatus(status);
                kamar->setHargaPerMalam(harga); // Override jika harga custom
                daftarKamar.tambah(kamar);
            }
        }
        
        // Build index untuk search
        rebuildIndex();
        
        cout << "[SUKSES] Berhasil memuat " << daftarKamar.ukuran() << " kamar" << endl;
        return true;
    }
    
    // Simpan semua kamar ke file
    bool simpanDataKamar() {
        string baris[1000];
        int jumlah = 0;
        
        daftarKamar.iterasi([&](Kamar* k) {
            if (jumlah < 1000) {
                baris[jumlah++] = k->toFileFormat();
            }
        });
        
        return PengelolaFile::tulisSemuaBaris(namaFileKamar, baris, jumlah);
    }
    
    // Tambah kamar baru
    bool tambahKamar(TipeKamar tipe, const string& nomor, int lantai,
                    bool balkon = false, bool pemandangan = false) {
        
        // Cek nomor sudah ada
        if (cariKamar(nomor) != nullptr) {
            cout << "[ERROR] Nomor kamar " << nomor << " sudah ada!" << endl;
            return false;
        }
        
        Kamar* kamarBaru = nullptr;
        
        switch (tipe) {
            case TipeKamar::STANDARD:
                kamarBaru = new KamarStandard(nomor, lantai);
                break;
            case TipeKamar::DELUXE:
                kamarBaru = new KamarDeluxe(nomor, lantai, balkon);
                break;
            case TipeKamar::SUITE:
                kamarBaru = new KamarSuite(nomor, lantai, pemandangan);
                break;
            case TipeKamar::PRESIDENTIAL:
                kamarBaru = new KamarPresidential(nomor, lantai);
                break;
        }
        
        if (kamarBaru == nullptr) {
            cout << "[ERROR] Gagal membuat kamar!" << endl;
            return false;
        }
        
        daftarKamar.tambah(kamarBaru);
        indexKamar.insert(kamarBaru, nomor);
        simpanDataKamar();
        
        cout << "[SUKSES] Kamar " << nomor << " (" << kamarBaru->getTipeString() 
             << ") berhasil ditambahkan!" << endl;
        
        return true;
    }
    
    // Cari kamar by nomor (menggunakan BST - O(log n))
    Kamar* cariKamar(const string& nomor) {
        Kamar** found = indexKamar.search(nomor);
        if (found != nullptr) {
            return *found;
        }
        return nullptr;
    }
    
    // Update status kamar
    bool updateStatusKamar(const string& nomor, StatusKamar statusBaru) {
        Kamar* kamar = cariKamar(nomor);
        
        if (kamar == nullptr) {
            cout << "[ERROR] Kamar " << nomor << " tidak ditemukan!" << endl;
            return false;
        }
        
        kamar->setStatus(statusBaru);
        simpanDataKamar();
        
        cout << "[SUKSES] Status kamar " << nomor << " diubah menjadi: " 
             << kamar->getStatusString() << endl;
        
        return true;
    }
    
    // Update harga kamar
    bool updateHargaKamar(const string& nomor, double hargaBaru) {
        Kamar* kamar = cariKamar(nomor);
        
        if (kamar == nullptr) {
            cout << "[ERROR] Kamar " << nomor << " tidak ditemukan!" << endl;
            return false;
        }
        
        double hargaLama = kamar->getHargaPerMalam();
        kamar->setHargaPerMalam(hargaBaru);
        simpanDataKamar();
        
        cout << "[SUKSES] Harga kamar " << nomor << " diubah dari " 
             << PengelolaFile::formatRupiah(hargaLama) << " menjadi " 
             << PengelolaFile::formatRupiah(hargaBaru) << endl;
        
        return true;
    }
    
    // Hapus kamar
    bool hapusKamar(const string& nomor) {
        Kamar* kamar = cariKamar(nomor);
        
        if (kamar == nullptr) {
            cout << "[ERROR] Kamar " << nomor << " tidak ditemukan!" << endl;
            return false;
        }
        
        // Cek status - jangan hapus jika sedang terisi
        if (kamar->getStatus() == StatusKamar::TERISI) {
            cout << "[ERROR] Tidak bisa menghapus kamar yang sedang terisi!" << endl;
            return false;
        }
        
        // Hapus dari list
        bool berhasil = daftarKamar.hapus([&](Kamar* k) {
            return k->getNomorKamar() == nomor;
        });
        
        if (berhasil) {
            delete kamar;
            indexKamar.hapus(nomor);
            simpanDataKamar();
            cout << "[SUKSES] Kamar " << nomor << " berhasil dihapus!" << endl;
            return true;
        }
        
        return false;
    }
    
    // Tampilkan semua kamar
    void tampilkanSemuaKamar() const {
        if (daftarKamar.kosong()) {
            cout << "[INFO] Belum ada kamar terdaftar." << endl;
            return;
        }
        
        cout << "\n+------------------------------------------------------------+" << endl;
        cout << "¦          DAFTAR SEMUA KAMAR HOTEL                          ¦" << endl;
        cout << "+------------------------------------------------------------+" << endl;
        
        int nomor = 1;
        daftarKamar.iterasi([&](Kamar* k) {
            cout << "\n[" << nomor++ << "] " << k->getNomorKamar() 
                 << " - " << k->getTipeString() << endl;
            cout << "Status      : " << k->getStatusString() << endl;
            cout << "Harga/Malam : " << PengelolaFile::formatRupiahLengkap(k->getHargaPerMalam()) << endl;
            cout << "Lantai      : " << k->getLantai() << endl;
            cout << "Kapasitas   : " << k->getKapasitasOrang() << " orang" << endl;
            cout << "Fasilitas   : " << k->getFasilitas().substr(0, 50);
            if (k->getFasilitas().length() > 50) cout << "...";
            cout << endl;
            cout << "-----------------------------------------------------------" << endl;
        });
    }
    
    // Tampilkan kamar tersedia saja
    void tampilkanKamarTersedia() const {
        int jumlahTersedia = 0;
        
        cout << "\n+------------------------------------------------------------+" << endl;
        cout << "¦          KAMAR TERSEDIA                                    ¦" << endl;
        cout << "+------------------------------------------------------------+" << endl;
        
        daftarKamar.iterasi([&](Kamar* k) {
            if (k->getStatus() == StatusKamar::TERSEDIA) {
                jumlahTersedia++;
                cout << "\n[" << jumlahTersedia << "] " << k->getNomorKamar() 
                     << " - " << k->getTipeString() << endl;
                cout << "Harga/Malam : " << PengelolaFile::formatRupiahLengkap(k->getHargaPerMalam()) << endl;
                cout << "Lantai " << k->getLantai() << " | Kapasitas " << k->getKapasitasOrang() << " orang";
                if (k->hasBalkon()) cout << " | Balkon";
                if (k->hasPemandanganLaut()) cout << " | View Laut";
                cout << endl;
                cout << "Fasilitas: " << k->getFasilitas().substr(0, 55);
                if (k->getFasilitas().length() > 55) cout << "...";
                cout << endl;
                cout << "-----------------------------------------------------------" << endl;
            }
        });
        
        if (jumlahTersedia == 0) {
            cout << "\n[INFO] Tidak ada kamar tersedia saat ini." << endl;
        } else {
            cout << "\nTotal: " << jumlahTersedia << " kamar tersedia" << endl;
        }
    }
    
    // Filter kamar by tipe
    void tampilkanKamarByTipe(TipeKamar tipe) const {
        int jumlah = 0;
        
        cout << "\n+------------------------------------------------------------+" << endl;
        
        string judulTipe = "";
        switch (tipe) {
            case TipeKamar::STANDARD: judulTipe = "STANDARD"; break;
            case TipeKamar::DELUXE: judulTipe = "DELUXE"; break;
            case TipeKamar::SUITE: judulTipe = "SUITE"; break;
            case TipeKamar::PRESIDENTIAL: judulTipe = "PRESIDENTIAL"; break;
        }
        
        cout << "¦          KAMAR TIPE " << judulTipe 
             << string(36 - judulTipe.length(), ' ') << "¦" << endl;
        cout << "+------------------------------------------------------------+" << endl;
        
        daftarKamar.iterasi([&](Kamar* k) {
            if (k->getTipe() == tipe) {
                jumlah++;
                cout << "\n" << k->getNomorKamar() << " - " << k->getStatusString() << endl;
                cout << "Harga: " << PengelolaFile::formatRupiahLengkap(k->getHargaPerMalam()) 
                     << " | Lantai " << k->getLantai() << endl;
            }
        });
        
        if (jumlah == 0) {
            cout << "\n[INFO] Tidak ada kamar tipe " << judulTipe << endl;
        } else {
            cout << "\nTotal: " << jumlah << " kamar" << endl;
        }
    }
    
    // Get statistics
    void tampilkanStatistik() const {
        int totalKamar = daftarKamar.ukuran();
        int tersedia = 0, terisi = 0, perbaikan = 0, dibersihkan = 0;
        
        daftarKamar.iterasi([&](Kamar* k) {
            switch (k->getStatus()) {
                case StatusKamar::TERSEDIA: tersedia++; break;
                case StatusKamar::TERISI: terisi++; break;
                case StatusKamar::PERBAIKAN: perbaikan++; break;
                case StatusKamar::DIBERSIHKAN: dibersihkan++; break;
            }
        });
        
        cout << "\n+------------------------------------------------------------+" << endl;
        cout << "¦          STATISTIK KAMAR                                   ¦" << endl;
        cout << "¦------------------------------------------------------------¦" << endl;
        cout << "¦  Total Kamar    : " << totalKamar << string(40, ' ') << "¦" << endl;
        cout << "¦  Tersedia       : " << tersedia << string(40, ' ') << "¦" << endl;
        cout << "¦  Terisi         : " << terisi << string(40, ' ') << "¦" << endl;
        cout << "¦  Perbaikan      : " << perbaikan << string(40, ' ') << "¦" << endl;
        cout << "¦  Dibersihkan    : " << dibersihkan << string(40, ' ') << "¦" << endl;
        cout << "¦------------------------------------------------------------¦" << endl;
        
        if (totalKamar > 0) {
            double occupancy = (double)terisi / totalKamar * 100.0;
            cout << "¦  Occupancy Rate : " << (int)occupancy << "%" 
                 << string(37, ' ') << "¦" << endl;
        }
        
        cout << "+------------------------------------------------------------+" << endl;
    }
    
    // Get daftar kamar (untuk keperluan lain)
    CircularLinkedList<Kamar*>& getDaftarKamar() {
        return daftarKamar;
    }
    
    // Get jumlah kamar
    int getJumlahKamar() const {
        return daftarKamar.ukuran();
    }
};

#endif
