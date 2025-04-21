// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SJAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAttributeChanged, AActor*, InstigatorActor, USJAttributeComponent*, OwningComp, float, NewValue, float, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOwnerKilled);

 
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SYNTYGAMEJAM_API USJAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USJAttributeComponent();



protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/*virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;*/

	UPROPERTY(VisibleAnywhere,  BlueprintReadOnly, Category = "Attributes")
	float Health;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes")
	float HealthMax;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes")
	int32 Reputation;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool Kill(AActor* InstigatorActor);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool IsFullHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetHealthMax() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	int32 GetReputation() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void GrantReputation(int32 ReputationGranted);

	UPROPERTY(BlueprintAssignable, Category = "Attributes")
	FOnAttributeChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnOwnerKilled OnOwnerKilled;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(AActor* InstigatorActor, float Delta);
};
