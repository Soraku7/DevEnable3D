// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
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

	FORCEINLINE bool GetIsJumping() const {return IsJumping;}

	FORCEINLINE bool GetIsAirJumping() const {return IsAirJumping;}

	bool GetIsFalling();

private:
	UPROPERTY(VisibleDefaultsOnly , Category = "Component" , meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MaskMeshComponent;

	UPROPERTY(VisibleDefaultsOnly , Category = "Component" , meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleDefaultsOnly , Category = "Component" , meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComponent;
	
	float DefaultMovementSpeed;

	bool IsJumping;

	bool IsAirJumping;

	int32 JumpCount;

	UPROPERTY(EditAnywhere , Category = "Movement" , meta = (AllowPrivateAccess = "true"))
	float WalkSpeedMultiplier = 0.5f;

	UPROPERTY(EditAnywhere , Category = "Movement" , meta = (AllowPrivateAccess = "true"))
	float SprintSpeedMultiplier = 2.0f;
	
	UFUNCTION(BlueprintCallable)
	void MoveForward(const float InputValue);
	
	UFUNCTION(BlueprintCallable)
	void MoveRight(const float InputValue);
	
	UFUNCTION(BlueprintCallable)
	void Sprint();

	UFUNCTION(BlueprintCallable)
	void Walk();
	
	void AirJump();
	
	void GroundJump();

	UFUNCTION(BlueprintCallable)
	void DoJump();
	
	UFUNCTION(BlueprintCallable)
	void ResetMoveSpeed();
	
	UFUNCTION()
	void OnCharacterLanded(const FHitResult& Hit);
	
	FVector GetMovementDirection(const FVector& InVector) const;
};
