// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "VivalandTestHUD.generated.h"

/**
 * 
 */
UCLASS()
class VIVALANDTEST_API AVivalandTestHUD : public AHUD
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintImplementableEvent)
	void ShowScoreboardGUI();

	UFUNCTION(BlueprintImplementableEvent)
	void ShowTeamSelectGUI();

	UFUNCTION(BlueprintImplementableEvent)
	void HideScoreboardGUI();

	UFUNCTION(BlueprintImplementableEvent)
	void HideTeamSelectGUI();
};
