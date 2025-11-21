// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BP_ActionCharactor.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "Player/ResourceComponent.h"
#include "Player/StatusComponent.h"
#include "Player/WeaponManagerComponent.h"
#include "Weapon/WeaponActor.h"
#include "Weapon/UsedWeapon.h"
#include "Weapon/ConsumableWeapon.h"
#include "Item/Pickupable.h"
#include "Item/Pickup.h"

// Sets default values
ABP_ActionCharactor::ABP_ActionCharactor()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 350.0f;
	SpringArm->SocketOffset = FVector(0, 0, 250);
	SpringArm->bUsePawnControlRotation = true;	// 스프링암의 회전을 컨트롤러에 맞춤

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(SpringArm);
	PlayerCamera->SetRelativeRotation(FRotator(-20.0f, 0.0f, 0.0f));

	DropLocation = CreateDefaultSubobject<USceneComponent>(TEXT("DropLocation"));
	DropLocation->SetupAttachment(RootComponent);
	DropLocation->SetRelativeLocation(FVector(80.0f, 30.0f, 0.0f));

	Resource = CreateDefaultSubobject<UResourceComponent>(TEXT("PlayerResource"));
	Status = CreateDefaultSubobject<UStatusComponent>(TEXT("PlayerStatus"));
	WeaponManager = CreateDefaultSubobject<UWeaponManagerComponent>(TEXT("WeaponManager"));

	bUseControllerRotationYaw = false;	// 컨트롤러의 Yaw 회전 사용 안함
	GetCharacterMovement()->bOrientRotationToMovement = true;	// 이동 방향으로 캐릭터 회전
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

void ABP_ActionCharactor::AddItem_Implementation(EItemCode Code, int32 Count)
{
	//const UEnum* EnumPtr = StaticEnum<EItemCode>();
	//UE_LOG(LogTemp, Log, TEXT("아이템 추가 : %s"), *EnumPtr->GetDisplayNameTextByValue(static_cast<int8>(Code)).ToString());

	EquipWeapon(Code);
	CurrentWeapon->OnWeaponPickuped(Count);
}

void ABP_ActionCharactor::EquipWeapon(EItemCode WeaponCode)
{
	if (CurrentWeapon.IsValid())
	{
		// 장비하고 있던 무기가 기본 무기가 아니면
		if (CurrentWeapon->GetWeaponID() != EItemCode::BasicWeapon	// 장비하고 있던 무기가 Consumable이고
			&& CurrentWeapon->GetWeaponID() != WeaponCode			// 새로 장비할 무기와 다른 종류고
			&& CurrentWeapon->CanAttack())							// 장비하고 있던 무기에 회수가 남아있는 상황이면
		{
			DropCurrentWeapon(CurrentWeapon->GetWeaponID());
		}

		// 장비하고 있던 무기는 해제
		CurrentWeapon->WeaponActivate(false);
	}

	// WeaponCode에 해당하는 무기 장비
	CurrentWeapon = WeaponManager->GetEquippedWeapon(WeaponCode);
	CurrentWeapon->WeaponActivate(true);
}

void ABP_ActionCharactor::OnAttackEnable(bool bEnable)
{
	if (CurrentWeapon.IsValid())
	{
		CurrentWeapon->AttackEnable(bEnable);
	}
}

void ABP_ActionCharactor::OnWeaponTrailEnable(bool bEnable)
{
	if (CurrentWeapon.IsValid())
	{
		CurrentWeapon->TrailEnable(bEnable);
	}
}

void ABP_ActionCharactor::OnAreaAttack()
{
	//UE_LOG(LogTemp, Log, TEXT("OnAreaAttack"));
	if (CurrentWeapon.IsValid())
	{
		//UE_LOG(LogTemp, Log, TEXT("OnAreaAttack : CurrentWeapon.IsValid()") );
		CurrentWeapon->DamageToArea();
	}
}

void ABP_ActionCharactor::TestDropUsedWeapon()
{
	if (CurrentWeapon.IsValid())
	{
		DropWeapon(CurrentWeapon->GetWeaponID());
	}
}

void ABP_ActionCharactor::TestDropCurrentWeapon()
{
	UE_LOG(LogTemp, Log, TEXT("TestDropCurrentWeapon"));
	DropCurrentWeapon(CurrentWeapon->GetWeaponID());
}

void ABP_ActionCharactor::OnMoveInput(const FInputActionValue& InValue)
{
	FVector2D inputDirection = InValue.Get<FVector2D>();
	//UE_LOG(LogTemp, Log, TEXT("Dir : (%.1f, %.1f)"), inputDirection.X, inputDirection.Y);
	//UE_LOG(LogTemp, Log, TEXT("Dir : (%s)"), *inputDirection.ToString());
	FVector moveDirection(inputDirection.Y, inputDirection.X, 0.0f);

	FQuat controlYawRotation = FQuat(FRotator(0, GetControlRotation().Yaw, 0));	// 컨트롤러의 Yaw회전을 따로 뽑아와서
	moveDirection = controlYawRotation.RotateVector(moveDirection);	// 이동 방향에 적용

	AddMovementInput(moveDirection);

}

void ABP_ActionCharactor::OnRollInput(const FInputActionValue& InValue)
{
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
	// 애님 인스턴스가 있고, 스태미너도 충분하고, 현재 무기가 공격을 할 수 있어야 한다.
	if (AnimInstance.IsValid() && Resource->HasEnoughStamina(AttackStaminaCost)
		&& (CurrentWeapon.IsValid() && CurrentWeapon->CanAttack()))
	{
		if (!AnimInstance->IsAnyMontagePlaying())	// 몽타주가 재생 중이 아닐 때
		{
			// 첫번째 공격			
			PlayAnimMontage(AttackMontage);	// 몽타주 재생

			FOnMontageEnded onMontageEnded;
			onMontageEnded.BindUObject(this, &ABP_ActionCharactor::OnAttackMontageEnded);
			AnimInstance->Montage_SetEndDelegate(onMontageEnded);	// 몽타주가 끝났을 때 델리게이트 발송(몽타주 플레이 이후에 등록해야 함)

			Resource->AddStamina(-AttackStaminaCost);	// 스태미너 감소
			if (CurrentWeapon.IsValid())
			{
				CurrentWeapon->OnAttack();	// 무기 공격시 처리(회수 차감)
			}
		}
		else if (AnimInstance->GetCurrentActiveMontage() == AttackMontage)	// 몽타주가 재생 중인데, AttackMontage가 재생중이면
		{
			// 콤보 공격
			SectionJumpForCombo();
		}
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
	//UE_LOG(LogTemp, Log, TEXT("Char overlap : other is %s"), *OtherActor->GetName());

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
		IPickupable::Execute_OnPickup(OtherActor, this);	// 구현이 되어 있으면 실행
	}
}

void ABP_ActionCharactor::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	// UE_LOG(LogTemp, Log, TEXT("공격 몽타주가 끝남"));
	if (CurrentWeapon.IsValid() && !CurrentWeapon->CanAttack())	// CurrentWeapon이 공격할 수 없으면(=사용회수가 안남았다)
	{
		//DropUsedWeapon();		
		DropWeapon(CurrentWeapon->GetWeaponID());	// 현재 사용 중인 무기 버리기
		EquipWeapon(EItemCode::BasicWeapon);
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
		if (CurrentWeapon.IsValid())
		{
			CurrentWeapon->OnAttack();
		}
	}
}

void ABP_ActionCharactor::SpendRunStamina(float DeltaTime)
{
	if ((bIsSprint && !GetVelocity().IsNearlyZero())							// 달리기 상태이고 움직이지 않고 있다.			
		&& (AnimInstance.IsValid() && !AnimInstance->IsAnyMontagePlaying()))	// 어떤 몽타쥬도 재생중이지 않다.(루트모션 때문에 Velocity 변경있음)
	{
		Resource->AddStamina(-SprintStaminaCost * DeltaTime);	// 스태미너 감소
		//UE_LOG(LogTemp, Log, TEXT("Velocity : %s"), *GetVelocity().ToString());
		//UE_LOG(LogTemp, Warning, TEXT("Stamina : %.1f"), Resource->GetCurrentStamina());
	}

	//GetWorld()->GetFirstPlayerController()->GetHUD();
}

void ABP_ActionCharactor::DropWeapon(EItemCode WeaponCode)
{
	UE_LOG(LogTemp, Log, TEXT("다쓴 무기 버리기"));
	if (TSubclassOf<AUsedWeapon> usedClass = WeaponManager->GetUsedWeaponClass(WeaponCode))
	{
		GetWorld()->SpawnActor<AActor>(
			usedClass,
			DropLocation->GetComponentLocation(),
			GetActorRotation());
	}
}

void ABP_ActionCharactor::DropCurrentWeapon(EItemCode WeaponCode)
{
	if (CurrentWeapon.IsValid() && CurrentWeapon->GetWeaponID() != EItemCode::BasicWeapon)
	{
		if (TSubclassOf<APickup> pickupClass = WeaponManager->GetPickupWeaponClass(WeaponCode))
		{
			APickup* pickup = GetWorld()->SpawnActor<APickup>(
				pickupClass,
				DropLocation->GetComponentLocation(),
				GetActorRotation()
			);

			// 새로 생긴 픽업에 남은 회수 넣기
			AConsumableWeapon* conWeapon = Cast<AConsumableWeapon>(CurrentWeapon);
			pickup->SetPickupCount(conWeapon->GetRemainingUseCount());

			FVector velocity = (GetActorForwardVector() + GetActorUpVector()) * 300.0f;
			pickup->AddImpulse(velocity);
		}
	}
}

