// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "Templates/SubclassOf.h"

#include "VivalandTestPlayerController.generated.h"

// Forward Declarations
class AVivalandTestAIController;
class AVivalandTestCharacter;
class UNiagaraSystem;
enum class EPlayerTeam : uint8;

/**
 *	The purpose of this class is handle user input actions
 */
UCLASS()
class AVivalandTestPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AVivalandTestPlayerController();
	AVivalandTestAIController* GetAIController();
	EPlayerTeam GetPlayerTeam();
	UFUNCTION(BlueprintCallable)
	void IncreasePlayerScore(int32 Value);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UNiagaraSystem* FXCursor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ScoreboardAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* SetDestinationClickAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ShootAction;

protected:
	// Overrides Section
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void SetupInputComponent() override;

	// Input Handlers Section
	void OnScoreboardReleased();
	void OnScoreboardStarted();
	void OnSetDestinationReleased();
	void OnShootStarted();

	// Functions Section
	UFUNCTION(Server, Reliable)
	void Server_MoveToLocation(FVector NewDestination);
	void Server_MoveToLocation_Implementation(FVector NewDestination);
	UFUNCTION(Server, Reliable)
	void Server_SpawnProjectile(FVector SpawnPosition, FRotator SpawnRotation);
	void Server_SpawnProjectile_Implementation(FVector SpawnPosition, FRotator SpawnRotation);

protected:
	uint32 bMoveToMouseCursor : 1;

private:
	AVivalandTestAIController* AIController;
	UClass* AIControllerClass;
	AVivalandTestCharacter* AICharacter;
	UClass* AICharacterClass;
	FVector CachedDestination;
	FRotator CachedRotation;
	UClass* ProjectileClass;
};


