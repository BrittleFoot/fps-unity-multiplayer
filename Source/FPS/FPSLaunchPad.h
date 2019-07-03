// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSLaunchPad.generated.h"


class UStaticMeshComponent;
class UBoxComponent;
class UDecalComponent;


UCLASS()
class FPS_API AFPSLaunchPad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSLaunchPad();

protected:

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UStaticMeshComponent* MeshComponent;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UBoxComponent* OverlapComponent;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UDecalComponent* DecalComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Effects")
    UParticleSystem* LaunchFX;

    UPROPERTY(EditInstanceOnly, Category = "Launch")
    float LaunchPitchAngle;

    UPROPERTY(EditInstanceOnly, Category = "Launch")
    float LaunchPower;

    UFUNCTION()
    void HandleOverlap(
            UPrimitiveComponent *OverlappedComp,
            AActor *OtherActor,
            UPrimitiveComponent *OtherComp,
            int32 OtherBodyIndex,
            bool bFromSweep,
            const FHitResult &SweepResult
    );

};
