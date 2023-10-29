// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "VivalandTestPlayerController.generated.h"

/** Forward declaration to improve compiling times */
class UNiagaraSystem;
enum class EPlayerTeam : uint8;

UCLASS()
class AVivalandTestPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AVivalandTestPlayerController();

	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UNiagaraSystem* FXCursor;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;
	
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* SetDestinationClickAction;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* SetDestinationTouchAction;

	/** Shoot Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ShootAction;

	/** Scoreboard Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ScoreboardAction;

	EPlayerTeam GetPlayerTeam();

	UFUNCTION(BlueprintCallable)
	void IncreasePlayerScore(int32 Value);

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	virtual void SetupInputComponent() override;
	
	// To add mapping context
	virtual void BeginPlay();

	/** Input handlers for SetDestination action. */
	void OnInputStarted();
	void OnSetDestinationTriggered();
	void OnSetDestinationReleased();
	void OnTouchTriggered();
	void OnTouchReleased();

	/** Input handlers for Shoot action. */
	void OnShootStarted();

	/** Input handlers for Scoreboard action. */
	void OnScoreboardStarted();
	void OnScoreboardReleased();

	UFUNCTION(Server, Reliable)
	void Server_SpawnProjectile(FVector SpawnPosition, FRotator SpawnRotation);
	void Server_SpawnProjectile_Implementation(FVector SpawnPosition, FRotator SpawnRotation);

private:
	FVector CachedDestination;

	UClass* ProjectileClass;

	bool bIsTouch; // Is it a touch device
	float FollowTime; // For how long it has been pressed
};


