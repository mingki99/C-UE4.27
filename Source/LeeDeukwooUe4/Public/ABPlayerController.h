// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LeeDeukwooUe4.h"
#include "GameFramework/PlayerController.h"
#include "ABPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class LEEDEUKWOOUE4_API AABPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AABPlayerController();

	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* aPawn) override;
	virtual void SetupInputComponent() override;

	class UABHUDWidget*  GetHUDWidget() const;
	void NPCKill(class AABCharacter* KilledNPC) const;
	void AddGameScore() const;

	// 기본을 GameMode로 설정
	void ChangeInputMode(bool bGameMode = true);

	void ShowResultUI();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UABHUDWidget> HUDWIdgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UABGameplayWidget> MenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UABGameplayResultWidget> ResultWidgetclass;

private:
	void OnGamePause();

	UPROPERTY()
	class UABHUDWidget* HUDWidget;

	UPROPERTY()
	class AABPlayerState* ABPlayerState;

	UPROPERTY()
	class UABGameplayWidget* MenuWidget;

	UPROPERTY()
	class UABGameplayResultWidget* ResultWidget;

	// 입력모드
	FInputModeGameOnly GameInputMode;
	FInputModeUIOnly UIInputMode;

};


