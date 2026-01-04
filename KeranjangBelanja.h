#ifndef KERANJANG_BELANJA_H
#define KERANJANG_BELANJA_H

#include <iostream>
#include <string>
#include "CircularLinkedList.h"
#include "Transaksi.h"
#include "Kamar.h"
#include "Layanan.h"
#include "PengelolaFile.h"
using namespace std;

// ============================================================================
// KERANJANG BELANJA - MULTI-ITEM BOOKING CART
// ============================================================================
// Fitur utama Fase 2: Customer bisa pilih banyak kamar + layanan sebelum checkout
// ============================================================================

class KeranjangBelanja {
private:
    string idPelanggan;
    string namaPelanggan;
    CircularLinkedList<ItemBooking*> daftarItem;
    double totalHarga;
    
    // Helper: Hitung total
    void hitungTotal() {
        totalHarga = 0.0;
        daftarItem.iterasi([&](ItemBooking* item) {
            totalHarga += item->getSubtotal();
        });
    }

public:
    // Constructor
    KeranjangBelanja(const string& _idPelanggan, const string& _namaPelanggan)
        : idPelanggan(_idPelanggan), namaPelanggan(_namaPelanggan), totalHarga(0.0) {}
    
    // Destructor
    ~KeranjangBelanja() {
        // Kosongkan keranjang dan hapus semua item
        kosongkan();
    }
    
    // Tambah kamar ke keranjang
    bool tambahKamar(Kamar* kamar, int jumlahMalam, 
                     const string& tanggalCheckIn, const string& tanggalCheckOut,
                     const string& keterangan = "") {
        
        if (kamar == nullptr) {
            cout << "[ERROR] Kamar tidak valid!" << endl;
            return false;
        }
        
        if (kamar->getStatus() != StatusKamar::TERSEDIA) {
            cout << "[ERROR] Kamar " << kamar->getNomorKamar() << " tidak tersedia!" << endl;
            return false;
        }
        
        if (jumlahMalam < 1) {
            cout << "[ERROR] Jumlah malam minimal 1!" << endl;
            return false;
        }
        
        // Buat item booking kamar
        ItemBooking* item = ItemBooking::buatBookingKamar(
            kamar->getNomorKamar(),
            kamar->getTipeString() + " - " + kamar->getNomorKamar(),
            kamar->getHargaPerMalam(),
            jumlahMalam,
            tanggalCheckIn,
            tanggalCheckOut,
            keterangan
        );
        
        daftarItem.tambah(item);
        hitungTotal();
        
        cout << "[SUKSES] Kamar " << kamar->getNomorKamar() << " ditambahkan ke keranjang" << endl;
        cout << "         " << jumlahMalam << " malam x " 
             << PengelolaFile::formatRupiah(kamar->getHargaPerMalam())
             << " = " << PengelolaFile::formatRupiah(item->getSubtotal()) << endl;
        
        return true;
    }
    
    // Tambah layanan ke keranjang
    bool tambahLayanan(Layanan* layanan, int kuantitas, 
                      const string& tanggalPenggunaan,
                      const string& keterangan = "") {
        
        if (layanan == nullptr) {
            cout << "[ERROR] Layanan tidak valid!" << endl;
            return false;
        }
        
        if (!layanan->isTersedia()) {
            cout << "[ERROR] Layanan " << layanan->getNamaLayanan() << " tidak tersedia!" << endl;
            return false;
        }
        
        if (kuantitas < layanan->getMinimumOrder()) {
            cout << "[ERROR] Minimum order untuk " << layanan->getNamaLayanan() 
                 << " adalah " << layanan->getMinimumOrder() << endl;
            return false;
        }
        
        // Buat item booking layanan
        ItemBooking* item = ItemBooking::buatBookingLayanan(
            layanan->getIdLayanan(),
            layanan->getNamaLayanan(),
            layanan->getHarga(),
            kuantitas,
            tanggalPenggunaan,
            keterangan
        );
        
        daftarItem.tambah(item);
        hitungTotal();
        
        cout << "[SUKSES] Layanan " << layanan->getNamaLayanan() << " ditambahkan ke keranjang" << endl;
        cout << "         " << kuantitas << " x " 
             << PengelolaFile::formatRupiah(layanan->getHarga())
             << " = " << PengelolaFile::formatRupiah(item->getSubtotal()) << endl;
        
        return true;
    }
    
    // Hapus item dari keranjang by index
    bool hapusItem(int index) {
        if (index < 1 || index > daftarItem.ukuran()) {
            cout << "[ERROR] Nomor item tidak valid!" << endl;
            return false;
        }
        
        // Dapatkan item untuk info (dereference karena return ItemBooking**)
        ItemBooking** itemPtr = daftarItem.dapatkanByIndex(index - 1);
        if (itemPtr == nullptr || *itemPtr == nullptr) return false;
        
        ItemBooking* item = *itemPtr;
        string namaItem = item->getNamaItem();
        
        // Hapus dari list (list akan otomatis delete item di destructor)
        bool berhasil = daftarItem.hapus([&](ItemBooking* it) {
            return it == item;
        });
        
        if (berhasil) {
            delete item; // Manual delete karena list tidak auto-delete
            hitungTotal();
            cout << "[SUKSES] " << namaItem << " dihapus dari keranjang" << endl;
            return true;
        }
        
        return false;
    }
    
    // Kosongkan keranjang
    void kosongkan() {
        // Delete semua item
        daftarItem.iterasi([](ItemBooking* item) {
            delete item;
        });
        
        // Reset
        daftarItem = CircularLinkedList<ItemBooking*>();
        totalHarga = 0.0;
        
        cout << "[INFO] Keranjang telah dikosongkan" << endl;
    }
    
    // Cek apakah keranjang kosong
    bool kosong() const {
        return daftarItem.kosong();
    }
    
    // Dapatkan jumlah item
    int jumlahItem() const {
        return daftarItem.ukuran();
    }
    
    // Dapatkan total harga (sebelum pajak)
    double getTotalHarga() const {
        return totalHarga;
    }
    
    // Tampilkan isi keranjang
    void tampilkanKeranjang() const {
        if (daftarItem.kosong()) {
            cout << "\n+------------------------------------------------------------+" << endl;
            cout << "¦          KERANJANG BELANJA KOSONG                          ¦" << endl;
            cout << "+------------------------------------------------------------+" << endl;
            return;
        }
        
        cout << "\n+------------------------------------------------------------+" << endl;
        cout << "¦          KERANJANG BELANJA - " << namaPelanggan.substr(0, 30) 
             << string(30 - min((int)namaPelanggan.length(), 30), ' ') << "¦" << endl;
        cout << "¦------------------------------------------------------------¦" << endl;
        
        int nomor = 1;
        daftarItem.iterasi([&](ItemBooking* item) {
            cout << "¦ [" << nomor++ << "] " << item->getJenisString() 
                 << " - " << item->getNamaItem().substr(0, 40) 
                 << string(40 - min((int)item->getNamaItem().length(), 40), ' ') << "¦" << endl;
            
            if (item->getJenis() == JenisItem::KAMAR) {
                cout << "¦     Check-in  : " << item->getTanggalMulai() 
                     << " | Check-out: " << item->getTanggalSelesai() << "      ¦" << endl;
                cout << "¦     Durasi    : " << item->getKuantitas() << " malam" 
                     << string(43, ' ') << "¦" << endl;
            } else {
                cout << "¦     Tanggal   : " << item->getTanggalMulai() 
                     << " | Kuantitas: " << item->getKuantitas() << "           ¦" << endl;
            }
            
            string hargaStr = PengelolaFile::formatRupiah(item->getHargaSatuan());
            string subtotalStr = PengelolaFile::formatRupiah(item->getSubtotal());
            cout << "¦     Harga     : " << hargaStr 
                 << " | Subtotal: " << subtotalStr 
                 << string(34 - hargaStr.length() - subtotalStr.length(), ' ') << "¦" << endl;
            
            if (!item->getKeterangan().empty()) {
                cout << "¦     Note      : " << item->getKeterangan().substr(0, 40)
                     << string(40 - min((int)item->getKeterangan().length(), 40), ' ') << "¦" << endl;
            }
            
            cout << "¦------------------------------------------------------------¦" << endl;
        });
        
        string totalStr = PengelolaFile::formatRupiahLengkap(totalHarga);
        cout << "¦ TOTAL (Belum termasuk pajak & service)                     ¦" << endl;
        cout << "¦ " << totalStr << string(57 - totalStr.length(), ' ') << "¦" << endl;
        cout << "+------------------------------------------------------------+" << endl;
        cout << "\nJumlah Item: " << daftarItem.ukuran() << endl;
    }
    
    // Checkout - Convert keranjang menjadi transaksi
    Transaksi* checkout(const string& tanggalTransaksi) {
        if (daftarItem.kosong()) {
            cout << "[ERROR] Keranjang kosong! Tidak bisa checkout." << endl;
            return nullptr;
        }
        
        // Generate ID transaksi
        static int counterTransaksi = 1;
        string idTransaksi = "T" + PengelolaFile::generateID("", counterTransaksi++);
        
        // Buat transaksi baru
        Transaksi* transaksi = new Transaksi(idTransaksi, idPelanggan, 
                                             namaPelanggan, tanggalTransaksi);
        
        // Copy semua item ke transaksi (deep copy)
        daftarItem.iterasi([&](ItemBooking* item) {
            // Buat copy item
            ItemBooking* itemCopy = nullptr;
            
            if (item->getJenis() == JenisItem::KAMAR) {
                itemCopy = ItemBooking::buatBookingKamar(
                    item->getIdItem(),
                    item->getNamaItem(),
                    item->getHargaSatuan(),
                    item->getKuantitas(),
                    item->getTanggalMulai(),
                    item->getTanggalSelesai(),
                    item->getKeterangan()
                );
            } else {
                itemCopy = ItemBooking::buatBookingLayanan(
                    item->getIdItem(),
                    item->getNamaItem(),
                    item->getHargaSatuan(),
                    item->getKuantitas(),
                    item->getTanggalMulai(),
                    item->getKeterangan()
                );
            }
            
            transaksi->tambahItem(itemCopy);
        });
        
        // Set tanggal check-in/out dari item pertama (jika ada kamar)
        ItemBooking** itemPertamaPtr = daftarItem.dapatkanByIndex(0);
        if (itemPertamaPtr && *itemPertamaPtr && (*itemPertamaPtr)->getJenis() == JenisItem::KAMAR) {
            ItemBooking* itemPertama = *itemPertamaPtr;
            transaksi->setTanggalCheckIn(itemPertama->getTanggalMulai());
            transaksi->setTanggalCheckOut(itemPertama->getTanggalSelesai());
        }
        
        cout << "\n[SUKSES] Transaksi berhasil dibuat!" << endl;
        cout << "ID Transaksi: " << idTransaksi << endl;
        cout << "Total Item  : " << daftarItem.ukuran() << endl;
        cout << "Total Bayar : " << PengelolaFile::formatRupiahLengkap(transaksi->getTotalAkhir()) << endl;
        
        return transaksi;
    }
    
    // Edit kuantitas item
    bool editKuantitas(int index, int kuantitasBaru) {
        if (index < 1 || index > daftarItem.ukuran()) {
            cout << "[ERROR] Nomor item tidak valid!" << endl;
            return false;
        }
        
        ItemBooking** itemPtr = daftarItem.dapatkanByIndex(index - 1);
        if (itemPtr == nullptr || *itemPtr == nullptr) return false;
        
        ItemBooking* item = *itemPtr;
        
        if (kuantitasBaru < 1) {
            cout << "[ERROR] Kuantitas minimal 1!" << endl;
            return false;
        }
        
        item->setKuantitas(kuantitasBaru);
        hitungTotal();
        
        cout << "[SUKSES] Kuantitas " << item->getNamaItem() 
             << " diubah menjadi " << kuantitasBaru << endl;
        
        return true;
    }
    
    // Info ringkas
    string getInfoRingkas() const {
        if (daftarItem.kosong()) {
            return "Keranjang kosong";
        }
        
        return to_string(daftarItem.ukuran()) + " item - Total: " + 
               PengelolaFile::formatRupiah(totalHarga);
    }
};

#endif
