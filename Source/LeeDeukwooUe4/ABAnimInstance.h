// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LeeDeukwooUe4.h"
#include "Animation/AnimInstance.h"
#include "ABAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class LEEDEUKWOOUE4_API UABAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UABAnimInstance();
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayAttackMontage();

private:
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Pawn, Meta= (AllowPrivateAccess = true))
	float CurrentPawnSpeed;
	
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsInAir;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontge;
};
