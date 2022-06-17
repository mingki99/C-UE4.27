// Fill out your copyright notice in the Description page of Project Settings.


#include "ABCharacter.h"
#include "ABAnimInstance.h"
#include "ABCharacterComponent.h"
#include "DrawDebugHelpers.h"

// ABWeapon ���͸� ����ϱ⿡ �ش��߰�
#include "ABWeapon.h"
#include "Components/WidgetComponent.h"
#include "ABCharacterWidget.h"


// Sets default values
AABCharacter::AABCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ������Ʈ ����
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

	// ĳ���� skeletalmesh ����
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
	// ���� �پ��� ��ġ Screen���� ����
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Game/Book/UI/UI_HPBar.UI_HPBar_C"));
	if (UI_HUD.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		// ���� ũ�� ����
		HPBarWidget->SetDrawSize(FVector2D(150.0f, 50.0f));
	}


	// view change�� �� springarm�� �ε巯�� ȿ���� ���� ����
	ArmLengthSpeed = 3.0f;
	ArmRotatoinSpeed = 10.0f;
	
	// ���� ���� ����
	GetCharacterMovement()->JumpZVelocity = 800.0f;
	
	// ������ check�� Ȱ��ȭ
	IsAttacking = false;

	// �޺� ����
	MaxCombo = 4;
	// comnostate Set ���ֱ�
	AttackEndComboState();

	// ���� ���� �ݸ����� character�� �ݸ��� �⺻������ ����
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("ABCharacter"));

	// ����� ����� ����
	AttackRange = 200.0f;
	AttackRadius = 50.0f;

	// ���̷�Ż ���� ���� �����ϱ�
	FName WeaponSocket(TEXT("hand_rSocket"));
	if (GetMesh()->DoesSocketExist(WeaponSocket))
	{
		Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_WEAPON(TEXT("/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_BlackKnight/SK_Blade_BlackKnight.SK_Blade_BlackKnight"));
			if (SK_WEAPON.Succeeded())
			{
				Weapon->SetSkeletalMesh(SK_WEAPON.Object);
			}
		// ���ϰ� ������ ���δ�.
		Weapon->SetupAttachment(GetMesh(), WeaponSocket);
	}

}

// Called when the game starts or when spawned
void AABCharacter::BeginPlay()
{
	Super::BeginPlay();

	// ���� ���۽� ABWepon Actor ���Ͽ� ����
	FName WeponSocket(TEXT("hand_rSocket"));
	auto CurWeapon = GetWorld()->SpawnActor<AABWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);
	if (nullptr != CurWeapon)
	{
		CurWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeponSocket);

	}

	auto CharacterWidget = Cast<UABCharacterWidget>(HPBarWidget->GetUserWidgetObject());
	if (nullptr != CharacterWidget)
	{
		// ���� ����
		CharacterWidget->BindCharacterStat(CharacterStat);
	}



}

// ���� ���� Ȯ��
bool AABCharacter::CanSetWeapon()
{
	return(nullptr == CurrentWeapon);
}

// ���� ���� �Լ�
void AABCharacter::SetWeapon(AABWeapon* NewWeapon)
{
	ABCHECK(nullptr != NewWeapon && nullptr == CurrentWeapon)
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

	// SpringArm ���� üũ
	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, ArmLengthTo, DeltaTime, ArmLengthSpeed);

	// Modeüũ
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
// �Է� �޴� �Լ�
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

// Mode ���� ������ ����
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
	case AABCharacter::EControlMode::GTA:
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
	case AABCharacter::EControlMode::DIABLO:
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
	default:
		break;
	}

}

// ī�޶� view ü����
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
	
	// OnMontageEnded ��������Ʈ�� ���� ���� �Լ� ����.
	ABAnim->OnMontageEnded.AddDynamic(this, &AABCharacter::OnAttackMontageEnded);

	// ��Ƽ�ɽ�Ʈ ��������Ʈ�� OnNextAttackCheck�� ���ٽ����� ǥ��
	ABAnim->OnNextAttackCheck.AddLambda([this]() -> void{
		ABLOG(Warning, TEXT("OnNextAttackCheck"));
		CanNextCombo = false;
	
		if (IsComboInputOn)
		{
			AttackStartComboState();
			ABAnim->JumpToAttackMontageSection(CurrentCombo);
		}
	});

	// Notify�� Ȱ���Ͽ� �ִϸ��̼� Ÿ�ֿ̹� AttackCheck�Լ� �ߵ�.
	ABAnim->OnAttackHitCheck.AddUObject(this, &AABCharacter::AttackCheck);

	// ��Ƽ�ɽ�Ʈ ��������Ʈ�� OnHPIsZero ���ٽ����� ǥ��
	CharacterStat->OnHPIsZero.AddLambda([this]() -> void {

		ABLOG(Warning, TEXT("OnHPIsZero"));
		ABAnim->SetDeadAnim();
		SetActorEnableCollision(false);

	});

	//auto CharacterWidget = Cast<UABCharacterWidget>(HPBarWidget-> GetUserWidgetObject());
	//if (nullptr != CharacterWidget)
	//{
	//	// ���� ����
	//	CharacterWidget->BindCharacterStat(CharacterStat);
	//}

}

void AABCharacter::Attack()
{
	// ABLOG_S(Warning);
	if (IsAttacking)
	{	
		// ��Ÿ�ӿ� ���� �߻��� ������ �����α׸� �Ѹ��� �Լ� ABCHEECK
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
}

void AABCharacter::AttackStartComboState()
{
	// ���� �޺� �̵� ���ɿ���
	CanNextCombo = true;
	// �޺� �Է� ����
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
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		// ������ ��ġ����
		GetActorLocation(),
		// ������ Forward x�� ���� 200cm ���� Check
		GetActorLocation() + GetActorForwardVector() * 200.0f,
		// ������ ȸ�� rotation
		FQuat::Identity,
		// traceä�� ����
		ECollisionChannel::ECC_GameTraceChannel2,
		// 50cm�� �������� ���� ��ü
		FCollisionShape::MakeSphere(50.0f),
		// Ž����� ������ ��Ƶ� ����ü
		Params);
	
	// ����� ����� ����, ����
	#if ENABLE_DRAW_DEBUG

		// Trace�� �ѱ���
		FVector TraceVec = GetActorForwardVector() * AttackRange;
		// ĸ���� ������ ���Ϳ� ��ġ vector
		FVector Center = GetActorLocation() + TraceVec * 0.5f;
		// ���ͺ��� circle�� ����
		float HalfHeight = AttackRange * 0.5f + AttackRadius;
		// TraceVec�� �������� �����Ǿ� z���� ���� ���ĳ�����.
		FQuat CapsulRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
		// ��Ʈ ó�� true ��: �׸�, false ��: Red
		FColor DrawColor = bResult ? FColor::Green : FColor::Red;
		// ������� ������ �ð�.
		float DebugLifeTime = 5.0f;

		// ĸ���� ����� ����� ����
		DrawDebugCapsule(GetWorld(),
		Center,
		HalfHeight,
		AttackRadius,
		CapsulRot,
		DrawColor,
		false,
		DebugLifeTime);

	#endif



	// ������ �÷��� ������ �������ͷ� ������ ������ �����Ϸ��� IsValid �Լ��� ����ؾ��Ѵ�.
	if (bResult)
	{
		if (HitResult.Actor.IsValid())
		{
			// Actor�� �̸�
			ABLOG(Warning, TEXT("Hit Actor Name : %s"), *HitResult.Actor->GetName());
			// Damage�� ������ ó��
			FDamageEvent DamageEvent;
			HitResult.Actor->TakeDamage(CharacterStat->GetAttack() , DamageEvent, GetController(), this);

		}
	}


}

float AABCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	ABLOG(Warning, TEXT("Actor : %s took Damage : %f"), *GetName(), FinalDamage);

	CharacterStat->SetDamage(FinalDamage);

	//if (FinalDamage > 0.0f)
	//{
	//	ABAnim->SetDeadAnim();
	//	SetActorEnableCollision(false);
	//}

	return FinalDamage;
}




