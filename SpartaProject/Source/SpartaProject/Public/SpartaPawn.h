#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SpartaPawn.generated.h"

class UBoxComponent;

class USpringArmComponent;
class UCameraComponent;

struct FInputActionValue;


UCLASS()
class SPARTAPROJECT_API ASpartaPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASpartaPawn();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* BoxComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* SkeletalMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;

	// 입력 바인딩을 처리할 함수
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void Ascend(const FInputActionValue& value);
	UFUNCTION()
	void Descend(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);
	UFUNCTION()
	void StartSprint(const FInputActionValue& value);
	UFUNCTION()
	void StopSprint(const FInputActionValue& value);


	// 이동 속도 관련 프로퍼티
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float NormalSpeed; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeedMultiplier;  // "기본 속도" 대비 몇 배로 빠르게 달릴지 결정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeed; 	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Force")
	float Gravity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Force")
	float DecelerationFactor;

	virtual void BeginPlay() override;
	void Tick(float DeltaTime);

private:
	void MoveForward(float value, float speed);
	void MoveRight(float value, float speed);
	void ApplyMovement(float DeltaTime);
	void UpdateRotation(float DeltaTime);
	FRotator TargetRotation = FRotator::ZeroRotator; // 기울기 상태를 저장하는 변수
	FVector CurrentVelocity = FVector::ZeroVector; // 현재 속도를 저장하는 변수
	float CurrentSpeed;
	

	bool bIsFlying;

	float TraceLength = 35.0f;
	void ApplyGravity(float DeltaTime);
	bool IsOnFoot();
};
