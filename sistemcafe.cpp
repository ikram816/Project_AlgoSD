#include <iostream>
#include <cstring>
using namespace std;

struct Menu {
    char namaMenu[50];
    int jumlahMenu;
    float hargaPerItem;
};

struct Pengunjung {
    char namaPengunjung[50];
    char noMeja[15];
    Menu daftarMenu[50];
    int jumlahPesanan;
    float totalHarga;
};

struct NodePengunjung {
    Pengunjung data;
    NodePengunjung* next;
    NodePengunjung* prev;
};

NodePengunjung* head = nullptr;
NodePengunjung* tail = nullptr;

// Tampilkan menu makanan dan minuman
void liatmenu() {
    cout << "Silakan pilih menu favorit Anda!\n";
    cout << "\nMenu Makanan:\n";
    cout << "1. Nasi Goreng - 10000\n";
    cout << "2. Ayam Geprek - 13000\n";
    cout << "3. Bakmi Goreng Afui - 15000\n";
    cout << "4. Roti Bakar - 8000\n";
    cout << "\nMenu Minuman:\n";
    cout << "1. Teh Es - 5000\n";
    cout << "2. Kopi Hitam - 4000\n";
    cout << "3. Kopi Susu - 5000\n";
    cout << "4. Es Jeruk - 6000\n";
}

// Fungsi untuk mendapatkan harga otomatis berdasarkan nama menu
float hargaotomatis(const char* namaMenu) {
    if (strcmp(namaMenu, "Nasi Goreng") == 0) return 10000;
    if (strcmp(namaMenu, "Ayam Geprek") == 0) return 13000;
    if (strcmp(namaMenu, "Bakmi Goreng Afui") == 0) return 15000;
    if (strcmp(namaMenu, "Roti Bakar") == 0) return 8000;
    if (strcmp(namaMenu, "Teh Es") == 0) return 5000;
    if (strcmp(namaMenu, "Kopi Hitam") == 0) return 4000;
    if (strcmp(namaMenu, "Kopi Susu") == 0) return 5000;
    if (strcmp(namaMenu, "Es Jeruk") == 0) return 6000;
    return -1;
}

// Membaca semua data pembayaran dari file
Pengunjung* bacasemuadata(int &count) {
    FILE* file = fopen("pembayaran.dat", "rb");
    if (!file) {
        count = 0;
        return nullptr;
    }
    fseek(file, 0, SEEK_END);
    long ukuranFile = ftell(file);
    count = ukuranFile / sizeof(Pengunjung);
    rewind(file);

    Pengunjung* arr = new Pengunjung[count];
    fread(arr, sizeof(Pengunjung), count, file);
    fclose(file);
    return arr;
}

// Simpan semua data pembayaran ke file
void simpansemuadata(Pengunjung* arr, int count) {
    FILE* file = fopen("pembayaran.dat", "wb");
    if (!file) {
        cout << "Gagal membuka file untuk menyimpan data.\n";
        return;
    }
    fwrite(arr, sizeof(Pengunjung), count, file);
    fclose(file);
}

// Input data pembayaran baru dan simpan ke file
void tulisdatabayar() {
    Pengunjung p;
    cout << "Masukkan Nama Pengunjung: ";
    cin.ignore();
    cin.getline(p.namaPengunjung, 50);

    cout << "Masukkan No Meja: "; // Ganti label
    cin.getline(p.noMeja, 15);

    cout << "Masukkan jumlah menu yang ingin dipesan: ";
    cin >> p.jumlahPesanan;
    cin.ignore();

    if (p.jumlahPesanan <= 0 || p.jumlahPesanan > 50) {
        cout << "Jumlah pesanan tidak valid (1-50).\n";
        cin.get();
        return;
    }

    cout << "\nMasukkan nama menu sesuai daftar!\n";
    cout << "Contoh: Nasi Goreng, Kopi Susu, Roti Bakar, dll.\n";
    cout << "=========================================\n";

    p.totalHarga = 0;

    for (int i = 0; i < p.jumlahPesanan; i++) {
        cout << "Menu ke-" << i + 1 << endl;

        cout << "Nama Menu: ";
        cin.getline(p.daftarMenu[i].namaMenu, 50);

        float harga = hargaotomatis(p.daftarMenu[i].namaMenu);
        if (harga == -1) {
            cout << "Menu tidak ditemukan. Silakan coba lagi.\n";
            i--;
            continue;
        }

        cout << "Jumlah: ";
        cin >> p.daftarMenu[i].jumlahMenu;
        cin.ignore();

        if (p.daftarMenu[i].jumlahMenu <= 0) {
            cout << "Jumlah tidak valid.\n";
            i--;
            continue;
        }

        p.daftarMenu[i].hargaPerItem = harga;
        p.totalHarga += harga * p.daftarMenu[i].jumlahMenu;

        cout << "Harga per item: " << harga << endl;
        cout << "Subtotal: " << harga * p.daftarMenu[i].jumlahMenu << endl << endl;
    }

    cout << "Total harga yang harus dibayar: " << p.totalHarga << endl;
    cout << "Tekan ENTER untuk melanjutkan...";
    cin.get();

    FILE* file = fopen("pembayaran.dat", "ab");
    if (!file) {
        cout << "Gagal membuka file untuk menyimpan data.\n";
        cin.get();
        return;
    }

    fwrite(&p, sizeof(Pengunjung), 1, file);
    fclose(file);
}

// Membaca dan menampilkan data pembayaran dari file
void bacadatabayar() {
    int count = 0;
    Pengunjung* arr = bacasemuadata(count);
    if (count == 0) {
        cout << "Data pembayaran kosong atau file tidak ditemukan.\n";
        cin.get();
        return;
    }

    cout << "Data Transaksi Pembayaran Cafe:\n";
    cout << "---------------------------------------\n";

    for (int i = 0; i < count; i++) {
        Pengunjung* p = &arr[i];
        cout << "Transaksi #" << i + 1 << endl;
        cout << "Nama: " << p->namaPengunjung << endl;
        cout << "No Meja: " << p->noMeja << endl; // Ganti label
        cout << "Pesanan:\n";

        for (int j = 0; j < p->jumlahPesanan; j++) {
            cout << " - " << p->daftarMenu[j].namaMenu
                 << ", Jumlah: " << p->daftarMenu[j].jumlahMenu
                 << ", Harga per item: " << p->daftarMenu[j].hargaPerItem << endl;
        }
        cout << "Total Harga: " << p->totalHarga << endl;
        cout << "---------------------------------------\n";
    }
    cout << "Tekan ENTER untuk kembali ke menu...";
    cin.ignore();
    cin.get();

    delete[] arr;
}

// === Searching ===
int sequentialsearch(Pengunjung* arr, int count, const char* target) {
    for (int i = 0; i < count; i++) {
        if (strcmp(arr[i].namaPengunjung, target) == 0) {
            return i;
        }
    }
    return -1;
}

int binarySearch(Pengunjung* arr, int count, const char* target) {
    int low = 0, high = count - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        int cmp = strcmp(arr[mid].namaPengunjung, target);
        if (cmp == 0) return mid;
        else if (cmp < 0) low = mid + 1;
        else high = mid - 1;
    }
    return -1;
}

// === Sorting ===
// Bubble Sort berdasarkan nama pengunjung
void bubbleSort(Pengunjung* arr, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - 1 - i; j++) {
            if (strcmp(arr[j].namaPengunjung, arr[j + 1].namaPengunjung) > 0) {
                Pengunjung temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void selectionSort(Pengunjung* arr, int count) {
    for (int i = 0; i < count - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < count; j++) {
            if (strcmp(arr[j].namaPengunjung, arr[minIdx].namaPengunjung) < 0) {
                minIdx = j;
            }
        }
        if (minIdx != i) {
            Pengunjung temp = arr[i];
            arr[i] = arr[minIdx];
            arr[minIdx] = temp;
        }
    }
}

void insertionSort(Pengunjung* arr, int count) {
    for (int i = 1; i < count; i++) {
        Pengunjung key = arr[i];
        int j = i - 1;
        while (j >= 0 && strcmp(arr[j].namaPengunjung, key.namaPengunjung) > 0) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void menuSorting(Pengunjung* arr, int count) {
    int pilih;
    cout << "\n=== Menu Sorting ===\n";
    cout << "1. Bubble Sort\n2. Selection Sort\n3. Insertion Sort\n";
    cout << "Pilih metode sorting: ";
    cin >> pilih;

    switch (pilih) {
        case 1:
            bubbleSort(arr, count);
            cout << "Data sudah diurutkan menggunakan Bubble Sort.\n";
            break;
        case 2:
            selectionSort(arr, count);
            cout << "Data sudah diurutkan menggunakan Selection Sort.\n";
            break;
        case 3:
            insertionSort(arr, count);
            cout << "Data sudah diurutkan menggunakan Insertion Sort.\n";
            break;
        default:
            cout << "Pilihan tidak valid.\n";
            return;
    }

    simpansemuadata(arr, count);
    bacadatabayar();
}

void menuSearching(Pengunjung* arr, int count) {
    int pilih;
    cout << "\n=== Menu Searching ===\n";
    cout << "1. Sequential Search\n2. Binary Search (Data harus sudah diurutkan)\n";
    cout << "Pilih metode searching: ";
    cin >> pilih;

    cin.ignore(100, '\n');
    char namaCari[50];
    cout << "Masukkan nama pengunjung yang dicari: ";
    cin.getline(namaCari, 50);

    int idx = -1;
    if (pilih == 1) {
        idx = sequentialsearch(arr, count, namaCari);
    } else if (pilih == 2) {
        idx = binarySearch(arr, count, namaCari);
    } else {
        cout << "Pilihan tidak valid.\n";
        return;
    }

    if (idx == -1) {
        cout << "Data tidak ditemukan.\n";
    } else {
        cout << "Data ditemukan pada indeks ke-" << idx << ":\n";
        Pengunjung* p = &arr[idx];
        cout << "Nama: " << p->namaPengunjung << endl;
        cout << "No Meja: " << p->noMeja << endl;
        cout << "Pesanan:\n";
        for (int i = 0; i < p->jumlahPesanan; i++) {
            cout << " - " << p->daftarMenu[i].namaMenu
                 << ", Jumlah: " << p->daftarMenu[i].jumlahMenu
                 << ", Harga per item: " << p->daftarMenu[i].hargaPerItem << endl;
        }
        cout << "Total Harga: " << p->totalHarga << endl;
    }

    cin.ignore(); // tunggu user tekan Enter sebelum kembali ke menu
    cin.get();
}

// === Linked List Pengunjung ===

void tambahPengunjung() {
    Pengunjung p;
    cout << "Masukkan Nama Pengunjung: ";
    cin.ignore();
    cin.getline(p.namaPengunjung, 50);

    cout << "Masukkan No Meja: "; // Ganti label
    cin.getline(p.noMeja, 15);

    cout << "Masukkan jumlah menu yang ingin dipesan: ";
    cin >> p.jumlahPesanan;
    cin.ignore();

    if (p.jumlahPesanan <= 0 || p.jumlahPesanan > 50) {
        cout << "Jumlah pesanan tidak valid (1-50).\n";
        cin.get();
        return;
    }

    cout << "\nMasukkan nama menu sesuai daftar!\n";
    cout << "Contoh: Nasi Goreng, Kopi Susu, Roti Bakar, dll.\n";
    cout << "=========================================\n";

    p.totalHarga = 0;

    for (int i = 0; i < p.jumlahPesanan; i++) {
        cout << "Menu ke-" << i + 1 << endl;

        cout << "Nama Menu: ";
        cin.getline(p.daftarMenu[i].namaMenu, 50);

        float harga = hargaotomatis(p.daftarMenu[i].namaMenu);
        if (harga == -1) {
            cout << "Menu tidak ditemukan. Silakan coba lagi.\n";
            i--;
            continue;
        }

        cout << "Jumlah: ";
        cin >> p.daftarMenu[i].jumlahMenu;
        cin.ignore();

        if (p.daftarMenu[i].jumlahMenu <= 0) {
            cout << "Jumlah tidak valid.\n";
            i--;
            continue;
        }

        p.daftarMenu[i].hargaPerItem = harga;
        p.totalHarga += harga * p.daftarMenu[i].jumlahMenu;

        cout << "Harga per item otomatis: " << harga << endl;
        cout << "Subtotal: " << harga * p.daftarMenu[i].jumlahMenu << endl << endl;
    }

    NodePengunjung* newNode = new NodePengunjung;
    newNode->data = p;
    newNode->next = nullptr;
    newNode->prev = tail;

    if (tail != nullptr)
        tail->next = newNode;
    tail = newNode;

    if (head == nullptr)
        head = newNode;

    cout << "Data pengunjung berhasil ditambahkan ke linked list.\n";
    cout << "Tekan ENTER untuk melanjutkan...";
    cin.get();
}

void tampilkanPengunjung() {
    if (head == nullptr) {
        cout << "Data pengunjung kosong.\n";
        cin.ignore();
        cin.get();
        return;
    }
    cout << "\n=== Daftar Pengunjung ===\n";
    NodePengunjung* cur = head;
    int nomor = 1;
    while (cur != nullptr) {
        Pengunjung* p = &cur->data;
        cout << "Pengunjung #" << nomor++ << endl;
        cout << "Nama: " << p->namaPengunjung << endl;
        cout << "No Meja: " << p->noMeja << endl; // Ganti label
        cout << "Pesanan:\n";
        for (int i = 0; i < p->jumlahPesanan; i++) {
            cout << " - " << p->daftarMenu[i].namaMenu
                 << ", Jumlah: " << p->daftarMenu[i].jumlahMenu
                 << ", Harga per item: " << p->daftarMenu[i].hargaPerItem << endl;
        }
        cout << "Total Harga: " << p->totalHarga << endl;
        cout << "--------------------------\n";
        cur = cur->next;
    }
    cout << "Tekan ENTER untuk kembali ke menu...";
    cin.ignore();
    cin.get();
}

void hapusPengunjung() {
    if (head == nullptr) {
        cout << "Data pengunjung kosong.\n";
        return;
    }
    char namaHapus[50];
    cout << "Masukkan nama pengunjung yang ingin dihapus: ";
    cin.ignore();
    cin.getline(namaHapus, 50);

    NodePengunjung* cur = head;
    while (cur != nullptr) {
        if (strcmp(cur->data.namaPengunjung, namaHapus) == 0) {
            if (cur->prev != nullptr)
                cur->prev->next = cur->next;
            else
                head = cur->next;

            if (cur->next != nullptr)
                cur->next->prev = cur->prev;
            else
                tail = cur->prev;

            delete cur;
            cout << "Data pengunjung berhasil dihapus.\n";
            return;
        }
        cur = cur->next;
    }
    cout << "Data pengunjung tidak ditemukan.\n";
}

int main() {
    Pengunjung* dataArr = nullptr;
    int countData = 0;

    int pilihan;
    do {
        system("cls"); 
        cout << "\n=============================================\n";
        cout << "|      Sistem Pemesanan Cafe KopiKita      |\n";
        cout << "=============================================\n";
        cout << "| 1. Lihat Menu                            |\n";
        cout << "---------------------------------------------\n";
        cout << "| 2. Tulis Data Pembayaran                 |\n";
        cout << "---------------------------------------------\n";
        cout << "| 3. Baca Data Pembayaran                  |\n";
        cout << "---------------------------------------------\n";
        cout << "| 4. Searching Data                        |\n";
        cout << "---------------------------------------------\n";
        cout << "| 5. Sorting Data                          |\n";
        cout << "---------------------------------------------\n";
        cout << "| 6. Tambah Data ke Linked List            |\n";
        cout << "---------------------------------------------\n";
        cout << "| 7. Tampilkan Data Pengunjung             |\n";
        cout << "---------------------------------------------\n";
        cout << "| 8. Hapus Data Pengunjung                 |\n";
        cout << "---------------------------------------------\n";
        cout << "| 9. Keluar                                |\n";
        cout << "=============================================\n";
        cout << "Pilihan Anda: ";
        cin >> pilihan;

        switch (pilihan) {
            case 1:
                system("cls");
                liatmenu();
                cout << "Tekan ENTER untuk kembali ke menu...";
                cin.ignore();
                cin.get();
                break;
            case 2:
                system("cls");
                tulisdatabayar();
                break;
            case 3:
                system("cls");
                bacadatabayar();
                break;
            case 4:
                system("cls");
                if (dataArr != nullptr) {
                    delete[] dataArr;
                    dataArr = nullptr;
                    countData = 0;
                }
                dataArr = bacasemuadata(countData);
                if (countData == 0) {
                    cout << "Data kosong.\n";
                    cin.ignore();
                    cin.get();
                } else {
                    menuSearching(dataArr, countData);
                }
                break;
            case 5:
                system("cls");
                if (dataArr != nullptr) {
                    delete[] dataArr;
                    dataArr = nullptr;
                    countData = 0;
                }
                dataArr = bacasemuadata(countData);
                if (countData == 0) {
                    cout << "Data kosong.\n";
                    cin.ignore();
                    cin.get();
                } else {
                    menuSorting(dataArr, countData);
                }
                break;
            case 6:
                system("cls");
                tambahPengunjung();
                break;
            case 7:
                system("cls");
                tampilkanPengunjung();
                break;
            case 8:
                system("cls");
                hapusPengunjung();
                cout << "Tekan ENTER untuk kembali ke menu...";
                cin.get();
                break;
            case 9:
                cout << "Terima kasih telah menggunakan program ini.\n";
                break;
            default:
                cout << "Pilihan tidak valid, silakan coba lagi.\n";
                cin.ignore();
                cin.get();
        }
    } while (pilihan != 9);

    if (dataArr != nullptr) {
        delete[] dataArr;
    }
    // Bersihkan linked list sebelum keluar program
    while (head != nullptr) {
        NodePengunjung* temp = head;
        head = head->next;
        delete temp;
    }

    return 0;
}
