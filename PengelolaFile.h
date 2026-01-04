#ifndef PENGELOLA_FILE_H
#define PENGELOLA_FILE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

// ============================================================================
// PENGELOLA FILE - UTILITY UNTUK BACA/TULIS FILE .TXT
// ============================================================================
// Format file: Setiap baris adalah 1 record, field dipisah dengan delimiter
// Contoh: PELANGGAN|P001|Budi|budi@email.com|081234567890|password123
// ============================================================================

class PengelolaFile {
public:
    // Delimiter default untuk pemisah field
    static const char DELIMITER = '|';
    
    // Helper: Trim whitespace dari string
    static string trim(const string& str) {
        size_t first = str.find_first_not_of(" \t\n\r");
        if (first == string::npos) return "";
        
        size_t last = str.find_last_not_of(" \t\n\r");
        return str.substr(first, (last - first + 1));
    }
    
    // Helper: Split string berdasarkan delimiter
    static void split(const string& str, char delim, string hasil[], int& jumlah) {
        stringstream ss(str);
        string item;
        jumlah = 0;
        
        while (getline(ss, item, delim)) {
            hasil[jumlah++] = trim(item);
        }
    }
    
    // Helper: Cek apakah file exists
    static bool fileExists(const string& namaFile) {
        ifstream file(namaFile);
        bool exists = file.good();
        file.close();
        return exists;
    }
    
    // Helper: Hitung jumlah baris dalam file
    static int hitungBaris(const string& namaFile) {
        ifstream file(namaFile);
        if (!file.is_open()) return 0;
        
        int jumlah = 0;
        string line;
        
        while (getline(file, line)) {
            if (!trim(line).empty()) {
                jumlah++;
            }
        }
        
        file.close();
        return jumlah;
    }
    
    // Baca file dan kembalikan semua baris (max 1000 baris)
    static int bacaSemuaBaris(const string& namaFile, string baris[], int maksimal = 1000) {
        ifstream file(namaFile);
        if (!file.is_open()) {
            cout << "[ERROR] Tidak dapat membuka file: " << namaFile << endl;
            return 0;
        }
        
        int jumlah = 0;
        string line;
        
        while (getline(file, line) && jumlah < maksimal) {
            line = trim(line);
            if (!line.empty()) {
                baris[jumlah++] = line;
            }
        }
        
        file.close();
        return jumlah;
    }
    
    // Tulis array string ke file (overwrite)
    static bool tulisSemuaBaris(const string& namaFile, const string baris[], int jumlah) {
        ofstream file(namaFile, ios::trunc);
        if (!file.is_open()) {
            cout << "[ERROR] Tidak dapat menulis ke file: " << namaFile << endl;
            return false;
        }
        
        for (int i = 0; i < jumlah; i++) {
            file << baris[i] << endl;
        }
        
        file.close();
        return true;
    }
    
    // Append satu baris ke file
    static bool tambahBaris(const string& namaFile, const string& baris) {
        ofstream file(namaFile, ios::app);
        if (!file.is_open()) {
            cout << "[ERROR] Tidak dapat menambah ke file: " << namaFile << endl;
            return false;
        }
        
        file << baris << endl;
        file.close();
        return true;
    }
    
    // Hapus file
    static bool hapusFile(const string& namaFile) {
        if (remove(namaFile.c_str()) != 0) {
            return false;
        }
        return true;
    }
    
    // Buat file kosong jika belum ada
    static bool buatFileJikaBelumAda(const string& namaFile) {
        if (fileExists(namaFile)) {
            return true; // File sudah ada
        }
        
        ofstream file(namaFile);
        if (!file.is_open()) {
            cout << "[ERROR] Tidak dapat membuat file: " << namaFile << endl;
            return false;
        }
        
        file.close();
        cout << "[INFO] File dibuat: " << namaFile << endl;
        return true;
    }
    
    // Gabungkan field menjadi satu string dengan delimiter
    static string gabungField(const string field[], int jumlah) {
        if (jumlah == 0) return "";
        
        string hasil = field[0];
        for (int i = 1; i < jumlah; i++) {
            hasil += DELIMITER;
            hasil += field[i];
        }
        
        return hasil;
    }
    
    // Parse tanggal format DD/MM/YYYY atau DD-MM-YYYY
    static bool parseTanggal(const string& tanggalStr, int& hari, int& bulan, int& tahun) {
        // Cari delimiter (/ atau -)
        char delim = '/';
        if (tanggalStr.find('-') != string::npos) {
            delim = '-';
        }
        
        stringstream ss(tanggalStr);
        string token;
        int parts[3];
        int idx = 0;
        
        while (getline(ss, token, delim) && idx < 3) {
            try {
                parts[idx++] = stoi(token);
            } catch (...) {
                return false;
            }
        }
        
        if (idx != 3) return false;
        
        hari = parts[0];
        bulan = parts[1];
        tahun = parts[2];
        
        // Validasi basic
        if (hari < 1 || hari > 31 || bulan < 1 || bulan > 12 || tahun < 1900) {
            return false;
        }
        
        return true;
    }
    
    // Format tanggal ke string DD/MM/YYYY
    static string formatTanggal(int hari, int bulan, int tahun) {
        string result = "";
        
        // Hari
        if (hari < 10) result += "0";
        result += to_string(hari) + "/";
        
        // Bulan
        if (bulan < 10) result += "0";
        result += to_string(bulan) + "/";
        
        // Tahun
        result += to_string(tahun);
        
        return result;
    }
    
    // Generate ID unik (simple: PREFIX + counter)
    static string generateID(const string& prefix, int counter) {
        string id = prefix;
        
        // Tambah leading zeros
        if (counter < 10) id += "00";
        else if (counter < 100) id += "0";
        
        id += to_string(counter);
        return id;
    }
    
    // Validasi email sederhana
    static bool validasiEmail(const string& email) {
        size_t atPos = email.find('@');
        size_t dotPos = email.find_last_of('.');
        
        if (atPos == string::npos || dotPos == string::npos) {
            return false;
        }
        
        if (atPos > dotPos) {
            return false;
        }
        
        if (atPos == 0 || dotPos == email.length() - 1) {
            return false;
        }
        
        return true;
    }
    
    // Validasi nomor telepon Indonesia (sederhana)
    static bool validasiNomorTelepon(const string& nomor) {
        if (nomor.length() < 10 || nomor.length() > 15) {
            return false;
        }
        
        // Harus dimulai dengan 0 atau +62
        if (nomor[0] != '0' && nomor.substr(0, 3) != "+62") {
            return false;
        }
        
        // Cek semua karakter adalah digit (kecuali + di awal)
        for (size_t i = (nomor[0] == '+' ? 1 : 0); i < nomor.length(); i++) {
            if (!isdigit(nomor[i])) {
                return false;
            }
        }
        
        return true;
    }
    
    // Convert string ke uppercase
    static string toUpper(const string& str) {
        string result = str;
        for (char& c : result) {
            c = toupper(c);
        }
        return result;
    }
    
    // Convert string ke lowercase
    static string toLower(const string& str) {
        string result = str;
        for (char& c : result) {
            c = tolower(c);
        }
        return result;
    }
    
    // Format rupiah (sederhana tanpa koma)
    static string formatRupiah(double nominal) {
        long long rupiah = static_cast<long long>(nominal);
        return "Rp " + to_string(rupiah);
    }
    
    // Format rupiah dengan separator ribuan
    static string formatRupiahLengkap(double nominal) {
        long long rupiah = static_cast<long long>(nominal);
        string str = to_string(rupiah);
        string result = "";
        int counter = 0;
        
        // Tambahkan separator dari belakang
        for (int i = str.length() - 1; i >= 0; i--) {
            if (counter == 3) {
                result = "." + result;
                counter = 0;
            }
            result = str[i] + result;
            counter++;
        }
        
        return "Rp " + result;
    }
};

#endif
