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


	bUseControllerRotationYaw = false; //컨트롤러의 Yaw회전
	GetCharacterMovement()->bOrientRotationToMovement = true; //이동 방향을 바라보게 회전
	GetCharacterMovement()->RotationRate = FRotator(0, 360, 0);

	CanRegenStamina = true;  // 처음에는 회복 가능 상태
	StaminaRegenDelay = 2.0f; // 2초 딜레이
	IsSprinting = false;
}

// Called when the game starts or when spawned
void ABP_ActionCharactor::BeginPlay()
{
	Super::BeginPlay();
	AnimInstance = GetMesh()->GetAnimInstance(); //ABP객체
}

// Called every frame
void ABP_ActionCharactor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsSprinting)
	{
		CurrentStamina -= SprintStamina * DeltaTime;
		UE_LOG(LogTemp, Warning, TEXT("스테미나 : %.1f"), CurrentStamina);
		if (CurrentStamina < 0.f)
		{
			CurrentStamina = 0.f;
			SetWalkingMode();
		}
	}
	else
	{		
		if (CanRegenStamina)
		{
			CurrentStamina += RecoveryStamina * DeltaTime;
			if (CurrentStamina > MaxStamina)
			{
				CurrentStamina = MaxStamina;
			}
			UE_LOG(LogTemp, Warning, TEXT("스테미나 : %.1f"), CurrentStamina);
		}
		// (bCanRegenStamina가 false인 동안은 '회복 대기' 중이므로 아무것도 하지 않습니다)
	}

}

// Called to bind functionality to input
void ABP_ActionCharactor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* enhanced = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (enhanced)	// 입력 컴포넌트가 향상된 입력 컴포넌트일 때
	{
		enhanced->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ABP_ActionCharactor::OnMoveInput);
		enhanced->BindActionValueLambda(IA_Sprint, ETriggerEvent::Started, [this](const FInputActionValue& _)
			{
				SetSprintMode();
			});
		enhanced->BindActionValueLambda(IA_Sprint, ETriggerEvent::Completed, [this](const FInputActionValue& _)
			{
				SetWalkingMode();
			});
		enhanced->BindAction(IA_Roll, ETriggerEvent::Triggered, this, &ABP_ActionCharactor::OnRollInput);
	}

}

void ABP_ActionCharactor::OnMoveInput(const FInputActionValue& InValue)
{
FVector2D inputDirection = InValue.Get<FVector2D>();
//UE_LOG(LogTemp, Log, TEXT("Dir : (%.1f, %.1f)"), inputDirection.X, inputDirection.Y);
//UE_LOG(LogTemp, Log, TEXT("Dir : (%s)"), *inputDirection.ToString());


FVector moveDirection(inputDirection.Y, inputDirection.X, 0.0f);

FQuat controlYawRotation = FQuat(FRotator(0, GetControlRotation().Yaw, 0)); //컨트롤러의 yaw회전을 따로 뽑아서
moveDirection = controlYawRotation.RotateVector(moveDirection); //이동방향에 적용

AddMovementInput(moveDirection);

//// 컨트롤러가 있는지, 그리고 입력값이 있는지 확인합니다.
//if (Controller && (inputDirection.X != 0.0f || inputDirection.Y != 0.0f))
//{
//	// 1. 컨트롤러의 회전값(카메라가 바라보는 방향)
//	const FRotator ControlRotation = Controller->GetControlRotation();
//
//	// 2. 우리는 수평(좌우) 회전값(Yaw)
//	const FRotator YawRotation(0, ControlRotation.Yaw, 0);
//
//	// 3. 컨트롤러의 '앞쪽' 방향 벡터
//	// EAxis::X는 '앞쪽'을 의미
//	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
//
//	// 4. 컨트롤러의 '오른쪽' 방향 벡터
//	// EAxis::Y는 '오른쪽'을 의미
//	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
//
//	// 5. 앞/뒤 입력(W, S 키)에 '앞쪽' 방향을 적용
//	AddMovementInput(ForwardDirection, inputDirection.Y);
//
//	// 6. 좌/우 입력(A, D 키)에 '오른쪽' 방향을 적용
//	AddMovementInput(RightDirection, inputDirection.X);
//}
}

void ABP_ActionCharactor::OnRollInput(const FInputActionValue& InValue)
{
	UE_LOG(LogTemp, Warning, TEXT("구르기 모드"));
	//if (AnimInstance.IsValid())
	//{
	//	if (!AnimInstance->IsAnyMontagePlaying())
	//	{
	//		//,ETeleportType 순간이동 관련 
	//		// 아래 0,0,0이 되서 마지막 방향 구르기되기 때문에 해당 조건문 넣어야 마지막에 바라본 방향으로 구름
	//		//if (!GetLastMovementInputVector().IsNearlyZero())
	//		//{
	//		//	SetActorRotation(GetLastMovementInputVector().Rotation());// 마지막 입력 방향으로 회전시키기
	//		//}
	//		PlayAnimMontage(RollMontage);			
	//		
	//	}		
	//}
	if (AnimInstance.IsValid() &&
		CurrentStamina >= RollStamina && // <<< [핵심] 스태미나가 충분한지 먼저 확인
		!AnimInstance->IsAnyMontagePlaying())
	{
		// 모든 조건 충족: 구르기 실행
		UE_LOG(LogTemp, Warning, TEXT("구르기 실행!"));	

		PlayAnimMontage(RollMontage);
		CurrentStamina -= RollStamina; // 스태미나 소모
		

		UE_LOG(LogTemp, Warning, TEXT("남은 스테미나 : %.1f"), CurrentStamina);
	}
	else
	{
		// 구르기에 실패한 로그
		if (!AnimInstance.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("구르기 실패: 애님 인스턴스가 유효하지 않습니다."));
		}
		else if (CurrentStamina < RollStamina)
		{
			
			UE_LOG(LogTemp, Warning, TEXT("구르기 실패: 스테미나가 모자랍니다!"));
		}
		else if (AnimInstance->IsAnyMontagePlaying())
		{
			UE_LOG(LogTemp, Warning, TEXT("구르기 실패: 다른 몽타주가 재생 중입니다."));
		}
	}
	
}



void ABP_ActionCharactor::SetSprintMode()
{
	//UE_LOG(LogTemp, Warning, TEXT("달리기 모드"));
	if (CurrentStamina > 0.f)
	{
		CanRegenStamina = false;
		IsSprinting = true;

		// [핵심] 만약 '회복 대기' 타이머가 돌고 있었다면(달리기를 멈춘 지 2초가 안 됐다면)
		// 그 타이머를 즉시 취소
		GetWorld()->GetTimerManager().ClearTimer(StaminaRegenTimerHandle);

		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
	
	
	
}

void ABP_ActionCharactor::SetWalkingMode()
{
	//UE_LOG(LogTemp, Warning, TEXT("걷기 모드"));
	IsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;	
	// 딜레이(StaminaRegenDelay) 이후에 OnStaminaRegenDelayFinished 함수를 "한 번만" 
	// 호출하도록 타이머를 설정
	GetWorld()->GetTimerManager().SetTimer(
		StaminaRegenTimerHandle,                   // 타이머 핸들
		this,                                      // 함수를 호출할 오브젝트
		&ABP_ActionCharactor::OnStaminaRegenDelayFinished, // 딜레이 후 호출될 함수
		StaminaRegenDelay,                         // 딜레이 시간 (예: 2.0초)
		false                                      // false: 반복 안 함 (한 번만 실행)
	);
}

void ABP_ActionCharactor::OnStaminaRegenDelayFinished()
{
	// 딜레이가 끝났으므로, 이제 스태미나 회복 가능 상태로 변경
	CanRegenStamina = true;
}

