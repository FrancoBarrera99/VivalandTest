// Copyright Epic Games, Inc. All Rights Reserved.

#include "VivalandTestGameMode.h"
#include "VivalandTestPlayerController.h"
#include "VivalandTestPlayerState.h"
#include "GameFramework/HUD.h"
#include "UObject/ConstructorHelpers.h"

AVivalandTestGameMode::AVivalandTestGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AVivalandTestPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}

	// set default HUD to our Blueprinted HUD
	static ConstructorHelpers::FClassFinder<AHUD> HUDBPClass(TEXT("/Game/TopDown/Blueprints/BP_VivalandTestHUD"));
	if (HUDBPClass.Class != NULL)
	{
		HUDClass = HUDBPClass.Class;
	}

	// set default PlayerState to our custom VivalandTestPlayerState
	PlayerStateClass = AVivalandTestPlayerState::StaticClass();

	TeamScores.Emplace(EPlayerTeam::Red, 0);
	TeamScores.Emplace(EPlayerTeam::Blue, 0);
}

void AVivalandTestGameMode::IncreaseTeamScore(EPlayerTeam Team, int32 Score)
{
	if (TeamScores.Contains(Team))
	{
		TeamScores[Team] += Score;
		FString Message = FString::Printf(TEXT("Equipo: %s - Puntaje: %d"), *UEnum::GetValueAsString(Team), TeamScores[Team]);
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, Message);
	}
}
