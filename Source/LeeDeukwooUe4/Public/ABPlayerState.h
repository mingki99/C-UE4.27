// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LeeDeukwooUe4.h"
#include "GameFramework/PlayerState.h"
#include "ABPlayerState.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnPlayerStateChangedDelegate)
/**
 * 
 */
UCLASS()
class LEEDEUKWOOUE4_API AABPlayerState : public APlayerState
{
	GENERATED_BODY()
	

public:
	AABPlayerState();

	int32 GetGameSocre() const;
	int32 GetGameHighScore() const;
	FString SaveSlotName;

	int32 GetCharacterLevel() const;
	int32 GetCharacterIndex() const;

	float GetExpRatio() const;
	bool AddExp(int32 IncomeExp);
	void AddGameScore();

	void InitPlayerData();
	void SavePlayerData();

	FOnPlayerStateChangedDelegate OnPlayerStateChanged;

protected:
	UPROPERTY(Transient)
	int32 GameScore;

	UPROPERTY(Transient)
	int32 GameHighScore;

	UPROPERTY(Transient)
	int32 CharacterLevel;

	// 플레이어 경험치.
	UPROPERTY(Transient)
	int32 Exp;

	UPROPERTY()
	int32 CharacterIndex;

private:
	void SetCharacterLevel(int32 NewCharacterLevel);
	struct FABCharacterData* CurrentStatData;






};
