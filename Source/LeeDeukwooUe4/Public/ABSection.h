// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// #include "LeeDeukwooUe4.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ABSection.generated.h"

UCLASS()
class LEEDEUKWOOUE4_API AABSection : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AABSection();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	/*UPROPERTY(VisibleAnywhere, Category = Mesh, Meta = (AllowPrivateAccess = true))
	class UStaticMeshComponent* Mesh;*/

};
