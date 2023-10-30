// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "VivalandTestPlayerController.generated.h"

/** Forward declaration to improve compiling times */
class UNiagaraSystem;
class AVivalandTestCharacter;
class AVivalandTestAIController;
enum class EPlayerTeam : uint8;

UCLASS()
class AVivalandTestPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AVivalandTestPlayerController();

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UNiagaraSystem* FXCursor;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;
	
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* SetDestinationClickAction;

	/** Shoot Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ShootAction;

	/** Scoreboard Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ScoreboardAction;

	EPlayerTeam GetPlayerTeam();

	UFUNCTION(BlueprintCallable)
	void IncreasePlayerScore(int32 Value);

	AVivalandTestAIController* GetAIController();

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	virtual void SetupInputComponent() override;
	
	// To add mapping context
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

	/** Input handlers for SetDestination action. */
	void OnInputStarted();
	void OnSetDestinationTriggered();
	void OnSetDestinationReleased();

	/** Input handlers for Shoot action. */
	void OnShootStarted();

	/** Input handlers for Scoreboard action. */
	void OnScoreboardStarted();
	void OnScoreboardReleased();

	UFUNCTION(Server, Reliable)
	void Server_SpawnProjectile(FVector SpawnPosition, FRotator SpawnRotation);
	void Server_SpawnProjectile_Implementation(FVector SpawnPosition, FRotator SpawnRotation);

	UFUNCTION(Server, Reliable)
	void Server_MoveToLocation(FVector NewDestination);
	void Server_MoveToLocation_Implementation(FVector NewDestination);

private:
	FVector CachedDestination;
	FRotator CachedRotation;
	UClass* ProjectileClass;
	AVivalandTestCharacter* AICharacter;
	AVivalandTestAIController* AIController;
	UClass* AICharacterClass;
	UClass* AIControllerClass;
};


