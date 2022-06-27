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
//	// �½�ũ �̸� �ٸ� �̸����� ǥ��.
//	NodeName = TEXT("FindPatrolPos");
//}
//
//// Task�� BeginePlay����
//EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8* NodeMemory)
//{
//	// Task�� �۵� ����� ��ȯ���ִ� ���
//	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
//
//	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
//
//	// Pawn�� ������ Failed
//	if (nullptr == ControllingPawn)
//		return EBTNodeResult::Failed;
//	
//	// NavBox ����
//	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
//	if (nullptr == NavSystem)
//		return EBTNodeResult::Failed;
//
//	// ù ���� ��ġ ����.
//	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(AABAIController::HomePosKey);
//	FNavLocation NextPatrol;
//
//	if (NavSystem->GetRandomPointInNavigableRadius(f, 500.0f, NextPatrol))
//	{
//		// ������ ��� Ʈ���� �����忡�ִ� PatorPosKey���
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

	//4.20 �������� UNavigationSystem�� UNavigationSystemV1�� �ٲ����.
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	//���� �Ž� ��ã���� False
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






