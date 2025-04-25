# 2308107010002_Tugas4_SDA_2025

# Tugas 4 - Struktur Data dan Algoritma

## Nama Mahasiswa
**SHAFA DISYA AULIA**

## NPM
**2308107010002**

## Deskripsi Tugas

Tugas ini bertujuan untuk melakukan implementasi dan analisis performa dari enam algoritma sorting, yaitu:

- Bubble Sort  
- Selection Sort  
- Insertion Sort  
- Merge Sort  
- Quick Sort  
- Shell Sort

Pengujian dilakukan terhadap dua jenis data:
1. **Data Angka (Number)**
2. **Data Kata (String)**

Ukuran data bervariasi dari 10.000 hingga 2.000.000 elemen. Untuk setiap algoritma dan ukuran data, dilakukan pencatatan:
- Waktu eksekusi (dalam detik)
- Penggunaan memori (dalam byte)
- Status eksekusi (berhasil/gagal)

Seluruh hasil pengujian disimpan ke dalam file `results.csv` dan divisualisasikan menggunakan grafik (matplotlib/seaborn) untuk membandingkan performa antar algoritma berdasarkan waktu dan penggunaan memori.

## Hasil Analisis Singkat

- Quick Sort dan Merge Sort menjadi algoritma tercepat secara umum.
- Shell Sort dan Quick Sort menunjukkan efisiensi memori yang baik.
- Bubble, Selection, dan Insertion Sort tidak cocok untuk data besar.
- Sorting data string lebih lambat dan lebih boros memori dibanding data angka karena komparasi string lebih kompleks.

