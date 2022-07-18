// Fill out your copyright notice in the Description page of Project Settings.


#include "ABHUDWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "ABCharacterComponent.h"
#include "ABPlayerState.h"

void UABHUDWidget::BindCharacterStat(UABCharacterComponent* CharacterStat)
{
	ABCHECK(nullptr != CharacterStat);
	CurrentCharacterStat = CharacterStat;
	// 델리게이트에 UpdatePlayerState() 함수 연결
	CharacterStat->OnHPChanged.AddUObject(this, &UABHUDWidget::UpdateCharacterStat);
}

void UABHUDWidget::BindPlayerState(AABPlayerState* PlayerState)
{
	ABCHECK(nullptr != PlayerState);
	CurrentPayerState = PlayerState;
	// 델리게이트에 UpdateCharacterStat() 함수 연결
	PlayerState->OnPlayerStateChanged.AddUObject(this, &UABHUDWidget::UpdatePlayerState);
}

void UABHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// WidgetComponent 이름으로 연결
	HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("pbHP")));
	ABCHECK(nullptr != HPBar);

	ExpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("pbExp")));
	ABCHECK(nullptr != ExpBar);


	PlayerName = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtPlayerName")));
	ABCHECK(nullptr != PlayerName);

	PlayerLevel = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtLevel")));
	ABCHECK(nullptr != PlayerLevel);

	CurrentScore = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtCurrentScore")));
	ABCHECK(nullptr != CurrentScore);

	HighScore = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtHighScore")));
	ABCHECK(nullptr != HighScore);

}

void UABHUDWidget::UpdateCharacterStat()
{
	ABCHECK(CurrentCharacterStat.IsValid());

	
	HPBar->SetPercent(CurrentCharacterStat->GetHPRatio());

}

void UABHUDWidget::UpdatePlayerState()
{
	ABCHECK(CurrentPayerState.IsValid());

	// 경험치 연결
	ExpBar->SetPercent(CurrentPayerState->GetExpRatio());

	// 데이터 TextBox에 연결
	PlayerName->SetText(FText::FromString(CurrentPayerState->GetPlayerName()));
	PlayerLevel->SetText(FText::FromString(FString::FromInt(CurrentPayerState->GetCharacterLevel())));
	CurrentScore->SetText(FText::FromString(FString::FromInt(CurrentPayerState->GetGameSocre())));
	HighScore->SetText(FText::FromString(FString::FromInt(CurrentPayerState->GetGameHighScore())));


}














