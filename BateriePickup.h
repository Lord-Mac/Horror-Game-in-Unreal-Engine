// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ChildActorComponent.h"
#include "BateriePickup.generated.h"


UCLASS()
class MYPROJECT_API ABateriePickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABateriePickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:
	// partea vizuala -- modelul 3D al bateriei
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componente")
	class UStaticMeshComponent* MeshBaterie;

	// sfera de coliziune --  detecteaza jucatorul
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componente")
	class USphereComponent* SferaColiziune;

	// cata energie da bateria 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setari Baterie")
	float EnergieData = 25.0f;

protected:
	// functia de overelap
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


public:
	// Componenta care va detine "Becul Defect"
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componente")
	class UChildActorComponent* BecComponent;

	// timpul de respawn --- 300 secunde = 5 minute
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setari Respawn")
	float TimpRespawn = 300.0f;

protected:
	FTimerHandle TimerRespawn;

	// functi de respawn -- face sa reapara din nou bateria
	void Respawn();
};
