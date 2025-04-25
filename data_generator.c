#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

// Fungsi untuk membangkitkan data angka acak
void generate_random_numbers(const char *filename, int count, int max_value) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        perror("File tidak dapat dibuka");
        return;
    }

    srand(time(NULL)); // Inisialisasi seed
    for (int i = 0; i < count; i++) {
        int num = rand() % max_value;
        fprintf(fp, "%d\n", num);

        if (i % 100000 == 0) {
            printf("Progress angka: %d/%d\n", i, count);
        }
    }

    fclose(fp);
    printf("Data angka berhasil dibangkitkan: %s\n", filename);
}

// Fungsi untuk membangkitkan satu kata acak
void random_word(char *word, int length) {
    static const char charset[] = "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < length; i++) {
        int key = rand() % (int)(sizeof(charset) - 1);
        word[i] = charset[key];
    }
    word[length] = '\0';
}

// Fungsi untuk membangkitkan data kata acak
void generate_random_words(const char *filename, int count, int max_word_length) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        perror("File tidak dapat dibuka");
        return;
    }

    srand(time(NULL));
    char word[100];
    for (int i = 0; i < count; i++) {
        int length = (rand() % (max_word_length - 3)) + 3;
        random_word(word, length);
        fprintf(fp, "%s\n", word);

        if (i % 100000 == 0) {
            printf("Progress kata: %d/%d\n", i, count);
        }
    }

    fclose(fp);
    printf("Data kata berhasil dibangkitkan: %s\n", filename);
}

int main() {
    generate_random_numbers("data/data_angka.txt", 2000000, 2000000);

#ifdef _WIN32
    Sleep(1000); // 1000 ms
#else
    sleep(1);    // 1 detik
#endif

    generate_random_words("data/data_kata.txt", 2000000, 20);

    return 0;
}
