#include "Validare_RSA.h"

long long UValidare_RSA::ModExp(long long base, long long exp, long long mod) {
    long long res = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) res = (res * base) % mod;
        exp = exp >> 1;
        base = (base * base) % mod;
    }
    return res;
}

// Adaptat pentru FString din Unreal
long long UValidare_RSA::HashSimplu(const FString& data, long long n) {
    long long hash = 0;
    // Iteram prin caracterele native Unreal (TCHAR)
    for (int32 i = 0; i < data.Len(); ++i) {
        TCHAR c = data[i];
        hash = (hash * 31 + c) % n;
    }
    return hash;
}

bool UValidare_RSA::ValideazaCheieRSA(FString CheieIntrodusa) {
    const long long n = 91687;
    const long long e = 17; // Cheia Publica

    // 1. Cautam ultima cratima
    int32 LastDashIndex;
    if (!CheieIntrodusa.FindLastChar('-', LastDashIndex)) {
        return false; // Format invalid, nu are cratima
    }

    // 2. Separam Datele de Semnatura
    FString SerialDataFStr = CheieIntrodusa.Left(LastDashIndex);
    FString SignatureFStr = CheieIntrodusa.Mid(LastDashIndex + 1);

    // 3. Protectie anti-crash: verificam daca semnatura contine doar cifre
    if (!SignatureFStr.IsNumeric()) {
        return false;
    }

    // 4. Convertim string-ul in int64 in mod sigur
    long long signature = FCString::Atoi64(*SignatureFStr);

    // 5. Trimitem FString-ul direct la functia noastra de Hash
    long long expectedHash = HashSimplu(SerialDataFStr, n);
    long long decryptedHash = ModExp(signature, e, n);

    return (expectedHash == decryptedHash);
}