// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LeeDeukwooUe4.h"


#include "GameFramework/Pawn.h"
#include "ABPawn.generated.h"

UCLASS()
class LEEDEUKWOOUE4_API AABPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AABPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;

	virtual void PossessedBy(APlayerController* NewController);

private:
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	
public:
	UPROPERTY(VisibleAnywhere, Category = Colliosin)
	class UCapsuleComponent* Capsule;

	// UCLASS()
	UPROPERTY(VisibleAnywhere, Category = Visual)
	class USkeletalMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, Category = Movement)
	class UFloatingPawnMovement* Movement;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* Camera;


};
