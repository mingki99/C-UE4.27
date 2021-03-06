// Fill out your copyright notice in the Description page of Project Settings.


#include "ABCharacterComponent.h"
#include "ABGameInstance.h"

// Sets default values for this component's properties
UABCharacterComponent::UABCharacterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	// Post보다 더 먼져 실행되는 InitializeComponent를 실행 하기위해 필요한 옵셤
	bWantsInitializeComponent = true;

	Level = 1;
	// ...
}

// post보다 먼져 실행되는 함수.
void UABCharacterComponent::InitializeComponent()
{
	Super::InitializeComponent();
	SetNewLevel(Level);
}

void UABCharacterComponent::SetNewLevel(int32 NewLevel)
{
	auto ABGameInstance = Cast<UABGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	ABCHECK(nullptr != ABGameInstance);
	CurrentStatData = ABGameInstance->GetABCharacterData(NewLevel);
	if (nullptr != CurrentStatData)
	{
		// 레벨 바뀔시 스탯 변경
		Level = NewLevel;
		SetHP(CurrentStatData->MaxHP);
	}
	else
	{
		ABLOG(Error, TEXT("Level, (%d) data doesn't exist"), NewLevel);
	}
}

// Called when the game starts
void UABCharacterComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UABCharacterComponent::SetDamage(float NewDamege)
{
	ABCHECK(nullptr != CurrentStatData);
	// HP 범위 0 ~ MaxHP 로 설정
	SetHP(FMath::Clamp<float>(CurrentHP - NewDamege, 0.0f, CurrentStatData->MaxHP));
}

void UABCharacterComponent::SetHP(float NewHP)
{
	CurrentHP = NewHP;
	OnHPChanged.Broadcast();
	if (CurrentHP < KINDA_SMALL_NUMBER)
	{
		CurrentHP = 0.0f;
		OnHPIsZero.Broadcast();
	}
}

float UABCharacterComponent::GetAttack() const
{
	ABCHECK(nullptr != CurrentStatData, 0.0f);
	return CurrentStatData->Attack;
}

float UABCharacterComponent::GetHPRatio() const
{
	ABCHECK(nullptr != CurrentStatData, 0.0f);

	return (CurrentStatData->MaxHP < KINDA_SMALL_NUMBER) ? 0.0f : (CurrentHP / CurrentStatData->MaxHP);
}


int32 UABCharacterComponent::GetDropExp() const
{
	return CurrentStatData->DropExp;
}