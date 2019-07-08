// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSAiGuard.generated.h"

class UPawnSensingComponent;
class ATargetPoint;


UENUM(BlueprintType)
enum class EAIState : uint8
{
    Idle,

    Suspicious,

    Alerted
};

UENUM(BlueprintType)
enum class EAIPatrollingBehaviour : uint8
{
    Loop,

    BackAndForward,

    Once
};

UCLASS()
class FPS_API AFPSAiGuard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSAiGuard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComponent;

	UPROPERTY(EditInstanceOnly, Category = "AI")
	TArray<ATargetPoint*> PatrolPath;

	UPROPERTY(EditInstanceOnly, Category = "AI")
    EAIPatrollingBehaviour PatrollingBehaviour;

	UPROPERTY(BlueprintReadOnly, Category = "AI")
	uint8 CurrentPatrolWaypointIndex;

	bool bForwardLoopTrace;

	bool bPathDone;

	UFUNCTION()
	void OnPawnSeen(APawn* SeenPawn);

	UFUNCTION()
	void OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume);

	UFUNCTION()
	void ResetOrientation();

	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void OnStateChanged(EAIState NewState);

	FRotator OriginalRotation;

    FTimerHandle TH_ResetOrientation;

    UPROPERTY(ReplicatedUsing=OnRep_GuardState)
    EAIState GuardState;

    UFUNCTION()
    void OnRep_GuardState();

    void SetGuardState(EAIState NewState);

    ATargetPoint* GetCurrentWaypoint();

    void SkipWaypoint();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
