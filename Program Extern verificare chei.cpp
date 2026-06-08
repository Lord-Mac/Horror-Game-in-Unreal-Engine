#include <iostream>
#include <string>
#include <sstream>

// Aceeasi functie de exponentiere modulara
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

// Acelasi algoritm de Hash
long long HashSimplu(const std::string& data, long long n) {
    long long hash = 0;
    for (char c : data) {
        hash = (hash * 31 + c) % n;
    }
    return hash;
}

bool ValideazaCheia(const std::string& cheieIntrodusa) {
    // 1. Definim cheia publica (Siguranta sa fie vazuta de oricine)
    const long long n = 91687;
    const long long e = 17; // Exponentul public
    
    // 2. Separam cheia in "Date" si "Semnatura"
    size_t lastDash = cheieIntrodusa.find_last_of('-');
    if (lastDash == std::string::npos) return false; // Format invalid
    
    std::string serialData = cheieIntrodusa.substr(0, lastDash);
    std::string signatureStr = cheieIntrodusa.substr(lastDash + 1);
    long long signature = std::stoll(signatureStr);
    
    // 3. Recalculam Hash-ul din textul de baza
    long long expectedHash = HashSimplu(serialData, n);
    
    // 4. VERIFICAREA RSA (Decriptam semnatura cu cheia publica)
    // Formula: H = (Semnatura ^ e) % n
    long long decryptedHash = ModExp(signature, e, n);
    
    // 5. Comparatia finala
    return (expectedHash == decryptedHash);
}

int main() {
    // Jucatorul introduce cheia generata anterior
    //std::string userInput = "PROIECT-LICENTA-FMI-48192";
    //std::string userInput = "PROIECT-LICENTA-FMI-4733";
    std::string userInput = "72YY-L42F-85581";
    
    if (ValideazaCheia(userInput)) {
        std::cout << "SUCCES! Jocul este activat." << "\n";
    } else {
        std::cout << "EROARE! Cheie falsa sau corupta." << "\n";
    }

    return 0;
}