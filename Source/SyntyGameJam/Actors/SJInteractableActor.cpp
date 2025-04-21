// Fill out your copyright notice in the Description page of Project Settings.


#include "SJInteractableActor.h"
#include "Components/SphereComponent.h"
#include "SyntyGameJam/SJBaseCharacter.h"

// Sets default values
ASJInteractableActor::ASJInteractableActor()
{
    PrimaryActorTick.bCanEverTick = false;

    InteractionZone = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionZone"));
    InteractionZone->SetSphereRadius(650.f);
    InteractionZone->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    InteractionZone->SetCollisionObjectType(ECC_WorldDynamic);
    InteractionZone->SetCollisionResponseToAllChannels(ECR_Ignore);
    InteractionZone->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    RootComponent = InteractionZone;
    
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(RootComponent);
    Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    Mesh->SetCollisionObjectType(ECC_WorldStatic);
    Mesh->SetCollisionResponseToAllChannels(ECR_Block);
}

// Called when the game starts or when spawned
void ASJInteractableActor::BeginPlay()
{
	Super::BeginPlay();
    InteractionZone->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapBegin);
    InteractionZone->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnOverlapEnd);
}

void ASJInteractableActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (ASJBaseCharacter* BaseCharacter = Cast<ASJBaseCharacter>(OtherActor))
    {
        BaseCharacter->SetCurrentInteractable(this);
    }
}

void ASJInteractableActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (ASJBaseCharacter* BaseCharacter = Cast<ASJBaseCharacter>(OtherActor))
    {
        BaseCharacter->SetCurrentInteractable(nullptr);
    }
}

//void ASJInteractableActor::Interact_Implementation(ACharacter* InteractingCharacter)
//{
//    ASJBaseCharacter* BaseCharacter = Cast<ASJBaseCharacter>(InteractingCharacter);
//    BaseCharacter->Interact();
//}

// Called every frame
void ASJInteractableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASJInteractableActor::Interact(ASJBaseCharacter* InteractingCharacter)
{

}

