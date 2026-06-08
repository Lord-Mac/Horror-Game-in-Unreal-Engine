#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Validare_RSA.generated.h"

UCLASS()
class MYPROJECT_API UValidare_RSA : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	// Nodul care va aparea in Blueprint
	UFUNCTION(BlueprintPure, Category = "Securitate")
	static bool ValideazaCheieRSA(FString CheieIntrodusa);

private:
	// Functiile ajutatoare
	static long long ModExp(long long base, long long exp, long long mod);

	// ATENTIE: Am schimbat din std::string in FString aici!
	static long long HashSimplu(const FString& data, long long n);
};