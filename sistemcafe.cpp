#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;

struct Menu {
    char namaMenu[50];
    int jumlahMenu;
    float hargaPerItem;
};

struct Pengunjung {
    char namaPengunjung[50];
    char noHP[15];
    Menu daftarMenu[50];
    int jumlahPesanan;
    float totalHarga;
};

void liatMenu() {
    cout << "Silakan pilih menu favorit Anda!" << endl;
    cout << "\nMenu Makanan:\n";
    cout << "1. Nasi Goreng - 10000\n";
    cout << "2. Ayam Geprek - 13000\n";
    cout << "3. Bakmi Rebus - 15000\n";
    cout << "4. Roti Bakar - 8000\n";
    cout << "\nMenu Minuman:\n";
    cout << "1. Kopi Susu - 5000\n";
    cout << "2. Kopi Hitam - 4000\n";
    cout << "3. Teh Tarik - 5000\n";
    cout << "4. Es Jeruk - 6000\n";
}

Pengunjung* bacaSemuaData(int &count) {
    FILE *file = fopen("pembayaran.dat", "rb");
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

void simpanSemuaData(Pengunjung* arr, int count) {
    FILE* file = fopen("pembayaran.dat", "wb");
    if (!file) {
        cout << "Gagal membuka file untuk menyimpan data.\n";
        return;
    }

    fwrite(arr, sizeof(Pengunjung), count, file);
    fclose(file);
}

void tulisDataBayar() {
    Pengunjung p;
    cout << "Masukkan Nama Pengunjung: ";
    cin.ignore();
    cin.getline(p.namaPengunjung, 50);

    cout << "Masukkan No HP: ";
    cin.getline(p.noHP, 15);

    cout << "Masukkan jumlah menu yang ingin dipesan: ";
    cin >> p.jumlahPesanan;
    if (p.jumlahPesanan <= 0 || p.jumlahPesanan > 50) {
        cout << "Jumlah pesanan tidak valid (1-50).\n";
        return;
    }

    cout << "\nContoh pesanan:\n";
    cout << "Nama Menu: Roti Bakar\nJumlah: 2\nHarga per item: 8000\n";
    cout << "=========================================\n";

    p.totalHarga = 0;

    for (int i = 0; i < p.jumlahPesanan; i++) {
        cout << "Menu ke-" << i + 1 << endl;

        cout << "Nama Menu: ";
        cin.ignore();
        cin.getline(p.daftarMenu[i].namaMenu, 50);

        cout << "Jumlah: ";
        cin >> p.daftarMenu[i].jumlahMenu;
        if (p.daftarMenu[i].jumlahMenu <= 0) {
            cout << "Jumlah tidak valid.\n";
            i--;
            continue;
        }

        cout << "Harga per item: ";
        cin >> p.daftarMenu[i].hargaPerItem;
        if (p.daftarMenu[i].hargaPerItem <= 0) {
            cout << "Harga tidak valid.\n";
            i--;
            continue;
        }

        p.totalHarga += p.daftarMenu[i].jumlahMenu * p.daftarMenu[i].hargaPerItem;
        cout << endl;
    }

    cout << "Total Harga: " << p.totalHarga << endl;

    FILE *file = fopen("pembayaran.dat", "ab");
    if (!file) {
        cout << "Gagal membuka file untuk menyimpan data.\n";
        return;
    }

    fwrite(&p, sizeof(Pengunjung), 1, file);
    fclose(file);
}

void bacaDataBayar() {
    int count = 0;
    Pengunjung* arr = bacaSemuaData(count);
    if (count == 0) {
        cout << "Data pembayaran kosong atau file tidak ditemukan.\n";
        return;
    }

    cout << "Data Transaksi Pembayaran Cafe:\n";
    cout << "---------------------------------------\n";

    for (int i = 0; i < count; i++) {
        Pengunjung* p = &arr[i]; // pointer ke elemen
        cout << "Transaksi #" << i+1 << endl;
        cout << "Nama: " << p->namaPengunjung << endl;
        cout << "No HP: " << p->noHP << endl;
        cout << "Pesanan:\n";

        for (int j = 0; j < p->jumlahPesanan; j++) {
            cout << " - " << p->daftarMenu[j].namaMenu
                 << ", Jumlah: " << p->daftarMenu[j].jumlahMenu
                 << ", Harga per item: " << p->daftarMenu[j].hargaPerItem << endl;
        }
        cout << "Total Harga: " << p->totalHarga << endl;
        cout << "---------------------------------------\n";
    }

    delete[] arr;
}

// === SEARCHING ====
// Sequential Search berdasarkan nama pengunjung, kembalikan indeks atau -1
int sequentialSearch(Pengunjung* arr, int count, const char* target) {
    for (int i = 0; i < count; i++) {
        if (strcmp(arr[i].namaPengunjung, target) == 0) {
            return i;
        }
    }
    return -1;
}

// Untuk Binary Search, data harus sorted dulu (ascending berdasarkan nama pengunjung)
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

// === SORTING ===
// Bubble Sort berdasarkan nama pengunjung
void bubbleSort(Pengunjung* arr, int count) {
    for (int i = 0; i < count-1; i++) {
        for (int j = 0; j < count-1-i; j++) {
            if (strcmp(arr[j].namaPengunjung, arr[j+1].namaPengunjung) > 0) {
                Pengunjung temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

void selectionSort(Pengunjung* arr, int count) {
    for (int i = 0; i < count-1; i++) {
        int minIdx = i;
        for (int j = i+1; j < count; j++) {
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
            arr[j+1] = arr[j];
            j--;
        }
        arr[j+1] = key;
    }
}

void shellSort(Pengunjung* arr, int count) {
    for (int gap = count/2; gap > 0; gap /= 2) {
        for (int i = gap; i < count; i++) {
            Pengunjung temp = arr[i];
            int j;
            for (j = i; j >= gap && strcmp(arr[j - gap].namaPengunjung, temp.namaPengunjung) > 0; j -= gap) {
                arr[j] = arr[j - gap];
            }
            arr[j] = temp;
        }
    }
}

int partition(Pengunjung* arr, int low, int high) {
    Pengunjung pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (strcmp(arr[j].namaPengunjung, pivot.namaPengunjung) <= 0) {
            i++;
            Pengunjung temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    Pengunjung temp = arr[i+1];
    arr[i+1] = arr[high];
    arr[high] = temp;
    return i+1;
}

void quickSort(Pengunjung* arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi-1);
        quickSort(arr, pi+1, high);
    }
}

void merge(Pengunjung* arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    Pengunjung* L = new Pengunjung[n1];
    Pengunjung* R = new Pengunjung[n2];

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (strcmp(L[i].namaPengunjung, R[j].namaPengunjung) <= 0) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    delete[] L;
    delete[] R;
}

void mergeSort(Pengunjung* arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid+1, right);
        merge(arr, left, mid, right);
    }
}

void menuSorting(Pengunjung* arr, int count) {
    if (count == 0) {
        cout << "Data kosong, tidak bisa di-sort.\n";
        return;
    }
    int pilihSort;
    cout << "\nMenu Sorting:\n";
    cout << "1. Bubble Sort\n";
    cout << "2. Selection Sort\n";
    cout << "3. Insertion Sort\n";
    cout << "4. Shell Sort\n";
    cout << "5. Quick Sort\n";
    cout << "6. Merge Sort\n";
    cout << "Pilih metode sorting: ";
    cin >> pilihSort;

    switch (pilihSort) {
        case 1:
            bubbleSort(arr, count);
            cout << "Data telah diurutkan dengan Bubble Sort.\n";
            break;
        case 2:
            selectionSort(arr, count);
            cout << "Data telah diurutkan dengan Selection Sort.\n";
            break;
        case 3:
            insertionSort(arr, count);
            cout << "Data telah diurutkan dengan Insertion Sort.\n";
            break;
        case 4:
            shellSort(arr, count);
            cout << "Data telah diurutkan dengan Shell Sort.\n";
            break;
        case 5:
            quickSort(arr, 0, count-1);
            cout << "Data telah diurutkan dengan Quick Sort.\n";
            break;
        case 6:
            mergeSort(arr, 0, count-1);
            cout << "Data telah diurutkan dengan Merge Sort.\n";
            break;
        default:
            cout << "Pilihan metode sorting tidak valid.\n";
            return;
    }

    cout << "Tekan Enter untuk melihat data yang sudah diurutkan...";
    cin.ignore();
    cin.get();

    for (int i = 0; i < count; i++) {
        cout << i+1 << ". " << arr[i].namaPengunjung << ", Total Harga: " << arr[i].totalHarga << endl;
    }

    cout << "Apakah Anda ingin menyimpan data yang sudah diurutkan ke file? (y/n): ";
    char saveChoice;
    cin >> saveChoice;
    if (saveChoice == 'y' || saveChoice == 'Y') {
        simpanSemuaData(arr, count);
        cout << "Data berhasil disimpan.\n";
    }
}

void menuSearching(Pengunjung* arr, int count) {
    if (count == 0) {
        cout << "Data kosong, tidak bisa melakukan pencarian.\n";
        return;
    }

    cout << "\nMenu Searching:\n";
    cout << "1. Sequential Search\n";
    cout << "2. Binary Search (Data harus sudah diurutkan)\n";
    cout << "Pilih metode searching: ";
    int pilihSearch;
    cin >> pilihSearch;
    cin.ignore();

    char target[50];
    cout << "Masukkan Nama Pengunjung yang dicari: ";
    cin.getline(target, 50);

    int idx = -1;
    switch (pilihSearch) {
        case 1:
            idx = sequentialSearch(arr, count, target);
            break;
        case 2:
            idx = binarySearch(arr, count, target);
            break;
        default:
            cout << "Pilihan metode searching tidak valid.\n";
            return;
    }

    if (idx == -1) {
        cout << "Data dengan nama tersebut tidak ditemukan.\n";
    } else {
        cout << "Data ditemukan pada indeks ke-" << idx+1 << ":\n";
        Pengunjung* p = &arr[idx];
        cout << "Nama: " << p->namaPengunjung << endl;
        cout << "No HP: " << p->noHP << endl;
        cout << "Pesanan:\n";
        for (int i = 0; i < p->jumlahPesanan; i++) {
            cout << " - " << p->daftarMenu[i].namaMenu
                 << ", Jumlah: " << p->daftarMenu[i].jumlahMenu
                 << ", Harga per item: " << p->daftarMenu[i].hargaPerItem << endl;
        }
        cout << "Total Harga: " << p->totalHarga << endl;
    }
}

int main() {
    Pengunjung* dataArr = nullptr;
    int countData = 0;

    int pilihan;
    do {
        cout << "\n====== Sistem Pemesanan Cafe ======\n";
        cout << "1. Lihat Menu\n";
        cout << "2. Tulis Data Pembayaran\n";
        cout << "3. Baca Data Pembayaran\n";
        cout << "4. Searching Data\n";
        cout << "5. Sorting Data\n";
        cout << "6. Keluar\n";
        cout << "Pilihan Anda: ";
        cin >> pilihan;

        switch (pilihan) {
            case 1:
                liatMenu();
                break;
            case 2:
                tulisDataBayar();
                break;
            case 3:
                bacaDataBayar();
                break;
            case 4:
                if (dataArr) delete[] dataArr; 
                dataArr = bacaSemuaData(countData);
                menuSearching(dataArr, countData);
                break;
            case 5:
                if (dataArr) delete[] dataArr; 
                dataArr = bacaSemuaData(countData);
                menuSorting(dataArr, countData);
                break;
            case 6:
                cout << "Terima kasih telah menggunakan sistem pemesanan cafe.\n";
                break;
            default:
                cout << "Pilihan tidak valid.\n";
                break;
        }
    } while (pilihan != 6);

    if (dataArr) delete[] dataArr;
    return 0;
}