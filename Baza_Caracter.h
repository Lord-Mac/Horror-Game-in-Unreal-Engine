// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Baza_Caracter.generated.h"

UCLASS()
class MYPROJECT_API ABaza_Caracter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaza_Caracter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	

    
public:
    // Baterie curenta : 
    UPROPERTY(BlueprintReadOnly, Category = "Lanterna")
    float BaterieCurenta;

    // Baterie initiala : 100
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lanterna")
    float BaterieMaxima = 100.0f;

    // Functia apelata din blueprint la apasarea tastei "F"
    UFUNCTION(BlueprintCallable, Category = "Lanterna")
    void SeteazaStareConsum(bool bActiv);

    // Event apelat din Blueprinty cand bateria e 0
    UFUNCTION(BlueprintImplementableEvent, Category = "Lanterna")
    void OnBaterieTerminata();

protected:
    FTimerHandle TimerBaterie;
    void ConsumaBaterie();
public:
    // functie apelata de bateria de pe jos 
    UFUNCTION(BlueprintCallable, Category = "Lanterna")
    void AdaugaBaterie(float CantitateEnergie);
};
