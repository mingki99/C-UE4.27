// Fill out your copyright notice in the Description page of Project Settings.


#include "ABPlayerController.h"

AABPlayerController::AABPlayerController()
{

}

void AABPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ABLOG_S(Warning);
}

void AABPlayerController::OnPossess(APawn* aPawn)
{
	ABLOG_S(Warning);
	Super::OnPossess(aPawn);


}