// Fill out your copyright notice in the Description page of Project Settings.


#include "SJPickeableActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SyntyGameJam/SyntyGameJam.h"
#include "SyntyGameJam/Interactions/SJPickeableReceiver.h"
#include "SyntyGameJam/SJBaseCharacter.h"

// Sets default values
ASJPickeableActor::ASJPickeableActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->InitSphereRadius(8.f);
	//Collision->SetCollisionProfileName(TEXT("BlockAll"));
	RootComponent = Collision;

	Collision->SetCollisionObjectType(ECC_Pickeable);
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Collision->SetCollisionResponseToAllChannels(ECR_Overlap);
	Collision->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Collision->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	Collision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); // Only the sphere collides

	InitialLifeSpan = 20.0f;
}

// Called when the game starts or when spawned
void ASJPickeableActor::BeginPlay()
{
	Super::BeginPlay();
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereOverlap);
}

void ASJPickeableActor::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResults)
{
	ASJBaseCharacter* BaseCharacter = Cast<ASJBaseCharacter>(OtherActor);
	if (!IsValid(BaseCharacter))
	{
		return;
	}

	if (PickupSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation(), 1.0f, 1.0f, 0.0f, BaseCharacter->SoundAttenuation);
	}

	ISJPickeableReceiver::Execute_ReceivePickeable(BaseCharacter, this);
}

// Called every frame
void ASJPickeableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

