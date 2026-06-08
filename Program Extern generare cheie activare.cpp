#include <iostream>
#include <string>
#include <sstream>

// Algoritm de exponentiere modulara: (baza^exponent) % mod
// Este obligatoriu deoarece numerele cresc exponential si depasesc memoria
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

// Functie simpla care transforma textul ("FMI-2026") intr-un numar < n (91687)
long long HashSimplu(const std::string& data, long long n) {
    long long hash = 0;
    for (char c : data) {
        hash = (hash * 31 + c) % n;
    }
    return hash;
}

int main() {
    // 1. Definim cheia privata (SECRETA)
    const long long n = 91687;
    const long long d = 16073; // Exponentul privat
    // const long long d = 53573; // Exponentul privat

    // 2. Partea de date a cheii de activare
    std::string serialData = "PROIECT-LICENTA-FMI";

    // 3. Calculam Hash-ul textului
    long long hashValue = HashSimplu(serialData, n);

    // 4. SEMNATURA DIGITALA (Criptam Hash-ul cu cheia privata)
    // Formula: S = (Hash ^ d) % n
    long long signature = ModExp(hashValue, d, n);

    // 5. Cheia finala pe care i-o dai jucatorului
    std::cout << "Cheia ta de activare este: \n";
    std::cout << serialData << "-" << signature << "\n";
    //std::cout  << signature << "\n";
    return 0;
}