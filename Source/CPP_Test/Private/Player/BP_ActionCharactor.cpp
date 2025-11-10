// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BP_ActionCharactor.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
ABP_ActionCharactor::ABP_ActionCharactor()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 350.0f;
	SpringArm->SocketOffset = FVector(0, 0, 250);
	SpringArm->bUsePawnControlRotation = true; //스프링암의 회전을 컨트롤러에 맞춤

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(SpringArm);
	PlayerCamera->SetRelativeRotation(FRotator(-20.0f, 0.0f, 0.0f));
	

	bUseControllerRotationYaw = true; //컨트롤러의 Yaw회전을 사용안함

	GetCharacterMovement()->bOrientRotationToMovement = true; //이동 방향을 바라보게 회전
	GetCharacterMovement()->RotationRate = FRotator(0, 360, 0);
}

// Called when the game starts or when spawned
void ABP_ActionCharactor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABP_ActionCharactor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABP_ActionCharactor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* enhanced = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (enhanced)	// 입력 컴포넌트가 향상된 입력 컴포넌트일 때
	{
		enhanced->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ABP_ActionCharactor::OnMoveInput);
	}

}

void ABP_ActionCharactor::OnMoveInput(const FInputActionValue& InValue)
{
FVector2D inputDirection = InValue.Get<FVector2D>();
//UE_LOG(LogTemp, Log, TEXT("Dir : (%.1f, %.1f)"), inputDirection.X, inputDirection.Y);
//UE_LOG(LogTemp, Log, TEXT("Dir : (%s)"), *inputDirection.ToString());

// 컨트롤러가 있는지, 그리고 입력값이 있는지 확인합니다.
if (Controller && (inputDirection.X != 0.0f || inputDirection.Y != 0.0f))
{
	// 1. 컨트롤러의 회전값(카메라가 바라보는 방향
	const FRotator ControlRotation = Controller->GetControlRotation();

	// 2. 우리는 수평(좌우) 회전값(Yaw)
	const FRotator YawRotation(0, ControlRotation.Yaw, 0);

	// 3. 컨트롤러의 '앞쪽' 방향 벡터
	// EAxis::X는 '앞쪽'을 의미
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	// 4. 컨트롤러의 '오른쪽' 방향 벡터
	// EAxis::Y는 '오른쪽'을 의미
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// 5. 앞/뒤 입력(W, S 키)에 '앞쪽' 방향을 적용
	AddMovementInput(ForwardDirection, inputDirection.Y);

	// 6. 좌/우 입력(A, D 키)에 '오른쪽' 방향을 적용
	AddMovementInput(RightDirection, inputDirection.X);
}
}

