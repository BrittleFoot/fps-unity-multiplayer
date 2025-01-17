// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSExtractionZone.generated.h"


class UBoxComponent;
class UDecalComponent;
class USoundBase;


UCLASS()
class FPS_API AFPSExtractionZone : public AActor {
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AFPSExtractionZone();

protected:

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UBoxComponent* OverlapComponent;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UDecalComponent* DecalComponent;

    UFUNCTION()
    void HandleOverlap(
            UPrimitiveComponent *OverlappedComp,
            AActor *OtherActor,
            UPrimitiveComponent *OtherComp,
            int32 OtherBodyIndex,
            bool bFromSweep,
            const FHitResult &SweepResult
    );

    UPROPERTY(EditDefaultsOnly, Category = "Sound")
    USoundBase* ObjectiveMissingSound;

};
