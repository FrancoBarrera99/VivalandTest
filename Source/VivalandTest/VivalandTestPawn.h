// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "VivalandTestPawn.generated.h"

// Forward Declarations
class AVivalandTestAIController;
class AVivalandTestCharacter;
class UCameraComponent;
class USpringArmComponent;

/**
 *	The purpose of this class is to follow an AI controlled character smoothly.
 */
UCLASS()
class VIVALANDTEST_API AVivalandTestPawn : public APawn
{
	GENERATED_BODY()

public:
	AVivalandTestPawn();
	void InitializePawn(AActor* NewFollowedActor);
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return CameraComponent; }

protected:
	virtual void BeginPlay() override;
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
	void HandleFollowActor();

protected:
	UPROPERTY(Replicated)
	AActor* FollowedActor;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;
	FTimerHandle FollowTimerHandle;
};