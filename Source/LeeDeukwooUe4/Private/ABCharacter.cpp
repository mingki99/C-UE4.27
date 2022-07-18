// Fill out your copyright notice in the Description page of Project Settings.


#include "ABCharacter.h"
#include "ABAnimInstance.h"
#include "ABCharacterComponent.h"
#include "DrawDebugHelpers.h"

// ABWeapon 액터를 사용하기에 해더추가
#include "ABWeapon.h"
#include "Components/WidgetComponent.h"
#include "ABCharacterWidget.h"
#include "ABAIController.h"

#include"ABCharacterSetting.h"
#include "ABGameInstance.h"
#include "ABPlayerController.h"
#include "ABPlayerState.h"
#include "ABHUDWidget.h"
#include "ABGameMode.h"

#include "VisualLogger/VisualLogger.h"


// Sets default values
AABCharacter::AABCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 컴포넌트 설정
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	CharacterStat = CreateDefaultSubobject<UABCharacterComponent>(TEXT("CHARACTERSTAT"));
	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));


	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);
	HPBarWidget->SetupAttachment(GetMesh());

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));

	// 캐릭터 skeletalmesh 설정
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CHARM_RAM(TEXT("/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Ram.SK_CharM_Ram"));
	if (SK_CHARM_RAM.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_CHARM_RAM.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance> WARRIOR_ANIM(TEXT("/Game/Book/Animations/ANBP_Warrior.ANBP_Warrior_C"));
	if (WARRIOR_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(WARRIOR_ANIM.Class);
	}

	HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	// 위젯 붙어질 위치 Screen으로 지정
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Game/Book/UI/UI_HPBar.UI_HPBar_C"));
	if (UI_HUD.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		// 위젯 크기 설정
		HPBarWidget->SetDrawSize(FVector2D(150.0f, 50.0f));
	}


	// view change될 때 springarm의 부드러운 효과를 위한 셋팅
	ArmLengthSpeed = 3.0f;
	ArmRotatoinSpeed = 10.0f;
	
	// 점프 높이 설정
	GetCharacterMovement()->JumpZVelocity = 800.0f;
	
	// 공격중 check값 활성화
	IsAttacking = false;

	// 콤보 설정
	MaxCombo = 4;
	// comnostate Set 해주기
	AttackEndComboState();

	// 새로 만든 콜리전을 character의 콜리전 기본값으로 설정
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("ABCharacter"));

	// 디버그 드로잉 설정
	AttackRange = 80.0f;
	AttackRadius = 50.0f;

	// 스켈레탈 소켓 무기 부착하기
	FName WeaponSocket(TEXT("hand_rSocket"));
	if (GetMesh()->DoesSocketExist(WeaponSocket))
	{
		Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_WEAPON(TEXT("/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_BlackKnight/SK_Blade_BlackKnight.SK_Blade_BlackKnight"));
			if (SK_WEAPON.Succeeded())
			{
				Weapon->SetSkeletalMesh(SK_WEAPON.Object);
			}
		// 소켓과 웨폰을 붙인다.
		Weapon->SetupAttachment(GetMesh(), WeaponSocket);
	}

	// AIController 속성 지정
	AIControllerClass = AABAIController::StaticClass();
	// AI 생성 옵션
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;


	//auto DefaultSetting = GetDefault<UABCharacterSetting>();
	//if (DefaultSetting->CharacterAssets.Num() > 0)
	//{
	//	for (auto CharacterAsset : DefaultSetting->CharacterAssets)
	//	{
	//		ABLOG(Warning, TEXT("Character Asset : %s"), *CharacterAsset.ToString());
	//	}
	//}

	
	AssetIndex = 4;

	SetActorHiddenInGame(true);
	HPBarWidget->SetHiddenInGame(true);
	SetCanBeDamaged(false);
	// bCanBeDamaged = false

	DeadTimer = 5.0f;
}

// Called when the game starts or when spawned
void AABCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// 플레이어인지 AI인지 BeginPlay에서 확인
	bIsPlayer = IsPlayerControlled();
	if (bIsPlayer)
	{
		// Player는 PlayerController컨트롤러 배정
		ABPlayerController = Cast<AABPlayerController>(GetController());
		ABCHECK(nullptr != ABPlayerController);
	}
	else
	{
		// AI는 AIController 배정
		ABAIController = Cast<AABAIController>(GetController());
		ABCHECK(nullptr != ABAIController);
	}

	auto DefaultSetting = GetDefault<UABCharacterSetting>();

	if (bIsPlayer)
	{
		auto ABPlayerState = Cast<AABPlayerState>(GetPlayerState());
		ABCHECK(nullptr != ABPlayerState);
		AssetIndex = ABPlayerState->GetCharacterIndex();
	}
	else
	{
		// AI는 랜덤한 Mesh를 가진다.
		AssetIndex = FMath::RandRange(0, DefaultSetting->CharacterAssets.Num() - 1);
	}

	CharacterAssetToLoad = DefaultSetting->CharacterAssets[AssetIndex];
	auto ABGameInstance = Cast<UABGameInstance>(GetGameInstance());
	AssetStreamingHandle = ABGameInstance->StreamableManager.RequestAsyncLoad(CharacterAssetToLoad, FStreamableDelegate::CreateUObject(this, &AABCharacter::OnAssetLoadCompleted));
	SetCharacterState(ECharacterState::LOADING);

	//if (!IsPlayerControlled())
	//{
	//	auto DefaultSetting = GetDefault<UABCharacterSetting>();
	//	// 랜덤으로 하나 가져오기 (왜 -1을 해주었을까? Null때문에?)
	//	int32 RandIndex = FMath::RandRange(0, DefaultSetting->CharacterAssets.Num() -1);
	//	CharacterAssetToLoad = DefaultSetting->CharacterAssets[RandIndex];

	//	auto ABGameInstance = Cast<UABGameInstance>(GetGameInstance());
	//	if (nullptr != ABGameInstance)
	//	{
	//		AssetStreamingHandle = ABGameInstance->StreamableManager.RequestAsyncLoad(CharacterAssetToLoad, FStreamableDelegate::CreateUObject(this, &AABCharacter::OnAssetLoadCompleted));
	//	}
	//}



	// 게임 시작시 ABWepon Actor 소켓에 부착
	FName WeponSocket(TEXT("hand_rSocket"));
	auto CurWeapon = GetWorld()->SpawnActor<AABWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);
	if (nullptr != CurWeapon)
	{
		CurWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeponSocket);

	}

	auto CharacterWidget = Cast<UABCharacterWidget>(HPBarWidget->GetUserWidgetObject());
	if (nullptr != CharacterWidget)
	{
		// 스텟 연결
		CharacterWidget->BindCharacterStat(CharacterStat);
	}



}

float AABCharacter::GetFinalAttackRange() const
{
	return (nullptr != CurrentWeapon) ? CurrentWeapon->GetAttackRange() : AttackRange;
}

// 무기 장착 확인
bool AABCharacter::CanSetWeapon()
{
	return true;
}

// 무기 장착 함수
void AABCharacter::SetWeapon(AABWeapon* NewWeapon)
{
	ABCHECK(nullptr != NewWeapon);

	if (nullptr != CurrentWeapon)
	{
		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentWeapon->Destroy();
		CurrentWeapon = nullptr;
	}

	FName WeaponSoket(TEXT("hand_rSocket"));
	if (nullptr != NewWeapon)
	{
		NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSoket);
		NewWeapon->SetOwner(this);

		CurrentWeapon = NewWeapon;
	}

}


// Called every frame
void AABCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// SpringArm 길이 체크
	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, ArmLengthTo, DeltaTime, ArmLengthSpeed);

	// Mode체크
	switch (CurrentControlMode)
	{
	case EControlMode::DIABLO:
		SpringArm->RelativeRotation = FMath::RInterpTo(SpringArm->RelativeRotation, ArmRotationTo, DeltaTime, ArmLengthSpeed);
		break;
	}

	switch (CurrentControlMode)
	{
	case EControlMode::DIABLO:
		
		if (DirectionToMove.SizeSquared() > 0.0f)
		{
			GetController()->SetControlRotation(FRotationMatrix::MakeFromX(DirectionToMove).Rotator());
			AddMovementInput(DirectionToMove);
		}
		break;
	}

}

// Called to bind functionality to input
// 입력 받는 함수
void AABCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("ViewChange"), EInputEvent::IE_Pressed, this, &AABCharacter::ViewChange);
	PlayerInputComponent->BindAction(TEXT("Jump"),EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AABCharacter::Attack);

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AABCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AABCharacter::LeftRight);

	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AABCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AABCharacter::Turn);


	// ControlGameMode
	// SetControlMode(EControlMode::GTA);
	SetControlMode(EControlMode::DIABLO);
}

// Mode 별로 움직임 구성
void AABCharacter::UpDown(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		AddMovementInput(FRotationMatrix(FRotator(0.0f,GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::X), NewAxisValue);
		break;
	case EControlMode::DIABLO:
		DirectionToMove.X = NewAxisValue;
		break;
	}
	
	
	// ABLOG(Warning, TEXT("%f"), NewAxisValue);
}

void AABCharacter::LeftRight(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::Y), NewAxisValue);
		break;
	case EControlMode::DIABLO:
		DirectionToMove.Y = NewAxisValue;
		break;
	}
	// ABLOG(Warning, TEXT("%f"), NewAxisValue);
}

void AABCharacter::LookUp(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		AddControllerPitchInput(NewAxisValue);
		break;
	}
	// ABLOG(Warning, TEXT("%f"), NewAxisValue);
}

void AABCharacter::Turn(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		AddControllerYawInput(NewAxisValue);
		break;
	}
	// ABLOG(Warning, TEXT("%f"), NewAxisValue);
}

void AABCharacter::SetControlMode(EControlMode NewControlMode)
{
	CurrentControlMode = NewControlMode;

	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		/*SpringArm->TargetArmLength = 450.0f;
		SpringArm->SetRelativeRotation(FRotator::ZeroRotator);*/

		ArmLengthTo = 450.0f;

		SpringArm->bUsePawnControlRotation = true;
		SpringArm->bInheritPitch = true;
		SpringArm->bInheritRoll = true;
		SpringArm->bInheritYaw = true;
		SpringArm->bDoCollisionTest = true;
		bUseControllerRotationYaw = false;

		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
		break;
	case EControlMode::DIABLO:
		/*SpringArm->TargetArmLength = 800.0f;
		SpringArm->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));*/

		ArmLengthTo = 800.0f;
		ArmRotationTo = FRotator(-45.0f, 0.0f, 0.0f);
		SpringArm->bUsePawnControlRotation = false;
		SpringArm->bInheritPitch = false;
		SpringArm->bInheritRoll = false;
		SpringArm->bInheritYaw = false;
		SpringArm->bDoCollisionTest = false;
		bUseControllerRotationYaw = true;
		bUseControllerRotationYaw = false;

		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
		break;

	case EControlMode::NPC:
		bUseControllerRotationYaw = false;
		// 무드러운 모션 끔.
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		// 캐릭터 회전(돌아보는지) 유무
		GetCharacterMovement()->bOrientRotationToMovement = true;
		// 돌아볼때 걸리는 시간
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);
		break;

	default:
		break;
	}

}

// 카메라 view 체인지
void AABCharacter::ViewChange()
{
	switch (CurrentControlMode)
	{
	case AABCharacter::EControlMode::GTA:
		GetController()->SetControlRotation(GetActorRotation());
		SetControlMode(EControlMode::DIABLO);

		break;
	case AABCharacter::EControlMode::DIABLO:
		GetController()->SetControlRotation(SpringArm->RelativeRotation);
		SetControlMode(EControlMode::GTA);
		break;
	default:
		break;
	}
}

void AABCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ABAnim = Cast<UABAnimInstance>(GetMesh()->GetAnimInstance());
	ABCHECK(nullptr != ABAnim);
	
	// OnMontageEnded 델리게이트와 내가 만든 함수 연결.
	ABAnim->OnMontageEnded.AddDynamic(this, &AABCharacter::OnAttackMontageEnded);

	// 멀티케스트 델리게이트인 OnNextAttackCheck를 람다식으로 표현
	ABAnim->OnNextAttackCheck.AddLambda([this]() -> void{
		ABLOG(Warning, TEXT("OnNextAttackCheck"));
		CanNextCombo = false;
	
		if (IsComboInputOn)
		{
			AttackStartComboState();
			ABAnim->JumpToAttackMontageSection(CurrentCombo);
		}
	});

	// Notify를 활용하여 애니매이션 타이밍에 AttackCheck함수 발동.
	ABAnim->OnAttackHitCheck.AddUObject(this, &AABCharacter::AttackCheck);

	// 멀티케스트 델리게이트인 OnHPIsZero 람다식으로 표현
	CharacterStat->OnHPIsZero.AddLambda([this]() -> void {

		ABLOG(Warning, TEXT("OnHPIsZero"));
		ABAnim->SetDeadAnim();
		SetActorEnableCollision(false);

	});

	//auto CharacterWidget = Cast<UABCharacterWidget>(HPBarWidget-> GetUserWidgetObject());
	//if (nullptr != CharacterWidget)
	//{
	//	// 스텟 연결
	//	CharacterWidget->BindCharacterStat(CharacterStat);
	//}

}

void AABCharacter::Attack()
{
	// ABLOG_S(Warning);
	if (IsAttacking)
	{	
		// 런타임에 문제 발생시 붉은색 에러로그를 뿌리는 함수 ABCHEECK
		ABCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 1, MaxCombo));
		if (CanNextCombo)
		{
			IsComboInputOn = true;
		}
	}
	else
	{
		ABCHECK(CurrentCombo == 0);
		AttackStartComboState();
		ABAnim->PlayAttackMontage();
		ABAnim->JumpToAttackMontageSection(CurrentCombo);
		IsAttacking = true;
	}

	// if (nullptr == ABAnim) return;
	// ABAnim->PlayAttackMontage();
	// IsAttacking = true;
}

void AABCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	ABCHECK(IsAttacking);
	ABCHECK(CurrentCombo > 0);
	IsAttacking = false;
	AttackEndComboState();

	OnAttackEnd.Broadcast();
}

void AABCharacter::AttackStartComboState()
{
	// 다음 콤보 이동 가능여부
	CanNextCombo = true;
	// 콤보 입력 여부
	IsComboInputOn = false;
	// 
	ABCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 0, MaxCombo - 1));

	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}

void AABCharacter::AttackEndComboState()
{
	
	IsComboInputOn = false;
	CanNextCombo = false;
	CurrentCombo = 0;
}

void AABCharacter::AttackCheck()
{
	float FinalAttackRange = GetFinalAttackRange();

	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		// 엑터의 위치부터
		GetActorLocation(),
		// 액터의 Forward x축 에서 FinalAttackRange 까지 Check
		GetActorLocation() + GetActorForwardVector() * FinalAttackRange,
		// 도형의 회전 rotation
		FQuat::Identity,
		// trace채널 설정
		ECollisionChannel::ECC_GameTraceChannel2,
		// 50cm의 반지름을 가진 구체
		FCollisionShape::MakeSphere(50.0f),
		// 탐색방법 설정을 모아둔 구조체
		Params);
	
	// 디버그 드로잉 선언, 구현
	#if ENABLE_DRAW_DEBUG

		// Trace의 총길이
		FVector TraceVec = GetActorForwardVector() * FinalAttackRange;
		// 캡슐이 생성될 센터와 위치 vector
		FVector Center = GetActorLocation() + TraceVec * 0.5f;
		// 센터부터 circle의 지름
		float HalfHeight = FinalAttackRange * 0.5f + AttackRadius;
		// TraceVec를 기준으로 생성되어 z축을 향해 뻗쳐나간다.
		FQuat CapsulRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
		// 히트 처리 true 왼: 그린, false 오: Red
		FColor DrawColor = bResult ? FColor::Green : FColor::Red;
		// 드로잉이 보여질 시간.
		float DebugLifeTime = 5.0f;

		// 캡슐형 디버그 드로잉 설정
		DrawDebugCapsule(GetWorld(),
		Center,
		HalfHeight,
		AttackRadius,
		CapsulRot,
		DrawColor,
		false,
		DebugLifeTime);

	UE_VLOG_LOCATION(this, TEXT("LeeDeukwooUe4"), Verbose, GetActorLocation(), 50.0f, FColor::Blue, TEXT("Attack Position"));

	UE_VLOG_CAPSULE(this, TEXT("LeeDeukwooUe4"), Verbose, GetActorLocation() - GetActorForwardVector() * AttackRadius, HalfHeight, AttackRadius, CapsulRot, DrawColor, TEXT("Attack Area"));


	#endif



	// 가비지 컬렉션 때문에 약포인터로 지정된 엑터의 접근하려면 IsValid 함수를 사용해아한다.
	if (bResult)
	{
		if (HitResult.Actor.IsValid())
		{
			// Actor의 이름
			ABLOG(Warning, TEXT("Hit Actor Name : %s"), *HitResult.Actor->GetName());
			// Damage를 입히는 처리
			FDamageEvent DamageEvent;
			HitResult.Actor->TakeDamage(GetFinalAttackDamage(), DamageEvent, GetController(), this);

		}
	}


}

float AABCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	ABLOG(Warning, TEXT("Actor : %s took Damage : %f"), *GetName(), FinalDamage);

	CharacterStat->SetDamage(FinalDamage);

	if (CurrentState == ECharacterState::DEAD)
	{
		if (EventInstigator->IsPlayerController())
		{
			// 가해자 인자 전달
			ABPlayerController = Cast<AABPlayerController>(EventInstigator);
			// 죽이는 자가 플레이어이면 Ture
			ABCHECK(nullptr != ABPlayerController, 0.0f);
			ABPlayerController->NPCKill(this);
		}
	}
	//if (FinalDamage > 0.0f)
	//{
	//	ABAnim->SetDeadAnim();
	//	SetActorEnableCollision(false);
	//}

	return FinalDamage;
}

void AABCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 플레이어가 아니라면 NPC컨트롤 모드로 전환
	//if (IsPlayerControlled())
	//{
	//	SetControlMode(EControlMode::DIABLO);
	//	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	//}
	//else
	//{
	//	SetControlMode(EControlMode::NPC);
	//	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	//}
}

void AABCharacter::OnAssetLoadCompleted()
{
	USkeletalMesh* AssetLoaded = Cast<USkeletalMesh>(AssetStreamingHandle->GetLoadedAsset());
	AssetStreamingHandle.Reset();
	ABCHECK(nullptr != AssetLoaded);
	GetMesh()->SetSkeletalMesh(AssetLoaded);

	SetCharacterState(ECharacterState::READY);

	/*if (nullptr != AssetLoaded)
	{
		GetMesh()->SetSkeletalMesh(AssetLoaded);
	}*/
}

void AABCharacter::SetCharacterState(ECharacterState NewState)
{
	ABCHECK(CurrentState != NewState);
	CurrentState = NewState;

	switch (CurrentState)
	{
	case ECharacterState::LOADING:
	{
		if (bIsPlayer)
		{
			DisableInput(ABPlayerController);

			// HUD에 캐릭터의 stat 넘겨주기.
			ABPlayerController->GetHUDWidget()->BindCharacterStat(CharacterStat);

			auto ABPlayerState = Cast<AABPlayerState>(GetPlayerState());
			ABCHECK(nullptr != ABPlayerState);
			CharacterStat->SetNewLevel(ABPlayerState->GetCharacterLevel());

		}
		else
		{
			// NPC
			auto ABGameMode = Cast<AABGameMode>(GetWorld()->GetAuthGameMode());
			ABCHECK(nullptr != ABGameMode);
			// Score를 곱하여 점점 레벨이 올라간다.
			int32 TargetLevel = FMath::CeilToInt(((float)ABGameMode->GetScore() * 0.8f));
			int32 FinalLevel = FMath::Clamp<int32>(TargetLevel, 1, 20);
			ABLOG(Warning, TEXT("New NPC Level : %d"), FinalLevel);
			CharacterStat->SetNewLevel(FinalLevel);
		}

		SetActorHiddenInGame(true);
		HPBarWidget->SetHiddenInGame(true);
		SetCanBeDamaged(false);
		break;
	}

	case ECharacterState::PREINIT:
		break;
	
	case ECharacterState::READY:
	{
		SetActorHiddenInGame(false);
		HPBarWidget->SetHiddenInGame(false);
		SetCanBeDamaged(true);

		// OnHPIsZero델리게이트에 SetCharacterState(ECharacterState::DEAD) 연결
		CharacterStat->OnHPIsZero.AddLambda([this]() -> void {
			
			SetCharacterState(ECharacterState::DEAD);
		});

		if (bIsPlayer)
		{
			SetControlMode(EControlMode::DIABLO);
			GetCharacterMovement()->MaxWalkSpeed = 600.0f;
			EnableInput(ABPlayerController);
		}
		else
		{
			SetControlMode(EControlMode::NPC);
			GetCharacterMovement()->MaxWalkSpeed = 400.0f;
			ABAIController->RunAI();
		}

		break;
		
	}
	case ECharacterState::DEAD:
	{
		
		SetActorEnableCollision(false);
		GetMesh()->SetHiddenInGame(false);
		HPBarWidget->SetHiddenInGame(true);
		ABAnim->SetDeadAnim();
		SetCanBeDamaged(false);

		if (bIsPlayer)
		{
			// 즉을시 움직임 멈추기.
			DisableInput(ABPlayerController);
		}
		else
		{
			// AI죽을 시 Tree멈춤.
			ABAIController->StopAI();
		}
		
		
		GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda([this]() -> void{
		
			if (bIsPlayer)
			{
				// Settimer로 5초후에 ShowResultUI
				ABPlayerController->ShowResultUI();
			}
			else
			{
				// Settimer로 5초후에 파괴된다.
				Destroy();
			}
		
		}), DeadTimer, false);

		break;
	}
	}
}

ECharacterState AABCharacter::GetCharacter() const
{
	return CurrentState;
}

int32 AABCharacter::GetExp() const
{
	return CharacterStat->GetDropExp();
}

float AABCharacter::GetFinalAttackDamage() const
{
	float AttackDamage = (nullptr != CurrentWeapon) ? 
	(CharacterStat->GetAttack() + CurrentWeapon->GetAttackDamage()) : CharacterStat->GetAttack();

	float AttackModifier = (nullptr != CurrentWeapon) ? CurrentWeapon->GetAttackModifier() : 1.0f;
	
	return AttackDamage * AttackModifier;
}




