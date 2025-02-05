# [7번 과제] Pawn 클래스로 3D 캐릭터 만들기

## 📌 프로젝트 개요
언리얼 엔진의 `Pawn` 클래스를 활용하여 직접 이동 로직을 구현하는 프로젝트입니다.

## 🎮 주요 기능
- `Pawn` 기반 3D 캐릭터 이동 및 회전 구현 (WASD + 마우스 입력)
- Enhanced Input을 활용한 키보드 및 마우스 입력 처리
- `SpringArm` 및 `CameraComponent`를 이용한 3인칭 카메라 설정
- `AddActorLocalOffset()`, `AddActorLocalRotation()`을 사용한 직접 이동 로직 구현

## 🛠 구현 내용
### ✅ 1. C++ Pawn 클래스 생성
- 충돌 컴포넌트 (`CapsuleComponent`)를 루트로 설정
- SkeletalMesh, SpringArm, CameraComponent 추가
- `GameMode`에서 `DefaultPawnClass`로 설정

### ✅ 2. Enhanced Input 설정
- Move (WASD), Look (마우스) 액션 생성 및 매핑
- `SetupPlayerInputComponent()`에서 입력 바인딩

### ✅ 3. 이동 및 회전 로직 구현
- `AddActorLocalOffset()`을 사용한 이동
- `AddActorLocalRotation()`을 사용한 회전

## 🚀 도전 과제
- **6 DOF 비행체 이동**: X/Y/Z 이동 및 Yaw/Pitch/Roll 회전 추가
- **중력 및 낙하 구현**: `CharacterMovementComponent` 없이 중력 적용 및 착지 로직 구현

## 📺 시연 영상
(https://youtu.be/W7yZX0KHs9M?si=a9dER4iP3Ta--56G)
