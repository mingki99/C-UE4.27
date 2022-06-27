// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Detect.h"
#include "ABAIController.h"
#include "ABCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	// TickNode ȣ���ϴ� �ֱ� 1�ʷ� ����
	Interval = 1.0f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) return;

	// 
	UWorld* World = ControllingPawn->GetWorld();
	//  NPC�� ��ġ ����
	FVector Center = ControllingPawn->GetActorLocation();
	// NPC�������� ���� ������ 600.0
	float DetectRadius = 600.0f;

	if (nullptr == World) return;
	
	// �ݰ� ���� ������ ��� ĳ���� ���� ��Ƶδ� �迭
	TArray<FOverlapResult> OverlapResults;

	FCollisionQueryParams CollisionQueryParam(NAME_None,false, ControllingPawn );

	// �ݰ泻�� �����ϴ� Overlap�Լ�
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel12,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);

	// overlap�� �ȵǸ� false
	if (bResult)
	{
		// ������ �� ��ü�� �迭�� ������ŭ �ݺ� (for each loop)
		for (auto const& OverlapResult : OverlapResults)
		{
			// ĳ���� ���ϱ����� ����
			AABCharacter* ABCharacter = Cast<AABCharacter>(OverlapResult.GetActor());
			// ĳ���Ͱ� ���� �÷����ϰ��ִ� ��Ʈ�ѷ��� �´��� ��
			if (ABCharacter && ABCharacter->GetController()->IsPlayerController())
			{
				// Target������ �� ĳ������ ���� ����
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AABAIController::TargetKey, ABCharacter);
				// �ʷϻ� ��ü ����
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);

				// ĳ���Ϳ��� �Ķ��� ����Ʈ�� �����.
				DrawDebugPoint(World, ABCharacter->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f);
				// ���� AI�� �÷��̾��� ĳ���Ϳ��� ������ �׾�����.
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), ABCharacter->GetActorLocation(), FColor::Blue, false, 0.2f);
				return;
			}
		}
	}



	// ������ �׷��ִ� ���
	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);

}