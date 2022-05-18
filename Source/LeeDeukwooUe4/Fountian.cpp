// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/RotatingMovementComponent.h"
#include "Fountian.h"

// Sets default values
AFountian::AFountian()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BODY"));
	Water = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WATER"));
	Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("LIGHT"));
	Splash = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SPLASH"));
	RMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RMOVEMENT"));

	RootComponent = Body;
	Water->SetupAttachment(Body);
	Light->SetupAttachment(Body);
	Splash->SetupAttachment(Body);

	Water->SetRelativeLocation(FVector(0.0f, 0.0f, 135.0f));
	Light->SetRelativeLocation(FVector(0.0f, 0.0f, 195.0f));
	Splash->SetRelativeLocation(FVector(0.0f, 0.0f, 195.0f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		SM_BODY(TEXT("/Game/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Ruins/StaticMesh/SM_Plains_Castle_Fountain_01.SM_Plains_Castle_Fountain_01"));

	if (SM_BODY.Succeeded())
	{
		Body->SetStaticMesh(SM_BODY.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		SM_WATAR(TEXT("/Game/InfinityBladeGrassLands/Effects/FX_Meshes/Env/SM_Plains_Fountain_02.SM_Plains_Fountain_02"));
	if (SM_WATAR.Succeeded())
	{
		Water->SetStaticMesh(SM_WATAR.Object);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		PS_SPLASH(TEXT("/Game/InfinityBladeGrassLands/Effects/FX_Ambient/Water/P_Water_Fountain_Splash_Base_01.P_Water_Fountain_Splash_Base_01"));
	if (PS_SPLASH.Succeeded())
	{
		Splash->SetTemplate(PS_SPLASH.Object);
	}


	RotateSpeed = 30.0f;
	RMovement->RotationRate = FRotator(0.0f, RotateSpeed, 0.0f);

}

// Called when the game starts or when spawned
void AFountian::BeginPlay()
{
	Super::BeginPlay();

	ABLOG_S(Warning);
	ABLOG(Warning, TEXT("Actor Name : %s, ID : %d, Location X : %3.f;"), *GetName(), ID, GetActorLocation().X);

	// UE_LOG(Lee, Warning, TEXT("Actor Name : %s, ID : %d, Location X : %3.f;"), *GetName(), ID, GetActorLocation().X);
	
}

// Called every frame
void AFountian::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFountian::EndPlay(const EEndPlayReason::Type EndPlayReson)
{
	Super::EndPlay(EndPlayReson);
	ABLOG_S(Warning);
}

void AFountian::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ABLOG_S(Warning);
}