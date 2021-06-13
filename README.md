# Laporan Resmi Soal Shift 4
Kelompok B05
* Amanda Rozi Kurnia 05111940000094
* Melanchthon Bonifacio Butarbutar 05111940000097
* Nadia Tiara Febriana 05111940000217

[Soal Shift 4](https://docs.google.com/document/d/1KlAG2thAMm42so8BZMylw216EETVGu_DAM5PfwJzl5U/edit)

## Table of Contents
1. [Soal 1](#soal1)
2. [Soal 2](#soal2)
3. [Soal 3](#soal3)
4. [Soal 4](#soal4)

<a name="soal1"></a>
## Soal 1
Kita diminta untuk membuat filesystem dengan ketentuan sebagai berikut.
- Jika sebuah direktori dibuat dengan awalan “AtoZ_”, maka direktori tersebut akan menjadi direktori ter-encode.
- Jika sebuah direktori di-rename dengan awalan “AtoZ_”, maka direktori tersebut akan menjadi direktori ter-encode.
- Apabila direktori yang terenkripsi di-rename menjadi tidak ter-encode, maka isi direktori tersebut akan terdecode.
- Setiap pembuatan direktori ter-encode (mkdir atau rename) akan tercatat ke sebuah log. Format: ``` /home/[USER]/Downloads/[Nama Direktori] → /home/[USER]/Downloads/AtoZ_[Nama Direktori]```
- Metode encode pada suatu direktori juga berlaku terhadap direktori yang ada di dalamnya (rekursif).

### Jawab soal 1
Fungsi yang dipakai untuk encode dan decode menggunakan Atbash cipher (mirror) adalah sebagai berikut. <br>
**Encode:**
```C
void encodeAtbash(char *str)
{
    if (strcmp(str, ".") == 0 || strcmp(str, "..") == 0)
        return;

    printf("Encode path Atbash: %s\n", str);

    int lenght = strlen(str);
    for (int i = 0; i < lenght; i++)
    {
        if (str[i] == '/')
            continue;
        if (str[i] == '.')
            break;

        if (str[i] >= 'A' && str[i] <= 'Z')
        {
            str[i] = 'Z' + 'A' - str[i];
        }
        else if (st[i] >= 'a' && str[i] <= 'z')
        {
            str[i] = 'z' + 'a' - str[i];
        }
    }
}
```
**Decode:**
```C
void decodeAtbash(char *str)
{
    if (strcmp(str, ".") == 0 || strcmp(str, "..") == 0 || strstr(str, "/") == NULL)
        return;

    printf("Decode path Atbash: %s\n", str);

    int length = strlen(str), s = 0;
    for (int i = length-1; i >= 0; i--)
    {
        if (str[i] == '/')
            break;

        if (str[i] == '.') // titik terakhir
        {
            length = i;
            break;
        }
    }

    for (int i = 0; i < length; i++)
    {
        if (str[i] == '/')
        {
            s = i + 1;
            break;
        }
    }

    for (int i = s; i < length; i++)
    {
        if (str[i] == '/')
        {
            continue;
        }
        
        if (str[i] >= 'A' && str[i] <= 'Z')
        {
            str[i] = 'Z' + 'A' - str[i];
        }
        else if (st[i] >= 'a' && str[i] <= 'z')
        {
            str[i] = 'z' + 'a' - str[i];
        }
    }
}
```
Penjelasan:
* Pemanggilan fungsi ```decodeAtbash()``` dilakukan pada tiap utility functions seperti ```getattr```, ```rename```, ```mkdir``` dan fungsi-fungi lain. 
* Pemanggilan fungsi ```encodeAtbash()``` dan ```decodeAtbash()``` dilakukan di utility function ```readdir``` karena FUSE akan melakukan decode di mount folder kemudian encode di FUSE saat  ```readdir```. 
* Pemanggilan dilakukan dengan pengecekan apakah string ```AtoZ_``` ada dalam path di masing-masing utility function. Pengecekan menggunakan fungsi ```strstr()```. 
* Fungsi tersebut akan mengabaikan ```/``` dan berhenti ketika bertemu ```.``` yang merupakan ekstensi dari file. <br><br>

Untuk poin d, yaitu pembuatan direktori ter-encode (mkdir atau rename) akan tercatat ke sebuah log dilakukan pada fungsi berikut.
```C
// log for mkdir and rename
void writingLog2(const char *from, char *to)
{
    FILE *fp = fopen("/home/yoursemicolon/fs.log", "a");

    fprintf(str, "%s -> %s\n", from, to);
    fclose(fp);
}
```
Fungsi akan dipanggil ketika membuat ```mkdir``` atau me-renname folder yang berawalan ```AtoZ_```. Jika file log belum ada di direktori, file log baru akan dibuat. 

### Screenshot Hasil Pengerjaan Nomor 1

<a name="soal2"></a>
## Soal 2
- Jika sebuah direktori dibuat dengan awalan “RX_[Nama]”, maka direktori tersebut akan menjadi direktori terencode beserta isinya dengan perubahan nama isi sesuai kasus nomor 1 dengan algoritma tambahan ROT13 (Atbash + ROT13).
- Jika sebuah direktori di-rename dengan awalan “RX_[Nama]”, maka direktori tersebut akan menjadi direktori terencode beserta isinya dengan perubahan nama isi sesuai dengan kasus nomor 1 dengan algoritma tambahan Vigenere Cipher dengan key “SISOP” (Case-sensitive, Atbash + Vigenere).
- Apabila direktori yang terencode di-rename (Dihilangkan “RX_” nya), maka folder menjadi tidak terencode dan isi direktori tersebut akan terdecode berdasar nama aslinya.
- Setiap pembuatan direktori terencode (mkdir atau rename) akan tercatat ke sebuah log file beserta methodnya (apakah itu mkdir atau rename).
- Pada metode enkripsi ini, file-file pada direktori asli akan menjadi terpecah menjadi file-file kecil sebesar 1024 bytes, sementara jika diakses melalui filesystem rancangan Sin dan Sei akan menjadi normal. Sebagai contoh, Suatu_File.txt berukuran 3 kiloBytes pada directory asli akan menjadi 3 file kecil yakni:
```
Suatu_File.txt.0000
Suatu_File.txt.0001
Suatu_File.txt.0002
```
Ketika diakses melalui filesystem hanya akan muncul Suatu_File.t

```
```

<a name="soal3"></a>
## Soal 3
- Jika sebuah direktori dibuat dengan awalan “A_is_a_”, maka direktori tersebut akan menjadi sebuah direktori spesial.
- Jika sebuah direktori di-rename dengan memberi awalan “A_is_a_”, maka direktori tersebut akan menjadi sebuah direktori spesial.
- Apabila direktori yang terenkripsi di-rename dengan menghapus “A_is_a_” pada bagian awal nama folder maka direktori tersebut menjadi direktori normal.
- Direktori spesial adalah direktori yang mengembalikan enkripsi/encoding pada direktori “AtoZ_” maupun “RX_” namun masing-masing aturan mereka tetap berjalan pada direktori di dalamnya (sifat recursive  “AtoZ_” dan “RX_” tetap berjalan pada subdirektori).
- Pada direktori spesial semua nama file (tidak termasuk ekstensi) pada fuse akan berubah menjadi lowercase insensitive dan diberi ekstensi baru berupa nilai desimal dari binner perbedaan namanya.
Contohnya jika pada direktori asli nama filenya adalah “FiLe_CoNtoH.txt” maka pada fuse akan menjadi “file_contoh.txt.1321”. 1321 berasal dari biner 10100101001.

```
```

<a name="soal4"></a>
## Soal 4
- Log system yang akan terbentuk bernama “SinSeiFS.log” pada direktori home pengguna (/home/[user]/SinSeiFS.log). Log system ini akan menyimpan daftar perintah system call yang telah dijalankan pada filesystem.
- Karena Sin dan Sei suka kerapian maka log yang dibuat akan dibagi menjadi dua level, yaitu INFO dan WARNING.
- Untuk log level WARNING, digunakan untuk mencatat syscall rmdir dan unlink.
- Sisanya, akan dicatat pada level INFO.
- Format untuk logging yaitu:
```
[Level]::[dd][mm][yyyy]-[HH]:[MM]:[SS]:[CMD]::[DESC :: DESC]

Level : Level logging, dd : 2 digit tanggal, mm : 2 digit bulan, yyyy : 4 digit tahun, HH : 2 digit jam (format 24 Jam),MM : 2 digit menit, SS : 2 digit detik, CMD : System Call yang terpanggil, DESC : informasi dan parameter tambahan

INFO::28052021-10:00:00:CREATE::/test.txt
INFO::28052021-10:01:00:RENAME::/test.txt::/rename.txt
```

```
```
