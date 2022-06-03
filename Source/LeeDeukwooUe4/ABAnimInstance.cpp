// Fill out your copyright notice in the Description page of Project Settings.


#include "ABAnimInstance.h"

UABAnimInstance::UABAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	IsInAir = false;
	IsDead = false;
	
	// Attack 몽타주 애니매이션 기본 값 설정
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT
	("/Game/Book/Animations/SK_Mannequin_Skeleton_Montage.SK_Mannequin_Skeleton_Montage"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontge = ATTACK_MONTAGE.Object;
	}
}

// Anim 전용 tick 함수 NativeUpdateAnimation
void UABAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	// 전역함수 IsValid
	if(!::IsValid(Pawn)) return;

	if (!IsDead)
	{
		// 폰의 움직임과 애니매이션의 움직임을 맞추기
		CurrentPawnSpeed = Pawn->GetVelocity().Size();

		// 폰을 캐스트 하고 IsFalling을 이용하여 점프를 하고있는지 체크
		auto Character = Cast<ACharacter>(Pawn);
		if (Character)
		{
			IsInAir = Character->GetMovementComponent()->IsFalling();
		}
	}
}

void UABAnimInstance::PlayAttackMontage()
{
	ABCHECK(!IsDead);
	Montage_Play(AttackMontge, 1.0f);
}

void UABAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	ABCHECK(!IsDead);
	ABCHECK(Montage_IsPlaying(AttackMontge));
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontge);
}

// 모든 함수를 호출하는 멀티캐스트 델리게이트 broadcast
//  Attack Anim Notify
void UABAnimInstance::AnimNotify_AttackHitCheck()
{
	// 총 4개의 AttackHitCheck가 Broadcast되었다.
	// ABLOG_S(Warning);
	OnAttackHitCheck.Broadcast();
}

void UABAnimInstance::AnimNotify_NextAttackCheck()
{
	// 총 3개의 NextAttackCheck가 Broadcast되었다.
	OnNextAttackCheck.Broadcast();
}

FName UABAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	ABCHECK(FMath::IsWithinInclusive<int32>(Section, 1, 4), NAME_None);
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}
