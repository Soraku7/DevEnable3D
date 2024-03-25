﻿// Fill out your copyright notice in the Description page of Project Settings.

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
	
public:
	FORCEINLINE void SetIsJumping(const bool InValue) {IsJump = InValue;} 

private:
	UPROPERTY(BlueprintReadOnly , Category = "Parameters" , meta = (AllowPrivateAccess = "true"))
	bool IsJump;
	
	UPROPERTY(BlueprintReadOnly , Category = "Parameters" , meta = (AllowPrivateAccess = "true"))
	bool IsInAir;
	
	UPROPERTY(BlueprintReadOnly , Category = "Parameters" , meta = (AllowPrivateAccess = "true"))
	float MoveSpeed;
	
	APlayerCharacter* PlayerCharacter;
	
	UFUNCTION(BlueprintCallable)
	void UpdateParameters();
};
