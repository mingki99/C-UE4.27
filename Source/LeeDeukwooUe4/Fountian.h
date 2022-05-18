// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LeeDeukwooUe4.h"
#include "GameFramework/Actor.h"


#include "Fountian.generated.h"

UCLASS()
class LEEDEUKWOOUE4_API AFountian : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFountian();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReson) override;
	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Body;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Water;

	UPROPERTY(VisibleAnywhere)
	class UPointLightComponent* Light;

	UPROPERTY(VisibleAnywhere)
	class UParticleSystemComponent* Splash;

	UPROPERTY(EditAnywhere, Category = ID)
	int32 ID;

	UPROPERTY(VisibleAnyWhere)
	class URotatingMovementComponent* RMovement;


private:
	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowprivateAccss = ture))
	float RotateSpeed;

};
