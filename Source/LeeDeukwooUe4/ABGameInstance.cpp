// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGameInstance.h"

UABGameInstance::UABGameInstance()
{
	FString CharacterDataPath = TEXT("/Game/Book/GameData/ABCharacterData.ABCharacterData");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ABCHARATER(*CharacterDataPath);
	ABCHECK(DT_ABCHARATER.Succeeded());
	ABCharacterTable = DT_ABCHARATER.Object;
	// table의 열이 1개라도 있으면  true
	ABCHECK(ABCharacterTable->GetRowMap().Num() > 0);
}

void UABGameInstance::Init()
{
	Super::Init();
	ABLOG(Warning, TEXT("DropExp of Level 20 ABCharacter : %d"), GetABCharacterData(20)->DropExp);
}

FABCharacterData* UABGameInstance::GetABCharacterData(int32 Level)
{
	return ABCharacterTable->FindRow<FABCharacterData>(*FString::FromInt(Level), TEXT(""));
}