// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LeeDeukwooUe4.h"
#include "GameFramework/Actor.h"
#include "ABWeapon.generated.h"

UCLASS()
class LEEDEUKWOOUE4_API AABWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AABWeapon();

	float GetAttackRange() const;
	float GetAttackDamage() const;
	float GetAttackModifier() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnyWhere, Category = Weapon)
	class USkeletalMeshComponent* Weapon;

protected:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Attack)
	float AttackRange;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Attack)
	float AttackDamageMin;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Attack)
	float AttackDamageMax;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Attack)
	float AttackModifierMin;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Attack)
	float AttackModifierMax;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Attack)
	float AttackDamage;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Attack)
	float AttackModifier;
};
