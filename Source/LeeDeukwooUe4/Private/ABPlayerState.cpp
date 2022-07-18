// Fill out your copyright notice in the Description page of Project Settings.


#include "ABPlayerState.h"
#include "ABGameInstance.h"
#include "ABSaveGame.h"

AABPlayerState::AABPlayerState()
{
	CharacterLevel = 1;
	GameScore = 0;
	GameHighScore = 15;

	// 처음 경험치
	Exp = 0;
	SaveSlotName = TEXT("Player1");
	CharacterIndex = 0;
}

int32 AABPlayerState::GetCharacterIndex() const
{
	return CharacterIndex;
}

int32 AABPlayerState::GetGameSocre() const
{
	return GameScore;
}

int32 AABPlayerState::GetGameHighScore() const
{
	return GameHighScore;
}

int32 AABPlayerState::GetCharacterLevel() const
{
	return CharacterLevel;
}

void AABPlayerState::InitPlayerData()
{
	auto ABSaveGame = Cast<UABSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	if (nullptr == ABSaveGame)
	{
		ABSaveGame = GetMutableDefault<UABSaveGame>();
	}

	// 캐릭터 초기 데이터
	SetPlayerName(ABSaveGame->PlayerName);
	SetCharacterLevel(ABSaveGame->Level);
	GameScore = 0;
	Exp = ABSaveGame->Exp;
	CharacterIndex = ABSaveGame->CharacterIndex;
	SavePlayerData();
}

float AABPlayerState::GetExpRatio() const
{
	// 경험치를 0과 비교할때 0이하로 못내려가는 구문.
	if (CurrentStatData->NextExp <= KINDA_SMALL_NUMBER)
		return 0.0f;
	
	// 현경험치와 총필요한 경험치의 백분율 (ProgressBar를 위함)
	float Result = (float)Exp / (float) CurrentStatData->NextExp;
	ABLOG(Warning, TEXT("Ratio : %f, Current : %d, Next : %d"), Result, Exp, CurrentStatData->NextExp);
	return Result;
}

void AABPlayerState::SavePlayerData()
{
	UABSaveGame* NewPlayerData = NewObject<UABSaveGame>();
	NewPlayerData->PlayerName = GetPlayerName();
	NewPlayerData->Level = CharacterLevel;
	NewPlayerData->Exp = Exp;
	NewPlayerData->HighScore = GameHighScore;
	NewPlayerData->CharacterIndex = CharacterIndex;

	if (!UGameplayStatics::SaveGameToSlot(NewPlayerData, SaveSlotName, 0))
	{
		ABLOG(Error, TEXT("SaveGameError!"));
	}
}

bool AABPlayerState::AddExp(int32 IncomeExp)
{
	if(CurrentStatData->NextExp == -1)
		return false;

	bool DidLevelUp = false;
	
	// 현 경험치와 들어온 경험치 더하기
	Exp = Exp+IncomeExp;

	// 현 경험치가 Max경험치보다 크다면 레벨업하는 if문
	if (Exp >= CurrentStatData->NextExp)
	{
		// Max를 현 경험치를 빼버림.
		Exp -= CurrentStatData->NextExp;
		// 레벨업
		SetCharacterLevel(CharacterLevel +1);
		DidLevelUp = true;
	}

	// Delegate호출
	OnPlayerStateChanged.Broadcast();
	SavePlayerData();
	return DidLevelUp;

}

void AABPlayerState::SetCharacterLevel(int32 NewCharacterLevel)
{
	auto ABGameInstance = Cast<UABGameInstance>(GetGameInstance());
	ABCHECK(nullptr != ABGameInstance);

	CurrentStatData = ABGameInstance->GetABCharacterData(NewCharacterLevel);
	ABCHECK(nullptr != CurrentStatData);

	CharacterLevel = NewCharacterLevel;
}

void AABPlayerState::AddGameScore()
{
	GameScore++;
	if (GameScore >= GameHighScore)
	{
		GameHighScore = GameScore;
	}
	OnPlayerStateChanged.Broadcast();
	SavePlayerData();
}



