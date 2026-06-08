#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "NavigationSystem.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "AITypes.h" 
#include "RRTMoveToAction.generated.h" 



DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRRTMoveOutputPin);//delegatul


// Structura pentru un Nod RRT 
struct FRRTNodeCPP
{
    FVector Location;
    int32 ParentIndex;
    FRRTNodeCPP(FVector InLoc, int32 InParent) : Location(InLoc), ParentIndex(InParent) {}
};

UCLASS()
class MYPROJECT_API URRTMoveToAction : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    // pinii asincroni din Blueprint -- aici se foloseste delegatul definit mai sus
    UPROPERTY(BlueprintAssignable)
    FRRTMoveOutputPin OnSuccess;

    UPROPERTY(BlueprintAssignable)
    FRRTMoveOutputPin OnFail;

    // functia care genereaza nodul in meniul Blueprint
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "AI|RRT")
    static URRTMoveToAction* RRTMoveTo(UObject* WorldContextObject, AAIController* Controller, FVector Destination, float AcceptanceRadius = 50.0f);

    virtual void Activate() override;

private:
    UObject* WorldContext;
    AAIController* AIController;
    FVector GoalLocation;
    float Radius;

    // functiile de matematica si verificare coliziuni
    TArray<FVector> CalculateRRTPath(FVector Start, FVector Goal, UNavigationSystemV1* NavSys);
    bool IsPathClear(FVector Start, FVector End, UNavigationSystemV1* NavSys);

    // functia care asculta cand se opreste caracterul din mers
    void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result);
};