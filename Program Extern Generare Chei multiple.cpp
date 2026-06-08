#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

// Functia de exponentiere modulara (neschimbata)
long long ModExp(long long base, long long exp, long long mod) {
    long long res = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) res = (res * base) % mod;
        exp = exp >> 1;
        base = (base * base) % mod;
    }
    return res;
}

// Functia de Hash (neschimbata)
long long HashSimplu(const std::string& data, long long n) {
    long long hash = 0;
    for (char c : data) {
        hash = (hash * 31 + c) % n;
    }
    return hash;
}

//  Genereaza un sir aleatoriu de tip "XXXX-YYYY"
std::string GenereazaTextAleatoriu() {
    const std::string caractere = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::string rezultat = "";
    
    for (int i = 0; i < 9; ++i) {
        if (i == 4) {
            rezultat += "-";
        } else {
            rezultat += caractere[rand() % caractere.length()];
        }
    }
    return rezultat;
}

int main() {
    // Initializam generatorul de numere aleatoare
    srand(time(0));

    // Cheia privata (SECRETA)
    const long long n = 91687;
    const long long d = 16073;

    int numarCheiDeGenerat = 5; // Modifica aici cate chei vrei sa generezi

    std::cout << "--- CHEI DE ACTIVARE GENERATE ---\n\n";

    for (int i = 0; i < numarCheiDeGenerat; ++i) {
        // 1. Generam o baza complet aleatorie pentru cheie
        std::string serialData = GenereazaTextAleatoriu();

        // 2. Calculam Hash-ul acestui text nou
        long long hashValue = HashSimplu(serialData, n);

        // 3. Semnam digital (Criptam Hash-ul cu cheia privata)
        long long signature = ModExp(hashValue, d, n);

        // 4. Afisam cheia finala
        std::cout << "Cheia " << (i + 1) << ": " << serialData << "-" << signature << "\n";
    }

    return 0;
}