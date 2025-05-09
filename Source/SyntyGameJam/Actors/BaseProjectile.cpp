#include "BaseProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SyntyGameJam/SyntyGameJam.h"
#include "SyntyGameJam/Actors/SJAttributeComponent.h"
#include "SyntyGameJam/SJBaseCharacter.h"

// Sets default values
ABaseProjectile::ABaseProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bTickEvenWhenPaused = false;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->InitSphereRadius(8.f);
	//Collision->SetCollisionProfileName(TEXT("BlockAll"));
	RootComponent = Collision;

	Collision->SetCollisionObjectType(ECC_Projectile);
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Collision->SetCollisionResponseToAllChannels(ECR_Ignore);
	Collision->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Collision->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	Collision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	//Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//Collision->SetCollisionObjectType(ECC_Projectile);
	//Collision->SetCollisionResponseToAllChannels(ECR_Ignore);
	//Collision->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	//Collision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	//Collision->SetNotifyRigidBodyCollision(true);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); // Only the sphere collides

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 1500.f;
	ProjectileMovement->MaxSpeed = 1500.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 0.f; // No arc

	InitialLifeSpan = 3.0f;
}

// Called when the game starts or when spawned
void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	//Collision->OnComponentHit.AddDynamic(this, &ThisClass::OnHit);
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereOverlap);
}

void ABaseProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	// TODO: Fix, posiblemente por ahora esten atravesando todo las balas....
	// 
	if (OtherActor && OtherActor != this && OtherComp)
	{
		// Optional: Apply damage or effects here
		// UGameplayStatics::ApplyDamage(...);

		Destroy(); // Destroy on impact
	}
}

void ABaseProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResults)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		// FIX THIS
		ASJBaseCharacter* BaseCharacter = Cast<ASJBaseCharacter>(OtherActor);
		if (IsValid(BaseCharacter))
		{
			BaseCharacter->LooseHealth(GetInstigator(), -DamageAmount);
		}

		Destroy();
	}
}

void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

