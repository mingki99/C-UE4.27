// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LeeDeukwooUe4.h"
#include "GameFramework/GameStateBase.h"
#include "ABGameState.generated.h"

/**
 * 
 */
UCLASS()
class LEEDEUKWOOUE4_API AABGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	AABGameState();

public:
	int32 GetToTalGameScore() const;
	void AddGameScore();
	void SetGameCleared();
	bool IsGameCleared() const;

private:
	UPROPERTY()
	int32 TotalGameScore;

	UPROPERTY()
	bool bGameCleared;

};
