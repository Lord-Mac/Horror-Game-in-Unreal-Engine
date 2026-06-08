// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bec_Defect.generated.h"

UCLASS()
class MYPROJECT_API ABec_Defect : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABec_Defect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



public:
	// Componenta vizuală a becului (Lumina)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Setari Bec")
	class UPointLightComponent* LuminaPunct;

	// Timpul la care să pâlpâie (în secunde) - EditAnywhere permite sa il modific in unreal
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setari Bec")
	float TimpFlicker = 0.5f;

	// Intensitatea default a becului
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setari Bec | Aspect")
	float IntensitateImplicita = 5000.0f;

	// Culoarea default becului
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setari Bec | Aspect")
	FLinearColor CuloareImplicita = FLinearColor::White;

	// Decide daca palpaie sau sta aprins
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setari Bec | Aspect")
	bool bPalpaie = true;

protected:
	// timerul ce numara secundele in funcal
	FTimerHandle TimerBec;

	// Fct ce stinge si aprinde becul
	UFUNCTION()
	void ToggleLumina();

};
