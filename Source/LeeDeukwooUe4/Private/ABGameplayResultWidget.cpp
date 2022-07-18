// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGameplayResultWidget.h"
#include "Components/TextBlock.h"
#include "ABGameState.h"

void UABGameplayResultWidget::BindGameState(AABGameState* GameState)
{
	ABCHECK(nullptr != GameState);
	CurrentGamestate = GameState;
}

void UABGameplayResultWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ABCHECK(CurrentGamestate.IsValid());

	auto Result = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtResult")));
	ABCHECK(nullptr != Result);
	// 게임 목표 달성 or 실패
	Result->SetText(FText::FromString(CurrentGamestate->IsGameCleared() ? TEXT("MissionComplete") : TEXT("MissionFailed")));


	auto ToTalScore = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtTotalScore")));
	ABCHECK(nullptr != ToTalScore);
	ToTalScore->SetText(FText::FromString(FString::FromInt(CurrentGamestate->GetToTalGameScore())));


}


