#pragma once

#include "CoreMinimal.h"

/**
 *
 */
UENUM(BlueprintType)
enum class EItemCode : uint8
{
	BasicWeapon		UMETA(DisplayName = "기본무기(도끼)"),
	Sword			UMETA(DisplayName = "대검"),
	Hammer			UMETA(DisplayName = "망치"),
};

// 비트플래그용 enum. 에디터에서 편집 가능
UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EBitFlag : uint8
{
	None = 0,
	Test1 = 1 << 0,
	Test2 = 1 << 1,
	Test3 = 1 << 2,
	Test4 = 1 << 3,
	Test5 = 1 << 4
};

// C++에서 Bitflag 변수 만들때
// UPROPERTY(EditAnywhere, meta = (Bitmask, BitmaskEnum = "EBitFlag"))
// int testBitFlag;

// 블루프린트에서 Bitflag 변수를 만들때 
// int로 변수를 만들고 bitflag를 체크한 후 enum타입을 설정