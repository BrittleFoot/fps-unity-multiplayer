// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGameState.h"
#include "Engine/World.h"
#include "FPSPlayerController.h"

void AFPSGameState::MulticastOnMissionComplete_Implementation(APawn *InstigatorPawn, bool bMissionSuccess)
{
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
    {
        auto *PlayerController = Cast<AFPSPlayerController>(It->Get());
        if (PlayerController && PlayerController->IsLocalController())
        {
            PlayerController->OnMissionCompleted(InstigatorPawn, bMissionSuccess);
            auto *MyPawn = PlayerController->GetPawn();
            if (MyPawn)
            {
                MyPawn->DisableInput(nullptr);
            }
        }
    }
}
