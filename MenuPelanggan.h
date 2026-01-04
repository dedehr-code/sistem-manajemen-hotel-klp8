#ifndef MENU_PELANGGAN_H
#define MENU_PELANGGAN_H

#include "KeranjangBelanja.h"
#include "ManajemenKamar.h"
#include "ManajemenLayanan.h"
#include "SistemPembayaran.h"
#include "Pengguna.h"
#include "EditProfil.h"
#include "Autentikasi.h"

class MenuPelanggan {
private:
    Pelanggan* pelanggan;
    ManajemenKamar* manajemenKamar;
    ManajemenLayanan* manajemenLayanan;
    SistemPembayaran* sistemPembayaran;
    KeranjangBelanja* keranjang;
    EditProfil* editProfil;

public:
    MenuPelanggan(Pelanggan* p, ManajemenKamar* mk, ManajemenLayanan* ml, SistemPembayaran* sp, SistemAutentikasi* auth)
        : pelanggan(p), manajemenKamar(mk), manajemenLayanan(ml), sistemPembayaran(sp) {
        keranjang = new KeranjangBelanja(p->getId(), p->getNama());
        editProfil = new EditProfil(auth);
    }
    
    ~MenuPelanggan() { 
        delete keranjang; 
        delete editProfil;
    }
    
    void tampilkanMenu() {
        int pilihan;
        do {
            cout << "\n+------------------------------------------------------------+\n";
            cout << "¦  MENU PELANGGAN - " << pelanggan->getNama().substr(0, 40) << string(40-min(40,(int)pelanggan->getNama().length()), ' ') << "¦\n";
            cout << "¦------------------------------------------------------------¦\n";
            cout << "¦  1. Lihat Katalog Kamar                                    ¦\n";
            cout << "¦  2. Lihat Katalog Layanan                                  ¦\n";
            cout << "¦  3. Keranjang Belanja                                      ¦\n";
            cout << "¦  4. Checkout & Bayar                                       ¦\n";
            cout << "¦  5. Riwayat Transaksi Saya                                 ¦\n";
            cout << "¦  6. Edit Profil Saya                                       ¦\n";
            cout << "¦  0. Logout                                                 ¦\n";
            cout << "+------------------------------------------------------------+\n";
            cout << "Keranjang: " << keranjang->getInfoRingkas() << endl;
            cout << "\nPilihan: "; cin >> pilihan; cin.ignore();
            
            switch(pilihan) {
                case 1: menuKatalogKamar(); break;
                case 2: menuKatalogLayanan(); break;
                case 3: menuKeranjang(); break;
                case 4: prosesCheckout(); break;
                case 5: sistemPembayaran->tampilkanTransaksiCustomer(pelanggan->getId()); break;
                case 6: editProfil->menuEditProfilPelanggan(pelanggan); break;
                case 0: cout << "\n[INFO] Logout berhasil!\n"; break;
                default: cout << "[ERROR] Pilihan tidak valid!\n";
            }
        } while(pilihan != 0);
    }

private:
    void menuKatalogKamar() {
        manajemenKamar->tampilkanKamarTersedia();
        cout << "\nTambah ke keranjang? (y/n): "; string jawab; getline(cin, jawab);
        if(jawab == "y" || jawab == "Y") {
            cout << "Nomor kamar: "; string nomor; getline(cin, nomor);
            Kamar* k = manajemenKamar->cariKamar(nomor);
            if(k) {
                cout << "Jumlah malam: "; int malam; cin >> malam; cin.ignore();
                cout << "Check-in (DD/MM/YYYY): "; string checkin; getline(cin, checkin);
                cout << "Check-out (DD/MM/YYYY): "; string checkout; getline(cin, checkout);
                keranjang->tambahKamar(k, malam, checkin, checkout);
            }
        }
    }
    
    void menuKatalogLayanan() {
        manajemenLayanan->tampilkanLayananTersedia();
        cout << "\nTambah ke keranjang? (y/n): "; string jawab; getline(cin, jawab);
        if(jawab == "y" || jawab == "Y") {
            cout << "ID Layanan: "; string id; getline(cin, id);
            Layanan* l = manajemenLayanan->cariLayanan(id);
            if(l) {
                cout << "Kuantitas: "; int qty; cin >> qty; cin.ignore();
                cout << "Tanggal (DD/MM/YYYY): "; string tgl; getline(cin, tgl);
                keranjang->tambahLayanan(l, qty, tgl);
            }
        }
    }
    
    void menuKeranjang() {
        int pilihan;
        do {
            keranjang->tampilkanKeranjang();
            cout << "\n1. Hapus Item  2. Edit Qty  3. Kosongkan  0. Kembali\nPilihan: ";
            cin >> pilihan; cin.ignore();
            
            if(pilihan == 1) {
                cout << "Nomor item: "; int idx; cin >> idx; cin.ignore();
                keranjang->hapusItem(idx);
            } else if(pilihan == 2) {
                cout << "Nomor item: "; int idx; cin >> idx; cin.ignore();
                cout << "Qty baru: "; int qty; cin >> qty; cin.ignore();
                keranjang->editKuantitas(idx, qty);
            } else if(pilihan == 3) {
                keranjang->kosongkan();
            }
        } while(pilihan != 0);
    }
    
    void prosesCheckout() {
        if(keranjang->kosong()) {
            cout << "[ERROR] Keranjang kosong!\n"; return;
        }
        
        keranjang->tampilkanKeranjang();
        cout << "\nLanjut checkout? (y/n): "; string jawab; getline(cin, jawab);
        if(jawab != "y" && jawab != "Y") return;
        
        cout << "\nMetode Pembayaran:\n1. Tunai  2. Transfer  3. Kartu Kredit  4. E-Wallet\nPilih: ";
        int metode; cin >> metode; cin.ignore();
        
        MetodePembayaran metodeBayar = MetodePembayaran::TUNAI;
        if(metode == 2) metodeBayar = MetodePembayaran::TRANSFER_BANK;
        else if(metode == 3) metodeBayar = MetodePembayaran::KARTU_KREDIT;
        else if(metode == 4) metodeBayar = MetodePembayaran::EWALLET;
        
        Transaksi* t = keranjang->checkout("03/01/2026");
        if(t && sistemPembayaran->prosesPembayaran(t, metodeBayar)) {
            sistemPembayaran->generateStruk(t);
            keranjang->kosongkan();
        }
    }
};

#endif
