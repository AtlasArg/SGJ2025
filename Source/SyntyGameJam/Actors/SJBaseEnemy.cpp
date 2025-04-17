// Fill out your copyright notice in the Description page of Project Settings.


#include "SJBaseEnemy.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SyntyGameJam/SJAIController.h"
#include "Components/WidgetComponent.h"
#include "SyntyGameJam/Actors/BaseProjectile.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
ASJBaseEnemy::ASJBaseEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// TODO: Check
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
}

void ASJBaseEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	SJAIController = Cast<ASJAIController>(NewController);
	SJAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	SJAIController->RunBehaviorTree(BehaviorTree);
	SJAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);
}

//void ASJBaseEnemy::FireProjectile(FVector Location)
//{
//	if (!bCanFire || Bullets <= 0)
//	{
//		return;
//	}
//
//	bCanFire = false;
//	GetWorld()->GetTimerManager().SetTimer(FireCooldownHandle, this, &ThisClass::ResetFire, FireCooldownSeconds, false);
//
//	if (!ProjectileClass)
//	{
//		return;
//	}
//	
//	UpdateFacingTarget(Location);
//
//	if (FireMontage && GetMesh() && GetMesh()->GetAnimInstance())
//	{
//		GetMesh()->GetAnimInstance()->Montage_Play(FireMontage);
//	}
//
//	FTransform MuzzleTransform = GetMesh()->GetSocketTransform(MuzzleSocket);
//	FVector MuzzleLocation = MuzzleTransform.GetLocation();
//
//	FVector ShootDirection = (Location - MuzzleLocation).GetSafeNormal();
//	FRotator MuzzleRotation = ShootDirection.Rotation();
//
//	FActorSpawnParameters SpawnParams;
//	SpawnParams.Owner = this;
//	SpawnParams.Instigator = GetInstigator();
//
//	GetWorld()->SpawnActor<ABaseProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
//	Bullets--;
//
//	if (OnFProjectileFired.IsBound())
//	{
//		OnFProjectileFired.Broadcast();
//	}
//}

// Called when the game starts or when spawned
void ASJBaseEnemy::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ASJBaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
