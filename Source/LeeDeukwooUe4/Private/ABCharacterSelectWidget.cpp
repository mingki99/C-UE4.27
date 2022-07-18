// Fill out your copyright notice in the Description page of Project Settings.


#include "ABCharacterSelectWidget.h"
#include "ABCharacterSetting.h"
#include "ABGameInstance.h"
#include "EngineUtils.h"
#include "Animation/SkeletalMeshActor.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "ABSaveGame.h"
#include "ABPlayerState.h"

void UABCharacterSelectWidget::NextCharacter(bool bForward)
{
	bForward ? CurrentIndex++ : CurrentIndex --;

	if(CurrentIndex == -1) CurrentIndex = MaxIndex - 1;
	if(CurrentIndex == MaxIndex) CurrentIndex = 0;

	// 캐릭터 정보 목록 가져오기.
	auto CharacterSetting = GetDefault<UABCharacterSetting>();
	// 캐릭터 정보 목록 0번째부터 지정
	auto AssetRef = CharacterSetting->CharacterAssets[CurrentIndex];

	auto ABGameInstance = GetWorld()->GetGameInstance<UABGameInstance>();
	ABCHECK(nullptr != ABGameInstance);
	ABCHECK(TargetComponent.IsValid());

	// 새로운 스켈레탈 메쉬 실시간으로 보여주기.
	USkeletalMesh* Asset = ABGameInstance->StreamableManager.LoadSynchronous<USkeletalMesh>(AssetRef);
	if (nullptr != Asset)
	{
		// 넣어주기.
		TargetComponent->SetSkeletalMesh(Asset);
	}
}

void UABCharacterSelectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CurrentIndex = 0;
	auto CharacterSetting = GetDefault<UABCharacterSetting>();
	MaxIndex = CharacterSetting->CharacterAssets.Num();

	for (TActorIterator<ASkeletalMeshActor> It(GetWorld());  It; ++It)
	{
		TargetComponent = It->GetSkeletalMeshComponent();
		break;
	}

	PrevButton = Cast<UButton>(GetWidgetFromName(TEXT("btnPrev")));
	ABCHECK(nullptr != PrevButton);

	NextButton = Cast<UButton>(GetWidgetFromName(TEXT("btnNext")));
	ABCHECK(nullptr != NextButton);

	TextBox = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("edtPlayerName")));
	ABCHECK(nullptr != TextBox);

	ConfirmButton = Cast<UButton>(GetWidgetFromName(TEXT("btnConfirm")));
	ABCHECK(nullptr != ConfirmButton);

	
	PrevButton->OnClicked.AddDynamic(this, &UABCharacterSelectWidget::OnPrevClicked);

	NextButton->OnClicked.AddDynamic(this, &UABCharacterSelectWidget::OnNextClicked);

	ConfirmButton->OnClicked.AddDynamic(this, &UABCharacterSelectWidget::OnConfirmClicked);

}


void UABCharacterSelectWidget::OnPrevClicked()
{
	NextCharacter(false);
}

void UABCharacterSelectWidget::OnNextClicked()
{
	NextCharacter(true);
}

void UABCharacterSelectWidget::OnConfirmClicked()
{
	FString CharacterName = TextBox->GetText().ToString();
	// 캐릭터의 이름이 0글자거나 10글자일경우 return
	if (CharacterName.Len() <= 0 || CharacterName.Len() > 10 ) return;

	UABSaveGame* NewPlayerData = NewObject<UABSaveGame>();
	// 캐릭터 정보 넘겨주기
	NewPlayerData->PlayerName = CharacterName;
	NewPlayerData->Level = 1;
	NewPlayerData->Exp = 0;
	NewPlayerData->HighScore = 0;
	NewPlayerData->CharacterIndex = CurrentIndex;

	auto ABPlayerState = GetDefault<AABPlayerState>();
	if (UGameplayStatics::SaveGameToSlot(NewPlayerData, ABPlayerState->SaveSlotName, 0))
	{
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("GamePlay"));
	}
	else
	{
		ABLOG(Error, TEXT("SaveGame Error!"));
	}
}


