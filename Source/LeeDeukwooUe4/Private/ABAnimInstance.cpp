// Fill out your copyright notice in the Description page of Project Settings.


#include "ABAnimInstance.h"

UABAnimInstance::UABAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	IsInAir = false;
	IsDead = false;
	
	// Attack ��Ÿ�� �ִϸ��̼� �⺻ �� ����
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT
	("/Game/Book/Animations/SK_Mannequin_Skeleton_Montage.SK_Mannequin_Skeleton_Montage"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontge = ATTACK_MONTAGE.Object;
	}
}

// Anim ���� tick �Լ� NativeUpdateAnimation
void UABAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	// �����Լ� IsValid
	if(!::IsValid(Pawn)) return;

	if (!IsDead)
	{
		// ���� �����Ӱ� �ִϸ��̼��� �������� ���߱�
		CurrentPawnSpeed = Pawn->GetVelocity().Size();

		// ���� ĳ��Ʈ �ϰ� IsFalling�� �̿��Ͽ� ������ �ϰ��ִ��� üũ
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

// ��� �Լ��� ȣ���ϴ� ��Ƽĳ��Ʈ ��������Ʈ broadcast
//  Attack Anim Notify
void UABAnimInstance::AnimNotify_AttackHitCheck()
{
	// �� 4���� AttackHitCheck�� Broadcast�Ǿ���.
	// ABLOG_S(Warning);
	OnAttackHitCheck.Broadcast();
}

void UABAnimInstance::AnimNotify_NextAttackCheck()
{
	// �� 3���� NextAttackCheck�� Broadcast�Ǿ���.
	OnNextAttackCheck.Broadcast();
}

FName UABAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	ABCHECK(FMath::IsWithinInclusive<int32>(Section, 1, 4), NAME_None);
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}
