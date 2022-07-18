// Fill out your copyright notice in the Description page of Project Settings.


#include "ABAIController.h"
//#include "NavigationSystem.h"
//#include "Blueprint/AIBlueprintHelperLibrary.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"


const FName AABAIController::HomePosKey(TEXT("HomePos"));
const FName AABAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName AABAIController::TargetKey(TEXT("Target"));

AABAIController::AABAIController()
{
	//// 3�ʸ��� �ߵ�.
	//RepeatInterval = 3.0f;
	
	// ������ ������ ��� Ʈ�� ����
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject (TEXT("/Game/Book/AI/BB_ABCharacter.BB_ABCharacter"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject (TEXT("/Game/Book/AI/BT_ABCharacter.BT_ABCharacter"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
;

}

void AABAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);	
}


void AABAIController::RunAI()
{
	//// 3�ʸ��� �Լ��� �߻� ��Ű�� Set �����ִ� Timer�� ����Ѵ�.
	//GetWorld()->GetTimerManager().SetTimer(RepeatTimerHanle, this, &AABAIController::OnRepeatTimer, RepeatInterval, true);

	// ������ �ߵ� ���� Ȯ���ϴ� if��
	if (UseBlackboard(BBAsset, Blackboard))
	{
		// ������ ��ġ�� HomePosKey�� Set���ش�.
		Blackboard->SetValueAsVector(HomePosKey, GetPawn()->GetActorLocation());
		if (!RunBehaviorTree(BTAsset))
		{
			ABLOG(Error, TEXT("AIController couldn't run behavior tree!"));
		}
	}

}

void AABAIController::StopAI()
{
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (nullptr != BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
	}
}

//void AABAIController::OnUnPossess()
//{
//	Super::OnUnPossess();
//	// Ÿ�̸� �����ֱ�.
//	GetWorld()->GetTimerManager().ClearTimer(RepeatTimerHanle);
//}

//void AABAIController::OnRepeatTimer()
//{
//	auto CurrentPawn = GetPawn();
//	ABCHECK(nullptr != CurrentPawn);
//
//	// World�� �ִ� �׺���̼� ����.
//	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
//	if (nullptr == NavSystem) return;
//
//	// ���ϴ� ������ ��ġ
//	FNavLocation NextLocation;
//	// �׺���̼��� ���� ��ġ ����.
//	if (NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 500.0f, NextLocation))
//	{
//		// ������ �������� ���� �����̴� �Լ�
//		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, NextLocation.Location);
//
//		ABLOG(Warning, TEXT("Next Location : %s"), *NextLocation.Location.ToString());
//	}
//}






