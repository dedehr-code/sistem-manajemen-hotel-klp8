#ifndef TRANSAKSI_H
#define TRANSAKSI_H

#include <iostream>
#include <string>
#include "PengelolaFile.h"
#include "Kamar.h"
#include "Layanan.h"
using namespace std;

// ============================================================================
// TRANSAKSI & BOOKING - Sistem pemesanan kamar & layanan
// ============================================================================

// Enum untuk jenis item yang dibooking
enum class JenisItem {
    KAMAR,
    LAYANAN
};

// Enum untuk status transaksi
enum class StatusTransaksi {
    PENDING,        // Menunggu verifikasi
    CONFIRMED,      // Sudah dikonfirmasi
    COMPLETED,      // Selesai (sudah checkout)
    CANCELLED       // Dibatalkan
};

// Enum untuk metode pembayaran
enum class MetodePembayaran {
    TUNAI,
    TRANSFER_BANK,
    KARTU_KREDIT,
    EWALLET
};

// ============================================================================
// ITEM BOOKING - Representasi 1 item dalam transaksi
// ============================================================================
class ItemBooking {
private:
    JenisItem jenis;
    string idItem;          // ID kamar atau ID layanan
    string namaItem;        // Nama untuk display
    double hargaSatuan;
    int kuantitas;          // Jumlah malam (kamar) atau jumlah unit (layanan)
    double subtotal;
    string tanggalMulai;    // DD/MM/YYYY
    string tanggalSelesai;  // DD/MM/YYYY (untuk kamar)
    string keterangan;      // Optional notes

public:
    // Constructor UNIFIED dengan parameter jenis eksplisit
    ItemBooking(JenisItem _jenis,
                const string& _id, const string& _nama, 
                double _harga, int _kuantitas,
                const string& _tanggalMulai, 
                const string& _tanggalSelesai = "",
                const string& _ket = "")
        : jenis(_jenis), idItem(_id), namaItem(_nama),
          hargaSatuan(_harga), kuantitas(_kuantitas),
          tanggalMulai(_tanggalMulai), tanggalSelesai(_tanggalSelesai),
          keterangan(_ket) {
        subtotal = hargaSatuan * kuantitas;
    }
    
    // Static factory methods untuk kemudahan penggunaan
    static ItemBooking* buatBookingKamar(
        const string& idKamar, const string& namaKamar,
        double harga, int jumlahMalam,
        const string& checkin, const string& checkout,
        const string& keterangan = "") {
        
        return new ItemBooking(JenisItem::KAMAR, idKamar, namaKamar,
                               harga, jumlahMalam, checkin, checkout, keterangan);
    }
    
    static ItemBooking* buatBookingLayanan(
        const string& idLayanan, const string& namaLayanan,
        double harga, int kuantitas, const string& tanggal,
        const string& keterangan = "") {
        
        return new ItemBooking(JenisItem::LAYANAN, idLayanan, namaLayanan,
                               harga, kuantitas, tanggal, "", keterangan);
    }
    
    // Getter
    JenisItem getJenis() const { return jenis; }
    string getIdItem() const { return idItem; }
    string getNamaItem() const { return namaItem; }
    double getHargaSatuan() const { return hargaSatuan; }
    int getKuantitas() const { return kuantitas; }
    double getSubtotal() const { return subtotal; }
    string getTanggalMulai() const { return tanggalMulai; }
    string getTanggalSelesai() const { return tanggalSelesai; }
    string getKeterangan() const { return keterangan; }
    
    // Setter
    void setKuantitas(int _kuantitas) {
        kuantitas = _kuantitas;
        subtotal = hargaSatuan * kuantitas;
    }
    
    void setKeterangan(const string& _ket) { keterangan = _ket; }
    
    // Helper
    string getJenisString() const {
        return (jenis == JenisItem::KAMAR) ? "KAMAR" : "LAYANAN";
    }
    
    // Tampilkan info item
    void tampilkanInfo() const {
        cout << "  " << getJenisString() << " - " << namaItem << endl;
        cout << "  ID: " << idItem << endl;
        
        if (jenis == JenisItem::KAMAR) {
            cout << "  Check-in : " << tanggalMulai << endl;
            cout << "  Check-out: " << tanggalSelesai << endl;
            cout << "  Durasi   : " << kuantitas << " malam" << endl;
        } else {
            cout << "  Tanggal  : " << tanggalMulai << endl;
            cout << "  Kuantitas: " << kuantitas << endl;
        }
        
        cout << "  Harga    : " << PengelolaFile::formatRupiahLengkap(hargaSatuan) << endl;
        cout << "  Subtotal : " << PengelolaFile::formatRupiahLengkap(subtotal) << endl;
        
        if (!keterangan.empty()) {
            cout << "  Keterangan: " << keterangan << endl;
        }
    }
    
    // Override operator <<
    friend ostream& operator<<(ostream& os, const ItemBooking& item) {
        os << item.getJenisString() << " - " << item.namaItem 
           << " x" << item.kuantitas 
           << " = " << PengelolaFile::formatRupiah(item.subtotal);
        return os;
    }
    
    // Format untuk disimpan ke file
    string toFileFormat() const {
        string fields[9];
        fields[0] = getJenisString();
        fields[1] = idItem;
        fields[2] = namaItem;
        fields[3] = to_string(static_cast<long long>(hargaSatuan));
        fields[4] = to_string(kuantitas);
        fields[5] = to_string(static_cast<long long>(subtotal));
        fields[6] = tanggalMulai;
        fields[7] = tanggalSelesai;
        fields[8] = keterangan;
        
        return PengelolaFile::gabungField(fields, 9);
    }
};

// ============================================================================
// TRANSAKSI - Representasi 1 transaksi lengkap dengan multiple items
// ============================================================================
class Transaksi {
private:
    string idTransaksi;
    string idPelanggan;
    string namaPelanggan;
    StatusTransaksi status;
    MetodePembayaran metodeBayar;
    
    // Array untuk menyimpan items (max 20 items per transaksi)
    ItemBooking* items[20];
    int jumlahItem;
    
    double totalHarga;
    double pajak;           // 10% dari total
    double biayaLayanan;    // 5% dari total
    double totalAkhir;
    
    string tanggalTransaksi;
    string tanggalCheckIn;
    string tanggalCheckOut;
    string catatan;
    
    // Helper untuk hitung total
    void hitungTotal() {
        totalHarga = 0.0;
        for (int i = 0; i < jumlahItem; i++) {
            totalHarga += items[i]->getSubtotal();
        }
        
        pajak = totalHarga * 0.10;          // PPN 10%
        biayaLayanan = totalHarga * 0.05;   // Service charge 5%
        totalAkhir = totalHarga + pajak + biayaLayanan;
    }

public:
    // Constructor
    Transaksi(const string& _idTransaksi, const string& _idPelanggan,
              const string& _namaPelanggan, const string& _tanggalTransaksi)
        : idTransaksi(_idTransaksi), idPelanggan(_idPelanggan),
          namaPelanggan(_namaPelanggan), status(StatusTransaksi::PENDING),
          metodeBayar(MetodePembayaran::TUNAI), jumlahItem(0),
          totalHarga(0.0), pajak(0.0), biayaLayanan(0.0), totalAkhir(0.0),
          tanggalTransaksi(_tanggalTransaksi), tanggalCheckIn(""),
          tanggalCheckOut(""), catatan("") {
        
        // Inisialisasi array items
        for (int i = 0; i < 20; i++) {
            items[i] = nullptr;
        }
    }
    
    // Destructor - hapus semua items untuk prevent memory leak
    ~Transaksi() {
        for (int i = 0; i < jumlahItem; i++) {
            delete items[i];
            items[i] = nullptr;
        }
    }
    
    // Copy constructor (deep copy)
    Transaksi(const Transaksi& other)
        : idTransaksi(other.idTransaksi), idPelanggan(other.idPelanggan),
          namaPelanggan(other.namaPelanggan), status(other.status),
          metodeBayar(other.metodeBayar), jumlahItem(other.jumlahItem),
          totalHarga(other.totalHarga), pajak(other.pajak),
          biayaLayanan(other.biayaLayanan), totalAkhir(other.totalAkhir),
          tanggalTransaksi(other.tanggalTransaksi),
          tanggalCheckIn(other.tanggalCheckIn),
          tanggalCheckOut(other.tanggalCheckOut), catatan(other.catatan) {
        
        // Deep copy items
        for (int i = 0; i < 20; i++) {
            if (i < other.jumlahItem && other.items[i] != nullptr) {
                items[i] = new ItemBooking(*other.items[i]);
            } else {
                items[i] = nullptr;
            }
        }
    }
    
    // Getter
    string getIdTransaksi() const { return idTransaksi; }
    string getIdPelanggan() const { return idPelanggan; }
    string getNamaPelanggan() const { return namaPelanggan; }
    StatusTransaksi getStatus() const { return status; }
    MetodePembayaran getMetodeBayar() const { return metodeBayar; }
    int getJumlahItem() const { return jumlahItem; }
    double getTotalHarga() const { return totalHarga; }
    double getPajak() const { return pajak; }
    double getBiayaLayanan() const { return biayaLayanan; }
    double getTotalAkhir() const { return totalAkhir; }
    string getTanggalTransaksi() const { return tanggalTransaksi; }
    string getTanggalCheckIn() const { return tanggalCheckIn; }
    string getTanggalCheckOut() const { return tanggalCheckOut; }
    string getCatatan() const { return catatan; }
    
    // Setter khusus untuk loading dari file (persistence)
    void setTotalDariFile(double _totalHarga, double _pajak, double _biayaLayanan, double _totalAkhir) {
        totalHarga = _totalHarga;
        pajak = _pajak;
        biayaLayanan = _biayaLayanan;
        totalAkhir = _totalAkhir;
    }
    
    void setJumlahItemDariFile(int jumlah) {
        jumlahItem = jumlah;
    }
    
    // Setter
    void setStatus(StatusTransaksi _status) { status = _status; }
    void setMetodeBayar(MetodePembayaran _metode) { metodeBayar = _metode; }
    void setTanggalCheckIn(const string& _tanggal) { tanggalCheckIn = _tanggal; }
    void setTanggalCheckOut(const string& _tanggal) { tanggalCheckOut = _tanggal; }
    void setCatatan(const string& _catatan) { catatan = _catatan; }
    
    // Helper: Convert status ke string
    string getStatusString() const {
        switch (status) {
            case StatusTransaksi::PENDING: return "PENDING";
            case StatusTransaksi::CONFIRMED: return "CONFIRMED";
            case StatusTransaksi::COMPLETED: return "COMPLETED";
            case StatusTransaksi::CANCELLED: return "CANCELLED";
            default: return "UNKNOWN";
        }
    }
    
    // Helper: Convert metode bayar ke string
    string getMetodeBayarString() const {
        switch (metodeBayar) {
            case MetodePembayaran::TUNAI: return "TUNAI";
            case MetodePembayaran::TRANSFER_BANK: return "TRANSFER_BANK";
            case MetodePembayaran::KARTU_KREDIT: return "KARTU_KREDIT";
            case MetodePembayaran::EWALLET: return "E-WALLET";
            default: return "UNKNOWN";
        }
    }
    
    // Tambah item ke transaksi
    bool tambahItem(ItemBooking* item) {
        if (jumlahItem >= 20) {
            cout << "[ERROR] Transaksi penuh! Maksimal 20 item." << endl;
            return false;
        }
        
        items[jumlahItem++] = item;
        hitungTotal();
        return true;
    }
    
    // Hapus item by index
    bool hapusItem(int index) {
        if (index < 0 || index >= jumlahItem) {
            return false;
        }
        
        delete items[index];
        
        // Shift items ke kiri
        for (int i = index; i < jumlahItem - 1; i++) {
            items[i] = items[i + 1];
        }
        
        items[jumlahItem - 1] = nullptr;
        jumlahItem--;
        
        hitungTotal();
        return true;
    }
    
    // Dapatkan item by index
    ItemBooking* getItem(int index) const {
        if (index < 0 || index >= jumlahItem) {
            return nullptr;
        }
        return items[index];
    }
    
    // Tampilkan struk/invoice
    void tampilkanStruk() const {
        cout << "\n";
        cout << "+------------------------------------------------------------+" << endl;
        cout << "¦              STRUK PEMBAYARAN - HOTEL PARADISE             ¦" << endl;
        cout << "¦------------------------------------------------------------¦" << endl;
        cout << "¦ ID Transaksi : " << idTransaksi << string(42 - idTransaksi.length(), ' ') << "¦" << endl;
        cout << "¦ Tanggal      : " << tanggalTransaksi << string(42 - tanggalTransaksi.length(), ' ') << "¦" << endl;
        cout << "¦ Pelanggan    : " << namaPelanggan << string(42 - namaPelanggan.length(), ' ') << "¦" << endl;
        cout << "¦ Status       : " << getStatusString() << string(42 - getStatusString().length(), ' ') << "¦" << endl;
        cout << "¦------------------------------------------------------------¦" << endl;
        cout << "¦ DETAIL ITEM:                                               ¦" << endl;
        cout << "¦------------------------------------------------------------¦" << endl;
        
        for (int i = 0; i < jumlahItem; i++) {
            cout << "¦ " << (i + 1) << ". ";
            string itemStr = items[i]->getNamaItem();
            if (itemStr.length() > 50) itemStr = itemStr.substr(0, 47) + "...";
            cout << itemStr << string(54 - itemStr.length(), ' ') << "¦" << endl;
            
            string detail = "    " + to_string(items[i]->getKuantitas()) + " x " + 
                           PengelolaFile::formatRupiah(items[i]->getHargaSatuan()) +
                           " = " + PengelolaFile::formatRupiah(items[i]->getSubtotal());
            if (detail.length() > 56) detail = detail.substr(0, 53) + "...";
            cout << "¦ " << detail << string(57 - detail.length(), ' ') << "¦" << endl;
        }
        
        cout << "¦------------------------------------------------------------¦" << endl;
        
        string subtotalStr = "Subtotal      : " + PengelolaFile::formatRupiahLengkap(totalHarga);
        cout << "¦ " << subtotalStr << string(57 - subtotalStr.length(), ' ') << "¦" << endl;
        
        string pajakStr = "Pajak (10%)   : " + PengelolaFile::formatRupiahLengkap(pajak);
        cout << "¦ " << pajakStr << string(57 - pajakStr.length(), ' ') << "¦" << endl;
        
        string serviceStr = "Service (5%)  : " + PengelolaFile::formatRupiahLengkap(biayaLayanan);
        cout << "¦ " << serviceStr << string(57 - serviceStr.length(), ' ') << "¦" << endl;
        
        cout << "¦------------------------------------------------------------¦" << endl;
        
        string totalStr = "TOTAL BAYAR   : " + PengelolaFile::formatRupiahLengkap(totalAkhir);
        cout << "¦ " << totalStr << string(57 - totalStr.length(), ' ') << "¦" << endl;
        
        string metodeStr = "Metode Bayar  : " + getMetodeBayarString();
        cout << "¦ " << metodeStr << string(57 - metodeStr.length(), ' ') << "¦" << endl;
        
        cout << "+------------------------------------------------------------+" << endl;
        cout << "\n       Terima kasih telah menginap di Hotel Paradise!" << endl;
        cout << "           Kami berharap dapat melayani Anda lagi\n" << endl;
    }
    
    // Tampilkan info ringkas
    void tampilkanInfo() const {
        cout << "\n=== Transaksi " << idTransaksi << " ===" << endl;
        cout << "Pelanggan    : " << namaPelanggan << endl;
        cout << "Tanggal      : " << tanggalTransaksi << endl;
        cout << "Status       : " << getStatusString() << endl;
        cout << "Jumlah Item  : " << jumlahItem << endl;
        cout << "Total Akhir  : " << PengelolaFile::formatRupiahLengkap(totalAkhir) << endl;
    }
    
    // Override operator <<
    friend ostream& operator<<(ostream& os, const Transaksi& t) {
        os << t.idTransaksi << " - " << t.namaPelanggan 
           << " - " << t.getStatusString()
           << " - " << PengelolaFile::formatRupiah(t.totalAkhir);
        return os;
    }
};

#endif
