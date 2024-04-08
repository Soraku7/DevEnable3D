// Fill out your copyright notice in the Description page of Project Settings.


#include "DevEnable3D/Public/PlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"


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
	DefaultMovementSpeed = GetCharacterMovement() -> MaxWalkSpeed;
	DefaultGravityScale = GetCharacterMovement() -> GravityScale;
	
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

	WallSlideCheck();

	if(IsWallSliding)
	{
		WallSliding(DeltaTime);
	}
	else
	{
		GetCharacterMovement() -> GravityScale = DefaultGravityScale;
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

bool APlayerCharacter::GetIsFalling()
{
	if(GetCharacterMovement() -> IsFalling() && GetVelocity().Z < 0.0f)
	{
		IsAirJumping = false;

		return true;
	}
	
	return false;
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
	HasZVelocityReset = false;

	GetCharacterMovement() -> GravityScale = DefaultGravityScale;
	
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

void APlayerCharacter::ResetZVelocity(const bool DoOnce)
{
	if(HasZVelocityReset) return;
	
	GetCharacterMovement() -> Velocity = FVector(GetVelocity().X , GetVelocity().Y , 0.0f);
	HasZVelocityReset = DoOnce ? true : false;
}

void APlayerCharacter::WallSliding(const float DeltaTime)
{

	IsWallJumping = false;
	
	const FVector TargetVelocity = FVector(0.0 , 0.0f , GetVelocity().Z);
	const FVector NewVelocity = FMath::VInterpTo(GetVelocity() , TargetVelocity , DeltaTime , WallSlideDeceleration);

	GetCharacterMovement() -> Velocity = NewVelocity;

	const FRotator WallHitRotation = UKismetMathLibrary::MakeRotFromX(WallCheckResult.Normal);
	const FRotator NewRotation = FRotator(0.0f , WallHitRotation.Yaw + 180, 0.0f);
	
	SetActorRotation(NewRotation);
		
	if(GetVelocity().Z >= 0) return;

	ResetZVelocity();
	GetCharacterMovement() -> GravityScale = WallSlideGravityScale;
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
	const FVector NewVelocity = GetVelocity() + FVector(0.0f , 0.0f , AirJumpForce);
	
	LaunchCharacter(NewVelocity , true , true);
	
	IsAirJumping = true;
	IsJumping = false;
}

void APlayerCharacter::GroundJump()
{
	Jump();
	IsJumping = true;
}

void APlayerCharacter::WallJump()
{
	JumpCount -= 1;
	
	const FVector ForwardVelocity = GetActorForwardVector() * WallJumpVelocity.X * -1.0f;
	const FVector UpwardVelocity = WallJumpVelocity.Z * FVector::UpVector;
	const FVector NewVelocity = ForwardVelocity + UpwardVelocity;
	LaunchCharacter(NewVelocity , true , true);

	FRotator NewRotation = NewVelocity.Rotation();
	NewRotation.Roll = 0.0f;
	NewRotation.Pitch = 0.0f;

	SetActorRotation(NewRotation);
	
	IsWallJumping = true;
}

void APlayerCharacter::DoJump()
{
	if(JumpCount >= JumpMaxCount) return;

	JumpCount++;
	UE_LOG(LogTemp , Log , TEXT("Jump Count %d MaxCount = %d") , JumpCount , JumpMaxCount);
	GetCharacterMovement() -> GravityScale = DefaultGravityScale;

	if(IsWallSliding)
	{
		UE_LOG(LogTemp , Log , TEXT("蹬墙跳"));
		WallJump();
		return;
	}
	
	if(GetMovementComponent() -> IsFalling())
	{
		AirJump();
		return;
	}
	
	if(GetMovementComponent() -> IsMovingOnGround())
	{
		GroundJump();
	
		return;
	}
	GroundJump();
	//
	// if(JumpCount < JumpMaxCount)
	// {
	// 	AirJump();
	// }
	
}

void APlayerCharacter::ResetMoveSpeed()
{
	GetCharacterMovement() -> MaxWalkSpeed = DefaultMovementSpeed;
}

FVector APlayerCharacter::GetMovementDirection(const FVector& InVector) const
{
	return FRotator(0.0f , GetControlRotation().Yaw , 0.0f).RotateVector(InVector);
}

void APlayerCharacter::WallSlideCheck()
{
	if(GetCharacterMovement() -> IsMovingOnGround()) return;
	
	const bool HitWall = GetWorld()->SweepSingleByChannel(WallCheckResult, GetActorLocation(), GetActorLocation(),
	                                 GetActorRotation().Quaternion(), ECC_Visibility, WallCheckCapsule, QueryParams);

	DrawDebugCapsule(GetWorld(), GetActorLocation(), WallCheckCapsule.GetCapsuleHalfHeight(),
	                 WallCheckCapsule.GetCapsuleRadius(), GetActorRotation().Quaternion(), FColor::Red, false, 5.0f);

	IsWallSliding = HitWall && WallCheckResult.GetActor() -> ActorHasTag(WallCheckTag);
}
