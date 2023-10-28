// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "VivalandTestGameMode.generated.h"

enum class EPlayerTeam : uint8;

UCLASS(minimalapi)
class AVivalandTestGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AVivalandTestGameMode();
	void IncreaseTeamScore(EPlayerTeam Team, int32 Score);

protected:

	TMap<EPlayerTeam, int32> TeamScores;
};



