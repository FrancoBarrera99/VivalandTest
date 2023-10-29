// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "VivalandTestGameMode.generated.h"

enum class EPlayerTeam : uint8;
class AVivalandTestCharacter;

UCLASS(minimalapi)
class AVivalandTestGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AVivalandTestGameMode();
	void RestartPlayer(AController* NewPlayer) override;
};



