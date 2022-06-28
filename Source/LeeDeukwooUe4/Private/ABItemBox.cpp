// Fill out your copyright notice in the Description page of Project Settings.


#include "ABItemBox.h"
#include "ABWeapon.h"
#include "ABCharacter.h"

// Sets default values
AABItemBox::AABItemBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// 컴포넌트 이름 설정
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	Box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BOX"));
	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EFFECT"));

	// 최상위 컴포넌트 설정, Box 루트 컴포넌트에 Attach
	RootComponent = Trigger;
	Box->SetupAttachment(RootComponent);
	Effect->SetupAttachment(RootComponent);


	// BoxCollision 크기 셋팅
	Trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Box(TEXT("/Game/InfinityBladeGrassLands/Environments/Breakables/StaticMesh/Box/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1"));
	if (SM_Box.Succeeded())
	{
		Box->SetStaticMesh(SM_Box.Object);
	}
	// BoxComponent 콜리전과 위치 맞추기
	Box->SetRelativeLocation(FVector(0.0f, 0.3f, -30.0f));

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_CHESTOPEN (TEXT("/Game/InfinityBladeGrassLands/Effects/FX_Treasure/Chest/P_TreasureChest_Open_Mesh.P_TreasureChest_Open_Mesh"));
	if (P_CHESTOPEN.Succeeded())
	{
		Effect->SetTemplate(P_CHESTOPEN.Object);
		// 처음부터 바로 시작되는 bool = false
		Effect->bAutoActivate = false;
	}


	//  만들었던 콜리전으로 Set
	Trigger->SetCollisionProfileName(TEXT("ItemBox"));
	Box->SetCollisionProfileName(TEXT("NoCollision"));

	WeaponItemClass = AABWeapon::StaticClass();
}

// Called when the game starts or when spawned
void AABItemBox::BeginPlay()
{
	Super::BeginPlay();
	
}

void AABItemBox::PostInitializeComponents()
{
	// 게임 OnCharacterOverlap를 OnComponentBeginOverlap과 바인딩해주기
	Super::PostInitializeComponents();
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AABItemBox::OnCharacterOverlap);
}

void AABItemBox::OnCharacterOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABLOG_S(Warning);

	auto ABCharacter = Cast<AABCharacter>(OtherActor);
	ABCHECK(nullptr != ABCharacter);

	if (nullptr != ABCharacter && nullptr != WeaponItemClass)
	{
		if (ABCharacter->CanSetWeapon())
		{
			// 새 무기 생성하여 무기 장착
			auto NewWeapon = GetWorld()->SpawnActor<AABWeapon>(WeaponItemClass, FVector::ZeroVector, FRotator::ZeroRotator);
			ABCharacter->SetWeapon(NewWeapon);
			Effect->Activate(true);
			Box->SetHiddenInGame(true, true);
			SetActorEnableCollision(false);
			// 함수 바인딩
			Effect->OnSystemFinished.AddDynamic(this, &AABItemBox::OnEffectFinished);



		}
		else
		{
			ABLOG(Warning, TEXT("%s can't equip weapon currently."), *ABCharacter->GetName());
		}
	}

}


// Called every frame
void AABItemBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AABItemBox::OnEffectFinished(UParticleSystemComponent* PSystem)
{
	Destroy();
}
