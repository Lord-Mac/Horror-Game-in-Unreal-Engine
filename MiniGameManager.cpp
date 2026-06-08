// Fill out your copyright notice in the Description page of Project Settings.
/*

#include "MiniGameManager.h"

// Sets default values
AMiniGameManager::AMiniGameManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMiniGameManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMiniGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

*/

#include "MiniGameManager.h"
#include "Math/UnrealMathUtility.h" //pentru FMath::RandRange -- nr aleatoare

// 1. CONSTRUCTORUL
AMiniGameManager::AMiniGameManager()
{
	//  ii spunem motorului sa apeleze Tick() in fiecare frame   !!! IMPORTANT
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;

	PlayerY = 0.0f;
	PlayerVelocity = 0.0f;
	ScorCurent = 0.0f;
}

void AMiniGameManager::BeginPlay()
{
	Super::BeginPlay();
}

// 2. FUNCTIILE DE CONTROL
void AMiniGameManager::StartJoc()
{
	// Resetam totul la inceperea unui joc nou
	PlayerY = PodeaY;
	PlayerVelocity = 0.0f;
	ScorCurent = 0.0f;
	VitezaObstacole = VitezaInitiala; // Ne asiguram ca "Play Again" incepe curat
	ObstacoleX.Empty(); // Golim lista de obstacole vechi
	
	ObstacoleX.Add(-2000.0f); // Index 0 (Alpha) -> Ascuns
	ObstacoleX.Add(-2000.0f); // Index 1 (Epsilon) -> Ascuns
	ObstacoleX.Add(-2000.0f); // Index 2 (Phi) -> Ascuns


	
	TipObstacol.Empty();
	TipObstacol.Add(0);
	TipObstacol.Add(0);
	TipObstacol.Add(0);
	
	bJocActiv = true;
}

void AMiniGameManager::Saritura()
{
	// jucatorul poate sari doar daca jocul a inceput si daca se afla fix pe podea (evitam zborul infinit)
	if (bJocActiv && PlayerY >= PodeaY)
	{
		// ii dam impulsul negativ ca sa il propulsam in sus pe ecran (adica pe axa y)
		PlayerVelocity = PutereSaritara;
	}
}

// 3. MOTORUL DE FIZICA (Ruleaza de aprox 60 de ori pe secunda)
void AMiniGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// daca jocul e pe pauza sau nu a inceput, nu calculam nimic ca sa salvam procesorul
	if (!bJocActiv) return;

	// crestem scorul atata timp cat jocul este activ
	ScorCurent += 100.0f * DeltaTime;//cresterea scorului

	// crestem viteza obstacolelor constant, facand jocul din ce in ce mai greu
	VitezaObstacole += Acceleratie * DeltaTime;

	// --- A. FIZICA JUCATORULUI ---
	// citeza creste constant din cauza gravitatiei
	PlayerVelocity += Gravitatie * DeltaTime;
	// pozitia se modifica in functie de viteza curenta
	PlayerY += PlayerVelocity * DeltaTime;

	// coliziunea cu podeaua (Daca scade sub podea, il fortam sa stea la nivelul ei)
	if (PlayerY >= PodeaY)
	{
		PlayerY = PodeaY;
		PlayerVelocity = 0.0f; // Se opreste din cazut
	}

	// --- B. GENERAREA (SPAWN-UL) DE OBSTACOLE ---
	// folosim o variabila statica ca sa tina minte timpul de la un frame la altul
	static float TimpPanaLaUrmatorulObstacol = 1.5f;
	TimpPanaLaUrmatorulObstacol -= DeltaTime;

	if (TimpPanaLaUrmatorulObstacol <= 0.0f)
	{
		// Adaugam o noua litera greceasca in afara ecranului, in dreapta (coordonata X = 1500)
		//ObstacoleX.Add(1500.0f);
		// 
		///////////////////////////////////////////////////////////////////////////////
		// Cautam prima litera disponibila (cea care este ascunsa in stanga, X < -200)
		for (int i = 0; i < ObstacoleX.Num(); i++)
		{
			if (ObstacoleX[i] < -200.0f)
			{
				ObstacoleX[i] = 2500.0f; // O mutam in dreapta ca sa vina spre jucator
				
				TipObstacol[i] = FMath::RandRange(0, 23); // <--- ALEGEM O LITERA ALEATOARE NOUA; intervalul fiind intre 0 si 4 putem alege doar 4 litere, dar putem sa modificam acest interval in 0,23 pentru 24 litere
				break; // ne oprim, am generat deja o litera in acest cadru
			}
		}
		/////////////////////////////////////////////////////////////////////////////
		// 
		// Resetam cronometrul cu o valoare aleatoare (intre 1 secunda si 2.5 secunde)
		TimpPanaLaUrmatorulObstacol = FMath::RandRange(1.0f, 2.5f);
	}

	// --- C. MISCAREA OBSTACOLELOR ---
	// Parcurgem lista invers pentru a putea sterge obstacolele in siguranta
	//for (int32 i = ObstacoleX.Num() - 1; i >= 0; --i)
	//{
	//	// Misca obstacolul spre stanga
	//	ObstacoleX[i] -= VitezaObstacole * DeltaTime;

	//	// Optimizare: Daca litera a iesit de pe ecran in stanga (X < -200), o stergem din memorie
	//	if (ObstacoleX[i] < -200.0f)
	//	{
	//		ObstacoleX.RemoveAt(i);
	//	}
	//}
	for (int i = 0; i < ObstacoleX.Num(); i++)
	{
		// miscam litera doar daca este vizibila pe ecran (X > -200)
		if (ObstacoleX[i] > -200.0f)
		{
			// 1. Miscam litera
			ObstacoleX[i] -= VitezaObstacole * DeltaTime;

			// 2. MATEMATICA COLIZIUNII AABB
			// Presupunem ca obstacolul sta mereu pe podea (Y = PodeaY)
			float ObstacolY = PodeaY;

			bool bColiziuneX = (PlayerX < ObstacoleX[i] + ObstacolSize.X) && (PlayerX + PlayerSize.X > ObstacoleX[i]);
			bool bColiziuneY = (PlayerY < ObstacolY + ObstacolSize.Y) && (PlayerY + PlayerSize.Y > ObstacolY);

			// Daca se suprapun atat pe orizontala, cat si pe verticala = IMPACT
			if (bColiziuneX && bColiziuneY)
			{
				bJocActiv = false; // Opreste fizica

				// Verificam daca am batut recordul
				if (ScorCurent > CelMaiBunScor)
				{
					CelMaiBunScor = ScorCurent;
				}


				OnGameOver.Broadcast();      // Cheama evenimentul in Blueprint
			}
		}
	}
}

