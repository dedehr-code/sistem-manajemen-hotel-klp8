#ifndef LAPORAN_KEUANGAN_H
#define LAPORAN_KEUANGAN_H

#include <iostream>
#include <string>
#include "CircularLinkedList.h"
#include "PengelolaFile.h"
#include "Transaksi.h"
#include "SistemPembayaran.h"
using namespace std;

// ============================================================================
// LAPORAN KEUANGAN - DETAIL REPORTING BY PERIOD
// ============================================================================
// Generate laporan: Harian, Mingguan, Bulanan dengan summary & breakdown
// ============================================================================

class LaporanKeuangan {
private:
    SistemPembayaran* sistemPembayaran;
    
    // Helper: Parse tanggal DD/MM/YYYY ke integer YYYYMMDD untuk comparison
    int parseTanggalKeInt(const string& tanggal) const {
        // Format: DD/MM/YYYY ? YYYYMMDD
        if (tanggal.length() < 10) return 0;
        
        string hari = tanggal.substr(0, 2);
        string bulan = tanggal.substr(3, 2);
        string tahun = tanggal.substr(6, 4);
        
        try {
            int y = stoi(tahun);
            int m = stoi(bulan);
            int d = stoi(hari);
            return y * 10000 + m * 100 + d;
        } catch (...) {
            return 0;
        }
    }
    
    // Helper: Cek apakah tanggal dalam range
    bool dalamRange(const string& tanggal, int tanggalMulai, int tanggalSelesai) const {
        int tgl = parseTanggalKeInt(tanggal);
        return tgl >= tanggalMulai && tgl <= tanggalSelesai;
    }
    
    // Helper: Get tanggal hari ini (simplified - gunakan tanggal sistem)
    string getTanggalHariIni() const {
        // Simplified: Return fixed date atau bisa pakai <ctime>
        return "03/01/2026";
    }
    
    // Helper: Calculate range untuk periode
    void hitungRangeTanggal(const string& periode, int& mulai, int& selesai) {
        // Simplified calculation
        if (periode == "hari_ini") {
            mulai = selesai = parseTanggalKeInt(getTanggalHariIni());
        } else if (periode == "7_hari") {
            selesai = parseTanggalKeInt(getTanggalHariIni());
            mulai = selesai - 7; // Simplified
        } else if (periode == "minggu_ini") {
            selesai = parseTanggalKeInt(getTanggalHariIni());
            mulai = selesai - 7;
        } else if (periode == "bulan_ini") {
            selesai = parseTanggalKeInt(getTanggalHariIni());
            mulai = 20260101; // Awal bulan (simplified)
        }
    }

public:
    LaporanKeuangan(SistemPembayaran* sp) : sistemPembayaran(sp) {}
    
    // Generate laporan by custom date range
    void laporanByRange(const string& tanggalMulai, const string& tanggalSelesai) {
        int tglMulai = parseTanggalKeInt(tanggalMulai);
        int tglSelesai = parseTanggalKeInt(tanggalSelesai);
        
        cout << "\n+------------------------------------------------------------+" << endl;
        cout << "¦          LAPORAN KEUANGAN - CUSTOM RANGE                   ¦" << endl;
        cout << "¦------------------------------------------------------------¦" << endl;
        cout << "¦  Period: " << tanggalMulai << " s/d " << tanggalSelesai 
             << string(26 - tanggalMulai.length() - tanggalSelesai.length(), ' ') << "¦" << endl;
        cout << "+------------------------------------------------------------+" << endl;
        
        // Data aggregation
        int totalTransaksi = 0;
        double totalPendapatan = 0.0;
        double pendapatanKamar = 0.0;
        double pendapatanLayanan = 0.0;
        int jumlahKamarTerjual = 0;
        int jumlahLayananTerjual = 0;
        
        // Iterate transaksi
        CircularLinkedList<Transaksi*>& daftarTransaksi = sistemPembayaran->getDaftarTransaksi();
        
        daftarTransaksi.iterasi([&](Transaksi* t) {
            if (dalamRange(t->getTanggalTransaksi(), tglMulai, tglSelesai)) {
                if (t->getStatus() == StatusTransaksi::CONFIRMED || 
                    t->getStatus() == StatusTransaksi::COMPLETED) {
                    
                    totalTransaksi++;
                    totalPendapatan += t->getTotalAkhir();
                    
                    // Breakdown by item type (simplified - bisa iterate items)
                    jumlahKamarTerjual++;
                    pendapatanKamar += t->getTotalAkhir() * 0.7; // Estimasi 70% dari kamar
                    pendapatanLayanan += t->getTotalAkhir() * 0.3; // 30% dari layanan
                }
            }
        });
        
        // Display summary
        cout << "\n+--------------------------- SUMMARY ------------------------+" << endl;
        cout << "¦ Total Transaksi         : " << totalTransaksi << string(32, ' ') << "¦" << endl;
        cout << "¦ Total Pendapatan        : " 
             << PengelolaFile::formatRupiahLengkap(totalPendapatan).substr(0, 32)
             << string(max(0, 32 - (int)PengelolaFile::formatRupiahLengkap(totalPendapatan).length()), ' ') << "¦" << endl;
        
        if (totalTransaksi > 0) {
            double rata = totalPendapatan / totalTransaksi;
            cout << "¦ Rata-rata per Transaksi : " 
                 << PengelolaFile::formatRupiah(rata).substr(0, 32)
                 << string(max(0, 32 - (int)PengelolaFile::formatRupiah(rata).length()), ' ') << "¦" << endl;
        }
        
        cout << "+------------------------------------------------------------+" << endl;
        
        cout << "\n+----------------------- BREAKDOWN --------------------------+" << endl;
        cout << "¦ Pendapatan Kamar        : " 
             << PengelolaFile::formatRupiah(pendapatanKamar).substr(0, 32)
             << string(max(0, 32 - (int)PengelolaFile::formatRupiah(pendapatanKamar).length()), ' ') << "¦" << endl;
        cout << "¦ Jumlah Kamar Terjual    : " << jumlahKamarTerjual << string(32, ' ') << "¦" << endl;
        cout << "¦ Pendapatan Layanan      : " 
             << PengelolaFile::formatRupiah(pendapatanLayanan).substr(0, 32)
             << string(max(0, 32 - (int)PengelolaFile::formatRupiah(pendapatanLayanan).length()), ' ') << "¦" << endl;
        cout << "¦ Jumlah Layanan Terjual  : " << jumlahLayananTerjual << string(32, ' ') << "¦" << endl;
        cout << "+------------------------------------------------------------+" << endl;
        
        if (totalTransaksi == 0) {
            cout << "\n[INFO] Tidak ada transaksi dalam periode ini." << endl;
        }
    }
    
    // Laporan harian (hari ini)
    void laporanHarian() {
        cout << "\n+------------------------------------------------------------+" << endl;
        cout << "¦          LAPORAN KEUANGAN HARIAN                           ¦" << endl;
        cout << "+------------------------------------------------------------+" << endl;
        
        cout << "\n1. Hari Ini" << endl;
        cout << "2. Kemarin" << endl;
        cout << "3. 7 Hari Terakhir" << endl;
        cout << "4. Custom Tanggal" << endl;
        cout << "\nPilih: ";
        
        int pilih; cin >> pilih; cin.ignore();
        
        if (pilih == 1) {
            string today = getTanggalHariIni();
            laporanByRange(today, today);
        } else if (pilih == 2) {
            // Simplified: assume kemarin = 02/01/2026
            laporanByRange("02/01/2026", "02/01/2026");
        } else if (pilih == 3) {
            laporanByRange("27/12/2025", getTanggalHariIni());
        } else if (pilih == 4) {
            cout << "Tanggal mulai (DD/MM/YYYY): "; string mulai; getline(cin, mulai);
            cout << "Tanggal selesai (DD/MM/YYYY): "; string selesai; getline(cin, selesai);
            laporanByRange(mulai, selesai);
        }
    }
    
    // Laporan mingguan
    void laporanMingguan() {
        cout << "\n+------------------------------------------------------------+" << endl;
        cout << "¦          LAPORAN KEUANGAN MINGGUAN                         ¦" << endl;
        cout << "+------------------------------------------------------------+" << endl;
        
        cout << "\n1. Minggu Ini (7 hari terakhir)" << endl;
        cout << "2. Minggu Lalu" << endl;
        cout << "3. 30 Hari Terakhir" << endl;
        cout << "\nPilih: ";
        
        int pilih; cin >> pilih; cin.ignore();
        
        if (pilih == 1) {
            laporanByRange("27/12/2025", getTanggalHariIni());
        } else if (pilih == 2) {
            laporanByRange("20/12/2025", "26/12/2025");
        } else if (pilih == 3) {
            laporanByRange("04/12/2025", getTanggalHariIni());
        }
    }
    
    // Laporan bulanan
    void laporanBulanan() {
        cout << "\n+------------------------------------------------------------+" << endl;
        cout << "¦          LAPORAN KEUANGAN BULANAN                          ¦" << endl;
        cout << "+------------------------------------------------------------+" << endl;
        
        cout << "\n1. Bulan Ini (Januari 2026)" << endl;
        cout << "2. Bulan Lalu (Desember 2025)" << endl;
        cout << "3. 3 Bulan Terakhir (Q4 2025 + Jan 2026)" << endl;
        cout << "\nPilih: ";
        
        int pilih; cin >> pilih; cin.ignore();
        
        if (pilih == 1) {
            laporanByRange("01/01/2026", "31/01/2026");
        } else if (pilih == 2) {
            laporanByRange("01/12/2025", "31/12/2025");
        } else if (pilih == 3) {
            laporanByRange("01/10/2025", "31/01/2026");
        }
    }
    
    // Menu laporan lengkap
    void menuLaporan() {
        int pilihan;
        do {
            cout << "\n+------------------------------------------------------------+" << endl;
            cout << "¦          LAPORAN KEUANGAN - ADVANCED                       ¦" << endl;
            cout << "¦------------------------------------------------------------¦" << endl;
            cout << "¦  1. Laporan Harian                                         ¦" << endl;
            cout << "¦  2. Laporan Mingguan                                       ¦" << endl;
            cout << "¦  3. Laporan Bulanan                                        ¦" << endl;
            cout << "¦  4. Custom Range                                           ¦" << endl;
            cout << "¦  0. Kembali                                                ¦" << endl;
            cout << "+------------------------------------------------------------+" << endl;
            cout << "Pilihan: "; cin >> pilihan; cin.ignore();
            
            switch(pilihan) {
                case 1: laporanHarian(); break;
                case 2: laporanMingguan(); break;
                case 3: laporanBulanan(); break;
                case 4: {
                    cout << "\nTanggal mulai (DD/MM/YYYY): "; 
                    string mulai; getline(cin, mulai);
                    cout << "Tanggal selesai (DD/MM/YYYY): "; 
                    string selesai; getline(cin, selesai);
                    laporanByRange(mulai, selesai);
                    break;
                }
                case 0: cout << "\n[INFO] Kembali ke menu utama.\n"; break;
                default: cout << "[ERROR] Pilihan tidak valid!\n";
            }
        } while(pilihan != 0);
    }
    
    // Export laporan ke file (bonus feature)
    bool exportLaporanToFile(const string& tanggalMulai, const string& tanggalSelesai, 
                            const string& namaFile = "laporan_keuangan.txt") {
        
        // Simplified: Write summary to file
        string baris[100];
        int idx = 0;
        
        baris[idx++] = "========================================";
        baris[idx++] = "LAPORAN KEUANGAN - HOTEL PARADISE";
        baris[idx++] = "Period: " + tanggalMulai + " s/d " + tanggalSelesai;
        baris[idx++] = "Generated: " + getTanggalHariIni();
        baris[idx++] = "========================================";
        baris[idx++] = "";
        
        // Calculate data
        int totalTransaksi = 0;
        double totalPendapatan = 0.0;
        
        int tglMulai = parseTanggalKeInt(tanggalMulai);
        int tglSelesai = parseTanggalKeInt(tanggalSelesai);
        
        sistemPembayaran->getDaftarTransaksi().iterasi([&](Transaksi* t) {
            if (dalamRange(t->getTanggalTransaksi(), tglMulai, tglSelesai)) {
                if (t->getStatus() == StatusTransaksi::CONFIRMED || 
                    t->getStatus() == StatusTransaksi::COMPLETED) {
                    totalTransaksi++;
                    totalPendapatan += t->getTotalAkhir();
                }
            }
        });
        
        baris[idx++] = "Total Transaksi: " + to_string(totalTransaksi);
        baris[idx++] = "Total Pendapatan: " + PengelolaFile::formatRupiahLengkap(totalPendapatan);
        baris[idx++] = "";
        baris[idx++] = "========================================";
        
        bool success = PengelolaFile::tulisSemuaBaris(namaFile, baris, idx);
        
        if (success) {
            cout << "\n[SUKSES] Laporan berhasil di-export ke: " << namaFile << endl;
        } else {
            cout << "\n[ERROR] Gagal export laporan!" << endl;
        }
        
        return success;
    }
};

#endif
