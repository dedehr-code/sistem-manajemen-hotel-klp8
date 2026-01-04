#ifndef KAMAR_H
#define KAMAR_H

#include <iostream>
#include <string>
#include "PengelolaFile.h"
using namespace std;

// ============================================================================
// KAMAR - BASE CLASS & TURUNAN
// ============================================================================
// Tipe kamar: Standard, Deluxe, Suite, Presidential
// ============================================================================

// Enum untuk status kamar
enum class StatusKamar {
    TERSEDIA,
    TERISI,
    PERBAIKAN,
    DIBERSIHKAN
};

// Enum untuk tipe kamar
enum class TipeKamar {
    STANDARD,
    DELUXE,
    SUITE,
    PRESIDENTIAL
};

// Base class: Kamar
class Kamar {
protected:
    string nomorKamar;      // Contoh: K101, K201
    TipeKamar tipe;
    StatusKamar status;
    double hargaPerMalam;
    int lantai;
    int kapasitasOrang;
    bool adaBalkon;
    bool pemandanganLaut;
    
public:
    // Constructor
    Kamar(const string& _nomor, TipeKamar _tipe, double _harga,
          int _lantai, int _kapasitas, bool _balkon = false, 
          bool _pemandangan = false)
        : nomorKamar(_nomor), tipe(_tipe), status(StatusKamar::TERSEDIA),
          hargaPerMalam(_harga), lantai(_lantai), kapasitasOrang(_kapasitas),
          adaBalkon(_balkon), pemandanganLaut(_pemandangan) {}
    
    virtual ~Kamar() {}
    
    // Getter
    string getNomorKamar() const { return nomorKamar; }
    TipeKamar getTipe() const { return tipe; }
    StatusKamar getStatus() const { return status; }
    double getHargaPerMalam() const { return hargaPerMalam; }
    int getLantai() const { return lantai; }
    int getKapasitasOrang() const { return kapasitasOrang; }
    bool hasBalkon() const { return adaBalkon; }
    bool hasPemandanganLaut() const { return pemandanganLaut; }
    
    // Setter
    void setStatus(StatusKamar _status) { status = _status; }
    void setHargaPerMalam(double _harga) { hargaPerMalam = _harga; }
    
    // Helper: Convert tipe ke string
    string getTipeString() const {
        switch (tipe) {
            case TipeKamar::STANDARD: return "STANDARD";
            case TipeKamar::DELUXE: return "DELUXE";
            case TipeKamar::SUITE: return "SUITE";
            case TipeKamar::PRESIDENTIAL: return "PRESIDENTIAL";
            default: return "UNKNOWN";
        }
    }
    
    // Helper: Convert status ke string
    string getStatusString() const {
        switch (status) {
            case StatusKamar::TERSEDIA: return "TERSEDIA";
            case StatusKamar::TERISI: return "TERISI";
            case StatusKamar::PERBAIKAN: return "PERBAIKAN";
            case StatusKamar::DIBERSIHKAN: return "DIBERSIHKAN";
            default: return "UNKNOWN";
        }
    }
    
    // Helper: Static function - string ke StatusKamar
    static StatusKamar stringKeStatus(const string& str) {
        string upper = PengelolaFile::toUpper(str);
        if (upper == "TERSEDIA") return StatusKamar::TERSEDIA;
        if (upper == "TERISI") return StatusKamar::TERISI;
        if (upper == "PERBAIKAN") return StatusKamar::PERBAIKAN;
        if (upper == "DIBERSIHKAN") return StatusKamar::DIBERSIHKAN;
        return StatusKamar::TERSEDIA;
    }
    
    // Helper: Static function - string ke TipeKamar
    static TipeKamar stringKeTipe(const string& str) {
        string upper = PengelolaFile::toUpper(str);
        if (upper == "STANDARD") return TipeKamar::STANDARD;
        if (upper == "DELUXE") return TipeKamar::DELUXE;
        if (upper == "SUITE") return TipeKamar::SUITE;
        if (upper == "PRESIDENTIAL") return TipeKamar::PRESIDENTIAL;
        return TipeKamar::STANDARD;
    }
    
    // Virtual function untuk deskripsi fasilitas
    virtual string getFasilitas() const {
        return "TV LED, AC, Wi-Fi gratis, Kamar mandi dalam";
    }
    
    // Tampilkan info kamar
    virtual void tampilkanInfo() const {
        cout << "\n=== Informasi Kamar ===" << endl;
        cout << "Nomor Kamar   : " << nomorKamar << endl;
        cout << "Tipe          : " << getTipeString() << endl;
        cout << "Status        : " << getStatusString() << endl;
        cout << "Harga/Malam   : " << PengelolaFile::formatRupiahLengkap(hargaPerMalam) << endl;
        cout << "Lantai        : " << lantai << endl;
        cout << "Kapasitas     : " << kapasitasOrang << " orang" << endl;
        cout << "Balkon        : " << (adaBalkon ? "Ya" : "Tidak") << endl;
        cout << "Pemandangan   : " << (pemandanganLaut ? "Laut" : "Kota") << endl;
        cout << "Fasilitas     : " << getFasilitas() << endl;
    }
    
    // Override operator <<
    friend ostream& operator<<(ostream& os, const Kamar& k) {
        os << k.nomorKamar << " - " << k.getTipeString() 
           << " - " << k.getStatusString()
           << " - " << PengelolaFile::formatRupiah(k.hargaPerMalam) << "/malam";
        return os;
    }
    
    // Format untuk disimpan ke file
    virtual string toFileFormat() const {
        string fields[9];
        fields[0] = getTipeString();
        fields[1] = nomorKamar;
        fields[2] = getStatusString();
        fields[3] = to_string(static_cast<long long>(hargaPerMalam));
        fields[4] = to_string(lantai);
        fields[5] = to_string(kapasitasOrang);
        fields[6] = (adaBalkon ? "1" : "0");
        fields[7] = (pemandanganLaut ? "1" : "0");
        fields[8] = getFasilitas();
        
        return PengelolaFile::gabungField(fields, 9);
    }
};

// ============================================================================
// KAMAR STANDARD - Kamar ekonomis dengan fasilitas standar
// ============================================================================
class KamarStandard : public Kamar {
public:
    KamarStandard(const string& _nomor, int _lantai = 1)
        : Kamar(_nomor, TipeKamar::STANDARD, 500000.0, _lantai, 2, false, false) {}
    
    string getFasilitas() const override {
        return "TV LED 32\", AC, Wi-Fi gratis, Kamar mandi dalam, Air panas";
    }
};

// ============================================================================
// KAMAR DELUXE - Kamar dengan fasilitas lebih lengkap
// ============================================================================
class KamarDeluxe : public Kamar {
public:
    KamarDeluxe(const string& _nomor, int _lantai = 2, bool _balkon = true)
        : Kamar(_nomor, TipeKamar::DELUXE, 1000000.0, _lantai, 2, _balkon, false) {}
    
    string getFasilitas() const override {
        return "TV LED 43\", AC, Wi-Fi gratis, Kamar mandi dalam, Air panas, "
               "Kulkas mini, Coffee maker, Sofa";
    }
};

// ============================================================================
// KAMAR SUITE - Kamar luas dengan ruang tamu terpisah
// ============================================================================
class KamarSuite : public Kamar {
public:
    KamarSuite(const string& _nomor, int _lantai = 3, bool _pemandangan = true)
        : Kamar(_nomor, TipeKamar::SUITE, 2500000.0, _lantai, 4, true, _pemandangan) {}
    
    string getFasilitas() const override {
        return "TV LED 55\" x2, AC, Wi-Fi premium, Kamar mandi luas + bathtub, "
               "Ruang tamu terpisah, Kulkas, Coffee/Tea maker, Sofa set, "
               "Meja kerja, Safe deposit box, Balkon";
    }
};

// ============================================================================
// KAMAR PRESIDENTIAL - Kamar termewah dengan fasilitas VIP
// ============================================================================
class KamarPresidential : public Kamar {
public:
    KamarPresidential(const string& _nomor, int _lantai = 5)
        : Kamar(_nomor, TipeKamar::PRESIDENTIAL, 5000000.0, _lantai, 6, true, true) {}
    
    string getFasilitas() const override {
        return "TV LED 65\" x3, AC, Wi-Fi premium, 2 Kamar tidur master, "
               "Jacuzzi & bathtub, Ruang tamu mewah, Ruang makan, Dapur kecil, "
               "Kulkas besar, Coffee/Tea maker premium, Home theater, "
               "Sofa set mewah, Meja kerja executive, Safe deposit box, "
               "Balkon luas dengan pemandangan laut, Butler service 24 jam";
    }
};

#endif
