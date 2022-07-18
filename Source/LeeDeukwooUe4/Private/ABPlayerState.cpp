// Fill out your copyright notice in the Description page of Project Settings.


#include "ABPlayerState.h"
#include "ABGameInstance.h"
#include "ABSaveGame.h"

AABPlayerState::AABPlayerState()
{
	CharacterLevel = 1;
	GameScore = 0;
	GameHighScore = 15;

	// ó�� ����ġ
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

	// ĳ���� �ʱ� ������
	SetPlayerName(ABSaveGame->PlayerName);
	SetCharacterLevel(ABSaveGame->Level);
	GameScore = 0;
	Exp = ABSaveGame->Exp;
	CharacterIndex = ABSaveGame->CharacterIndex;
	SavePlayerData();
}

float AABPlayerState::GetExpRatio() const
{
	// ����ġ�� 0�� ���Ҷ� 0���Ϸ� ���������� ����.
	if (CurrentStatData->NextExp <= KINDA_SMALL_NUMBER)
		return 0.0f;
	
	// ������ġ�� ���ʿ��� ����ġ�� ����� (ProgressBar�� ����)
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
	
	// �� ����ġ�� ���� ����ġ ���ϱ�
	Exp = Exp+IncomeExp;

	// �� ����ġ�� Max����ġ���� ũ�ٸ� �������ϴ� if��
	if (Exp >= CurrentStatData->NextExp)
	{
		// Max�� �� ����ġ�� ������.
		Exp -= CurrentStatData->NextExp;
		// ������
		SetCharacterLevel(CharacterLevel +1);
		DidLevelUp = true;
	}

	// Delegateȣ��
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



