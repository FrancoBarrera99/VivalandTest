// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "VivalandTestCharacter.h"
#include "VivalandTestGameMode.generated.h"

UCLASS(minimalapi)
class AVivalandTestGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AVivalandTestGameMode();
	void IncreaseTeamScore(TEnumAsByte<EPlayerTeam> Team, int32 Score);

protected:

	TMap<TEnumAsByte<EPlayerTeam>, int32> TeamScores;
};



