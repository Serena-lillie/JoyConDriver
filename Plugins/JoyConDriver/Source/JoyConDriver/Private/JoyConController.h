// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "hidapi.h"
#include "InputCoreTypes.h"
#include "JoyConInformation.h"
#include "Containers/Queue.h"
#include "GenericPlatform/GenericApplicationMessageHandler.h"
#include "HAL/Runnable.h"


enum EJoyConState {
	Not_Attached,
	Dropped,
	No_JoyCons,
	Attached,
	Input_Mode_0_X30,
	Imu_Data_OK,
};

enum EButton : int {
	JoyCon_DPad_Up = 0,
	JoyCon_DPad_Left = 1,
	JoyCon_DPad_Right = 2,
	JoyCon_DPad_Down = 3,
	
	JoyCon_Minus = 4,
	JoyCon_Plus = 5,
	JoyCon_Home = 6,
	JoyCon_Capture = 7,
	
	JoyCon_Analog_Click = 8,
	
	JoyCon_Sr = 9,
	JoyCon_Sl = 10,
	
	JoyCon_Shoulder_1 = 11,
	JoyCon_Shoulder_2 = 12,
};

struct FJoyConKey {
	static const FKey JoyCon_DPad_Up;
	static const FKey JoyCon_DPad_Left;
	static const FKey JoyCon_DPad_Right;
	static const FKey JoyCon_DPad_Down;

	static const FKey JoyCon_Minus;
	static const FKey JoyCon_Plus;
	static const FKey JoyCon_Home;
	static const FKey JoyCon_Capture;

	static const FKey JoyCon_Analog_Click;

	static const FKey JoyCon_Sr;
	static const FKey JoyCon_Sl;

	static const FKey JoyCon_Shoulder_1;
	static const FKey JoyCon_Shoulder_2;
};

class FJoyConButtonState {	
public:
	FGamepadKeyNames::Type KeyName;
	bool Pressed;
	bool Updated;
	bool Repeat;

	explicit FJoyConButtonState(const FName TempKeyName) {
		KeyName = TempKeyName;
		Pressed = false;
		Updated = false;
		Repeat = false;
	}

	void Update(const bool Result) {
		Updated = true;
		Pressed = Result;
	}

	void Reset() {
		Updated = false;
	}
};

struct FReport {
	uint8* ReportData;
	FDateTime Time;

	FReport() {
		ReportData = nullptr;
	}

	FReport(uint8_t* TempReportData, const FDateTime TempTime) {
		ReportData = TempReportData;
		Time = TempTime;
	}

	FDateTime GetTime() const {
		return Time;
	}

	void CopyBuffer(uint8* DestinationArray) const {
		for (auto i = 0; i < 49; ++i) {
			DestinationArray[i] = ReportData[i];
		}
	}
};

class FJoyConController : public FRunnable {

public:
	FJoyConController(FJoyConInformation TempJoyConInformation, hid_device* Device, const bool UseImu, const bool UseLocalize, float Alpha, const bool IsLeft);
	~FJoyConController();

	void Attach(uint8 Leds);
	void Update();
	void Pool();
	void Detach();

	/*bool GetButtonDown(EButton Button);
	bool GetButton(EButton Button);
	bool GetButtonUp(EButton Button);*/
	FVector2D GetStick();
	FVector GetGyroscope() const;
	FVector GetAccelerometer() const;
	FRotator GetVector() const;
	void ReCenter();
	
	void SetFilterCoefficient(float Coefficient);

private:
	void DumpCalibrationData();
	int32 ReceiveRaw();
	void ExtractImuValues(uint8 ReportBuf[], int32 N);
	int32 ProcessImu(uint8 ReportBuf[]);
	int32 ProcessButtonsAndStick(uint8 ReportBuf[]);
	void CenterSticks(uint16 Values[]);

	uint8* SendCommand(uint8 Sc, uint8 TempBuf[], uint8 Len);
	uint8* ReadSpi(uint8 Address1, uint8 Address2, uint32_t Len);

	static void ArrayCopy(uint8* SourceArray, int SourceIndex, uint8* DestinationArray, int DestinationIndex, int Length);
	static void ArrayCopy(const uint8* SourceArray, int SourceIndex, uint8* DestinationArray, int DestinationIndex, int Length);

private:
	hid_device* HidHandle;
	EJoyConState State;

	bool bStopPolling;
	bool bImuEnabled;
	bool bIsLeft;
	bool bDoLocalize;

	uint8 GlobalCount;
	uint32 ReadAttempts = 0;

	const uint32 ReportLen = 49;
	const uint8 DefaultBuf[8] = { 0x0, 0x1, 0x40, 0x40, 0x0, 0x1, 0x40, 0x40 };

	// Buttons
	FJoyConButtonState* Buttons;
	/*bool ButtonsDown[13];
	bool ButtonsUp[13];
	bool Buttons[13];
	bool Down[13];*/

	// Analog Stick Variables
	float Stick[2] = { 0, 0 };
	uint8 StickRaw[3] = { 0, 0, 0 };
	uint16 StickCalibration[6] = { 0, 0, 0, 0, 0, 0 };
	uint16 DeadZone;
	uint16 StickPreCalibration[2] = { 0, 0 };

	// Accelerometer and Gyroscope Variables
	int16 GyrNeutral[3] = { 0, 0, 0 };
	int16 GyrR[3] = { 0, 0, 0 };
	int16 AccR[3] = { 0, 0, 0 };
	FVector AccG;
	FVector GyrG;

	float Max[3] = { 0, 0, 0 };
	float Sum[3] = { 0, 0, 0 };
	int Timestamp;
	TQueue<FReport> Reports;
	uint8 TsDequeue;
	uint8 TsEnqueue;
	FDateTime TsPrevious;
	float FilterWeight;
	float Err;
	bool FirstImuPacket = true;
	FVector I_B;
	FVector J_B;
	FVector K_B;
	FVector K_Acc;
	FVector Wa;
	FVector Wg;
    FVector DTheta;
	FVector IB2;

	FRunnableThread* Thread;
	FCriticalSection Mutex;

public:
	FJoyConInformation JoyConInformation;

	// FRunnable interface overrides
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	
};