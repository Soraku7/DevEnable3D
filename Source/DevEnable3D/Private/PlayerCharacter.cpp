// Fill out your copyright notice in the Description page of Project Settings.


#include "DevEnable3D/Public/PlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
	bUseControllerRotationYaw = false;
	JumpMaxHoldTime = 0.25f;
	JumpMaxCount = 2;
	
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh() -> SetRelativeLocation(FVector(0.0f , 0.0f , GetCapsuleComponent() -> GetScaledCapsuleHalfHeight()));

	MaskMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Mask Mesh");
	MaskMeshComponent -> SetupAttachment(GetMesh() , FName("head_001Socket"));

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("Camera Boom");
	SpringArmComponent -> SetupAttachment(RootComponent);
	SpringArmComponent -> bUsePawnControlRotation = true;
	SpringArmComponent -> TargetArmLength = 600.0f;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera");
	CameraComponent -> SetupAttachment(SpringArmComponent);

	GetCharacterMovement() ->bOrientRotationToMovement = true;
	GetCharacterMovement() -> GravityScale = 4.0f;
	GetCharacterMovement() -> JumpZVelocity = 1000.0f;
	GetCharacterMovement() -> AirControl = 1.0f;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	DefaultMovementSpeed = GetCharacterMovement() -> MaxWalkSpeed;
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APlayerCharacter::MoveForward(const float InputValue)
{
	AddMovementInput(GetMovementDirection(FVector::ForwardVector) , InputValue);
}

void APlayerCharacter::MoveRight(const float InputValue)
{
	AddMovementInput(GetMovementDirection(FVector::RightVector) , InputValue);
}

void APlayerCharacter::Sprint()
{
	GetCharacterMovement() -> MaxWalkSpeed = DefaultMovementSpeed * SprintSpeedMultiplier;
}

void APlayerCharacter::Walk()
{
	GetCharacterMovement() -> MaxWalkSpeed = DefaultMovementSpeed * WalkSpeedMultiplier;
}

void APlayerCharacter::DoJump()
{
	Jump();
	IsJumping = true;
}

void APlayerCharacter::ResetMoveSpeed()
{
	GetCharacterMovement() -> MaxWalkSpeed = DefaultMovementSpeed;
}

FVector APlayerCharacter::GetMovementDirection(const FVector& InVector) const
{
	return FRotator(0.0f , GetControlRotation().Yaw , 0.0f).RotateVector(InVector);
}
