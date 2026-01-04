🏨 Sistem Manajemen Hotel - C++

Program sistem manajemen hotel berbasis console dengan implementasi manual struktur data (Circular Linked List, Stack, Queue, Binary Search Tree) untuk multi-user system, booking terintegrasi, dan laporan keuangan otomatis.

================================================================
👨‍💻 Pembuat

Kelompok 8:
- Muhammad Rivaldi Abduulah (2400018166)
- Dede Hermawan (2400018171)
- Fadhilah Aviyo Destra Kencana (2400018178)
================================================================

🚀 Cara Menjalankan

Compile Program
C++: main.cpp

Login Default
Pemilik (Owner):
- Username: OWNER
- Password: admin123

Pegawai (Employee):
- Username: pegawai01
- Password: pegawai123

Pelanggan (Customer):
- Silakan registrasi melalui menu utama


✨ Fitur Utama

🔐 Sistem Multi-User
- Login Pelanggan, Pegawai & Pemilik: dengan hak akses berbeda
- Registrasi akun pelanggan: dengan validasi data
- Manajemen akun pegawai: oleh pemilik hotel

🛏️ Manajemen Kamar & Layanan
- Browse katalog kamar (Standard, Deluxe, Suite, Presidential)
- Layanan hotel lengkap (Ballroom, Restaurant, Laundry, dll)
- Update status kamar (Tersedia, Terpakai, Maintenance, Cleaning)
- Pencarian kamar berdasarkan harga, tipe, lantai, kapasitas (BST-based)
- Filter dan sorting untuk pengalaman user yang optimal

🛒 Sistem Booking Multi-Item
- Keranjang belanja untuk booking beberapa kamar + layanan sekaligus
- Pilih tanggal check-in & check-out dengan validasi
- Undo/remove item dari keranjang sebelum checkout
- Estimasi total harga secara real-time
- Queue system untuk antrian booking dan waiting list

💳 Pembayaran & Transaksi
- Multiple metode pembayaran (Cash, Transfer, E-Wallet)
- Generate invoice otomatis setelah pembayaran
- Verifikasi pembayaran oleh pegawai
- Track status pembayaran (Unpaid, Partial, Paid)
- History transaksi dengan Stack (10 transaksi terakhir)

📊 Laporan & Analytics (Owner)
- Laporan keuangan harian dengan breakdown revenue
- Laporan mingguan dengan perbandingan periode sebelumnya
- Laporan bulanan dengan analisis profit/loss
- Occupancy rate dan statistik hunian kamar
- Customer aktif dan analisis demografis
- Performance pegawai dan aktivitas sistem

👥 Manajemen Data
- Data pelanggan dengan history booking lengkap
- Data pegawai dengan shift schedule dan attendance log
- Data kamar dengan maintenance record
- Data booking dengan status real-time
- Activity logging untuk audit trail

🛠️ Teknologi & Struktur Data

Struktur Data yang Diimplementasikan
- Circular Linked List: Penyimpanan utama (kamar, pelanggan, booking, layanan)
- Stack: History transaksi, undo operations, navigation history
- Queue: Antrian booking, waiting list, service request
- Binary Search Tree: Pencarian cepat O(log n), sorting otomatis, range query

Spesifikasi Teknis
- Bahasa: C++ (Standard C++11 atau lebih baru)
- Paradigma: Object-Oriented Programming (OOP)
- Memory Managemen: Manual allocation/deallocation (no vector/STL containers)
- Persistensi Data: File TXT dengan delimiter (|)
- Naming Convention: Bahasa Indonesia untuk semua variabel, fungsi, dan komentar

📁 Struktur Project

HotelManagementSystem/
│
├── Main.cpp                        # Entry point aplikasi
│
├── headers/                        # Semua file header (*.h)
│   ├── Autentikasi.h               # Modul autentikasi
│   ├── BinarySearchTree.h          # Struktur data BST
│   ├── CircularLinkedList.h        # Struktur data circular linked list
│   ├── DataManager.h               # Manajemen data
│   ├── EditProfil.h                # Menu mengedit profil
│   ├── Kamar.h                     # Manajemen kamar
│   ├── KeranjangBelanja.h          # Fitur keranjang belanja
│   ├── LaporanKeuangan.h           # Laporan keuangan
│   ├── Layanan.h                   # Data layanan & operasi
│   ├── ManajemenKamar.h            # Modul manajemen kamar
│   ├── ManajemenLayanan.h          # Modul manajemen layanan
│   ├── MenuPegawai.h               # Menu khusus pegawai
│   ├── MenuPelanggan.h             # Menu khusus pelanggan
│   ├── MenuPemilik.h               # Menu khusus pemilik hotel
│   ├── PengelolaFile.h             # File manager / handler
│   ├── Penggunan.h                 # Data pengguna (nama file sesuai permintaan)
│   ├── Queue.h                     # Struktur data Queue
│   ├── SistemPembayaran.h          # Sistem pembayaran
│   ├── Stack.h                     # Struktur data Stack
│   └── Transaksi.h                 # Modul transaksi
│
└── data/                           # Semua file data (*.txt)
    ├── kamar.txt                   # Data kamar
    ├── layanan.txt                 # Data layanan
    ├── penggunan.txt               # Data pengguna
    └── transaksi.txt               # Data transaksi


📝 Catatan Penting

- Program ini tidak menggunakan STL containers (vector, list, dll)
- Semua struktur data diimplementasikan secara manual
- Data persisten disimpan dalam file TXT dan tetap ada setelah program ditutup
- Implementasi memory management yang baik untuk mencegah memory leak
