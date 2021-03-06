// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JoyConInformation.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "JoyConDriverFunctionLibrary.generated.h"

/**
 *
 */
UCLASS()
class JOYCONDRIVER_API UJoyConDriverFunctionLibrary : public UBlueprintFunctionLibrary {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "JoyCon", meta = (Keywords = "Nintendo Switch Joy Con Cons JoyCon JoyCons Attach"))
		static void AttachJoyCon(FJoyConInformation JoyConInformation, bool& Success, int& ControllerIndex);

	UFUNCTION(BlueprintPure, Category = "JoyCon", meta = (Keywords = "Nintendo Switch Joy Con Cons JoyCon JoyCons Search"))
		static void SearchForJoyCons(TArray<FJoyConInformation>& JoyCons);

	UFUNCTION(BlueprintPure, Category = "JoyCon", meta = (Keywords = "Nintendo Switch Joy Con Cons JoyCon JoyCons Detach"))
		static void DetachJoyCon(int ControllerIndex, bool& Success);

	UFUNCTION(BlueprintPure, Category = "JoyCon", meta = (Keywords = "Nintendo Switch Joy Con Cons JoyCon JoyCons Accelerometer"))
		static void GetJoyConAccelerometer(int ControllerIndex, bool& Success, FVector& Accelerometer);

	UFUNCTION(BlueprintPure, Category = "JoyCon", meta = (Keywords = "Nintendo Switch Joy Con Cons JoyCon JoyCons Gyroscope"))
		static void GetJoyConGyroscope(int ControllerIndex, bool& Success, FVector& Gyroscope);

	UFUNCTION(BlueprintPure, Category = "JoyCon", meta = (Keywords = "Nintendo Switch Joy Con Cons JoyCon JoyCons Gyroscope"))
		static void GetJoyConVector(int ControllerIndex, bool& Success, FRotator& Vector);
};
