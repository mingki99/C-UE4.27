// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ABCharacterSetting.generated.h"

/**
 * 
 */
 // 언리얼 오브젝트 기본값을 INI 파일에서 불러들이기
UCLASS(config = ArenaBattle)
class ARENABATTLESETTING_API UABCharacterSetting : public UObject
{
	GENERATED_BODY()
	
public:
	UABCharacterSetting();

	UPROPERTY(config)
	// 애셋 경로 정보 보관하기위한 클래스
	TArray<FSoftObjectPath> CharacterAssets;

};
