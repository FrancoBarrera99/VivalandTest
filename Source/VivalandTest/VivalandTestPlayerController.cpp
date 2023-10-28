// Copyright Epic Games, Inc. All Rights Reserved.

#include "VivalandTestPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "VivalandTestCharacter.h"
#include "VivalandTestProjectile.h"
#include "VivalandTestPlayerState.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

AVivalandTestPlayerController::AVivalandTestPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;

	static ConstructorHelpers::FClassFinder<AActor> ProjectileBPClass(TEXT("/Game/TopDown/Blueprints/BP_VivalandTestProjectile"));
	if (ProjectileBPClass.Class != nullptr)
	{
		ProjectileClass = ProjectileBPClass.Class;
	}
}

void AVivalandTestPlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

EPlayerTeam AVivalandTestPlayerController::GetPlayerTeam()
{
	AVivalandTestPlayerState* PS = Cast<AVivalandTestPlayerState>(PlayerState);
	if (PS != nullptr)
	{
		return PS->GetPlayerTeam();
	}
	return EPlayerTeam::None;
}

void AVivalandTestPlayerController::IncreasePlayerScore(int32 Value)
{
	AVivalandTestPlayerState* PS = Cast<AVivalandTestPlayerState>(PlayerState);
	if (PS != nullptr)
	{
		PS->IncreasePlayerScore(Value);
	}
}

void AVivalandTestPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		// Setup mouse input events
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &AVivalandTestPlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &AVivalandTestPlayerController::OnSetDestinationTriggered);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &AVivalandTestPlayerController::OnSetDestinationReleased);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &AVivalandTestPlayerController::OnSetDestinationReleased);

		// Setup touch input events
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Started, this, &AVivalandTestPlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Triggered, this, &AVivalandTestPlayerController::OnTouchTriggered);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Completed, this, &AVivalandTestPlayerController::OnTouchReleased);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Canceled, this, &AVivalandTestPlayerController::OnTouchReleased);

		// Setup keybord input events
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &AVivalandTestPlayerController::OnShootStarted);
	}
}

void AVivalandTestPlayerController::OnInputStarted()
{
	StopMovement();
}

// Triggered every frame when the input is held down
void AVivalandTestPlayerController::OnSetDestinationTriggered()
{
	// We flag that the input is being pressed
	FollowTime += GetWorld()->GetDeltaSeconds();
	
	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bool bHitSuccessful = false;
	if (bIsTouch)
	{
		bHitSuccessful = GetHitResultUnderFinger(ETouchIndex::Touch1, ECollisionChannel::ECC_Visibility, true, Hit);
	}
	else
	{
		bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	}

	// If we hit a surface, cache the location
	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
	}
	
	// Move towards mouse pointer or touch
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn != nullptr)
	{
		FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
	}
}

void AVivalandTestPlayerController::OnSetDestinationReleased()
{
	// If it was a short press
	if (FollowTime <= ShortPressThreshold)
	{
		// We move there and spawn some particles
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}

	FollowTime = 0.f;
}

// Triggered every frame when the input is held down
void AVivalandTestPlayerController::OnTouchTriggered()
{
	bIsTouch = true;
	OnSetDestinationTriggered();
}

void AVivalandTestPlayerController::OnTouchReleased()
{
	bIsTouch = false;
	OnSetDestinationReleased();
}

void AVivalandTestPlayerController::OnShootStarted()
{
	FVector SpawnPosition = GetPawn()->GetActorLocation();
	FRotator SpawnRotation = GetPawn()->GetActorRotation();

	Server_SpawnProjectile(SpawnPosition, SpawnRotation);
}

void AVivalandTestPlayerController::Server_SpawnProjectile_Implementation(FVector SpawnPosition, FRotator SpawnRotation)
{
	if (ProjectileClass != nullptr)
	{
		FTransform SpawnTransform(SpawnRotation, SpawnPosition, FVector::OneVector);
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = GetPawn();
		AActor* SpawnedProjectile = GetWorld()->SpawnActor(ProjectileClass, &SpawnTransform, SpawnParameters);
		GetPawn()->MoveIgnoreActorAdd(SpawnedProjectile);
	}
}
