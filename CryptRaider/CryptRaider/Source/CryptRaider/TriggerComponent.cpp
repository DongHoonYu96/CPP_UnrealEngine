// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"

UTriggerComponent::UTriggerComponent()
{
	//문제 : 컴포넌트는 틱 작동안하게 되어잇음 (성능문제)
	//해결 : tick 함수 활성화
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();


}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* Actor = GetAcceptableActor();
	if (Actor != nullptr) {
		//1. 액터의부모받아옴 / 타입 : 씬컴포넌트
		//2. 캐스팅<타입>(대상) => primitive이면 그거리턴, 아니면 null 리턴
		UPrimitiveComponent* Component=
			Cast<UPrimitiveComponent>(Actor->GetRootComponent());

		//프리미티브 타입이면, 물리엔진끄기 => 조각상이 문안에 가만히 있도록
		if (Component != nullptr) {
			
			Component->SetSimulatePhysics(false);
		}

		//트리거(this) 컴포넌트에 부착 => 같이내려가도록
		Actor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);


		Mover->SetShouldMove(true);
	}
	else {
		Mover->SetShouldMove(false);
	}
	
}

void UTriggerComponent::SetMover(UMover* NewMover)
{
	Mover = NewMover;
}



/*
* 태그에 해당하는 액터포인터 반환
*/
AActor* UTriggerComponent::GetAcceptableActor() const
{
	//액터들 포인터 저장하는 벡터
	TArray<AActor*> Actors;
	GetOverlappingActors(Actors);	//오버랩된 액터들을 포인터로 Actors에 담아줌.

	for (auto Actor : Actors) {
		//태그가 언락일때만 반응하도록

		//문제 : 사용자가 그랩중인데 문이 낚아채버림
		//해결 : 태그목록에 있음 and Grab중 아닌 액터만 리턴
		if (Actor->ActorHasTag(AcceptableActorTag) &&
			!Actor->ActorHasTag("Grabbed")) {
			return Actor;
		}

	}

	return nullptr;
}
