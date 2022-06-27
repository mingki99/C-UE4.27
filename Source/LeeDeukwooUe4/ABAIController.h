// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LeeDeukwooUe4.h"
#include "AIController.h"
#include "ABAIController.generated.h"

/**
 * 
 */
UCLASS()
class LEEDEUKWOOUE4_API AABAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AABAIController();

	virtual void OnPossess(APawn* InPawn) override;
	// virtual void OnUnPossess() override;


	// 절대 이름이 변하지 않는다는 가정하여 static 선언.
	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;

private:
	//void OnRepeatTimer();

	//FTimerHandle RepeatTimerHanle;
	//float RepeatInterval;

	UPROPERTY()
	class UBehaviorTree* BTAsset;

	UPROPERTY()
	class UBlackboardData* BBAsset;

};
