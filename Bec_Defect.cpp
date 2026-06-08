// Fill out your copyright notice in the Description page of Project Settings.

/*   
#include "Bec_Defect.h"

// Sets default values
ABec_Defect::ABec_Defect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABec_Defect::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABec_Defect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

*/


#include "Bec_Defect.h"
#include "Components/PointLightComponent.h"
#include "TimerManager.h"

// 1. CONSTRUCTORUL -- asamblarea becului inainte sa inceapa jocul 
ABec_Defect::ABec_Defect()
{
	// Oprim Tick-ul ca sa nu manance resurse, vom folosi timer
	PrimaryActorTick.bCanEverTick = false;

	// cream becul in cod
	LuminaPunct = CreateDefaultSubobject<UPointLightComponent>(TEXT("BeculMeu"));

	// ii face rootcomponent ca sa il putem muta in harta
	RootComponent = LuminaPunct;
}

// 2. BEGIN PLAY -- dam drumul la timer cand incepe jocul.
void ABec_Defect::BeginPlay()
{
	Super::BeginPlay();

	if (LuminaPunct)
	{
		LuminaPunct->SetIntensity(IntensitateImplicita);
		LuminaPunct->SetLightColor(CuloareImplicita);
	}
	// Pornim cronometrul DOAR daca i-am spus sa palpaie din interfata
	if (bPalpaie)
	{
		GetWorldTimerManager().SetTimer(TimerBec, this, &ABec_Defect::ToggleLumina, TimpFlicker, true);
	}

	// Pornim cronometrul care apeleaza fct ToggleLumina la fiecare x secunde (TimpFlicker)
	GetWorldTimerManager().SetTimer(TimerBec, this, &ABec_Defect::ToggleLumina, TimpFlicker, true);
}

// 3. Functia de palpait -- ii spunem ce sa faca
void ABec_Defect::ToggleLumina()
{
	// verificam daca becul exista  -- masura de protectie ca sa nu dea crash
	if (LuminaPunct)
	{
		// alterneaza intre stins si aprins
		LuminaPunct->ToggleVisibility();
	}
}

// 4. TICK -- lasam functia goala, chiar daca nu face nimic deoarece e ceruta de unreal
void ABec_Defect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}