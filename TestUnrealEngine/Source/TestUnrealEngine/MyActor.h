// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyActor.generated.h"

UCLASS()
class TESTUNREALENGINE_API AMyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere);	//포인터타입 : visible->밖의 그래픽으로 조정가능
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category=BattleStat);//일반타입 edit->밖의 그래픽조정
	int32 Hp;

	UPROPERTY(EditAnywhere, Category = BattleStat);
	int32 Mp;
};
