// Fill out your copyright notice in the Description page of Project Settings.


#include "ABPlayerController.h"
#include "ABCharacter.h"
#include "ABGameMode.h"
#include "ABPawn.h"

AABGameMode::AABGameMode()
{
	DefaultPawnClass = AABCharacter::StaticClass();
	PlayerControllerClass = AABPlayerController::StaticClass();
	/*static ConstructorHelpers::FClassFinder<APawn> BP_PAwn_C(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter.ThirdPersonCharacter_C"));
	if (BP_PAwn_C.Succeeded())
	{
		DefaultPawnClass = BP_PAwn_C.Class;
	}*/
}

void AABGameMode::PostLogin(APlayerController* NewPlayer)
{
	ABLOG(Warning, TEXT("PostLogin Begin"));
	Super::PostLogin(NewPlayer);
	ABLOG(Warning, TEXT("PostLogin end"));
	
}