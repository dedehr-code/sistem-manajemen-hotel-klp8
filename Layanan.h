#ifndef LAYANAN_H
#define LAYANAN_H

#include <iostream>
#include <string>
#include "PengelolaFile.h"
using namespace std;

// ============================================================================
// LAYANAN HOTEL - BASE CLASS & TURUNAN
// ============================================================================
// Kategori: Ballroom, Restaurant, Laundry, Kolam Renang, Gym, 
//           Transfer Bandara, Catering
// ============================================================================

// Enum untuk kategori layanan
enum class KategoriLayanan {
    BALLROOM,
    RESTAURANT,
    LAUNDRY,
    KOLAM_RENANG,
    GYM,
    TRANSFER_BANDARA,
    CATERING,
    LAINNYA
};

// Enum untuk satuan harga
enum class SatuanHarga {
    PER_EVENT,      // Ballroom
    PER_ORANG,      // Restaurant, Catering
    PER_KG,         // Laundry
    PER_HARI,       // Kolam Renang
    PER_SESI,       // Gym
    PER_TRIP,       // Transfer Bandara
    PER_JAM         // Sewa ruangan per jam
};

// Base class: Layanan
class Layanan {
protected:
    string idLayanan;
    string namaLayanan;
    KategoriLayanan kategori;
    double harga;
    SatuanHarga satuan;
    bool tersedia;
    string deskripsi;
    int minimumOrder; // Minimum pemesanan (misalnya catering min 20 pax)

public:
    // Constructor
    Layanan(const string& _id, const string& _nama, KategoriLayanan _kategori,
            double _harga, SatuanHarga _satuan, const string& _deskripsi = "",
            int _minOrder = 1)
        : idLayanan(_id), namaLayanan(_nama), kategori(_kategori),
          harga(_harga), satuan(_satuan), tersedia(true), 
          deskripsi(_deskripsi), minimumOrder(_minOrder) {}
    
    virtual ~Layanan() {}
    
    // Getter
    string getIdLayanan() const { return idLayanan; }
    string getNamaLayanan() const { return namaLayanan; }
    KategoriLayanan getKategori() const { return kategori; }
    double getHarga() const { return harga; }
    SatuanHarga getSatuan() const { return satuan; }
    bool isTersedia() const { return tersedia; }
    string getDeskripsi() const { return deskripsi; }
    int getMinimumOrder() const { return minimumOrder; }
    
    // Setter
    void setHarga(double _harga) { harga = _harga; }
    void setTersedia(bool _tersedia) { tersedia = _tersedia; }
    void setDeskripsi(const string& _deskripsi) { deskripsi = _deskripsi; }
    void setMinimumOrder(int _min) { minimumOrder = _min; }
    
    // Helper: Convert kategori ke string
    string getKategoriString() const {
        switch (kategori) {
            case KategoriLayanan::BALLROOM: return "BALLROOM";
            case KategoriLayanan::RESTAURANT: return "RESTAURANT";
            case KategoriLayanan::LAUNDRY: return "LAUNDRY";
            case KategoriLayanan::KOLAM_RENANG: return "KOLAM_RENANG";
            case KategoriLayanan::GYM: return "GYM";
            case KategoriLayanan::TRANSFER_BANDARA: return "TRANSFER_BANDARA";
            case KategoriLayanan::CATERING: return "CATERING";
            case KategoriLayanan::LAINNYA: return "LAINNYA";
            default: return "UNKNOWN";
        }
    }
    
    // Helper: Convert satuan ke string
    string getSatuanString() const {
        switch (satuan) {
            case SatuanHarga::PER_EVENT: return "Per Event";
            case SatuanHarga::PER_ORANG: return "Per Orang";
            case SatuanHarga::PER_KG: return "Per Kg";
            case SatuanHarga::PER_HARI: return "Per Hari";
            case SatuanHarga::PER_SESI: return "Per Sesi";
            case SatuanHarga::PER_TRIP: return "Per Trip";
            case SatuanHarga::PER_JAM: return "Per Jam";
            default: return "Unknown";
        }
    }
    
    // Static: string ke kategori
    static KategoriLayanan stringKeKategori(const string& str) {
        string upper = PengelolaFile::toUpper(str);
        if (upper == "BALLROOM") return KategoriLayanan::BALLROOM;
        if (upper == "RESTAURANT") return KategoriLayanan::RESTAURANT;
        if (upper == "LAUNDRY") return KategoriLayanan::LAUNDRY;
        if (upper == "KOLAM_RENANG") return KategoriLayanan::KOLAM_RENANG;
        if (upper == "GYM") return KategoriLayanan::GYM;
        if (upper == "TRANSFER_BANDARA") return KategoriLayanan::TRANSFER_BANDARA;
        if (upper == "CATERING") return KategoriLayanan::CATERING;
        return KategoriLayanan::LAINNYA;
    }
    
    // Static: string ke satuan
    static SatuanHarga stringKeSatuan(const string& str) {
        string upper = PengelolaFile::toUpper(str);
        if (upper == "PER_EVENT" || upper == "PER EVENT") return SatuanHarga::PER_EVENT;
        if (upper == "PER_ORANG" || upper == "PER ORANG") return SatuanHarga::PER_ORANG;
        if (upper == "PER_KG" || upper == "PER KG") return SatuanHarga::PER_KG;
        if (upper == "PER_HARI" || upper == "PER HARI") return SatuanHarga::PER_HARI;
        if (upper == "PER_SESI" || upper == "PER SESI") return SatuanHarga::PER_SESI;
        if (upper == "PER_TRIP" || upper == "PER TRIP") return SatuanHarga::PER_TRIP;
        if (upper == "PER_JAM" || upper == "PER JAM") return SatuanHarga::PER_JAM;
        return SatuanHarga::PER_EVENT;
    }
    
    // Hitung total harga berdasarkan kuantitas
    virtual double hitungTotal(int kuantitas) const {
        if (kuantitas < minimumOrder) {
            return harga * minimumOrder;
        }
        return harga * kuantitas;
    }
    
    // Tampilkan info layanan
    virtual void tampilkanInfo() const {
        cout << "\n=== Informasi Layanan ===" << endl;
        cout << "ID Layanan    : " << idLayanan << endl;
        cout << "Nama          : " << namaLayanan << endl;
        cout << "Kategori      : " << getKategoriString() << endl;
        cout << "Harga         : " << PengelolaFile::formatRupiahLengkap(harga) 
             << " " << getSatuanString() << endl;
        cout << "Status        : " << (tersedia ? "Tersedia" : "Tidak Tersedia") << endl;
        cout << "Min. Order    : " << minimumOrder << endl;
        cout << "Deskripsi     : " << deskripsi << endl;
    }
    
    // Override operator <<
    friend ostream& operator<<(ostream& os, const Layanan& l) {
        os << l.idLayanan << " - " << l.namaLayanan 
           << " - " << PengelolaFile::formatRupiah(l.harga) 
           << " " << l.getSatuanString();
        return os;
    }
    
    // Format untuk disimpan ke file
    virtual string toFileFormat() const {
        string fields[8];
        fields[0] = getKategoriString();
        fields[1] = idLayanan;
        fields[2] = namaLayanan;
        fields[3] = to_string(static_cast<long long>(harga));
        fields[4] = getSatuanString();
        fields[5] = (tersedia ? "1" : "0");
        fields[6] = to_string(minimumOrder);
        fields[7] = deskripsi;
        
        return PengelolaFile::gabungField(fields, 8);
    }
};

// ============================================================================
// BALLROOM - Ruang serbaguna untuk event
// ============================================================================
class LayananBallroom : public Layanan {
private:
    int kapasitas;
    bool adaProyektor;
    bool adaSoundSystem;

public:
    LayananBallroom(const string& _id, const string& _nama, double _harga,
                    int _kapasitas, bool _proyektor = true, bool _sound = true)
        : Layanan(_id, _nama, KategoriLayanan::BALLROOM, _harga, 
                  SatuanHarga::PER_EVENT, 
                  "Ballroom untuk wedding, seminar, meeting - Kapasitas: " + 
                  to_string(_kapasitas) + " orang", 1),
          kapasitas(_kapasitas), adaProyektor(_proyektor), 
          adaSoundSystem(_sound) {}
    
    int getKapasitas() const { return kapasitas; }
    
    void tampilkanInfo() const override {
        Layanan::tampilkanInfo();
        cout << "Kapasitas     : " << kapasitas << " orang" << endl;
        cout << "Proyektor     : " << (adaProyektor ? "Ya" : "Tidak") << endl;
        cout << "Sound System  : " << (adaSoundSystem ? "Ya" : "Tidak") << endl;
    }
};

// ============================================================================
// RESTAURANT - Layanan makanan & minuman
// ============================================================================
class LayananRestaurant : public Layanan {
private:
    string jenisMenu; // Prasmanan, A la carte, Set menu

public:
    LayananRestaurant(const string& _id, const string& _nama, double _harga,
                      const string& _jenisMenu = "Prasmanan")
        : Layanan(_id, _nama, KategoriLayanan::RESTAURANT, _harga,
                  SatuanHarga::PER_ORANG, 
                  "Fine dining restaurant - Menu: " + _jenisMenu, 1),
          jenisMenu(_jenisMenu) {}
    
    string getJenisMenu() const { return jenisMenu; }
};

// ============================================================================
// LAUNDRY - Layanan cuci & setrika
// ============================================================================
class LayananLaundry : public Layanan {
private:
    int waktuPengerjaanHari; // Berapa hari selesai

public:
    LayananLaundry(const string& _id, double _harga, int _waktuHari = 2)
        : Layanan(_id, "Laundry Service", KategoriLayanan::LAUNDRY, _harga,
                  SatuanHarga::PER_KG, 
                  "Cuci + setrika + pewangi - Selesai " + to_string(_waktuHari) + " hari", 1),
          waktuPengerjaanHari(_waktuHari) {}
    
    int getWaktuPengerjaan() const { return waktuPengerjaanHari; }
};

// ============================================================================
// KOLAM RENANG - Akses ke fasilitas kolam renang
// ============================================================================
class LayananKolamRenang : public Layanan {
private:
    string jamOperasional;

public:
    LayananKolamRenang(const string& _id, double _harga, 
                       const string& _jamOps = "06:00 - 22:00")
        : Layanan(_id, "Swimming Pool Access", KategoriLayanan::KOLAM_RENANG, 
                  _harga, SatuanHarga::PER_HARI,
                  "Akses kolam renang - Jam operasional: " + _jamOps, 1),
          jamOperasional(_jamOps) {}
    
    string getJamOperasional() const { return jamOperasional; }
};

// ============================================================================
// GYM & FITNESS - Layanan kebugaran
// ============================================================================
class LayananGym : public Layanan {
private:
    bool personalTrainer;

public:
    LayananGym(const string& _id, double _harga, bool _trainer = false)
        : Layanan(_id, "Gym & Fitness Center", KategoriLayanan::GYM, _harga,
                  SatuanHarga::PER_SESI,
                  "Akses gym lengkap" + 
                  string(_trainer ? " + Personal Trainer" : ""), 1),
          personalTrainer(_trainer) {}
    
    bool hasPersonalTrainer() const { return personalTrainer; }
};

// ============================================================================
// TRANSFER BANDARA - Layanan antar-jemput bandara
// ============================================================================
class LayananTransferBandara : public Layanan {
private:
    string jenisKendaraan; // Sedan, SUV, Van

public:
    LayananTransferBandara(const string& _id, double _harga,
                           const string& _kendaraan = "Sedan")
        : Layanan(_id, "Airport Transfer", KategoriLayanan::TRANSFER_BANDARA,
                  _harga, SatuanHarga::PER_TRIP,
                  "Layanan antar-jemput bandara - Kendaraan: " + _kendaraan + 
                  " (24/7 available)", 1),
          jenisKendaraan(_kendaraan) {}
    
    string getJenisKendaraan() const { return jenisKendaraan; }
};

// ============================================================================
// CATERING - Layanan katering untuk event
// ============================================================================
class LayananCatering : public Layanan {
private:
    string paketMenu;

public:
    LayananCatering(const string& _id, double _harga,
                    const string& _paketMenu = "Paket Premium", int _minOrder = 20)
        : Layanan(_id, "Catering Service", KategoriLayanan::CATERING, _harga,
                  SatuanHarga::PER_ORANG,
                  "Layanan catering untuk event - " + _paketMenu, _minOrder),
          paketMenu(_paketMenu) {}
    
    string getPaketMenu() const { return paketMenu; }
};

#endif
