#ifndef SISTEM_PEMBAYARAN_H
#define SISTEM_PEMBAYARAN_H

#include <iostream>
#include <string>
#include "CircularLinkedList.h"
#include "Stack.h"
#include "PengelolaFile.h"
#include "Transaksi.h"
#include "Pengguna.h"
using namespace std;

// ============================================================================
// SISTEM PEMBAYARAN - PAYMENT PROCESSING
// ============================================================================
// Mengelola: Proses pembayaran, Generate struk, History transaksi
// ============================================================================

class SistemPembayaran {
private:
    CircularLinkedList<Transaksi*> daftarTransaksi;
    Stack<string> historyPembayaran; // Stack untuk history (struk ID)
    string namaFileTransaksi;
    int counterTransaksi;
    
public:
    // Constructor
    SistemPembayaran(const string& namaFile = "transaksi.txt")
        : namaFileTransaksi(namaFile), counterTransaksi(1) {
        PengelolaFile::buatFileJikaBelumAda(namaFileTransaksi);
    }
    
    // Destructor
    ~SistemPembayaran() {
        daftarTransaksi.iterasi([](Transaksi* t) {
            delete t;
        });
    }
    
    // Proses pembayaran
    bool prosesPembayaran(Transaksi* transaksi, MetodePembayaran metode) {
        if (transaksi == nullptr) {
            cout << "[ERROR] Transaksi tidak valid!" << endl;
            return false;
        }
        
        // Set metode pembayaran
        transaksi->setMetodeBayar(metode);
        
        // Simulasi proses pembayaran
        cout << "\n+------------------------------------------------------------+" << endl;
        cout << "¦          MEMPROSES PEMBAYARAN...                           ¦" << endl;
        cout << "+------------------------------------------------------------+" << endl;
        
        cout << "\nMetode Pembayaran: " << transaksi->getMetodeBayarString() << endl;
        cout << "Total Pembayaran : " << PengelolaFile::formatRupiahLengkap(transaksi->getTotalAkhir()) << endl;
        
        // Simulasi delay (real system akan connect ke payment gateway)
        cout << "\nMemverifikasi pembayaran";
        for (int i = 0; i < 3; i++) {
            cout << ".";
            cout.flush();
            // Sleep simulation (simplified)
        }
        cout << endl;
        
        // Update status transaksi
        transaksi->setStatus(StatusTransaksi::CONFIRMED);
        
        // Simpan transaksi
        daftarTransaksi.tambah(transaksi);
        
        // Simpan ke file
        simpanTransaksi(transaksi);
        
        // Push ke history stack
        historyPembayaran.push(transaksi->getIdTransaksi());
        
        cout << "\n? PEMBAYARAN BERHASIL!" << endl;
        cout << "ID Transaksi: " << transaksi->getIdTransaksi() << endl;
        
        return true;
    }
    
    // Generate dan tampilkan struk
    void generateStruk(Transaksi* transaksi) {
        if (transaksi == nullptr) {
            cout << "[ERROR] Transaksi tidak valid!" << endl;
            return;
        }
        
        transaksi->tampilkanStruk();
        
        // Tanya mau simpan struk ke file atau tidak
        cout << "\nStruk telah ditampilkan. ID Transaksi: " << transaksi->getIdTransaksi() << endl;
    }
    
    // Simpan transaksi ke file (append)
    bool simpanTransaksi(Transaksi* transaksi) {
        if (transaksi == nullptr) return false;
        
        // Format: ID|IDPelanggan|NamaPelanggan|Status|TotalAkhir|Tanggal|JumlahItem
        string fields[10];
        fields[0] = transaksi->getIdTransaksi();
        fields[1] = transaksi->getIdPelanggan();
        fields[2] = transaksi->getNamaPelanggan();
        fields[3] = transaksi->getStatusString();
        fields[4] = to_string(static_cast<long long>(transaksi->getTotalAkhir()));
        fields[5] = transaksi->getTanggalTransaksi();
        fields[6] = to_string(transaksi->getJumlahItem());
        fields[7] = transaksi->getMetodeBayarString();
        fields[8] = transaksi->getTanggalCheckIn();
        fields[9] = transaksi->getTanggalCheckOut();
        
        string line = PengelolaFile::gabungField(fields, 10);
        
        return PengelolaFile::tambahBaris(namaFileTransaksi, line);
    }
    
    // Simpan SEMUA transaksi ke file (untuk update/sync)
    bool simpanSemuaTransaksi() {
        string baris[1000];
        int jumlah = 0;
        
        daftarTransaksi.iterasi([&](Transaksi* t) {
            if (jumlah >= 1000) return; // Safety limit
            
            string fields[10];
            fields[0] = t->getIdTransaksi();
            fields[1] = t->getIdPelanggan();
            fields[2] = t->getNamaPelanggan();
            fields[3] = t->getStatusString();
            fields[4] = to_string(static_cast<long long>(t->getTotalAkhir()));
            fields[5] = t->getTanggalTransaksi();
            fields[6] = to_string(t->getJumlahItem());
            fields[7] = t->getMetodeBayarString();
            fields[8] = t->getTanggalCheckIn();
            fields[9] = t->getTanggalCheckOut();
            
            baris[jumlah] = PengelolaFile::gabungField(fields, 10);
            jumlah++;
        });
        
        return PengelolaFile::tulisSemuaBaris(namaFileTransaksi, baris, jumlah);
    }
    
    // Load semua transaksi dari file
    bool muatDataTransaksi() {
        string baris[1000];
        int jumlah = PengelolaFile::bacaSemuaBaris(namaFileTransaksi, baris, 1000);
        
        if (jumlah == 0) {
            return true; // File kosong, tidak perlu print
        }
        
        int maxId = 0;
        int loaded = 0;
        
        // Parse dan load setiap transaksi
        for (int i = 0; i < jumlah; i++) {
            string fields[15];
            int fieldCount;
            PengelolaFile::split(baris[i], '|', fields, fieldCount);
            
            if (fieldCount < 9) continue; // Skip baris yang tidak valid
            
            // Parse fields: ID|IDPelanggan|Nama|Status|Total|Tanggal|JumlahItem|Metode|CheckIn|CheckOut
            string id = PengelolaFile::trim(fields[0]);
            string idPelanggan = PengelolaFile::trim(fields[1]);
            string namaPelanggan = PengelolaFile::trim(fields[2]);
            string statusStr = PengelolaFile::trim(fields[3]);
            double total = stod(PengelolaFile::trim(fields[4]));
            string tanggal = PengelolaFile::trim(fields[5]);
            int jumlahItem = stoi(PengelolaFile::trim(fields[6]));
            string metodeStr = PengelolaFile::trim(fields[7]);
            string checkIn = (fieldCount > 8) ? PengelolaFile::trim(fields[8]) : "";
            string checkOut = (fieldCount > 9) ? PengelolaFile::trim(fields[9]) : "";
            
            // Parse status
            StatusTransaksi status = StatusTransaksi::PENDING;
            if (statusStr == "CONFIRMED") status = StatusTransaksi::CONFIRMED;
            else if (statusStr == "COMPLETED") status = StatusTransaksi::COMPLETED;
            else if (statusStr == "CANCELLED") status = StatusTransaksi::CANCELLED;
            
            // Parse metode pembayaran
            MetodePembayaran metode = MetodePembayaran::TUNAI;
            if (metodeStr == "TRANSFER_BANK") metode = MetodePembayaran::TRANSFER_BANK;
            else if (metodeStr == "KARTU_KREDIT") metode = MetodePembayaran::KARTU_KREDIT;
            else if (metodeStr == "EWALLET") metode = MetodePembayaran::EWALLET;
            
            // Buat transaksi baru dengan constructor yang benar (4 params)
            Transaksi* t = new Transaksi(id, idPelanggan, namaPelanggan, tanggal);
            t->setStatus(status);
            t->setTanggalCheckIn(checkIn);
            t->setTanggalCheckOut(checkOut);
            t->setJumlahItemDariFile(jumlahItem);
            t->setMetodeBayar(metode);
            
            // ? SET TOTAL DARI FILE! (CRITICAL FIX)
            // Total dari file sudah include PPN + Service
            // Reverse calculate: total = subtotal * 1.15
            double subtotal = total / 1.15;
            double ppn = subtotal * 0.10;
            double service = subtotal * 0.05;
            t->setTotalDariFile(subtotal, ppn, service, total);
            
            // Tambahkan ke daftar
            daftarTransaksi.tambah(t);
            loaded++;
            
            // ? FIX COUNTER - Track max ID untuk auto-increment
            if (id.length() > 1 && id[0] == 'T') {
                try {
                    int num = stoi(id.substr(1));
                    if (num > maxId) maxId = num;
                } catch (...) {
                    // Skip jika parse gagal
                }
            }
        }
        
        // ? SET COUNTER PROPERLY
        counterTransaksi = maxId + 1;
        
        // Silent load - tidak perlu print
        return true;
    }
    
    // Lihat history transaksi (menggunakan Stack)
    void tampilkanHistoryPembayaran() const {
        cout << "\n+------------------------------------------------------------+" << endl;
        cout << "¦          HISTORY PEMBAYARAN (STACK - Recent First)        ¦" << endl;
        cout << "+------------------------------------------------------------+" << endl;
        
        if (historyPembayaran.kosong()) {
            cout << "\n[INFO] Belum ada history pembayaran." << endl;
            return;
        }
        
        historyPembayaran.tampilkan();
    }
    
    // Lihat semua transaksi customer
    void tampilkanTransaksiCustomer(const string& idPelanggan) const {
        bool found = false;
        int nomor = 1;
        
        cout << "\n+------------------------------------------------------------+" << endl;
        cout << "¦          RIWAYAT TRANSAKSI                                 ¦" << endl;
        cout << "+------------------------------------------------------------+" << endl;
        
        daftarTransaksi.iterasi([&](Transaksi* t) {
            if (t->getIdPelanggan() == idPelanggan) {
                found = true;
                cout << "\n[" << nomor++ << "] " << t->getIdTransaksi() << endl;
                cout << "Tanggal     : " << t->getTanggalTransaksi() << endl;
                cout << "Status      : " << t->getStatusString() << endl;
                cout << "Jumlah Item : " << t->getJumlahItem() << endl;
                cout << "Total Bayar : " << PengelolaFile::formatRupiahLengkap(t->getTotalAkhir()) << endl;
                cout << "Metode Bayar: " << t->getMetodeBayarString() << endl;
                cout << "-----------------------------------------------------------" << endl;
            }
        });
        
        if (!found) {
            cout << "\n[INFO] Belum ada transaksi." << endl;
        }
    }
    
    // Lihat semua transaksi (untuk owner/pegawai)
    void tampilkanSemuaTransaksi() const {
        if (daftarTransaksi.kosong()) {
            cout << "[INFO] Belum ada transaksi." << endl;
            return;
        }
        
        cout << "\n+------------------------------------------------------------+" << endl;
        cout << "¦          SEMUA TRANSAKSI                                   ¦" << endl;
        cout << "+------------------------------------------------------------+" << endl;
        
        int nomor = 1;
        daftarTransaksi.iterasi([&](Transaksi* t) {
            cout << "\n[" << nomor++ << "] " << t->getIdTransaksi() << endl;
            cout << "Customer    : " << t->getNamaPelanggan() << " (" << t->getIdPelanggan() << ")" << endl;
            cout << "Tanggal     : " << t->getTanggalTransaksi() << endl;
            cout << "Status      : " << t->getStatusString() << endl;
            cout << "Total       : " << PengelolaFile::formatRupiahLengkap(t->getTotalAkhir()) << endl;
            cout << "-----------------------------------------------------------" << endl;
        });
    }
    
    // Update status transaksi (untuk pegawai)
    bool updateStatusTransaksi(const string& idTransaksi, StatusTransaksi statusBaru) {
        Transaksi* transaksi = nullptr;
        
        daftarTransaksi.iterasi([&](Transaksi* t) {
            if (t->getIdTransaksi() == idTransaksi) {
                transaksi = t;
            }
        });
        
        if (transaksi == nullptr) {
            cout << "[ERROR] Transaksi dengan ID " << idTransaksi << " tidak ditemukan!" << endl;
            return false;
        }
        
        transaksi->setStatus(statusBaru);
        
        // ? AUTO SAVE TO FILE AFTER UPDATE! (CRITICAL FIX)
        simpanSemuaTransaksi();
        
        cout << "[SUKSES] Status transaksi " << idTransaksi << " diubah menjadi: " 
             << transaksi->getStatusString() << endl;
        
        return true;
    }
    
    // Statistik keuangan (untuk owner)
    void tampilkanStatistikKeuangan() const {
        if (daftarTransaksi.kosong()) {
            cout << "[INFO] Belum ada data transaksi untuk statistik." << endl;
            return;
        }
        
        double totalPendapatan = 0.0;
        int totalTransaksiSelesai = 0;
        int totalTransaksiPending = 0;
        int totalTransaksiBatal = 0;
        
        daftarTransaksi.iterasi([&](Transaksi* t) {
            if (t->getStatus() == StatusTransaksi::CONFIRMED || 
                t->getStatus() == StatusTransaksi::COMPLETED) {
                totalPendapatan += t->getTotalAkhir();
                totalTransaksiSelesai++;
            } else if (t->getStatus() == StatusTransaksi::PENDING) {
                totalTransaksiPending++;
            } else if (t->getStatus() == StatusTransaksi::CANCELLED) {
                totalTransaksiBatal++;
            }
        });
        
        double rataRataTransaksi = (totalTransaksiSelesai > 0) ? 
                                    totalPendapatan / totalTransaksiSelesai : 0.0;
        
        cout << "\n+------------------------------------------------------------+" << endl;
        cout << "¦          STATISTIK KEUANGAN                                ¦" << endl;
        cout << "¦------------------------------------------------------------¦" << endl;
        cout << "¦  Total Pendapatan    : " 
             << PengelolaFile::formatRupiahLengkap(totalPendapatan).substr(0, 35)
             << string(35 - min(35, (int)PengelolaFile::formatRupiahLengkap(totalPendapatan).length()), ' ') << "¦" << endl;
        cout << "¦  Transaksi Selesai   : " << totalTransaksiSelesai << string(35, ' ') << "¦" << endl;
        cout << "¦  Transaksi Pending   : " << totalTransaksiPending << string(35, ' ') << "¦" << endl;
        cout << "¦  Transaksi Dibatal   : " << totalTransaksiBatal << string(35, ' ') << "¦" << endl;
        cout << "¦  Rata-rata/Transaksi : " 
             << PengelolaFile::formatRupiah(rataRataTransaksi).substr(0, 35)
             << string(35 - min(35, (int)PengelolaFile::formatRupiah(rataRataTransaksi).length()), ' ') << "¦" << endl;
        cout << "+------------------------------------------------------------+" << endl;
    }
    
    // Generate ID transaksi baru
    string generateIDTransaksi() {
        return "T" + PengelolaFile::generateID("", counterTransaksi++);
    }
    
    // Get daftar transaksi
    CircularLinkedList<Transaksi*>& getDaftarTransaksi() {
        return daftarTransaksi;
    }
};

#endif
