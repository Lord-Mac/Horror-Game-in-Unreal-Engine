// Fill out your copyright notice in the Description page of Project Settings.

#include "Baza_Caracter.h"
#include "TimerManager.h" //pentru cronometru.

// Sets default values
ABaza_Caracter::ABaza_Caracter()
{
    // Set this character to call Tick() every frame.
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABaza_Caracter::BeginPlay()
{
    Super::BeginPlay();

    BaterieCurenta = BaterieMaxima; // incepem cu bateria plina
}

// Called every frame
void ABaza_Caracter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABaza_Caracter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// ============ LOGICA BATERIE ====================================================================

void ABaza_Caracter::SeteazaStareConsum(bool bActiv)
{
    if (bActiv && BaterieCurenta > 0)
    {
        // pornim timer-ul, scade 1 la fiecare secundă
        GetWorldTimerManager().SetTimer(TimerBaterie, this, &ABaza_Caracter::ConsumaBaterie, 1.0f, true);
    }
    else
    {
        // oprim timer-ul
        GetWorldTimerManager().ClearTimer(TimerBaterie);
    }
}

void ABaza_Caracter::ConsumaBaterie()
{
    BaterieCurenta -= 1.0f;

    if (BaterieCurenta <= 0)
    {
        BaterieCurenta = 0;
        GetWorldTimerManager().ClearTimer(TimerBaterie); // oprim timer-ul intern
        OnBaterieTerminata(); // trimitem semnal catre Blueprint sa stinga becul
    }
}

void ABaza_Caracter::AdaugaBaterie(float CantitateEnergie)
{
    // Adaugam energia
    BaterieCurenta += CantitateEnergie;

    // ne asiguram ca nu depasim maximul ,adica sa nu fim peste 100
    if (BaterieCurenta > BaterieMaxima)
    {
        BaterieCurenta = BaterieMaxima;
    }
}