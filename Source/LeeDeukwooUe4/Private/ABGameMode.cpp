// Fill out your copyright notice in the Description page of Project Settings.

#include "ABGameMode.h"
#include "ABPlayerController.h"
#include "ABCharacter.h"
#include "ABPlayerState.h"
#include "ABGameState.h"

AABGameMode::AABGameMode()
{
	DefaultPawnClass = AABCharacter::StaticClass();
	PlayerControllerClass = AABPlayerController::StaticClass();
	PlayerStateClass = AABPlayerState::StaticClass();
	GameStateClass = AABGameState::StaticClass();
	/*static ConstructorHelpers::FClassFinder<APawn> BP_PAwn_C(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter.ThirdPersonCharacter_C"));
	if (BP_PAwn_C.Succeeded())
	{
		DefaultPawnClass = BP_PAwn_C.Class;
	}*/

	// 테스트 2점을 따면 Clear
	ScoreToClear = 2;
}

void AABGameMode::PostLogin(APlayerController* NewPlayer)
{
	/*ABLOG(Warning, TEXT("PostLogin Begin"));*/
	Super::PostLogin(NewPlayer);
	/*ABLOG(Warning, TEXT("PostLogin end"));*/
	
	auto ABPlayerState = Cast<AABPlayerState>(NewPlayer->PlayerState);
	ABCHECK(nullptr != ABPlayerState);
	ABPlayerState->InitPlayerData();

}

void AABGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ABGameState = Cast<AABGameState>(GameState);
}

void AABGameMode::AddScore(class AABPlayerController* ScoredPlayer)
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		const auto ABPlayerController = Cast<AABPlayerController>(It->Get());
		if ((nullptr != ABPlayerController) && (ScoredPlayer == ABPlayerController))
		{
			ABPlayerController->AddGameScore();
			break;
		}
	}

	ABGameState->AddGameScore();

	if (GetScore() >= ScoreToClear)
	{
		ABGameState->SetGameCleared();

		for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
		{
			// 모든 Pawn들이 동작 멈춤 상태
			(*It)->TurnOff();
		}

		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			const auto ABPlayerController = Cast<AABPlayerController>(It->Get());
			if (nullptr != ABPlayerController)
			{
				ABPlayerController->ShowResultUI();
			}
		}
	}
}

int32 AABGameMode::GetScore() const
{
	return ABGameState->GetToTalGameScore();
}




