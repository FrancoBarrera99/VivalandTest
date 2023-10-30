// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "VivalandTestCharacter.generated.h"

// Forward Declarations
class AVivalandTestPlayerController;

/**
 *	The purpose of this class is to be an AI controlled character.
 */
UCLASS(Blueprintable)
class AVivalandTestCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AVivalandTestCharacter();
	void InitializeCharacter(AVivalandTestPlayerController* NewUserPlayerController);
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	AVivalandTestPlayerController* GetUserPlayerController();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;
	AVivalandTestPlayerController* UserPlayerController;
};

