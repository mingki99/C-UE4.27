// Fill out your copyright notice in the Description page of Project Settings.


#include "ABPlayerController.h"
#include "ABHUDWidget.h"
#include "ABPlayerState.h"
#include "ABCharacter.h"
#include "ABGameplayWidget.h"
#include "ABGameplayResultWidget.h"
#include "ABGameState.h"

AABPlayerController::AABPlayerController()
{
	static ConstructorHelpers::FClassFinder<UABHUDWidget> UI_HUD_C (TEXT("/Game/Book/UI/UI_HUD.UI_HUD_C"));
	if (UI_HUD_C.Succeeded())
	{
		HUDWIdgetClass = UI_HUD_C.Class;
	}

	static ConstructorHelpers::FClassFinder<UABGameplayWidget> UI_Menu_C(TEXT("/Game/Book/UI/UI_Menu.UI_Menu_C"));
	if (UI_Menu_C.Succeeded())
	{
		MenuWidgetClass = UI_Menu_C.Class;
	}

	static ConstructorHelpers::FClassFinder<UABGameplayResultWidget> UI_Result_C(TEXT("/Game/Book/UI/UI_Result.UI_Result_C"));
	if (UI_Result_C.Succeeded())
	{
		ResultWidgetclass = UI_Result_C.Class;  
	}

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

void AABPlayerController::BeginPlay()
{
	Super::BeginPlay();
	ChangeInputMode(true);
	
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	HUDWidget = CreateWidget<UABHUDWidget>(this, HUDWIdgetClass);
	ABCHECK(nullptr != HUDWidget);
	HUDWidget->AddToViewport(1);

	ResultWidget = CreateWidget<UABGameplayResultWidget>(this, ResultWidgetclass);
	ABCHECK(nullptr != ResultWidget);

	// HUD에 플레이어의 Stat를 넘격준다.
	ABPlayerState = Cast<AABPlayerState>(PlayerState);
	ABCHECK(nullptr != ABPlayerState);
	HUDWidget->BindPlayerState(ABPlayerState);
	ABPlayerState->OnPlayerStateChanged.Broadcast();



}

void AABPlayerController::ShowResultUI()
{
	auto ABGameState = Cast<AABGameState>(UGameplayStatics::GetGameState(this));
	ABCHECK(nullptr != ABGameState);
	ResultWidget->BindGameState(ABGameState);

	ResultWidget->AddToViewport();
	ChangeInputMode(false);
}

UABHUDWidget* AABPlayerController::GetHUDWidget() const
{
	return HUDWidget;
}

void AABPlayerController::NPCKill(AABCharacter* KilledNPC) const
{
	// 죽음 확인되면 경험치 획득.
	ABPlayerState->AddExp(KilledNPC->GetExp() + 40);
}

void AABPlayerController::AddGameScore() const
{
	ABPlayerState->AddGameScore();
}

void AABPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction(TEXT("GamePause"), EInputEvent::IE_Pressed, this, &AABPlayerController::OnGamePause);

}

void AABPlayerController::OnGamePause()
{
	MenuWidget = CreateWidget<UABGameplayWidget>(this, MenuWidgetClass);
	ABCHECK(nullptr != MenuWidget);
	MenuWidget->AddToViewport(3);

	SetPause(true);
	ChangeInputMode(false);
}

void AABPlayerController::ChangeInputMode(bool bGameMode)
{
	if (bGameMode)
	{
		SetInputMode(GameInputMode);
		bShowMouseCursor = false;
	}
	else
	{
		SetInputMode(UIInputMode);
		bShowMouseCursor = true;
	}
}





