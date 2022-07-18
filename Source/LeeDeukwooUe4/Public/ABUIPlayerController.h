// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LeeDeukwooUe4.h"
#include "GameFramework/PlayerController.h"
#include "ABUIPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class LEEDEUKWOOUE4_API AABUIPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	

	virtual void BeginPlay() override;

	// 위젯 디테일 페털에서 보여주기
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UUserWidget> UIWidgetClass;

	UPROPERTY()
	class UUserWidget* UIWidgetInstance;
};
