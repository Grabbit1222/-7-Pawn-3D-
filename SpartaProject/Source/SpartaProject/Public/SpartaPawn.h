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

	// �Է� ���ε��� ó���� �Լ�
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


	// �̵� �ӵ� ���� ������Ƽ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float NormalSpeed; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeedMultiplier;  // "�⺻ �ӵ�" ��� �� ��� ������ �޸��� ����
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
	FRotator TargetRotation = FRotator::ZeroRotator; // ���� ���¸� �����ϴ� ����
	FVector CurrentVelocity = FVector::ZeroVector; // ���� �ӵ��� �����ϴ� ����
	float CurrentSpeed;
	

	bool bIsFlying;

	float TraceLength = 35.0f;
	void ApplyGravity(float DeltaTime);
	bool IsOnFoot();
};
