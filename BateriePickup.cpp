// Fill out your copyright notice in the Description page of Project Settings.

/*
#include "BateriePickup.h"

// Sets default values
ABateriePickup::ABateriePickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABateriePickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABateriePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

*/

#include "BateriePickup.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/ChildActorComponent.h"
#include "Baza_Caracter.h" // bateria trebuie sa stie cum arata jucatorul tau

// 1. CONSTRUCTORUL
ABateriePickup::ABateriePickup()
{
	PrimaryActorTick.bCanEverTick = false;

	// cream colision box ul si o setam ca rootcomponent
	SferaColiziune = CreateDefaultSubobject<USphereComponent>(TEXT("SferaColiziune"));
	SferaColiziune->InitSphereRadius(50.0f); // raza sferei de coliziune
	SferaColiziune->SetCollisionProfileName(TEXT("Trigger")); // functioneaza pe trigger base
	RootComponent = SferaColiziune;

	// crearea mesh si atasare de sfera 
	MeshBaterie = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshBaterie"));
	MeshBaterie->SetupAttachment(RootComponent);
	MeshBaterie->SetCollisionEnabled(ECollisionEnabled::NoCollision); // mesh-ul e doar de forma

	// legam fct de overlap la evenimentul sferei 
	SferaColiziune->OnComponentBeginOverlap.AddDynamic(this, &ABateriePickup::OnOverlapBegin);


	// cream containerul pt bec si il atasam
	BecComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("BecComponent"));
	BecComponent->SetupAttachment(RootComponent);
}

// 2. BEGIN PLAY
void ABateriePickup::BeginPlay()
{
	Super::BeginPlay();
}

// 3. LOGICA DE ATINGERE
void ABateriePickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this))
	{
		ABaza_Caracter* Jucator = Cast<ABaza_Caracter>(OtherActor);
		if (Jucator)
		{
			Jucator->AdaugaBaterie(EnergieData);

			// --- LOGICA DE DISPARITIE ( NU DE DESTROY ) 

			// 1. ascundem actorul -- adica ascundem cubul si becul atasat 
			SetActorHiddenInGame(true);

			// 2. oprim coliziunea ca sa nu o mai putem lua a doua oara imediat
			SetActorEnableCollision(false);

			// 3. pornim cronometrul pentru respawn
			GetWorldTimerManager().SetTimer(TimerRespawn, this, &ABateriePickup::Respawn, TimpRespawn, false);
		}
	}
}


void ABateriePickup::Respawn()
{
	// --- LOGICA DE RESPAWN 

	SetActorHiddenInGame(false); // il facem vizibil din nou
	SetActorEnableCollision(true); // Reactivam coliziunea
}


// 4. TICK
void ABateriePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}