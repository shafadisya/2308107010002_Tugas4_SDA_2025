/**
 * sorting.h
 * File header yang berisi implementasi berbagai algoritma sorting
 * untuk Tugas 4 Struktur Data dan Algoritma
 */

 #ifndef SORTING_H
 #define SORTING_H
 
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 
 /**
  * Bubble Sort
  * Prinsip kerja: Mengiterasi array dan membandingkan pasangan elemen bersebelahan,
  * kemudian menukar posisinya jika tidak berurutan. Algoritma ini akan mengulang
  * proses hingga tidak ada lagi pertukaran yang dilakukan.
  * 
  * Kompleksitas Waktu: O(n²) dalam kasus rata-rata dan terburuk.
  * Kompleksitas Ruang: O(1) karena sorting dilakukan secara in-place.
  */
 void bubbleSort(int arr[], int n) {
     int i, j, temp;
     int swapped;
     
     for (i = 0; i < n - 1; i++) {
         swapped = 0;
         for (j = 0; j < n - i - 1; j++) {
             if (arr[j] > arr[j + 1]) {
                 // Tukar elemen jika tidak berurutan
                 temp = arr[j];
                 arr[j] = arr[j + 1];
                 arr[j + 1] = temp;
                 swapped = 1;
             }
         }
         
         // Jika tidak ada pertukaran di iterasi ini, array sudah terurut
         if (swapped == 0)
             break;
     }
 }
 
 /**
  * Bubble Sort untuk string
  * Prinsip kerja sama dengan bubble sort untuk angka, tetapi menggunakan
  * strcmp untuk membandingkan string.
  */
 void bubbleSortString(char *arr[], int n) {
     int i, j;
     char *temp;
     int swapped;
     
     for (i = 0; i < n - 1; i++) {
         swapped = 0;
         for (j = 0; j < n - i - 1; j++) {
             if (strcmp(arr[j], arr[j + 1]) > 0) {
                 // Tukar elemen jika tidak berurutan
                 temp = arr[j];
                 arr[j] = arr[j + 1];
                 arr[j + 1] = temp;
                 swapped = 1;
             }
         }
         
         // Jika tidak ada pertukaran di iterasi ini, array sudah terurut
         if (swapped == 0)
             break;
     }
 }
 
 /**
  * Selection Sort
  * Prinsip kerja: Memilih elemen terkecil dari bagian array yang belum diurutkan
  * dan menukarnya dengan elemen di posisi paling depan dari bagian yang belum diurutkan.
  * 
  * Kompleksitas Waktu: O(n²) dalam semua kasus.
  * Kompleksitas Ruang: O(1) karena sorting dilakukan secara in-place.
  */
 void selectionSort(int arr[], int n) {
     int i, j, min_idx, temp;
     
     for (i = 0; i < n - 1; i++) {
         // Cari elemen minimal dari array yang belum diurutkan
         min_idx = i;
         for (j = i + 1; j < n; j++) {
             if (arr[j] < arr[min_idx]) {
                 min_idx = j;
             }
         }
         
         // Tukar elemen minimum dengan elemen pertama
         if (min_idx != i) {
             temp = arr[i];
             arr[i] = arr[min_idx];
             arr[min_idx] = temp;
         }
     }
 }
 
 /**
  * Selection Sort untuk string
  * Prinsip kerja sama dengan selection sort untuk angka,
  * tetapi menggunakan strcmp untuk membandingkan string.
  */
 void selectionSortString(char *arr[], int n) {
     int i, j, min_idx;
     char *temp;
     
     for (i = 0; i < n - 1; i++) {
         // Cari elemen minimal dari array yang belum diurutkan
         min_idx = i;
         for (j = i + 1; j < n; j++) {
             if (strcmp(arr[j], arr[min_idx]) < 0) {
                 min_idx = j;
             }
         }
         
         // Tukar elemen minimum dengan elemen pertama
         if (min_idx != i) {
             temp = arr[i];
             arr[i] = arr[min_idx];
             arr[min_idx] = temp;
         }
     }
 }
 
 /**
  * Insertion Sort
  * Prinsip kerja: Membangun array terurut secara berurutan, satu per satu.
  * Algoritma ini mengambil satu elemen dari input pada setiap iterasi dan
  * menempatkannya di posisi yang tepat pada bagian array yang sudah diurutkan.
  * 
  * Kompleksitas Waktu: O(n²) pada kasus rata-rata dan terburuk, O(n) pada kasus terbaik.
  * Kompleksitas Ruang: O(1) karena sorting dilakukan secara in-place.
  */
 void insertionSort(int arr[], int n) {
     int i, key, j;
     
     for (i = 1; i < n; i++) {
         key = arr[i];
         j = i - 1;
         
         // Geser elemen arr[0..i-1] yang lebih besar dari key
         // ke satu posisi di depan posisi mereka saat ini
         while (j >= 0 && arr[j] > key) {
             arr[j + 1] = arr[j];
             j = j - 1;
         }
         arr[j + 1] = key;
     }
 }
 
 /**
  * Insertion Sort untuk string
  * Prinsip kerja sama dengan insertion sort untuk angka,
  * tetapi menggunakan strcmp untuk membandingkan string.
  */
 void insertionSortString(char *arr[], int n) {
     int i, j;
     char *key;
     
     for (i = 1; i < n; i++) {
         key = arr[i];
         j = i - 1;
         
         // Geser elemen arr[0..i-1] yang lebih besar dari key
         // ke satu posisi di depan posisi mereka saat ini
         while (j >= 0 && strcmp(arr[j], key) > 0) {
             arr[j + 1] = arr[j];
             j = j - 1;
         }
         arr[j + 1] = key;
     }
 }
 
 /**
  * Merge Sort
  * Prinsip kerja: Algoritma berbasis divide-and-conquer yang membagi array
  * menjadi dua bagian, mengurutkan kedua bagian tersebut secara rekursif,
  * kemudian menggabungkan kedua bagian yang telah diurutkan.
  * 
  * Kompleksitas Waktu: O(n log n) dalam semua kasus.
  * Kompleksitas Ruang: O(n) karena membutuhkan array tambahan untuk proses penggabungan.
  */
 void merge(int arr[], int left, int mid, int right) {
     int i, j, k;
     int n1 = mid - left + 1;
     int n2 = right - mid;
     
     // Buat array temporary
     int *L = (int *) malloc(n1 * sizeof(int));
     int *R = (int *) malloc(n2 * sizeof(int));
     
     // Salin data ke array temporary
     for (i = 0; i < n1; i++)
         L[i] = arr[left + i];
     for (j = 0; j < n2; j++)
         R[j] = arr[mid + 1 + j];
     
     // Gabungkan array temporary kembali ke arr[left..right]
     i = 0;
     j = 0;
     k = left;
     while (i < n1 && j < n2) {
         if (L[i] <= R[j]) {
             arr[k] = L[i];
             i++;
         } else {
             arr[k] = R[j];
             j++;
         }
         k++;
     }
     
     // Salin elemen yang tersisa dari L[], jika ada
     while (i < n1) {
         arr[k] = L[i];
         i++;
         k++;
     }
     
     // Salin elemen yang tersisa dari R[], jika ada
     while (j < n2) {
         arr[k] = R[j];
         j++;
         k++;
     }
     
     // Bebaskan memori
     free(L);
     free(R);
 }
 
 void mergeSortImpl(int arr[], int left, int right) {
     if (left < right) {
         int mid = left + (right - left) / 2;
         
         // Urutkan dua bagian
         mergeSortImpl(arr, left, mid);
         mergeSortImpl(arr, mid + 1, right);
         
         // Gabungkan bagian yang telah diurutkan
         merge(arr, left, mid, right);
     }
 }
 
 void mergeSort(int arr[], int n) {
     mergeSortImpl(arr, 0, n - 1);
 }
 
 /**
  * Merge Sort untuk string
  */
 void mergeString(char *arr[], int left, int mid, int right) {
     int i, j, k;
     int n1 = mid - left + 1;
     int n2 = right - mid;
     
     // Buat array temporary
     char **L = (char **) malloc(n1 * sizeof(char *));
     char **R = (char **) malloc(n2 * sizeof(char *));
     
     // Salin data ke array temporary
     for (i = 0; i < n1; i++)
         L[i] = arr[left + i];
     for (j = 0; j < n2; j++)
         R[j] = arr[mid + 1 + j];
     
     // Gabungkan array temporary kembali ke arr[left..right]
     i = 0;
     j = 0;
     k = left;
     while (i < n1 && j < n2) {
         if (strcmp(L[i], R[j]) <= 0) {
             arr[k] = L[i];
             i++;
         } else {
             arr[k] = R[j];
             j++;
         }
         k++;
     }
     
     // Salin elemen yang tersisa dari L[], jika ada
     while (i < n1) {
         arr[k] = L[i];
         i++;
         k++;
     }
     
     // Salin elemen yang tersisa dari R[], jika ada
     while (j < n2) {
         arr[k] = R[j];
         j++;
         k++;
     }
     
     // Bebaskan memori
     free(L);
     free(R);
 }
 
 void mergeSortStringImpl(char *arr[], int left, int right) {
     if (left < right) {
         int mid = left + (right - left) / 2;
         
         // Urutkan dua bagian
         mergeSortStringImpl(arr, left, mid);
         mergeSortStringImpl(arr, mid + 1, right);
         
         // Gabungkan bagian yang telah diurutkan
         mergeString(arr, left, mid, right);
     }
 }
 
 void mergeSortString(char *arr[], int n) {
     mergeSortStringImpl(arr, 0, n - 1);
 }
 
 /**
  * Quick Sort
  * Prinsip kerja: Algoritma berbasis divide-and-conquer yang memilih satu elemen
  * sebagai pivot dan mempartisi array berdasarkan pivot tersebut. Semua elemen yang
  * lebih kecil dari pivot diletakkan di kiri pivot, dan yang lebih besar di kanan pivot.
  * 
  * Kompleksitas Waktu: O(n log n) pada kasus rata-rata, O(n²) pada kasus terburuk.
  * Kompleksitas Ruang: O(log n) untuk rekursi stack.
  */
 int partition(int arr[], int low, int high) {
     int pivot = arr[high];
     int i = (low - 1);
     int j, temp;
     
     for (j = low; j <= high - 1; j++) {
         // Jika elemen saat ini lebih kecil dari pivot
         if (arr[j] < pivot) {
             i++;
             // Tukar arr[i] dan arr[j]
             temp = arr[i];
             arr[i] = arr[j];
             arr[j] = temp;
         }
     }
     
     // Tukar arr[i + 1] dan arr[high] (atau pivot)
     temp = arr[i + 1];
     arr[i + 1] = arr[high];
     arr[high] = temp;
     
     return (i + 1);
 }
 
 void quickSortImpl(int arr[], int low, int high) {
     if (low < high) {
         // pi adalah indeks partisi
         int pi = partition(arr, low, high);
         
         // Urutkan elemen sebelum dan sesudah partisi
         quickSortImpl(arr, low, pi - 1);
         quickSortImpl(arr, pi + 1, high);
     }
 }
 
 void quickSort(int arr[], int n) {
     quickSortImpl(arr, 0, n - 1);
 }
 
 /**
  * Quick Sort untuk string
  */
 int partitionString(char *arr[], int low, int high) {
     char *pivot = arr[high];
     int i = (low - 1);
     int j;
     char *temp;
     
     for (j = low; j <= high - 1; j++) {
         // Jika elemen saat ini lebih kecil dari pivot
         if (strcmp(arr[j], pivot) < 0) {
             i++;
             // Tukar arr[i] dan arr[j]
             temp = arr[i];
             arr[i] = arr[j];
             arr[j] = temp;
         }
     }
     
     // Tukar arr[i + 1] dan arr[high] (atau pivot)
     temp = arr[i + 1];
     arr[i + 1] = arr[high];
     arr[high] = temp;
     
     return (i + 1);
 }
 
 void quickSortStringImpl(char *arr[], int low, int high) {
     if (low < high) {
         // pi adalah indeks partisi
         int pi = partitionString(arr, low, high);
         
         // Urutkan elemen sebelum dan sesudah partisi
         quickSortStringImpl(arr, low, pi - 1);
         quickSortStringImpl(arr, pi + 1, high);
     }
 }
 
 void quickSortString(char *arr[], int n) {
     quickSortStringImpl(arr, 0, n - 1);
 }
 
 /**
  * Shell Sort
  * Prinsip kerja: Merupakan varian dari insertion sort yang memungkinkan pertukaran
  * elemen yang jauh satu sama lain. Algoritma ini membandingkan elemen yang berjarak
  * tertentu, kemudian mengurangi jarak tersebut secara bertahap hingga jarak = 1.
  * 
  * Kompleksitas Waktu: Tergantung dari urutan gap, umumnya O(n log² n).
  * Kompleksitas Ruang: O(1) karena sorting dilakukan secara in-place.
  */
 void shellSort(int arr[], int n) {
     int gap, i, j, temp;
     
     // Mulai dengan gap besar, kemudian kurangi
     for (gap = n/2; gap > 0; gap /= 2) {
         // Lakukan insertion sort untuk elemen-elemen yang jaraknya = gap
         for (i = gap; i < n; i++) {
             temp = arr[i];
             
             // Geser elemen-elemen yang sudah diurutkan sampai
             // posisi yang tepat untuk arr[i] ditemukan
             for (j = i; j >= gap && arr[j - gap] > temp; j -= gap) {
                 arr[j] = arr[j - gap];
             }
             
             // Tempatkan temp (elemen asli arr[i]) di posisinya
             arr[j] = temp;
         }
     }
 }
 
 /**
  * Shell Sort untuk string
  */
 void shellSortString(char *arr[], int n) {
     int gap, i, j;
     char *temp;
     
     // Mulai dengan gap besar, kemudian kurangi
     for (gap = n/2; gap > 0; gap /= 2) {
         // Lakukan insertion sort untuk elemen-elemen yang jaraknya = gap
         for (i = gap; i < n; i++) {
             temp = arr[i];
             
             // Geser elemen-elemen yang sudah diurutkan sampai
             // posisi yang tepat untuk arr[i] ditemukan
             for (j = i; j >= gap && strcmp(arr[j - gap], temp) > 0; j -= gap) {
                 arr[j] = arr[j - gap];
             }
             
             // Tempatkan temp (elemen asli arr[i]) di posisinya
             arr[j] = temp;
         }
     }
 }
 
 #endif // SORTING_H