// Fill out your copyright notice in the Description page of Project Settings.


#include "ABCharacterComponent.h"
#include "ABGameInstance.h"

// Sets default values for this component's properties
UABCharacterComponent::UABCharacterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	// Post���� �� ���� ����Ǵ� InitializeComponent�� ���� �ϱ����� �ʿ��� �ɼ�
	bWantsInitializeComponent = true;

	Level = 1;
	// ...
}

// post���� ���� ����Ǵ� �Լ�.
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
		// ���� �ٲ�� ���� ����
		Level = NewLevel;
		CurrentHP = CurrentStatData->MaxHP;
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
	// 0 ~ MaxHP �� ���� ����
	CurrentHP = FMath::Clamp<float>(CurrentHP - NewDamege, 0.0f, CurrentStatData->MaxHP);
	if (CurrentHP <= 0.0f)
	{
		OnHPIsZero.Broadcast();
	}
}

float UABCharacterComponent::GetAttack()
{
	ABCHECK(nullptr != CurrentStatData, 0.0f);
	return CurrentStatData->Attack;
}