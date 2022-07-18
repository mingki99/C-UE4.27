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
	//// 3초마다 발동.
	//RepeatInterval = 3.0f;
	
	// 블랙보드 비헤이 비어 트리 셋팅
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
	//// 3초마다 함수를 발생 시키고 Set 시켜주는 Timer를 사용한다.
	//GetWorld()->GetTimerManager().SetTimer(RepeatTimerHanle, this, &AABAIController::OnRepeatTimer, RepeatInterval, true);

	// 셋팅이 잘되 었나 확인하는 if문
	if (UseBlackboard(BBAsset, Blackboard))
	{
		// 현재의 위치를 HomePosKey에 Set해준다.
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
//	// 타이머 없애주기.
//	GetWorld()->GetTimerManager().ClearTimer(RepeatTimerHanle);
//}

//void AABAIController::OnRepeatTimer()
//{
//	auto CurrentPawn = GetPawn();
//	ABCHECK(nullptr != CurrentPawn);
//
//	// World에 있는 네비게이션 지정.
//	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
//	if (nullptr == NavSystem) return;
//
//	// 향하는 목적지 위치
//	FNavLocation NextLocation;
//	// 네비게이션의 랜덤 위치 지정.
//	if (NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 500.0f, NextLocation))
//	{
//		// 간단히 목적지를 향해 움직이는 함수
//		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, NextLocation.Location);
//
//		ABLOG(Warning, TEXT("Next Location : %s"), *NextLocation.Location.ToString());
//	}
//}






