// Copyright Epic Games, Inc. All Rights Reserved.

#include "VivalandTestPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "VivalandTestCharacter.h"
#include "VivalandTestPawn.h"
#include "VivalandTestAIController.h"
#include "VivalandTestProjectile.h"
#include "VivalandTestPlayerState.h"
#include "VivalandTestHUD.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

AVivalandTestPlayerController::AVivalandTestPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;

	//Find AIControlled Classes
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		AICharacterClass = PlayerPawnBPClass.Class;
	}
	AIControllerClass = AVivalandTestAIController::StaticClass();

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

void AVivalandTestPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (AICharacter != nullptr || AIController != nullptr)
		return;

	//Spawn AIControlled character
	AVivalandTestPawn* VivalandPawn = Cast<AVivalandTestPawn>(InPawn);
	if (VivalandPawn != nullptr && AICharacterClass != nullptr && AIControllerClass != nullptr)
	{
		FTransform SpawnTransform = VivalandPawn->GetActorTransform();
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = this;
		AICharacter = Cast<AVivalandTestCharacter>(GetWorld()->SpawnActor(AICharacterClass, &SpawnTransform, SpawnParameters));
		AIController = Cast<AVivalandTestAIController>(GetWorld()->SpawnActor(AIControllerClass, &SpawnTransform, SpawnParameters));
		if (AIController != nullptr && AICharacter != nullptr)
		{
			AIController->Possess(AICharacter);
			VivalandPawn->InitializePawn(AICharacter);
		}
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

AVivalandTestAIController* AVivalandTestPlayerController::GetAIController()
{
	return AIController;
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

		// Setup keybord input events
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &AVivalandTestPlayerController::OnShootStarted);
		EnhancedInputComponent->BindAction(ScoreboardAction, ETriggerEvent::Started, this, &AVivalandTestPlayerController::OnScoreboardStarted);
		EnhancedInputComponent->BindAction(ScoreboardAction, ETriggerEvent::Completed, this, &AVivalandTestPlayerController::OnScoreboardReleased);
		EnhancedInputComponent->BindAction(ScoreboardAction, ETriggerEvent::Canceled, this, &AVivalandTestPlayerController::OnScoreboardReleased);

	}
}

void AVivalandTestPlayerController::OnInputStarted()
{
	//StopMovement();
}

// Triggered every frame when the input is held down
void AVivalandTestPlayerController::OnSetDestinationTriggered()
{
	
}

void AVivalandTestPlayerController::OnSetDestinationReleased()
{
	FHitResult Hit;
	bool bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);

	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;

		if (GetPawn() != nullptr)
		{
			FVector Direction = (CachedDestination - GetPawn()->GetActorLocation()).GetSafeNormal();
			float DesiredYaw = FMath::Atan2(Direction.Y, Direction.X) * 180.0f / PI;
			CachedRotation = FRotator(0.0f, DesiredYaw, 0.0f);
		}
	}

	Server_MoveToLocation(CachedDestination);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
}

void AVivalandTestPlayerController::OnShootStarted()
{
	FVector SpawnPosition = GetPawn()->GetActorLocation();
	Server_SpawnProjectile(SpawnPosition, CachedRotation);
}

void AVivalandTestPlayerController::OnScoreboardStarted()
{
	AVivalandTestHUD* HUD = Cast<AVivalandTestHUD>(this->GetHUD());
	if (HUD != nullptr)
	{
		HUD->ShowScoreboardGUI();
	}
}

void AVivalandTestPlayerController::OnScoreboardReleased()
{
	AVivalandTestHUD* HUD = Cast<AVivalandTestHUD>(this->GetHUD());
	if (HUD != nullptr)
	{
		HUD->HideScoreboardGUI();
	}
}

void AVivalandTestPlayerController::Server_SpawnProjectile_Implementation(FVector SpawnPosition, FRotator SpawnRotation)
{
	if (ProjectileClass != nullptr)
	{
		const float SpawnOffset = 75.0f; 

		FVector OffsetDirection = SpawnRotation.Vector();
		OffsetDirection.Normalize();
		FVector Offset = OffsetDirection * SpawnOffset;
		FVector OffsetSpawnPosition = SpawnPosition + Offset;
		FTransform SpawnTransform(SpawnRotation, OffsetSpawnPosition, FVector::OneVector);

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = this;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AVivalandTestProjectile* SpawnedProjectile = Cast<AVivalandTestProjectile>(GetWorld()->SpawnActor(ProjectileClass, &SpawnTransform, SpawnParameters));
		if (SpawnedProjectile != nullptr)
		{
			SpawnedProjectile->InitializeProjectile({ GetPawn(), AICharacter });
			GetPawn()->MoveIgnoreActorAdd(SpawnedProjectile);
			AICharacter->MoveIgnoreActorAdd(SpawnedProjectile);
		}
	}
}

void AVivalandTestPlayerController::Server_MoveToLocation_Implementation(FVector NewDestination)
{
	if (AIController != nullptr)
	{
		AIController->MoveToLocation(NewDestination);
	}
}
