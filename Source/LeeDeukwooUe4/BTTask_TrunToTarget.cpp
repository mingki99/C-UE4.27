// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_TrunToTarget.h"
#include "ABCharacter.h"
#include "ABAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TrunToTarget::UBTTask_TrunToTarget()
{
	NodeName = TEXT("Trun");
}

EBTNodeResult::Type UBTTask_TrunToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ABCharacter = Cast<AABCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ABCharacter)
		return EBTNodeResult::Failed;
	
	auto Target = Cast<AABCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AABAIController::TargetKey));
	if (nullptr == Target)
		return EBTNodeResult::Failed;
	
	// 타겟과 나의 거리 구하기.
	FVector LookVector = Target->GetActorLocation() - ABCharacter->GetActorLocation();

	// z값은 0
	LookVector.Z = 0.0f;

	// 바라보는 시점인 X축 가져오기.
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();

	// RInterpTo를 
	ABCharacter->SetActorRotation(FMath::RInterpTo(ABCharacter->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 2.0f));
	
	return EBTNodeResult::Succeeded;
}
