// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LeeDeukwooUe4.h"
#include "Blueprint/UserWidget.h"
#include "ABHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class LEEDEUKWOOUE4_API UABHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindCharacterStat(class UABCharacterComponent* CharacterStat);
	void BindPlayerState(class AABPlayerState* PlayerState);

protected:
	virtual void NativeConstruct() override;
	void UpdateCharacterStat();
	void UpdatePlayerState();

private:
	TWeakObjectPtr<class UABCharacterComponent> CurrentCharacterStat;
	TWeakObjectPtr<class AABPlayerState> CurrentPayerState;

	UPROPERTY()
	class UProgressBar* HPBar;

	UPROPERTY()
		class UProgressBar* ExpBar;


	UPROPERTY()
		class UTextBlock* PlayerName;

	UPROPERTY()
		class UTextBlock* PlayerLevel;

	UPROPERTY()
		class UTextBlock* CurrentScore;

	UPROPERTY()
		class UTextBlock* HighScore;
};
