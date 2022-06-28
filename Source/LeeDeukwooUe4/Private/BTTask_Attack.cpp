// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "ABAIController.h"
#include "ABCharacter.h"

UBTTask_Attack::UBTTask_Attack()
{
	// tick��� Ȱ��ȭ
	bNotifyTick = true;
	IsAttacking = false;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	
	auto ABCharacter = Cast<AABCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ABCharacter)
		return EBTNodeResult::Failed;

	// NPC ���� ����
	ABCharacter->Attack();
	IsAttacking = true;

	// OnAttack ���� �� IsAttacking = false
	ABCharacter->OnAttackEnd.AddLambda([this]() -> void {IsAttacking = false; });

	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	// ���� ���ҽ� �׽�ũ ����
	if (!IsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	
}
