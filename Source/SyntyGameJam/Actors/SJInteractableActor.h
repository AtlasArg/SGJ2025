// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SyntyGameJam/Interactions/SJInteractableInterface.h"
#include "SJInteractableActor.generated.h"

class USphereComponent;
class ASJBaseCharacter;
class UWidgetComponent;

UCLASS()
class SYNTYGAMEJAM_API ASJInteractableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASJInteractableActor();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Interact(ASJBaseCharacter* InteractingCharacter);

	virtual bool CanInteract(ASJBaseCharacter* InteractingCharacter);

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> InteractionZone;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> InteractTextWidget;

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
