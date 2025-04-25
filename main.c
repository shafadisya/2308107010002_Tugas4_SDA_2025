/**
 * main.c
 * Program utama untuk menguji performa algoritma sorting
 * untuk Tugas 4 Struktur Data dan Algoritma
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <time.h>
 #include <windows.h> // Untuk penggunaan GetProcessMemoryInfo() di Windows
 #include <psapi.h>   // Untuk pengukuran memori di Windows
 #include "sorting.h"
 
 // Definisi konstanta
 #define MAX_LINE_LENGTH 100
 #define MAX_FILE_PATH 256
 #define MAX_ALGORITHM_NAME 20
 #define CSV_FILENAME "results.csv"
 
 // Definisi jumlah elemen yang akan diuji
 int DATA_SIZES[] = {10000, 50000, 100000, 250000, 500000, 1000000, 1500000, 2000000};
 int NUM_DATA_SIZES = 8;
 
 // Enum untuk tipe algoritma sorting
 typedef enum {
     BUBBLE_SORT,
     SELECTION_SORT,
     INSERTION_SORT,
     MERGE_SORT,
     QUICK_SORT,
     SHELL_SORT
 } SortAlgorithm;
 
 // Enum untuk tipe data
 typedef enum {
     NUMBER,
     STRING
 } DataType;
 
 // Struktur untuk menyimpan hasil pengujian
 typedef struct {
     char algorithm[MAX_ALGORITHM_NAME];
     int data_size;
     char data_type[10];
     double execution_time;
     size_t memory_usage;
     int status; // 0: sukses, -1: error
 } TestResult;
 
 // Fungsi untuk mendapatkan penggunaan memori saat ini (Windows)
 size_t getCurrentMemoryUsage() {
     PROCESS_MEMORY_COUNTERS pmc;
     GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
     return pmc.WorkingSetSize;
 }
 
 // Fungsi untuk membaca data angka dari file ke array
 int* readNumbers(const char* filename, int count) {
     FILE* file = fopen(filename, "r");
     if (!file) {
         perror("Error opening file");
         return NULL;
     }
     
     int* numbers = (int*)malloc(count * sizeof(int));
     if (!numbers) {
         fclose(file);
         perror("Memory allocation failed");
         return NULL;
     }
     
     char line[MAX_LINE_LENGTH];
     int i = 0;
     
     while (i < count && fgets(line, MAX_LINE_LENGTH, file)) {
         numbers[i++] = atoi(line);
     }
     
     fclose(file);
     return numbers;
 }
 
 // Fungsi untuk membaca data kata dari file ke array
 char** readStrings(const char* filename, int count) {
     FILE* file = fopen(filename, "r");
     if (!file) {
         perror("Error opening file");
         return NULL;
     }
     
     char** strings = (char**)malloc(count * sizeof(char*));
     if (!strings) {
         fclose(file);
         perror("Memory allocation failed");
         return NULL;
     }
     
     char line[MAX_LINE_LENGTH];
     int i = 0;
     
     while (i < count && fgets(line, MAX_LINE_LENGTH, file)) {
         // Hapus newline character
         size_t len = strlen(line);
         if (len > 0 && line[len-1] == '\n') {
             line[len-1] = '\0';
         }
         
         strings[i] = strdup(line);
         if (!strings[i]) {
             // Clean up if strdup fails
             for (int j = 0; j < i; j++) {
                 free(strings[j]);
             }
             free(strings);
             fclose(file);
             perror("Memory allocation failed");
             return NULL;
         }
         i++;
     }
     
     fclose(file);
     return strings;
 }
 
 // Fungsi untuk membebaskan memori array string
 void freeStrings(char** strings, int count) {
     for (int i = 0; i < count; i++) {
         free(strings[i]);
     }
     free(strings);
 }
 
 // Fungsi untuk menampilkan progress bar
 void displayProgressBar(int current, int total) {
     const int bar_width = 50;
     float progress = (float)current / total;
     int pos = bar_width * progress;
     
     printf("[");
     for (int i = 0; i < bar_width; i++) {
         if (i < pos) printf("=");
         else if (i == pos) printf(">");
         else printf(" ");
     }
     printf("] %d%% (%d/%d)\r", (int)(progress * 100.0), current, total);
     fflush(stdout);
 }
 
 // Fungsi untuk menyimpan hasil pengujian ke file CSV
 void saveResultToCSV(TestResult result) {
     FILE* file;
     int fileExists = 0;
     
     // Cek apakah file sudah ada
     file = fopen(CSV_FILENAME, "r");
     if (file != NULL) {
         fileExists = 1;
         fclose(file);
     }
     
     // Buka file untuk append
     file = fopen(CSV_FILENAME, "a");
     if (!file) {
         perror("Error opening CSV file");
         return;
     }
     
     // Tulis header jika file baru
     if (!fileExists) {
         fprintf(file, "Algorithm,Data Size,Data Type,Execution Time (s),Memory Usage (bytes),Status\n");
     }
     
     // Tulis hasil
     fprintf(file, "%s,%d,%s,%.6f,%zu,%s\n", 
             result.algorithm, 
             result.data_size, 
             result.data_type, 
             result.execution_time, 
             result.memory_usage,
             (result.status == 0) ? "Success" : "Error");
     
     fclose(file);
 }
 
 // Fungsi untuk menampilkan hasil pengujian dalam format tabel
 void displayResultTable(TestResult* results, int count) {
     printf("\n+----------------------+----------+----------+----------------------+----------------------+----------+\n");
     printf("| %-20s | %-8s | %-8s | %-20s | %-20s | %-8s |\n", "Algorithm", "Size", "Type", "Time (s)", "Memory (bytes)", "Status");
     printf("+----------------------+----------+----------+----------------------+----------------------+----------+\n");
     
     for (int i = 0; i < count; i++) {
         printf("| %-20s | %-8d | %-8s | %-20.6f | %-20zu | %-8s |\n",
                results[i].algorithm,
                results[i].data_size,
                results[i].data_type,
                results[i].execution_time,
                results[i].memory_usage,
                (results[i].status == 0) ? "Success" : "Error");
     }
     
     printf("+----------------------+----------+----------+----------------------+----------------------+----------+\n");
 }
 
 // Fungsi untuk melakukan pengujian algoritma sorting untuk angka
 TestResult testNumberSorting(SortAlgorithm algo, int* arr, int size, const char* algoName) {
     TestResult result;
     strcpy(result.algorithm, algoName);
     result.data_size = size;
     strcpy(result.data_type, "number");
     result.status = 0;
     
     // Ukur penggunaan memori sebelum sorting
     size_t memBefore = getCurrentMemoryUsage();
     
     // Ukur waktu eksekusi
     clock_t start = clock();
     
     // Eksekusi algoritma sorting yang sesuai
     switch (algo) {
         case BUBBLE_SORT:
             bubbleSort(arr, size);
             break;
         case SELECTION_SORT:
             selectionSort(arr, size);
             break;
         case INSERTION_SORT:
             insertionSort(arr, size);
             break;
         case MERGE_SORT:
             mergeSort(arr, size);
             break;
         case QUICK_SORT:
             quickSort(arr, size);
             break;
         case SHELL_SORT:
             shellSort(arr, size);
             break;
         default:
             result.status = -1;
             break;
     }
     
     clock_t end = clock();
     
     // Ukur penggunaan memori setelah sorting
     size_t memAfter = getCurrentMemoryUsage();
     
     // Hitung waktu eksekusi dan penggunaan memori
     result.execution_time = (double)(end - start) / CLOCKS_PER_SEC;
     result.memory_usage = memAfter - memBefore;
     
     return result;
 }
 
 // Fungsi untuk melakukan pengujian algoritma sorting untuk string
 TestResult testStringSorting(SortAlgorithm algo, char** arr, int size, const char* algoName) {
     TestResult result;
     strcpy(result.algorithm, algoName);
     result.data_size = size;
     strcpy(result.data_type, "string");
     result.status = 0;
     
     // Ukur penggunaan memori sebelum sorting
     size_t memBefore = getCurrentMemoryUsage();
     
     // Ukur waktu eksekusi
     clock_t start = clock();
     
     // Eksekusi algoritma sorting yang sesuai
     switch (algo) {
         case BUBBLE_SORT:
             bubbleSortString(arr, size);
             break;
         case SELECTION_SORT:
             selectionSortString(arr, size);
             break;
         case INSERTION_SORT:
             insertionSortString(arr, size);
             break;
         case MERGE_SORT:
             mergeSortString(arr, size);
             break;
         case QUICK_SORT:
             quickSortString(arr, size);
             break;
         case SHELL_SORT:
             shellSortString(arr, size);
             break;
         default:
         result.status = -1;
         break;
 }
 
 clock_t end = clock();
 
 // Ukur penggunaan memori setelah sorting
 size_t memAfter = getCurrentMemoryUsage();
 
 // Hitung waktu eksekusi dan penggunaan memori
 result.execution_time = (double)(end - start) / CLOCKS_PER_SEC;
 result.memory_usage = memAfter - memBefore;
 
 return result;
}

// Fungsi untuk melakukan pengujian semua algoritma sorting dengan satu ukuran data dan tipe data
void runTestsForDataSize(int size, DataType dataType) {
 const char* algorithmNames[] = {
     "Bubble Sort", "Selection Sort", "Insertion Sort",
     "Merge Sort", "Quick Sort", "Shell Sort"
 };
 
 SortAlgorithm algorithms[] = {
     BUBBLE_SORT, SELECTION_SORT, INSERTION_SORT,
     MERGE_SORT, QUICK_SORT, SHELL_SORT
 };
 
 int numAlgorithms = sizeof(algorithms) / sizeof(algorithms[0]);
 TestResult* results = malloc(numAlgorithms * sizeof(TestResult));
 
 if (dataType == NUMBER) {
     printf("\nPengujian untuk %d angka:\n", size);
     int* data = readNumbers("data/data_angka.txt", size);
     
     if (!data) {
         printf("Error: Gagal memuat data angka\n");
         free(results);
         return;
     }
     
     for (int i = 0; i < numAlgorithms; i++) {
         printf("- Menguji %s: ", algorithmNames[i]);
         
         // Buat salinan array untuk tiap algoritma
         int* arr = malloc(size * sizeof(int));
         if (!arr) {
             perror("Memory allocation failed");
             free(data);
             free(results);
             return;
         }
         memcpy(arr, data, size * sizeof(int));
         
         // Tampilkan progress bar
         displayProgressBar(0, 1);
         
         // Jalankan pengujian
         results[i] = testNumberSorting(algorithms[i], arr, size, algorithmNames[i]);
         
         // Update progress bar
         displayProgressBar(1, 1);
         printf("\n");
         
         // Simpan hasil ke CSV
         saveResultToCSV(results[i]);
         
         free(arr);
     }
     
     free(data);
 } else if (dataType == STRING) {
     printf("\nPengujian untuk %d kata:\n", size);
     char** data = readStrings("data/data_kata.txt", size);
     
     if (!data) {
         printf("Error: Gagal memuat data kata\n");
         free(results);
         return;
     }
     
     for (int i = 0; i < numAlgorithms; i++) {
         printf("- Menguji %s: ", algorithmNames[i]);
         
         // Buat salinan array untuk tiap algoritma
         char** arr = malloc(size * sizeof(char*));
         if (!arr) {
             perror("Memory allocation failed");
             freeStrings(data, size);
             free(results);
             return;
         }
         
         for (int j = 0; j < size; j++) {
             arr[j] = strdup(data[j]);
             if (!arr[j]) {
                 // Clean up if strdup fails
                 for (int k = 0; k < j; k++) {
                     free(arr[k]);
                 }
                 free(arr);
                 freeStrings(data, size);
                 free(results);
                 perror("Memory allocation failed");
                 return;
             }
         }
         
         // Tampilkan progress bar
         displayProgressBar(0, 1);
         
         // Jalankan pengujian
         results[i] = testStringSorting(algorithms[i], arr, size, algorithmNames[i]);
         
         // Update progress bar
         displayProgressBar(1, 1);
         printf("\n");
         
         // Simpan hasil ke CSV
         saveResultToCSV(results[i]);
         
         // Bebaskan memori
         freeStrings(arr, size);
     }
     
     freeStrings(data, size);
 }
 
 // Tampilkan hasil dalam bentuk tabel
 displayResultTable(results, numAlgorithms);
 
 free(results);
}

// Fungsi untuk memeriksa keberadaan file data
int checkDataFiles() {
 FILE* number_file = fopen("data/data_angka.txt", "r");
 FILE* word_file = fopen("data/data_kata.txt", "r");
 
 int result = (number_file != NULL) && (word_file != NULL);
 
 if (number_file) fclose(number_file);
 if (word_file) fclose(word_file);
 
 return result;
}

// Fungsi untuk menampilkan menu utama
void displayMainMenu() {
 printf("\n=================================================\n");
 printf("    PROGRAM ANALISIS PERFORMA ALGORITMA SORTING    \n");
 printf("=================================================\n");
 printf("1. Generate data angka dan kata\n");
 printf("2. Uji semua algoritma dengan data angka\n");
 printf("3. Uji semua algoritma dengan data kata\n");
 printf("4. Uji algoritma tertentu dengan data angka\n");
 printf("5. Uji algoritma tertentu dengan data kata\n");
 printf("6. Reset hasil pengujian\n");
 printf("0. Keluar\n");
 printf("-------------------------------------------------\n");
 printf("Pilihan Anda: ");
}

// Fungsi untuk menampilkan menu pemilihan algoritma
void displayAlgorithmMenu() {
 printf("\nPilih algoritma sorting:\n");
 printf("1. Bubble Sort\n");
 printf("2. Selection Sort\n");
 printf("3. Insertion Sort\n");
 printf("4. Merge Sort\n");
 printf("5. Quick Sort\n");
 printf("6. Shell Sort\n");
 printf("0. Kembali\n");
 printf("-------------------------------------------------\n");
 printf("Pilihan Anda: ");
}

// Fungsi untuk menampilkan menu pemilihan ukuran data
int displayDataSizeMenu() {
 printf("\nPilih ukuran data:\n");
 for (int i = 0; i < NUM_DATA_SIZES; i++) {
     printf("%d. %d data\n", i+1, DATA_SIZES[i]);
 }
 printf("0. Kembali\n");
 printf("-------------------------------------------------\n");
 printf("Pilihan Anda: ");
 
 int choice;
 scanf("%d", &choice);
 
 if (choice <= 0 || choice > NUM_DATA_SIZES) {
     return -1;
 }
 
 return DATA_SIZES[choice-1];
}

// Fungsi untuk menjalankan pengujian untuk satu algoritma dan tipe data tertentu
void runSingleAlgorithmTest(SortAlgorithm algo, const char* algoName, DataType dataType) {
 int dataSize = displayDataSizeMenu();
 if (dataSize == -1) return;
 
 printf("\nMenjalankan pengujian %s untuk %s dengan ukuran %d...\n", 
        algoName, (dataType == NUMBER) ? "angka" : "kata", dataSize);
 
 TestResult result;
 
 if (dataType == NUMBER) {
     int* data = readNumbers("data/data_angka.txt", dataSize);
     if (!data) {
         printf("Error: Gagal memuat data angka\n");
         return;
     }
     
     // Jalankan pengujian
     displayProgressBar(0, 1);
     result = testNumberSorting(algo, data, dataSize, algoName);
     displayProgressBar(1, 1);
     printf("\n");
     
     free(data);
 } else {
     char** data = readStrings("data/data_kata.txt", dataSize);
     if (!data) {
         printf("Error: Gagal memuat data kata\n");
         return;
     }
     
     // Jalankan pengujian
     displayProgressBar(0, 1);
     result = testStringSorting(algo, data, dataSize, algoName);
     displayProgressBar(1, 1);
     printf("\n");
     
     freeStrings(data, dataSize);
 }
 
 // Simpan dan tampilkan hasil
 saveResultToCSV(result);
 
 TestResult results[1] = {result};
 displayResultTable(results, 1);
}

// Fungsi untuk menjalankan pengujian untuk semua algoritma dan tipe data tertentu
void runAllTests(DataType dataType) {
 for (int i = 0; i < NUM_DATA_SIZES; i++) {
     printf("\n=================================================\n");
     printf("Ukuran data: %d\n", DATA_SIZES[i]);
     printf("=================================================\n");
     
     runTestsForDataSize(DATA_SIZES[i], dataType);
 }
}

// Fungsi untuk menghapus file hasil pengujian
void resetResults() {
 if (remove(CSV_FILENAME) == 0) {
     printf("File hasil pengujian berhasil dihapus.\n");
 } else {
     if (fopen(CSV_FILENAME, "r") == NULL) {
         printf("File hasil pengujian belum ada.\n");
     } else {
         printf("Gagal menghapus file hasil pengujian.\n");
     }
 }
}

int main() {
 int choice, algoChoice;
 
 const char* algorithmNames[] = {
     "Bubble Sort", "Selection Sort", "Insertion Sort",
     "Merge Sort", "Quick Sort", "Shell Sort"
 };
 
 SortAlgorithm algorithms[] = {
     BUBBLE_SORT, SELECTION_SORT, INSERTION_SORT,
     MERGE_SORT, QUICK_SORT, SHELL_SORT
 };
 
 while (1) {
     displayMainMenu();
     scanf("%d", &choice);
     
     switch (choice) {
         case 0:
             printf("\nTerima kasih telah menggunakan program ini!\n");
             return 0;
             
         case 1:
             printf("\nMemulai generate data...\n");
             system("gcc data_generator.c -o data_generator");
             system("data_generator");
             break;
             
         case 2:
             if (!checkDataFiles()) {
                 printf("\nError: File data tidak ditemukan. Silakan generate data terlebih dahulu.\n");
                 break;
             }
             printf("\nMemulai pengujian semua algoritma dengan data angka...\n");
             runAllTests(NUMBER);
             printf("\nPengujian selesai! Hasil disimpan di %s\n", CSV_FILENAME);
             break;
             
         case 3:
             if (!checkDataFiles()) {
                 printf("\nError: File data tidak ditemukan. Silakan generate data terlebih dahulu.\n");
                 break;
             }
             printf("\nMemulai pengujian semua algoritma dengan data kata...\n");
             runAllTests(STRING);
             printf("\nPengujian selesai! Hasil disimpan di %s\n", CSV_FILENAME);
             break;
             
         case 4:
             if (!checkDataFiles()) {
                 printf("\nError: File data tidak ditemukan. Silakan generate data terlebih dahulu.\n");
                 break;
             }
             displayAlgorithmMenu();
             scanf("%d", &algoChoice);
             if (algoChoice >= 1 && algoChoice <= 6) {
                 runSingleAlgorithmTest(algorithms[algoChoice-1], algorithmNames[algoChoice-1], NUMBER);
             } else if (algoChoice != 0) {
                 printf("\nPilihan tidak valid.\n");
             }
             break;
             
         case 5:
             if (!checkDataFiles()) {
                 printf("\nError: File data tidak ditemukan. Silakan generate data terlebih dahulu.\n");
                 break;
             }
             displayAlgorithmMenu();
             scanf("%d", &algoChoice);
             if (algoChoice >= 1 && algoChoice <= 6) {
                 runSingleAlgorithmTest(algorithms[algoChoice-1], algorithmNames[algoChoice-1], STRING);
             } else if (algoChoice != 0) {
                 printf("\nPilihan tidak valid.\n");
             }
             break;
             
         case 6:
             resetResults();
             break;
             
         default:
             printf("\nPilihan tidak valid. Silakan coba lagi.\n");
             break;
     }
 }
 
 return 0;
}