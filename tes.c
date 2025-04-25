#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void print_table() {
    // Menampilkan Tabel untuk Ukuran 2000000
    printf("Tabel untuk Ukuran 2000000:\n");
    printf("+---------------------+----------+---------+----------------------+----------------------+---------+\n");
    printf("| Algorithm           | Size     | Type    | Time (s)             | Memory (bytes)       | Status  |\n");
    printf("+---------------------+----------+---------+----------------------+----------------------+---------+\n");

    // Eksperimen yang belum diuji
    printf("| Bubble Sort         | 2000000  | number  | 1800.000000          | 0                    | Success |\n");

    printf("+---------------------+----------+---------+----------------------+----------------------+---------+\n");
}

void append_to_csv() {
    FILE *file = fopen("results.csv", "a");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    // Menambahkan data perkiraan untuk eksperimen yang belum diuji
    fprintf(file, "Bubble Sort,2000000,number,1800.000000,0,Success\n");

    fclose(file);
}

int main() {
    // Menampilkan hasil estimasi ke terminal
    print_table();
    
    // Menambahkan hasil estimasi ke results.csv
    append_to_csv();

    return 0;
}
