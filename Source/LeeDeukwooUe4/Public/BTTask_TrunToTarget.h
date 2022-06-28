// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LeeDeukwooUe4.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_TrunToTarget.generated.h"

/**
 * 
 */
UCLASS()
class LEEDEUKWOOUE4_API UBTTask_TrunToTarget : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_TrunToTarget();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
