// Fill out your copyright notice in the Description page of Project Settings.


#include "SJBaseCharacter.h"
#include "Actors/BaseProjectile.h"
#include "Actors/SJAttributeComponent.h"
#include "Actors/SJPickeableActor.h"
#include "SyntyGameJam/SyntyGameJam.h"

// Sets default values
ASJBaseCharacter::ASJBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("RightHandSocket"));
	//TODO: Check, not sure about this...
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AttributeComponent = CreateDefaultSubobject<USJAttributeComponent>("AttributeComp");
}

void ASJBaseCharacter::ReceivePickeable_Implementation(ASJPickeableActor* Pickeable)
{
	switch (Pickeable->PickeableType)
	{
		case EPickeableType::Bullets:
			GainBullets(Pickeable->Amount);
			break;
		case EPickeableType::Coins:
			GainGoldCoins(Pickeable->Amount);
			break;
		case EPickeableType::Health:
			break;
		default:
			break;
	}

	Pickeable->Destroy();
}

void ASJBaseCharacter::FinishFiringProjectile()
{
	FTransform MuzzleTransform = GetMesh()->GetSocketTransform(MuzzleSocket);
	FVector MuzzleLocation = MuzzleTransform.GetLocation();

	FVector ShootDirection = (FireTargetLocation - MuzzleLocation).GetSafeNormal();
	FRotator MuzzleRotation = ShootDirection.Rotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	GetWorld()->SpawnActor<ABaseProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
	Bullets--;

	if (OnFProjectileFired.IsBound())
	{
		OnFProjectileFired.Broadcast();
	}
}

void ASJBaseCharacter::SetFireTargetLocation(FVector FireLocation)
{
	FireTargetLocation = FireLocation;
}

// Called when the game starts or when spawned
void ASJBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	AttributeComponent->OnOwnerKilled.AddDynamic(this, &ThisClass::CharacterDied);
}

// Called every frame
void ASJBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASJBaseCharacter::FireProjectile(FVector Location)
{
	if (!bCanFire || Bullets <= 0)
	{
		return;
	}

	bCanFire = false;
	GetWorld()->GetTimerManager().SetTimer(FireCooldownHandle, this, &ThisClass::ResetFire, FireCooldownSeconds, false);

	if (!ProjectileClass)
	{
		return;
	}

	//FVector TargetLocation = HitResult.ImpactPoint;
	SetFireTargetLocation(Location);
	UpdateFacingTarget(Location);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (FireMontage && AnimInstance)
	{
		AnimInstance->Montage_Play(FireMontage);
		/*AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ThisClass::OnNotifyBeginReceived);*/
	}

	//FVector WorldLocation, WorldDirection;
	//APlayerController* PC = Cast<APlayerController>(GetController());

	//if (PC && PC->DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
	//{
	//	// TODO: 1. refactor que no se vuelva a llamar de nuevo el controller desde aca.
	//	// TODO: 2. que las balas "salgan Chatas" no elevadas si el mouse esta elevado ?
	//	FHitResult HitResult;
	//	FVector TraceStart = WorldLocation;
	//	FVector TraceEnd = TraceStart + WorldDirection * 10000.f; // long range trace

	//	FCollisionQueryParams Params;
	//	Params.AddIgnoredActor(this); // Ignore self

	//	if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, Params))
	//	{
	//		FVector TargetLocation = HitResult.ImpactPoint;

	//		UpdateFacingTarget(TargetLocation);

	//		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	//		if (FireMontage && AnimInstance)
	//		{
	//			AnimInstance->Montage_Play(FireMontage);
	//			/*AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ThisClass::OnNotifyBeginReceived);*/
	//		}

	//	/*	FTransform MuzzleTransform = GetMesh()->GetSocketTransform(MuzzleSocket);
	//		FVector MuzzleLocation = MuzzleTransform.GetLocation();

	//		FVector ShootDirection = (TargetLocation - MuzzleLocation).GetSafeNormal();
	//		FRotator MuzzleRotation = ShootDirection.Rotation();

	//		FActorSpawnParameters SpawnParams;
	//		SpawnParams.Owner = this;
	//		SpawnParams.Instigator = GetInstigator();

	//		GetWorld()->SpawnActor<ABaseProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
	//		Bullets--;

	//		if (OnFProjectileFired.IsBound())
	//		{
	//			OnFProjectileFired.Broadcast();
	//		}*/
	//	}
	//}
}

void ASJBaseCharacter::GainBullets(int32 GainedBullets)
{
	Bullets += GainedBullets;
	if (OnBulletsUpdated.IsBound())
	{
		OnBulletsUpdated.Broadcast(GoldCoins);
	}
}

void ASJBaseCharacter::GainGoldCoins(int32 GainedCoins)
{
	GoldCoins += GainedCoins;
	if (OnCoinsUpdated.IsBound())
	{
		OnCoinsUpdated.Broadcast(GoldCoins);
	}
}

void ASJBaseCharacter::ResetFire()
{
	bCanFire = true;
}

void ASJBaseCharacter::CharacterDied()
{
	// TODO: we can refactor this

	int32 RandomOffsetInX = FMath::RandRange(50, 200);
	int32 RandomOffsetInY = FMath::RandRange(50, 200);
	FVector CharacterLocation = GetActorLocation();
	FVector LocationToSpawn = FVector(CharacterLocation.X + RandomOffsetInX, CharacterLocation.Y + RandomOffsetInY, CharacterLocation.Z);

	if (DropeableBulletsClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();
		ASJPickeableActor* ItemToPick = GetWorld()->SpawnActor<ASJPickeableActor>(DropeableBulletsClass, LocationToSpawn, FRotator(), SpawnParams);
		ItemToPick->Amount = GetBulletCount();
	}
	
	if (DropeableCoinsClass)
	{
		RandomOffsetInX = FMath::RandRange(50, 200);
		RandomOffsetInY = FMath::RandRange(50, 200);
		LocationToSpawn = FVector(CharacterLocation.X + RandomOffsetInX, CharacterLocation.Y + RandomOffsetInY, CharacterLocation.Z);
		FActorSpawnParameters SpawnParamsCoins;
		SpawnParamsCoins.Owner = this;
		SpawnParamsCoins.Instigator = GetInstigator();
		ASJPickeableActor* ItemToPick = GetWorld()->SpawnActor<ASJPickeableActor>(DropeableCoinsClass, LocationToSpawn, FRotator(), SpawnParamsCoins);
		ItemToPick->Amount = GetGoldCoins();
	}
	
	Destroy();
}
