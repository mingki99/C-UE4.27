// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Detect.h"
#include "ABAIController.h"
#include "ABCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	// TickNode 호출하는 주기 1초로 설정
	Interval = 1.0f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) return;

	// 
	UWorld* World = ControllingPawn->GetWorld();
	//  NPC현 위치 정보
	FVector Center = ControllingPawn->GetActorLocation();
	// NPC기준으로 원형 반지름 600.0
	float DetectRadius = 600.0f;

	if (nullptr == World) return;
	
	// 반겨 내에 감지된 모든 캐릭터 정보 담아두는 배열
	TArray<FOverlapResult> OverlapResults;

	FCollisionQueryParams CollisionQueryParam(NAME_None,false, ControllingPawn );

	// 반경내에 감지하는 Overlap함수
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel12,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);

	// overlap이 안되면 false
	if (bResult)
	{
		// 오버랩 된 물체의 배열의 갯수만큼 반복 (for each loop)
		for (auto const& OverlapResult : OverlapResults)
		{
			// 캐릭터 비교하기위해 생성
			AABCharacter* ABCharacter = Cast<AABCharacter>(OverlapResult.GetActor());
			// 캐릭터가 현재 플레이하고있는 컨트롤러가 맞는지 비교
			if (ABCharacter && ABCharacter->GetController()->IsPlayerController())
			{
				// Target변수를 이 캐릭터의 정보 저장
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AABAIController::TargetKey, ABCharacter);
				// 초록색 구체 생성
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);

				// 캐릭터에게 파랑색 포인트가 생긴다.
				DrawDebugPoint(World, ABCharacter->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f);
				// 나의 AI와 플레이어의 캐릭터에게 라인이 그어진다.
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), ABCharacter->GetActorLocation(), FColor::Blue, false, 0.2f);
				return;
			}
		}
	}



	// 구형을 그려주는 기능
	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);

}