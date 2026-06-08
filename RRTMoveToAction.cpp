#pragma once
#include "RRTMoveToAction.h"
#include "NavigationPath.h"
#include "DrawDebugHelpers.h" //  pentru a putea desena liniile verzi ale grafului de cautare in spatiul 3D

URRTMoveToAction* URRTMoveToAction::RRTMoveTo(UObject* WorldContextObject, AAIController* Controller, FVector Destination, float AcceptanceRadius)
{
    URRTMoveToAction* Action = NewObject<URRTMoveToAction>();
    Action->WorldContext = WorldContextObject;
    Action->AIController = Controller;
    Action->GoalLocation = Destination;
    Action->Radius = AcceptanceRadius;
    Action->RegisterWithGameInstance(WorldContextObject);
    return Action;
}

void URRTMoveToAction::Activate()
{
    if (!AIController || !AIController->GetPawn())
    {
        OnFail.Broadcast();
        SetReadyToDestroy();
        return;
    }

    FVector StartLocation = AIController->GetPawn()->GetActorLocation();
    UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(WorldContext);

    if (!NavSys)
    {
        OnFail.Broadcast();
        SetReadyToDestroy();
        return;
    }

    // 1. calculare cale RRT
    TArray<FVector> PathPoints = CalculateRRTPath(StartLocation, GoalLocation, NavSys);

    if (PathPoints.Num() > 0)
    {
        // afisare pe ecran ca a gasit un drum 
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, TEXT("RRT: Drum gasit cu succes!"));

        // 2. extragem componenta care se ocupa de miscarea fizica a AI ului
        UPathFollowingComponent* PathComp = AIController->GetPathFollowingComponent();

        if (PathComp)
        {
            PathComp->OnRequestFinished.AddUObject(this, &URRTMoveToAction::OnMoveCompleted);
        }

        // 3. Trimitem cererea de miscare 
        // 
        //Exista mai multe optiuni, fie cea directa data de Unreal Engine :
        // 
        //AIController->MoveToLocation(GoalLocation, Radius, true, true, false, true, nullptr, true);// aceasta linie face miscarea folosind Ai ul din Unreal engine /////////////////////////////////////////////////////////////////////
        // versiunea data de unreal engine ^^
        


        //fie optiunea de a urma arborele generat de RRT.

        ///versiunea urmarii rutei RRT incepe aici:========================================================================================================================================================================================
       // 3. Trimitem cererea de miscare fortata pe punctele RRT

        // A. Cream un traseu nou, complet gol
        FNavPathSharedPtr CustomPath = MakeShareable(new FNavigationPath());

        // B. Luam fiecare punct calculat de RRT-ul nostru si il bagam în acest traseu
        for (const FVector& Point : PathPoints)
        {
            CustomPath->GetPathPoints().Add(FNavPathPoint(Point));
        }

       
        // ii spunem motorului Unreal ca s-au adaugat toate punbctele -- adica ca traseul este pregatit
        CustomPath->MarkReady();

        // C. Cream cererea de miscare catre destinatie
        FAIMoveRequest MoveReq(GoalLocation);
        MoveReq.SetAcceptanceRadius(Radius);

        // D.  ii spunem motorului Unreal sa nu  mai folosească Pathfinding-ul lui ---!!! IMPORTANT
        MoveReq.SetUsePathfinding(false);

        // E. ii dam lui PathComp cererea si traseul nostru exact
        PathComp->RequestMove(MoveReq, CustomPath);
        /// si versiunea RRT se termina aici==============================================================================================================================================================================================================
    }
    else
    {
        // Printează pe ecran cu roșu dacă a eșuat
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("RRT: FAIL! Nu s-a putut gasi un drum."));

        OnFail.Broadcast();
        SetReadyToDestroy();
    }
}

void URRTMoveToAction::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
    if (AIController && AIController->GetPathFollowingComponent())
    {
        AIController->GetPathFollowingComponent()->OnRequestFinished.RemoveAll(this);
    }

    if (Result.IsSuccess())
    {
        OnSuccess.Broadcast();
    }
    else
    {
        OnFail.Broadcast();
    }

    SetReadyToDestroy();
}

// =========================================================================
// LOGICA MATEMATICA RRT 
// =========================================================================
TArray<FVector> URRTMoveToAction::CalculateRRTPath(FVector Start, FVector Goal, UNavigationSystemV1* NavSys)
{
    TArray<FVector> Path;
    TArray<FRRTNodeCPP> NodeList;
    NodeList.Add(FRRTNodeCPP(Start, -1));

    int32 MaxIter = 10000;//importante in eficienta si in gasirea rutei
    float StepSize = 100.0f;// important pt eficienta si gasirea rutei, este necesar un echilibru intre cele doua 

    for (int32 i = 0; i < MaxIter; i++)
    {
        FVector RndPoint;
        if (FMath::RandRange(0, 100) > 10)
        {
            RndPoint = Start + FVector(FMath::RandRange(-2000.f, 2000.f), FMath::RandRange(-2000.f, 2000.f), 0.f);
        }
        else
        {
            RndPoint = Goal;
        }

        int32 NearestInd = 0;
        float MinDistSq = FVector::DistSquared(NodeList[0].Location, RndPoint);
        for (int32 j = 1; j < NodeList.Num(); j++)
        {
            float DistSq = FVector::DistSquared(NodeList[j].Location, RndPoint);
            if (DistSq < MinDistSq)
            {
                MinDistSq = DistSq;
                NearestInd = j;
            }
        }

        FVector NearestLoc = NodeList[NearestInd].Location;
        FVector Direction = (RndPoint - NearestLoc).GetSafeNormal();
        FVector NewLoc = NearestLoc + Direction * StepSize;
        NewLoc.Z = Start.Z;

        // Check Collision 
        if (IsPathClear(NearestLoc, NewLoc, NavSys))
        {
            NodeList.Add(FRRTNodeCPP(NewLoc, NearestInd));

            // REPREZENTAREA VIZUALA : Desenam ramurile copacului RRT in timp real cu linii verzi in spatiul 3D folosind linia de mai jos  :
            //
            //DrawDebugLine(WorldContext->GetWorld(), NearestLoc, NewLoc, FColor::Green, false, 3.0f, 0, 3.0f);

            if (FVector::Dist(NewLoc, Goal) <= Radius)
            {
                int32 CurrentInd = NodeList.Num() - 1;
                while (CurrentInd != -1)
                {
                    Path.Insert(NodeList[CurrentInd].Location, 0);

                    // Desenam linia finala cu rosu (adica calea valida a traseului)
                    if (NodeList[CurrentInd].ParentIndex != -1)
                    {
                        //DrawDebugLine(WorldContext->GetWorld(), NodeList[CurrentInd].Location, NodeList[NodeList[CurrentInd].ParentIndex].Location, FColor::Red, false, 4.0f, 0, 8.0f);
                    }

                    CurrentInd = NodeList[CurrentInd].ParentIndex;
                }
                Path.Add(Goal);
                return Path;
            }
        }
    }

    return Path;
}

bool URRTMoveToAction::IsPathClear(FVector Start, FVector End, UNavigationSystemV1* NavSys)
{
    FVector HitLocation;

    //  Ridicăm punctele de control cu 50cm mai sus (pe axa Z)  pentru a nu trage razele direct prin podea.
    
    FVector AdjustedStart = Start + FVector(0.f, 0.f, 50.f);
    FVector AdjustedEnd = End + FVector(0.f, 0.f, 50.f);

    bool bHitObstacle = NavSys->NavigationRaycast(WorldContext, AdjustedStart, AdjustedEnd, HitLocation);

    return !bHitObstacle;
}