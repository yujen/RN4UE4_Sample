// Fill out your copyright notice in the Description page of Project Settings.



#include "RN4UE4_Sample.h"

#include "IRakNet.h"

#include "RakNetTypes.h"
#include "RakPeerInterface.h"
#include "GetTime.h"
#include "BitStream.h"
#include "MessageIdentifiers.h"
#include "Gets.h"

#include "Ping.h"



DEFINE_LOG_CATEGORY(RN4UE4Sample);


// Sets default values
APing::APing()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APing::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APing::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void APing::StartServer(FString responseString)
{
	RakNet::RakPeerInterface* server = RakNet::RakPeerInterface::GetInstance();
	int i = server->GetNumberOfAddresses();

	char* charString = TCHAR_TO_UTF8(*responseString);
	//char* charString = TCHAR_TO_UTF8((L""));
	UE_LOG(RN4UE4Sample, Log, TEXT("Server response string length: %d"), strlen(charString));
	UE_LOG(RN4UE4Sample, Log, TEXT("Server response string: %s"), *FString(UTF8_TO_TCHAR(charString)));
	//char* charString = TCHAR_TO_ANSI(L"abc123");
	//char enumData[512] = TCHAR_TO_ANSI(L"abc123中文字串あいうえお");
	//char enumData[512] = "abc123";
	server->SetOfflinePingResponse(charString, (const unsigned int)strlen(charString) + 1);

	// The server has to be started to respond to pings.
	RakNet::SocketDescriptor socketDescriptor(60000, 0);
	bool b = server->Startup(2, &socketDescriptor, 1) == RakNet::RAKNET_STARTED;
	server->SetMaximumIncomingConnections(2);
	if (b)
	{
		UE_LOG(RN4UE4Sample, Log, TEXT("Server started, waiting for connections."));
		
	}
	
	//TestStr testStr;
	//UE_LOG(RN4UE4Sample, Log, TEXT("Server started, waiting for connections.  %d"), testStr.testfunc());
	//RakNet::TestClass<int>* tc = new RakNet::TestClass<int>();
	//UE_LOG(RN4UE4Sample, Log, TEXT("Server started, waiting for connections.  %d"), tc->testClass());
}

void APing::ClientPing()
{
	UE_LOG(RN4UE4Sample, Log, TEXT("OnRakNetPingClient"));
	RakNet::RakPeerInterface *client = RakNet::RakPeerInterface::GetInstance();

	// Holds packets
	RakNet::Packet* p;


	RakNet::SocketDescriptor socketDescriptor(0, 0);
	client->Startup(1, &socketDescriptor, 1);
	client->Ping("127.0.0.1", 60000, false);

	while (true)
	{
		p = client->Receive();
		if (p == 0)
		{
			continue;
		}

		// Check if this is a network message packet
		switch (p->data[0])
		{
		case ID_UNCONNECTED_PONG:
		{
			unsigned int dataLength;
			RakNet::TimeMS time;
			RakNet::BitStream bsIn(p->data, p->length, false);
			bsIn.IgnoreBytes(1);
			bsIn.Read(time);
			dataLength = p->length - sizeof(unsigned char) - sizeof(RakNet::TimeMS);

			FString sysAddress = FString(p->systemAddress.ToString(true));
			UE_LOG(RN4UE4Sample, Log, TEXT("ID_UNCONNECTED_PONG from SystemAddress %s."), *sysAddress);
			UE_LOG(RN4UE4Sample, Log, TEXT("Time is %i"), time);
			UE_LOG(RN4UE4Sample, Log, TEXT("Data is %i bytes long"), dataLength);


			if (dataLength > 0)
			{
				char* charData = (char*)(p->data + sizeof(unsigned char) + sizeof(RakNet::TimeMS));
				FString strData = FString(UTF8_TO_TCHAR(charData));
				UE_LOG(RN4UE4Sample, Log, TEXT("String is %s, length is %d "), *strData, strlen(charData));

			}
			
			// In this sample since the client is not running a game we can save CPU cycles by
			// Stopping the network threads after receiving the pong.
			client->Shutdown(100);
		}
		break;
		case ID_UNCONNECTED_PING:
			break;
		case ID_UNCONNECTED_PING_OPEN_CONNECTIONS:
			break;
		}

		client->DeallocatePacket(p);
		break;
	}

	RakNet::RakPeerInterface::DestroyInstance(client);
	UE_LOG(RN4UE4Sample, Log, TEXT("client finished."));
	
}
