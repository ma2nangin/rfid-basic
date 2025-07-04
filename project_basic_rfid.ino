/*
   Tujuan:
   Membaca UID (Unique ID) dari tag RFID/NFC (kartu atau gantungan kunci)
   menggunakan modul MFRC522 dan menampilkannya di Serial Monitor Arduino.

   Persyaratan:
   - Arduino Uno (atau kompatibel) -- <--- KODINGAN INI UNTUK ARDUINO UNO
   - Modul RFID-RC522
   - Library MFRC522 by Udo Klein (atau yang setara)

   Koneksi (Wiring) Arduino Uno dengan MFRC522:
   --------------------------------------------------------------
   MFRC522 Pin | Arduino Uno Pin | Keterangan
   --------------------------------------------------------------
   SDA (SS)    | D10             | Slave Select (Pin SPI SS pada Uno)
   SCK         | D13             | Serial Clock (Pin SPI SCK pada Uno)
   MOSI        | D11             | Master Out Slave In (Pin SPI MOSI pada Uno)
   MISO        | D12             | Master In Slave Out (Pin SPI MISO pada Uno)
   IRQ         | (Tidak terhubung/opsional dalam contoh ini)
   GND         | GND             | Ground
   RST         | D9              | Reset (Pin Digital biasa, bisa diubah)
   3.3V        | 3.3V            | Power Supply (PENTING: JANGAN 5V!)
   --------------------------------------------------------------
*/

#include <SPI.h>       // Library untuk komunikasi SPI (digunakan oleh Arduino Uno untuk MFRC522)
#include <MFRC522.h>   // Library untuk modul MFRC522

// Definisikan pin SDA (SS) dan RST yang terhubung ke Arduino
// Pin-pin ini adalah standar atau sering digunakan untuk MFRC522 pada Arduino Uno
#define SS_PIN 10  // Pin SDA (SS) MFRC522 terhubung ke pin digital 10 Arduino
#define RST_PIN 9  // Pin RST MFRC522 terhubung ke pin digital 9 Arduino

// Inisialisasi objek MFRC522
MFRC522 rfid(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(9600); // Mulai komunikasi serial pada baud rate 9600
  while (!Serial);    // Tunggu hingga Serial Monitor siap (khusus untuk board tertentu)

  SPI.begin();        // Inisialisasi komunikasi SPI (penting untuk Arduino Uno)
  rfid.PCD_Init();    // Inisialisasi modul MFRC522

  Serial.println("Tempelkan tag RFID pada reader...");
  Serial.println(""); // Baris kosong untuk kerapian
}

void loop() {
  // Mencari tag baru
  // PCD_IsNewCardPresent() mengembalikan true jika ada tag baru terdeteksi
  if ( ! rfid.PICC_IsNewCardPresent()) {
    return; // Tidak ada tag baru, keluar dari loop()
  }

  // Membaca UID dari tag
  // PICC_ReadCardSerial() mengembalikan true jika UID berhasil dibaca
  if ( ! rfid.PICC_ReadCardSerial()) {
    return; // Gagal membaca UID, keluar dari loop()
  }

  Serial.print("UID Tag: ");
  // Mencetak UID dalam format heksadesimal
  for (byte i = 0; i < rfid.uid.size; i++) {
    Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "); // Tambahkan '0' di depan jika kurang dari 16 (heksadesimal)
    Serial.print(rfid.uid.uidByte[i], HEX); // Cetak byte UID dalam format heksadesimal
  }
  Serial.println(); // Pindah baris

  // Menghentikan pembacaan tag saat ini untuk menghindari duplikasi
  rfid.PICC_HaltA(); // Menghentikan PICC yang dipilih
  rfid.PCD_StopCrypto1(); // Menghentikan enkripsi PCD

  Serial.println(""); // Baris kosong untuk kerapian
  Serial.println("Tempelkan tag RFID lainnya...");
}