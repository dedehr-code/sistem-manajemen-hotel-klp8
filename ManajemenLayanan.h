#ifndef MANAJEMEN_LAYANAN_H
#define MANAJEMEN_LAYANAN_H

#include <iostream>
#include <string>
#include "CircularLinkedList.h"
#include "BinarySearchTree.h"
#include "PengelolaFile.h"
#include "Layanan.h"
using namespace std;

// ============================================================================
// MANAJEMEN LAYANAN - CRUD & SEARCH
// ============================================================================
// Mengelola semua layanan hotel: Ballroom, Restaurant, Laundry, dll
// ============================================================================

class ManajemenLayanan {
private:
    CircularLinkedList<Layanan*> daftarLayanan;
    BinarySearchTree<Layanan*, string> indexLayanan; // BST untuk search by ID
    string namaFileLayanan;
    int counterID;
    
    // Helper: Rebuild BST index
    void rebuildIndex() {
        indexLayanan.clear();
        daftarLayanan.iterasi([&](Layanan* l) {
            indexLayanan.insert(l, l->getIdLayanan());
        });
    }

public:
    // Constructor
    ManajemenLayanan(const string& namaFile = "layanan.txt") 
        : namaFileLayanan(namaFile), counterID(1) {
        PengelolaFile::buatFileJikaBelumAda(namaFileLayanan);
    }
    
    // Destructor
    ~ManajemenLayanan() {
        daftarLayanan.iterasi([](Layanan* l) {
            delete l;
        });
    }
    
    // Load data layanan dari file
    bool muatDataLayanan() {
        string baris[1000];
        int jumlah = PengelolaFile::bacaSemuaBaris(namaFileLayanan, baris, 1000);
        
        if (jumlah == 0) {
            cout << "[INFO] File layanan kosong. Membuat data default..." << endl;
            return false;
        }
        
        int maxIdNum = 0;
        
        for (int i = 0; i < jumlah; i++) {
            string fields[10];
            int fieldCount;
            PengelolaFile::split(baris[i], '|', fields, fieldCount);
            
            if (fieldCount < 8) continue;
            
            string kategoriStr = fields[0];
            string id = fields[1];
            string nama = fields[2];
            double harga = stod(fields[3]);
            string satuanStr = fields[4];
            bool tersedia = (fields[5] == "1");
            int minOrder = stoi(fields[6]);
            string deskripsi = fields[7];
            
            KategoriLayanan kategori = Layanan::stringKeKategori(kategoriStr);
            SatuanHarga satuan = Layanan::stringKeSatuan(satuanStr);
            
            Layanan* layanan = nullptr;
            
            // Buat layanan sesuai kategori
            switch (kategori) {
                case KategoriLayanan::BALLROOM:
                    layanan = new LayananBallroom(id, nama, harga, 500);
                    break;
                case KategoriLayanan::RESTAURANT:
                    layanan = new LayananRestaurant(id, nama, harga);
                    break;
                case KategoriLayanan::LAUNDRY:
                    layanan = new LayananLaundry(id, harga);
                    break;
                case KategoriLayanan::KOLAM_RENANG:
                    layanan = new LayananKolamRenang(id, harga);
                    break;
                case KategoriLayanan::GYM:
                    layanan = new LayananGym(id, harga);
                    break;
                case KategoriLayanan::TRANSFER_BANDARA:
                    layanan = new LayananTransferBandara(id, harga);
                    break;
                case KategoriLayanan::CATERING:
                    layanan = new LayananCatering(id, harga, "Paket Premium", minOrder);
                    break;
                default:
                    layanan = new Layanan(id, nama, kategori, harga, satuan, deskripsi, minOrder);
                    break;
            }
            
            if (layanan != nullptr) {
                layanan->setTersedia(tersedia);
                layanan->setDeskripsi(deskripsi);
                layanan->setMinimumOrder(minOrder);
                daftarLayanan.tambah(layanan);
                
                // Track max ID
                if (id[0] == 'L' && id.length() > 1) {
                    int num = stoi(id.substr(1));
                    if (num > maxIdNum) maxIdNum = num;
                }
            }
        }
        
        counterID = maxIdNum + 1;
        rebuildIndex();
        
        cout << "[SUKSES] Berhasil memuat " << daftarLayanan.ukuran() << " layanan" << endl;
        return true;
    }
    
    // Simpan semua layanan ke file
    bool simpanDataLayanan() {
        string baris[1000];
        int jumlah = 0;
        
        daftarLayanan.iterasi([&](Layanan* l) {
            if (jumlah < 1000) {
                baris[jumlah++] = l->toFileFormat();
            }
        });
        
        return PengelolaFile::tulisSemuaBaris(namaFileLayanan, baris, jumlah);
    }
    
    // Tambah layanan baru (generic)
    bool tambahLayanan(KategoriLayanan kategori, const string& nama, 
                      double harga, int minOrder = 1, 
                      const string& deskripsi = "") {
        
        string id = "L" + PengelolaFile::generateID("", counterID++);
        
        Layanan* layananBaru = nullptr;
        SatuanHarga satuan = SatuanHarga::PER_EVENT;
        
        // Tentukan satuan berdasarkan kategori
        switch (kategori) {
            case KategoriLayanan::BALLROOM:
                layananBaru = new LayananBallroom(id, nama, harga, 500);
                break;
            case KategoriLayanan::RESTAURANT:
                layananBaru = new LayananRestaurant(id, nama, harga);
                break;
            case KategoriLayanan::LAUNDRY:
                layananBaru = new LayananLaundry(id, harga);
                break;
            case KategoriLayanan::KOLAM_RENANG:
                layananBaru = new LayananKolamRenang(id, harga);
                break;
            case KategoriLayanan::GYM:
                layananBaru = new LayananGym(id, harga);
                break;
            case KategoriLayanan::TRANSFER_BANDARA:
                layananBaru = new LayananTransferBandara(id, harga);
                break;
            case KategoriLayanan::CATERING:
                layananBaru = new LayananCatering(id, harga, "Custom", minOrder);
                break;
            default:
                layananBaru = new Layanan(id, nama, kategori, harga, satuan, deskripsi, minOrder);
                break;
        }
        
        if (layananBaru == nullptr) {
            cout << "[ERROR] Gagal membuat layanan!" << endl;
            return false;
        }
        
        if (!deskripsi.empty()) {
            layananBaru->setDeskripsi(deskripsi);
        }
        
        daftarLayanan.tambah(layananBaru);
        indexLayanan.insert(layananBaru, id);
        simpanDataLayanan();
        
        cout << "[SUKSES] Layanan " << nama << " (ID: " << id << ") berhasil ditambahkan!" << endl;
        
        return true;
    }
    
    // Cari layanan by ID (menggunakan BST - O(log n))
    Layanan* cariLayanan(const string& id) {
        Layanan** found = indexLayanan.search(id);
        if (found != nullptr) {
            return *found;
        }
        return nullptr;
    }
    
    // Update harga layanan
    bool updateHargaLayanan(const string& id, double hargaBaru) {
        Layanan* layanan = cariLayanan(id);
        
        if (layanan == nullptr) {
            cout << "[ERROR] Layanan dengan ID " << id << " tidak ditemukan!" << endl;
            return false;
        }
        
        double hargaLama = layanan->getHarga();
        layanan->setHarga(hargaBaru);
        simpanDataLayanan();
        
        cout << "[SUKSES] Harga " << layanan->getNamaLayanan() << " diubah dari " 
             << PengelolaFile::formatRupiah(hargaLama) << " menjadi " 
             << PengelolaFile::formatRupiah(hargaBaru) << endl;
        
        return true;
    }
    
    // Update ketersediaan layanan
    bool updateKetersediaan(const string& id, bool tersedia) {
        Layanan* layanan = cariLayanan(id);
        
        if (layanan == nullptr) {
            cout << "[ERROR] Layanan dengan ID " << id << " tidak ditemukan!" << endl;
            return false;
        }
        
        layanan->setTersedia(tersedia);
        simpanDataLayanan();
        
        cout << "[SUKSES] Layanan " << layanan->getNamaLayanan() 
             << " diubah menjadi: " << (tersedia ? "Tersedia" : "Tidak Tersedia") << endl;
        
        return true;
    }
    
    // Hapus layanan
    bool hapusLayanan(const string& id) {
        Layanan* layanan = cariLayanan(id);
        
        if (layanan == nullptr) {
            cout << "[ERROR] Layanan dengan ID " << id << " tidak ditemukan!" << endl;
            return false;
        }
        
        string nama = layanan->getNamaLayanan();
        
        bool berhasil = daftarLayanan.hapus([&](Layanan* l) {
            return l->getIdLayanan() == id;
        });
        
        if (berhasil) {
            delete layanan;
            indexLayanan.hapus(id);
            simpanDataLayanan();
            cout << "[SUKSES] Layanan " << nama << " berhasil dihapus!" << endl;
            return true;
        }
        
        return false;
    }
    
    // Tampilkan semua layanan
    void tampilkanSemuaLayanan() const {
        if (daftarLayanan.kosong()) {
            cout << "[INFO] Belum ada layanan terdaftar." << endl;
            return;
        }
        
        cout << "\n+------------------------------------------------------------+" << endl;
        cout << "¦          DAFTAR SEMUA LAYANAN HOTEL                        ¦" << endl;
        cout << "+------------------------------------------------------------+" << endl;
        
        int nomor = 1;
        daftarLayanan.iterasi([&](Layanan* l) {
            cout << "\n[" << nomor++ << "] " << l->getIdLayanan() 
                 << " - " << l->getNamaLayanan() << endl;
            cout << "Kategori    : " << l->getKategoriString() << endl;
            cout << "Harga       : " << PengelolaFile::formatRupiahLengkap(l->getHarga()) 
                 << " " << l->getSatuanString() << endl;
            cout << "Status      : " << (l->isTersedia() ? "Tersedia" : "Tidak Tersedia") << endl;
            cout << "Min. Order  : " << l->getMinimumOrder() << endl;
            cout << "Deskripsi   : " << l->getDeskripsi().substr(0, 50);
            if (l->getDeskripsi().length() > 50) cout << "...";
            cout << endl;
            cout << "-----------------------------------------------------------" << endl;
        });
    }
    
    // Tampilkan layanan tersedia saja
    void tampilkanLayananTersedia() const {
        int jumlahTersedia = 0;
        
        cout << "\n+------------------------------------------------------------+" << endl;
        cout << "¦          LAYANAN TERSEDIA                                  ¦" << endl;
        cout << "+------------------------------------------------------------+" << endl;
        
        daftarLayanan.iterasi([&](Layanan* l) {
            if (l->isTersedia()) {
                jumlahTersedia++;
                cout << "\n[" << jumlahTersedia << "] " << l->getIdLayanan() 
                     << " - " << l->getNamaLayanan() << endl;
                cout << "Kategori : " << l->getKategoriString() << endl;
                cout << "Harga    : " << PengelolaFile::formatRupiahLengkap(l->getHarga()) 
                     << " " << l->getSatuanString() << endl;
                if (l->getMinimumOrder() > 1) {
                    cout << "Min Order: " << l->getMinimumOrder() << endl;
                }
                cout << "Info     : " << l->getDeskripsi().substr(0, 55);
                if (l->getDeskripsi().length() > 55) cout << "...";
                cout << endl;
                cout << "-----------------------------------------------------------" << endl;
            }
        });
        
        if (jumlahTersedia == 0) {
            cout << "\n[INFO] Tidak ada layanan tersedia saat ini." << endl;
        } else {
            cout << "\nTotal: " << jumlahTersedia << " layanan tersedia" << endl;
        }
    }
    
    // Filter by kategori
    void tampilkanLayananByKategori(KategoriLayanan kategori) const {
        int jumlah = 0;
        
        string judulKategori = "";
        switch (kategori) {
            case KategoriLayanan::BALLROOM: judulKategori = "BALLROOM"; break;
            case KategoriLayanan::RESTAURANT: judulKategori = "RESTAURANT"; break;
            case KategoriLayanan::LAUNDRY: judulKategori = "LAUNDRY"; break;
            case KategoriLayanan::KOLAM_RENANG: judulKategori = "KOLAM RENANG"; break;
            case KategoriLayanan::GYM: judulKategori = "GYM & FITNESS"; break;
            case KategoriLayanan::TRANSFER_BANDARA: judulKategori = "TRANSFER BANDARA"; break;
            case KategoriLayanan::CATERING: judulKategori = "CATERING"; break;
            case KategoriLayanan::LAINNYA: judulKategori = "LAINNYA"; break;
        }
        
        cout << "\n+------------------------------------------------------------+" << endl;
        cout << "¦          LAYANAN KATEGORI " << judulKategori 
             << string(31 - judulKategori.length(), ' ') << "¦" << endl;
        cout << "+------------------------------------------------------------+" << endl;
        
        daftarLayanan.iterasi([&](Layanan* l) {
            if (l->getKategori() == kategori) {
                jumlah++;
                cout << "\n" << l->getIdLayanan() << " - " << l->getNamaLayanan() << endl;
                cout << "Harga: " << PengelolaFile::formatRupiahLengkap(l->getHarga()) 
                     << " " << l->getSatuanString() << endl;
                cout << "Status: " << (l->isTersedia() ? "Tersedia" : "Tidak Tersedia") << endl;
            }
        });
        
        if (jumlah == 0) {
            cout << "\n[INFO] Tidak ada layanan kategori " << judulKategori << endl;
        } else {
            cout << "\nTotal: " << jumlah << " layanan" << endl;
        }
    }
    
    // Get daftar layanan (untuk keperluan lain)
    CircularLinkedList<Layanan*>& getDaftarLayanan() {
        return daftarLayanan;
    }
    
    // Get jumlah layanan
    int getJumlahLayanan() const {
        return daftarLayanan.ukuran();
    }
};

#endif
