// Fill out your copyright notice in the Description page of Project Settings.


#include "DevEnable3D/Public/PlayerCharacterAnimInstance.h"

#include "PlayerCharacter.h"

void UPlayerCharacterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	
	PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
}

void UPlayerCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	UpdateParameters();
}

void UPlayerCharacterAnimInstance::UpdateParameters()
{
	if(PlayerCharacter == nullptr) return;

	MoveSpeed = PlayerCharacter -> GetVelocity().Length();
	IsJump = PlayerCharacter -> GetIsJumping();
	IsAirJumping = PlayerCharacter -> GetIsAirJumping();
	IsFalling = PlayerCharacter -> GetIsFalling();
}
