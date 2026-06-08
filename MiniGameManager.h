#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MiniGameManager.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameOverDelegate);

UCLASS()
class MYPROJECT_API AMiniGameManager : public AActor
{
	GENERATED_BODY()

public:
	AMiniGameManager();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// --- SISTEMUL DE FIZICA (JUCATOR) ---

	// Pozitia curenta pe axa y (sus/jos) a caracterului
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MiniGame | Jucator")
	float PlayerY;

	// viteza de cadere/urcare in frame-ul curent
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MiniGame | Jucator")
	float PlayerVelocity;

	// cat de repede este tras in jos caracterul (pozitiv pentru ca y creste in jos pe ecran)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiniGame | Fizica")
	float Gravitatie = 2000.0f;

	// puterea sariturii (negativ pentru a-l impinge in sus spre y=0)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiniGame | Fizica")
	float PutereSaritara = -900.0f;

	// pozitia y de baza - unde incepe si unde se opreste din cazut
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiniGame | Fizica")
	float PodeaY = 0.0f;

	// --- SISTEMUL DE OBSTACOLE ---

	// o lista cu coordonatele x ale tuturor literelor grecesti care vin spre jucator
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MiniGame | Obstacole")
	TArray<float> ObstacoleX;

	
	// o lista paralela care ne spune ce litera este (0 = Alpha, 1 = Epsilon, etc.)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MiniGame | Obstacole")
	TArray<int32> TipObstacol;


	// viteza cu care se misca literele spre stanga
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiniGame | Obstacole")
	float VitezaObstacole = 500.0f;

	// viteza de la care porneste jocul la fiecare restart
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiniGame | Dificultate")
	float VitezaInitiala = 500.0f;

	// cu cat creste viteza in fiecare secunda
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiniGame | Dificultate")
	float Acceleratie = 15.0f;

	// --- STAREA JOCULUI ---

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MiniGame | Stare")
	bool bJocActiv = false;

	// --- FUNCTII CONTROLATE DIN INTERFATA (WIDGET) ---

	UFUNCTION(BlueprintCallable, Category = "MiniGame | Comenzi")
	void StartJoc();

	UFUNCTION(BlueprintCallable, Category = "MiniGame | Comenzi")
	void Saritura();



	// --- SISTEMUL DE COLIZIUNE (HITBOX) ---

	// pozitia fixa a jucatorului pe axa x (pe ecran el nu merge inainte/inapoi)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiniGame | Hitbox")
	float PlayerX = 100.0f;

	// marimea dreptunghiului jucatorului (latime x, inaltime y)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiniGame | Hitbox")
	FVector2D PlayerSize = FVector2D(50.0f, 50.0f);

	// marimea dreptunghiului pentru literele grecesti
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiniGame | Hitbox")
	FVector2D ObstacolSize = FVector2D(40.0f, 40.0f);

	// o declaram in C++, dar o scriem in Blueprint
	/*UFUNCTION(BlueprintImplementableEvent, Category = "MiniGame | Evenimente")
	void OnGameOver();*/


	// --- SISTEMUL DE COLIZIUNE ---
	UPROPERTY(BlueprintAssignable, Category = "MiniGame | Evenimente")
	FOnGameOverDelegate OnGameOver;



	// --- SISTEMUL DE SCOR ---

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MiniGame | Scor")
	float ScorCurent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MiniGame | Scor")
	float CelMaiBunScor = 0.0f; // acesta ramane in memorie intre runde
};