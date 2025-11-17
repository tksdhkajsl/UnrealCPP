// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BP_ActionCharactor.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Player/ResourceComponent.h"
#include "Player/StatusComponent.h"
#include "Weapon/WeaponActor.h"
#include "Item/Pickupable.h"

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


	Resource = CreateDefaultSubobject<UResourceComponent>(TEXT("PlayerResource"));
	Status = CreateDefaultSubobject<UStatusComponent>(TEXT("PlayerStatus"));

	bUseControllerRotationYaw = false; //컨트롤러의 Yaw회전
	GetCharacterMovement()->bOrientRotationToMovement = true; //이동 방향을 바라보게 회전
	GetCharacterMovement()->RotationRate = FRotator(0, 360, 0);

	
}

// Called when the game starts or when spawned
void ABP_ActionCharactor::BeginPlay()
{
	if (Resource)
	{
		Resource->OnStaminaEmpty.AddDynamic(this, &ABP_ActionCharactor::SetWalkMode);
		if (Status)
		{
			Resource->SetMaxHealth(Status->GetMaxHealth());
			Resource->SetMaxStamina(Status->GetMaxStamina());			
		}
	}

	Super::BeginPlay();
	if (GetMesh())
	{
		AnimInstance = GetMesh()->GetAnimInstance();	// ABP 객체 가져오기
	}
	// 게임 진행 중에 자주 변경되는 값은 시작 시점에서 리셋을 해주는 것이 좋다.
	bIsSprint = false;

	//캐릭터이 다른 엑터가 오버랩되었을 때 실행하기 위한 바인딩
	OnActorBeginOverlap.AddDynamic(this, &ABP_ActionCharactor::OnBeginOverlap);
}

// Called every frame
void ABP_ActionCharactor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	SpendRunStamina(DeltaTime);
}

// Called to bind functionality to input
void ABP_ActionCharactor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* enhanced = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (enhanced)	// 입력 컴포넌트가 향상된 입력 컴포넌트일 때
	{
		enhanced->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ABP_ActionCharactor::OnMoveInput);
		enhanced->BindActionValueLambda(IA_Sprint, ETriggerEvent::Started,
			[this](const FInputActionValue& _) {
				SetSprintMode();
			});
		enhanced->BindActionValueLambda(IA_Sprint, ETriggerEvent::Completed,
			[this](const FInputActionValue& _) {
				SetWalkMode();
			});
		enhanced->BindAction(IA_Roll, ETriggerEvent::Triggered, this, &ABP_ActionCharactor::OnRollInput);
		enhanced->BindAction(IA_Attack, ETriggerEvent::Triggered, this, &ABP_ActionCharactor::OnAttackInput);
	}

}

void ABP_ActionCharactor::AddItem_Implementation(EItemCode Code)
{
	const UEnum* EnumPtr = StaticEnum<EItemCode>();
	UE_LOG(LogTemp, Log, TEXT("아이템 추가 : %s"), *EnumPtr->GetDisplayNameTextByValue(static_cast<int8>(Code)).ToString());
}

void ABP_ActionCharactor::OnAttackEnable(bool bEnable)
{
	if (CurrentWeapon.IsValid())
	{
		CurrentWeapon->AttackEnable(bEnable);
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
	if (AnimInstance.IsValid())
	{
		if (!AnimInstance->IsAnyMontagePlaying()
			&& Resource->HasEnoughStamina(RollStaminaCost))	// 몽타주 재생중이 아니고 충분한 스태미너가 있을 때만 작동
		{
			//if (!GetLastMovementInputVector().IsNearlyZero())	// 입력을 하는 중에만 즉시 회전
			//{
			//	SetActorRotation(GetLastMovementInputVector().Rotation());	// 마지막 입력 방향으로 즉시 회전 시키기
			//}
			Resource->AddStamina(-RollStaminaCost);	// 스태미너 감소
			PlayAnimMontage(RollMontage);
		}
	}
	
}

void ABP_ActionCharactor::OnAttackInput(const FInputActionValue& InValue)
{
	if (AnimInstance.IsValid() && Resource->HasEnoughStamina(AttackStaminaCost)) // 애님 인스턴스가 있고 스태미너도 충분할 때
	{
		if (!AnimInstance->IsAnyMontagePlaying())	// 몽타주가 재생 중이 아닐 때
		{
			// 첫번째 공격
			PlayAnimMontage(AttackMontage);
			Resource->AddStamina(-AttackStaminaCost);	// 스태미너 감소
		}
		else if (AnimInstance->GetCurrentActiveMontage() == AttackMontage)	// 몽타주가 재생 중인데, AttackMontage가 재생중이면
		{
			// 콤보 공격
			SectionJumpForCombo();
		}
	}
}

void ABP_ActionCharactor::SectionJumpForCombo()
{
	if (SectionJumpNotify.IsValid() && bComboReady)	// SectionJumpNotify가 있고 콤보가 가능한 상태이면
	{
		UAnimMontage* current = AnimInstance->GetCurrentActiveMontage();
		AnimInstance->Montage_SetNextSection(					// 다음 섹션으로 점프하기
			AnimInstance->Montage_GetCurrentSection(current),		// 현재 섹션
			SectionJumpNotify->GetNextSectionName(),				// 다음 섹션의 이름
			current);												// 실행될 몽타주

		bComboReady = false;	// 중복실행 방지
		Resource->AddStamina(-AttackStaminaCost);	// 스태미너 감소
	}
}

void ABP_ActionCharactor::SpendRunStamina(float DeltaTime)
{
	//*GetVelocity().ToString() 해당 값을 str로 넘겨준다.
	if ((bIsSprint && !GetVelocity().IsNearlyZero())  //달리기 상태고 //움직임 없고
		&& (AnimInstance.IsValid() && !AnimInstance->IsAnyMontagePlaying())) //어떤 몽타주도 재생중이지 않다.(루트모션 때문에 velocity 변경있음)
	{
		Resource->AddStamina(-SprintStaminaCost * DeltaTime);	// 스태미너 감소
	}
	
}

void ABP_ActionCharactor::SetSprintMode()
{
	//UE_LOG(LogTemp, Warning, TEXT("달리기 모드"));
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	bIsSprint = true;
	
	
}

void ABP_ActionCharactor::SetWalkMode()
{
	//UE_LOG(LogTemp, Warning, TEXT("걷기 모드"));
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	bIsSprint = false;
}

void ABP_ActionCharactor::OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	UE_LOG(LogTemp, Log, TEXT("Char overlap : other is %s"), *OtherActor->GetName());

	// Cast를 이용한 인터페이스 사용
	//IPickupable* test = Cast<IPickupable>(OtherActor);
	//if (test)
	//{
	//	IPickupable::Execute_OnPickup(OtherActor);	// 만약에 블루프린트 구현이 있을 경우. 블루프린트의 구현이 실행된다.
	//	//test->OnPickup_Implementation();	// 블루프린트 구현은 무시
	//}

	// Implements를 이용한 인터페이스 사용
	if (OtherActor->Implements<UPickupable>())	// OtherActor가 IPickable인터페이스를 구현했는지 확인
	{
		IPickupable::Execute_OnPickup(OtherActor,this);	// 구현이 되어 있으면 실행
	}
}



