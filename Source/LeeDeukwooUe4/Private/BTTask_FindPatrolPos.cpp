//// Fill out your copyright notice in the Description page of Project Settings.
//
//
//#include "BTTask_FindPatrolPos.h"
//#include "ABAIController.h"
//#include "BehaviorTree/BlackboardComponent.h"
//#include "NavigationSystem.h"
//
//UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
//{
//	// 태스크 이름 다른 이름으로 표시.
//	NodeName = TEXT("FindPatrolPos");
//}
//
//// Task의 BeginePlay역할
//EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8* NodeMemory)
//{
//	// Task의 작동 결과를 반환해주는 결과
//	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
//
//	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
//
//	// Pawn이 없으면 Failed
//	if (nullptr == ControllingPawn)
//		return EBTNodeResult::Failed;
//	
//	// NavBox 생성
//	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
//	if (nullptr == NavSystem)
//		return EBTNodeResult::Failed;
//
//	// 첫 시작 위치 저장.
//	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(AABAIController::HomePosKey);
//	FNavLocation NextPatrol;
//
//	if (NavSystem->GetRandomPointInNavigableRadius(f, 500.0f, NextPatrol))
//	{
//		// 비헤이 비어 트리가 블랙보드에있는 PatorPosKey사용
//		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AABAIController::PatorlPosKey, NextPatrol.Location);
//		return EBTNodeResult::Succeeded;
//	}
//
//	return EBTNodeResult::Failed;
//}

#include "BTTask_FindPatrolPos.h"
#include "ABAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
	NodeName = TEXT("FindPatrolPos");
}

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	//4.20 버전부터 UNavigationSystem은 UNavigationSystemV1로 바뀌었다.
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	//내비 매쉬 못찾으면 False
	if (nullptr == NavSystem)
		return EBTNodeResult::Failed;

	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(AABAIController::HomePosKey);
	FNavLocation NextPatrol;

	if (NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 500.0f, NextPatrol))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AABAIController::PatrolPosKey, NextPatrol.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}






