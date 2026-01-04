#ifndef PENGGUNA_H
#define PENGGUNA_H

#include <iostream>
#include <string>
#include "PengelolaFile.h"
using namespace std;

// ============================================================================
// PENGGUNA - BASE CLASS & TURUNAN
// ============================================================================
// Hierarchy: Pengguna ? Pelanggan, Pegawai, Pemilik
// ============================================================================

// Enum untuk role (untuk keamanan tipe)
enum class RolePengguna {
    PELANGGAN,
    PEGAWAI,
    PEMILIK
};

// Base class: Pengguna
class Pengguna {
protected:
    string id;
    string nama;
    string email;
    string nomorTelepon;
    string password;
    RolePengguna role;
    bool aktif; // Status akun

public:
    // Constructor
    Pengguna(const string& _id, const string& _nama, const string& _email,
             const string& _nomorTelepon, const string& _password, 
             RolePengguna _role)
        : id(_id), nama(_nama), email(_email), nomorTelepon(_nomorTelepon),
          password(_password), role(_role), aktif(true) {}
    
    // Virtual destructor untuk polymorphism
    virtual ~Pengguna() {}
    
    // Getter
    string getId() const { return id; }
    string getNama() const { return nama; }
    string getEmail() const { return email; }
    string getNomorTelepon() const { return nomorTelepon; }
    string getPassword() const { return password; }
    RolePengguna getRole() const { return role; }
    bool isAktif() const { return aktif; }
    
    // Setter
    void setNama(const string& _nama) { nama = _nama; }
    void setEmail(const string& _email) { email = _email; }
    void setNomorTelepon(const string& _nomor) { nomorTelepon = _nomor; }
    void setPassword(const string& _password) { password = _password; }
    void setAktif(bool _aktif) { aktif = _aktif; }
    
    // Helper: Convert role ke string
    string getRoleString() const {
        switch (role) {
            case RolePengguna::PELANGGAN: return "PELANGGAN";
            case RolePengguna::PEGAWAI: return "PEGAWAI";
            case RolePengguna::PEMILIK: return "PEMILIK";
            default: return "UNKNOWN";
        }
    }
    
    // Virtual function untuk display info (polymorphism)
    virtual void tampilkanInfo() const {
        cout << "\n=== Informasi Pengguna ===" << endl;
        cout << "ID           : " << id << endl;
        cout << "Nama         : " << nama << endl;
        cout << "Email        : " << email << endl;
        cout << "No. Telepon  : " << nomorTelepon << endl;
        cout << "Role         : " << getRoleString() << endl;
        cout << "Status       : " << (aktif ? "Aktif" : "Non-aktif") << endl;
    }
    
    // Verifikasi password
    bool verifikasiPassword(const string& inputPassword) const {
        return password == inputPassword;
    }
    
    // Override operator << untuk output
    friend ostream& operator<<(ostream& os, const Pengguna& p) {
        os << p.id << " - " << p.nama << " (" << p.getRoleString() << ")";
        return os;
    }
    
    // Format untuk disimpan ke file
    virtual string toFileFormat() const {
        string fields[7];
        fields[0] = getRoleString();
        fields[1] = id;
        fields[2] = nama;
        fields[3] = email;
        fields[4] = nomorTelepon;
        fields[5] = password;
        fields[6] = (aktif ? "1" : "0");
        
        return PengelolaFile::gabungField(fields, 7);
    }
};

// ============================================================================
// PELANGGAN - Customer yang booking kamar & layanan
// ============================================================================
class Pelanggan : public Pengguna {
private:
    string alamat;
    int totalTransaksi;
    double totalBelanja;

public:
    // Constructor
    Pelanggan(const string& _id, const string& _nama, const string& _email,
              const string& _nomorTelepon, const string& _password,
              const string& _alamat = "")
        : Pengguna(_id, _nama, _email, _nomorTelepon, _password, RolePengguna::PELANGGAN),
          alamat(_alamat), totalTransaksi(0), totalBelanja(0.0) {}
    
    // Getter khusus pelanggan
    string getAlamat() const { return alamat; }
    int getTotalTransaksi() const { return totalTransaksi; }
    double getTotalBelanja() const { return totalBelanja; }
    
    // Setter
    void setAlamat(const string& _alamat) { alamat = _alamat; }
    void tambahTransaksi(double nominal) {
        totalTransaksi++;
        totalBelanja += nominal;
    }
    
    // Override tampilkanInfo
    void tampilkanInfo() const override {
        Pengguna::tampilkanInfo();
        cout << "Alamat       : " << alamat << endl;
        cout << "Total Transaksi: " << totalTransaksi << " kali" << endl;
        cout << "Total Belanja: " << PengelolaFile::formatRupiahLengkap(totalBelanja) << endl;
    }
    
    // Override toFileFormat
    string toFileFormat() const override {
        string fields[10];
        fields[0] = getRoleString();
        fields[1] = id;
        fields[2] = nama;
        fields[3] = email;
        fields[4] = nomorTelepon;
        fields[5] = password;
        fields[6] = (aktif ? "1" : "0");
        fields[7] = alamat;
        fields[8] = to_string(totalTransaksi);
        fields[9] = to_string(static_cast<long long>(totalBelanja));
        
        return PengelolaFile::gabungField(fields, 10);
    }
};

// ============================================================================
// PEGAWAI - Employee yang mengelola operasional hotel
// ============================================================================
class Pegawai : public Pengguna {
private:
    string posisi;      // Resepsionis, HouseKeeping, Manager, dll
    string shift;       // Pagi, Siang, Malam
    double gaji;
    int jumlahBookingDitangani;

public:
    // Constructor
    Pegawai(const string& _id, const string& _nama, const string& _email,
            const string& _nomorTelepon, const string& _password,
            const string& _posisi, const string& _shift = "Pagi",
            double _gaji = 5000000.0)
        : Pengguna(_id, _nama, _email, _nomorTelepon, _password, RolePengguna::PEGAWAI),
          posisi(_posisi), shift(_shift), gaji(_gaji), jumlahBookingDitangani(0) {}
    
    // Getter
    string getPosisi() const { return posisi; }
    string getShift() const { return shift; }
    double getGaji() const { return gaji; }
    int getJumlahBookingDitangani() const { return jumlahBookingDitangani; }
    
    // Setter
    void setPosisi(const string& _posisi) { posisi = _posisi; }
    void setShift(const string& _shift) { shift = _shift; }
    void setGaji(double _gaji) { gaji = _gaji; }
    void tambahBookingDitangani() { jumlahBookingDitangani++; }
    
    // Override tampilkanInfo
    void tampilkanInfo() const override {
        Pengguna::tampilkanInfo();
        cout << "Posisi       : " << posisi << endl;
        cout << "Shift        : " << shift << endl;
        cout << "Gaji         : " << PengelolaFile::formatRupiahLengkap(gaji) << endl;
        cout << "Booking Ditangani: " << jumlahBookingDitangani << " kali" << endl;
    }
    
    // Override toFileFormat
    string toFileFormat() const override {
        string fields[11];
        fields[0] = getRoleString();
        fields[1] = id;
        fields[2] = nama;
        fields[3] = email;
        fields[4] = nomorTelepon;
        fields[5] = password;
        fields[6] = (aktif ? "1" : "0");
        fields[7] = posisi;
        fields[8] = shift;
        fields[9] = to_string(static_cast<long long>(gaji));
        fields[10] = to_string(jumlahBookingDitangani);
        
        return PengelolaFile::gabungField(fields, 11);
    }
};

// ============================================================================
// PEMILIK - Owner dengan akses penuh
// ============================================================================
class Pemilik : public Pengguna {
public:
    // Constructor
    Pemilik(const string& _id, const string& _nama, const string& _email,
            const string& _nomorTelepon, const string& _password)
        : Pengguna(_id, _nama, _email, _nomorTelepon, _password, RolePengguna::PEMILIK) {}
    
    // Pemilik punya akses penuh, tidak perlu field tambahan
    
    // Override tampilkanInfo
    void tampilkanInfo() const override {
        Pengguna::tampilkanInfo();
        cout << "Hak Akses    : FULL ACCESS (Owner)" << endl;
    }
};

#endif
