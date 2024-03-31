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

void APlayerCharacter::InitialProperties()
{
	WallCheckCapsule = FCollisionShape::MakeCapsule(
		GetCapsuleComponent()->GetScaledCapsuleRadius() + 1.0f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight());

	QueryParams.AddIgnoredActor(this);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	DefaultMovementSpeed = GetCharacterMovement() -> MaxWalkSpeed;

	InitialProperties();
	
	LandedDelegate.AddDynamic(this , &APlayerCharacter::OnCharacterLanded);
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	IsTouchingWall();
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

bool APlayerCharacter::GetIsFalling()
{
	IsAirJumping = false;
	
	return GetCharacterMovement() -> IsFalling() && GetVelocity().Z < 0.0f;
}

bool APlayerCharacter::GetIsGround() const
{
	return GetCharacterMovement() -> IsMovingOnGround();
}

void APlayerCharacter::OnCharacterLanded(const FHitResult& Hit)
{ 
	IsJumping = false;
	IsAirJumping = false;
	IsWallSliding = false;
	JumpCount = 0;
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

void APlayerCharacter::AirJump()
{
	Jump();
	IsAirJumping = true;
	IsJumping = false;
	JumpCount ++;
}

void APlayerCharacter::GroundJump()
{
	Jump();
	IsJumping = true;
	JumpCount++;
}

void APlayerCharacter::DoJump()
{
	if(GetMovementComponent() -> IsMovingOnGround())
	{
		GroundJump();

		return;
	}

	if(JumpCount < JumpMaxCount)
	{
		AirJump();
	}
	
}

void APlayerCharacter::ResetMoveSpeed()
{
	GetCharacterMovement() -> MaxWalkSpeed = DefaultMovementSpeed;
}

FVector APlayerCharacter::GetMovementDirection(const FVector& InVector) const
{
	return FRotator(0.0f , GetControlRotation().Yaw , 0.0f).RotateVector(InVector);
}

void APlayerCharacter::IsTouchingWall()
{
	if(GetCharacterMovement() -> IsMovingOnGround()) return;
	
	const bool HitWall = GetWorld()->SweepSingleByChannel(WallCheckResult, GetActorLocation(), GetActorLocation(),
	                                 GetActorRotation().Quaternion(), ECC_Visibility, WallCheckCapsule, QueryParams);

	DrawDebugCapsule(GetWorld(), GetActorLocation(), WallCheckCapsule.GetCapsuleHalfHeight(),
	                 WallCheckCapsule.GetCapsuleRadius(), GetActorRotation().Quaternion(), FColor::Red, false, 5.0f);

	IsWallSliding = HitWall && WallCheckResult.GetActor() -> ActorHasTag(WallCheckTag);
}
