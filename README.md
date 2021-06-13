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
Kondisi awal
<img src="https://github.com/yoursemicolon/soal-shift-sisop-modul-4-B05-2021/blob/main/screenshot/screenshot-1.png"></img><br>

Kondisi setelah encode
<img src="https://github.com/yoursemicolon/soal-shift-sisop-modul-4-B05-2021/blob/main/screenshot/screenshot-2.png"></img><br>

Kondisi setelah decode
<img src="https://github.com/yoursemicolon/soal-shift-sisop-modul-4-B05-2021/blob/main/screenshot/screenshot-3.png"></img><br>

Hasil log
<img src="https://github.com/yoursemicolon/soal-shift-sisop-modul-4-B05-2021/blob/main/screenshot/screenshot-4.png"></img><br>


<a name="soal2"></a>
## Soal 2
Pada soal ini kita diminta untuk membuat metode enkripsi tambahan untuk filesystem dengan ketentuan sebagai berikut.
1. (a) Jika sebuah direktori dibuat dengan awalan “RX_[Nama]”, maka direktori tersebut akan menjadi direktori terencode beserta isinya dengan perubahan nama isi sesuai kasus nomor 1 dengan algoritma tambahan ROT13 (Atbash + ROT13).
2. (b) Jika sebuah direktori di-rename dengan awalan “RX_[Nama]”, maka direktori tersebut akan menjadi direktori terencode beserta isinya dengan perubahan nama isi sesuai dengan kasus nomor 1 dengan algoritma tambahan Vigenere Cipher dengan key “SISOP” (Case-sensitive, Atbash + Vigenere).
3. (c) Apabila direktori yang terencode di-rename (Dihilangkan “RX_” nya), maka folder menjadi tidak terencode dan isi direktori tersebut akan terdecode berdasar nama aslinya.
4. (d) Setiap pembuatan direktori terencode (mkdir atau rename) akan tercatat ke sebuah log file beserta methodnya (apakah itu mkdir atau rename).
5. (e) Pada metode enkripsi ini, file-file pada direktori asli akan menjadi terpecah menjadi file-file kecil sebesar 1024 bytes, sementara jika diakses melalui filesystem rancangan Sin dan Sei akan menjadi normal. Sebagai contoh, Suatu_File.txt berukuran 3 kiloBytes pada directory asli akan menjadi 3 file kecil yakni:
```
Suatu_File.txt.0000
Suatu_File.txt.0001
Suatu_File.txt.0002
```
Ketika diakses melalui filesystem hanya akan muncul Suatu_File.t

### Jawaban Soal 2
Pada utility function ```rename``` dilakukan pengecekan apakah direktori di-rename dengan menambahkan atau menghilangkan ```RX_```. Pengecekan dilakukan menggunakan fungsi ```strstr()```.

### Screenshot Hasil Pengerjaan Soal 2

<a name="soal3"></a>
## Soal 3


<a name="soal4"></a>
## Soal 4
Membuat log system dengan ketentuan sebagai berikut.
1. (a) Log system yang akan terbentuk bernama “SinSeiFS.log” pada direktori home pengguna (/home/[user]/SinSeiFS.log). Log system ini akan menyimpan daftar perintah system call yang telah dijalankan pada filesystem.
2. (b) Karena Sin dan Sei suka kerapian maka log yang dibuat akan dibagi menjadi dua level, yaitu INFO dan WARNING.
3. (c) Untuk log level WARNING, digunakan untuk mencatat syscall rmdir dan unlink.
4. (d) Sisanya, akan dicatat pada level INFO.
5. (e) Format untuk logging yaitu:
[Level]::[dd][mm][yyyy]-[HH]:[MM]:[SS]:[CMD]::[DESC :: DESC] <br>

Level : Level logging, dd : 2 digit tanggal, mm : 2 digit bulan, yyyy : 4 digit tahun, HH : 2 digit jam (format 24 Jam),MM : 2 digit menit, SS : 2 digit detik, CMD : System Call yang terpanggil, DESC : informasi dan parameter tambahan <br>

INFO::28052021-10:00:00:CREATE::/test.txt <br>
INFO::28052021-10:01:00:RENAME::/test.txt::/rename.txt


### Jawaban Soal 3
Fungsi untuk pembuatan dan pencatatan log system adalah sebagai berikut.
```C
// create log system
void writingLog(char *str, int logType)
{
    time_t rawtime;
    time(&rawtime);
    struct tm *info = = localtime(&rawtime);

    char curtime[30];
    strftime(curtime, 30, "%d%m%Y-%H:%M:%S", info);

    FILE *logFile = fopen(logpath, "a");

    if (logType == 1)
    {
        fprintf(logFile, "INFO::%s:%s\n", curtime, str);
    }
    else if (logType == 2)
    {
        fprintf(logFile, "WARNING::%s:%s\n", curtime, str);
    }
    fclose(logFile);
}
```
Jika syscall adalah ```rmdir``` atau ```unlink```, maka log levelnya akan dicatat sebagai ```WARNING```. Selebihnya akan dicatat sebagai ```INFO```. Selain itu, pencatatan juga dilengkapi dengan timestamp, system call yang terpanggil serta informasi dan parameter tambahan. Berikut contoh pemanggilan pada fungsi ```mkdir``` dan ```rmdir```. <BR>
Utility function xmp_mkdir():
```C
static int xmp_mkdir(const char *path, mode_t mode)
{
    ...

    sprintf(str, "MKDIR::%s", path);
    writingLog(str, 1);

    return 0;
}
```
Utility function xmp_rmdir():
```C
static int xmp_rmdir(const char *path)
{
    ...

    sprintf(str, "RMDIR::%s", path);
    writingLog(str, 2);

    return 0;
}
```

### Screenshot Hasil Pengerjaan Soal 4
Hasil log file SinSeiFS_B05.log
<img src="https://github.com/yoursemicolon/soal-shift-sisop-modul-4-B05-2021/blob/main/screenshot/screenshot-5.png"></img><br><br>

## Referensi
1. https://www.tutorialspoint.com/c_standard_library/c_function_localtime.htm
2. https://www.tutorialspoint.com/c_standard_library/c_function_strftime.htm
3. http://khyar28.blogspot.com/2015/11/file-system-fuse-implementasi-file.html
4. https://github.com/infosec-au/learning-c/blob/master/Week4/ROT13.c
