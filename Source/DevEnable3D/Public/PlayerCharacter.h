// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class DEVENABLE3D_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(VisibleDefaultsOnly , Category = "Component" , meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MaskMeshComponent;

	UPROPERTY(VisibleDefaultsOnly , Category = "Component" , meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleDefaultsOnly , Category = "Component" , meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComponent;

	UFUNCTION(BlueprintCallable)
	void MoveForward(const float InputValue);

	UFUNCTION(BlueprintCallable)
	void MoveRight(const float InputValue);
	
	FVector GetMovementDirection(const FVector& InVector) const;
};
