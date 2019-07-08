// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "FPSGameState.h"
#include "Engine/World.h"


AFPSGameMode::AFPSGameMode()
        : Super()
{
    // set default pawn class to our Blueprinted character
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(
            TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter")
    );

    DefaultPawnClass = PlayerPawnClassFinder.Class;

    // use our custom HUD class
    HUDClass = AFPSHUD::StaticClass();

    GameStateClass = AFPSGameState::StaticClass();
}

void AFPSGameMode::CompleteMission(APawn *InstigatorPawn, bool bIsMissionSuccess)
{
    if (InstigatorPawn)
    {
        if (SpectatingViewpointClass)
        {
            TArray<AActor *> ReturnedActors;
            UGameplayStatics::GetAllActorsOfClass(this, SpectatingViewpointClass, ReturnedActors);

            if (ReturnedActors.Num() > 0)
            {
                for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
                {
                    auto *PC = It->Get();
                    if (PC)
                    {
                        PC->SetViewTargetWithBlend(ReturnedActors[0], 0.5f, EViewTargetBlendFunction::VTBlend_Cubic);
                    }
                }
            }

        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("SpectatingViewpointClass is null. Cannot change spectating view."))
        }
    }

    auto *GameState = GetGameState<AFPSGameState>();
    if (GameState)
    {
        GameState->MulticastOnMissionComplete(InstigatorPawn, bIsMissionSuccess);
    }
    OnMissionCompleted(InstigatorPawn, bIsMissionSuccess);
}
