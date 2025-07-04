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
