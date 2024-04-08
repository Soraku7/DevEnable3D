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

	FORCEINLINE bool GetIsWallSliding() const {return IsWallSliding;}

	FORCEINLINE bool GetIsWallJumping() const {return IsWallJumping;}

	FORCEINLINE bool GetIsSliding() const {return IsSliding;}

	bool GetIsFalling();

	bool GetIsGround() const;

private:
	UPROPERTY(VisibleDefaultsOnly , Category = "Component" , meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MaskMeshComponent;

	UPROPERTY(VisibleDefaultsOnly , Category = "Component" , meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleDefaultsOnly , Category = "Component" , meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComponent;

	FHitResult WallCheckResult;

	FCollisionShape WallCheckCapsule;

	FCollisionQueryParams QueryParams;

	FVector SlideDirection;
	
	float DefaultMovementSpeed;
	float DefaultGravityScale;

	bool IsJumping;

	bool IsAirJumping;

	bool IsWallSliding;

	bool IsWallJumping;

	bool IsSliding;

	bool HasZVelocityReset;

	bool IsHeadBlocked;

	int32 JumpCount;

	FName WallCheckTag = FName("Wall");

	UPROPERTY(EditAnywhere , Category = "Movement" , meta = (AllowPrivateAccess = "true"))
	float WalkSpeedMultiplier = 0.5f;

	UPROPERTY(EditAnywhere , Category = "Movement" , meta = (AllowPrivateAccess = "true"))
	float SprintSpeedMultiplier = 2.0f;

	UPROPERTY(EditAnywhere , Category = "Wall Slide" , meta = (AllowPrivateAccess = "true"))
	float WallSlideGravityScale = 0.2f;

	UPROPERTY(EditAnywhere , Category = "Wall Slide" , meta = (AllowPrivateAccess = "true"))
	float WallSlideDeceleration = 10.0f;

	UPROPERTY(EditAnywhere , Category = "Air Jump" , meta = (AllowPrivateAccess = "true"))
	float AirJumpForce = 1000.0f;
	
	UPROPERTY(EditAnywhere , Category = "Sliding" , meta = (AllowPrivateAccess = "true"))
	float SlideDuration = 0.8f;

	UPROPERTY(EditAnywhere , Category = "Wall Jump" , meta = (AllowPrivateAccess = "true"))
	FVector WallJumpVelocity = FVector(800.f , 0.f , 1500.f);
	
	UFUNCTION(BlueprintCallable)
	void MoveForward(const float InputValue);
	
	UFUNCTION(BlueprintCallable)
	void MoveRight(const float InputValue);
	
	UFUNCTION(BlueprintCallable)
	void Sprint();

	void WallSliding(const float DeltaTime);

	UFUNCTION(BlueprintCallable)
	void Walk();

	UFUNCTION(BlueprintCallable)
	void Slide();
	
	void StopSlide();
	
	void AirJump();
	
	void GroundJump();

	void WallJump();

	UFUNCTION(BlueprintCallable)
	void DoJump();
	
	UFUNCTION(BlueprintCallable)
	void ResetMoveSpeed();
	
	UFUNCTION()
	void OnCharacterLanded(const FHitResult& Hit);
	
	FVector GetMovementDirection(const FVector& InVector) const;

	void WallSlideCheck();

	void InitialProperties();

	void ResetZVelocity(bool DoOnce = true);
};
