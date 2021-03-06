// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LeeDeukwooUe4.h"
#include "GameFramework/Actor.h"
#include "ABItemBox.generated.h"

UCLASS()
class LEEDEUKWOOUE4_API AABItemBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AABItemBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnyWhere, Category = Box)
	class UBoxComponent* Trigger;

	UPROPERTY(VisibleAnyWhere, Category = Box)
	class UStaticMeshComponent* Box; 

	UPROPERTY(VisibleAnyWhere, Category = Effect)
	class UParticleSystemComponent* Effect;


	UPROPERTY(EditInstanceOnly, Category = Box)
	TSubclassOf<class AABWeapon> WeaponItemClass;

private:
	UFUNCTION()
	void OnCharacterOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEffectFinished(class UParticleSystemComponent* PSystem);

};
