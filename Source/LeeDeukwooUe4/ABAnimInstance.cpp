// Fill out your copyright notice in the Description page of Project Settings.


#include "ABAnimInstance.h"

UABAnimInstance::UABAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	IsInAir = false;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/Book/Animations/SK_Mannequin_Skeleton_Montage.SK_Mannequin_Skeleton_Montage"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontge = ATTACK_MONTAGE.Object;
	}


}

void UABAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();

		auto Character = Cast<ACharacter>(Pawn);
		if (Character)
		{
			IsInAir = Character->GetMovementComponent()->IsFalling();
		}
	}
}

void UABAnimInstance::PlayAttackMontage()
{
	//if (!montage_isplaying(attackmontge))
	//{
	//	montage_play(attackmontge, 1.0f);
	//}

	Montage_Play(AttackMontge, 1.0f);
}

void UABAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	ABCHECK(Montage_IsPlaying(AttackMontge));
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontge);
}

// 모든 함수를 호출하는 멀티캐스트 델리게이트 broadcast
//  Attack Anim Notify
void UABAnimInstance::AnimNotify_AttackHitCheck()
{
	// ABLOG_S(Warning);
	OnAttackHitCheck.Broadcast();
}

void UABAnimInstance::AnimNotify_NextAttackCheck()
{
	OnNextAttackCheck.Broadcast();
}

FName UABAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	ABCHECK(FMath::IsWithinInclusive<int32>(Section, 1, 4), NAME_None);
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}
