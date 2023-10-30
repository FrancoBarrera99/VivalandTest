// Copyright Epic Games, Inc. All Rights Reserved.

#include "VivalandTestGameMode.h"

#include "GameFramework/HUD.h"
#include "UObject/ConstructorHelpers.h"
#include "VivalandTestCharacter.h"
#include "VivalandTestPawn.h"
#include "VivalandTestPlayerController.h"
#include "VivalandTestPlayerState.h"

AVivalandTestGameMode::AVivalandTestGameMode()
{
	PlayerControllerClass = AVivalandTestPlayerController::StaticClass();
	DefaultPawnClass = AVivalandTestPawn::StaticClass();
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
	static ConstructorHelpers::FClassFinder<AHUD> HUDBPClass(TEXT("/Game/TopDown/Blueprints/BP_VivalandTestHUD"));
	if (HUDBPClass.Class != NULL)
	{
		HUDClass = HUDBPClass.Class;
	}
	PlayerStateClass = AVivalandTestPlayerState::StaticClass();
}

void AVivalandTestGameMode::RestartPlayer(AController* NewPlayer)
{
	Super::RestartPlayer(NewPlayer);
	if (NewPlayer == nullptr)
		return;

	AActor* PlayerStart = FindPlayerStart(NewPlayer);
	AActor* PlayerPawn = NewPlayer->GetPawn();
	if (PlayerPawn == nullptr || PlayerStart == nullptr)
		return;

	FVector NewPosition = PlayerStart->GetActorLocation();
	PlayerPawn->SetActorLocation(NewPosition);
}
