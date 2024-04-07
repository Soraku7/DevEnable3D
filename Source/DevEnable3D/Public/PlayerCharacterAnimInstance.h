// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerCharacterAnimInstance.generated.h"

class APlayerCharacter;
/**
 * 
 */
UCLASS()
class DEVENABLE3D_API UPlayerCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	virtual void NativeBeginPlay() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
public:

private:
	UPROPERTY(BlueprintReadOnly , Category = "Parameters" , meta = (AllowPrivateAccess = "true"))
	bool IsJump;
	
	UPROPERTY(BlueprintReadOnly , Category = "Parameters" , meta = (AllowPrivateAccess = "true"))
	bool IsFalling;

	UPROPERTY(BlueprintReadOnly , Category = "Parameters" , meta = (AllowPrivateAccess = "true"))
	bool IsAirJumping;

	UPROPERTY(BlueprintReadOnly , Category = "Parameters" , meta = (AllowPrivateAccess = "true"))
	bool IsWallSliding;

	UPROPERTY(BlueprintReadOnly , Category = "Parameters" , meta = (AllowPrivateAccess = "true"))
	bool IsWallJumping;

	UPROPERTY(BlueprintReadOnly , Category = "Parameters" , meta = (AllowPrivateAccess = "true"))
	bool IsGrounding;
	
	UPROPERTY(BlueprintReadOnly , Category = "Parameters" , meta = (AllowPrivateAccess = "true"))
	float MoveSpeed;
	
	APlayerCharacter* PlayerCharacter;
	
	void UpdateParameters();
};
