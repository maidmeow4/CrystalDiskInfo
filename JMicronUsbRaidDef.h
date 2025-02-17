/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : https://crystalmark.info/
//      License : The MIT License
/*---------------------------------------------------------------------------*/

#pragma once

//-----------------------------------------------------------------------------
//
// Struct Defines
//
//-----------------------------------------------------------------------------

#define NUMBER_ATA_SMART_ATTRIBUTES 30

#pragma pack(push,1)

typedef struct _IDENTIFY_DEVICE
{
	WORD		GeneralConfiguration;					//0
	WORD		LogicalCylinders;						//1	Obsolete
	WORD		SpecificConfiguration;					//2
	WORD		LogicalHeads;							//3 Obsolete
	WORD		Retired1[2];							//4-5
	WORD		LogicalSectors;							//6 Obsolete
	DWORD		ReservedForCompactFlash;				//7-8
	WORD		Retired2;								//9
	CHAR		SerialNumber[20];						//10-19
	WORD		Retired3;								//20
	WORD		BufferSize;								//21 Obsolete
	WORD		Obsolute4;								//22
	CHAR		FirmwareRev[8];							//23-26
	CHAR		Model[40];								//27-46
	WORD		MaxNumPerInterupt;						//47
	WORD		Reserved1;								//48
	WORD		Capabilities1;							//49
	WORD		Capabilities2;							//50
	DWORD		Obsolute5;								//51-52
	WORD		Field88and7064;							//53
	WORD		Obsolute6[5];							//54-58
	WORD		MultSectorStuff;						//59
	DWORD		TotalAddressableSectors;				//60-61
	WORD		Obsolute7;								//62
	WORD		MultiWordDma;							//63
	WORD		PioMode;								//64
	WORD		MinMultiwordDmaCycleTime;				//65
	WORD		RecommendedMultiwordDmaCycleTime;		//66
	WORD		MinPioCycleTimewoFlowCtrl;				//67
	WORD		MinPioCycleTimeWithFlowCtrl;			//68
	WORD		Reserved2[6];							//69-74
	WORD		QueueDepth;								//75
	WORD		SerialAtaCapabilities;					//76
	WORD		SerialAtaAdditionalCapabilities;		//77
	WORD		SerialAtaFeaturesSupported;				//78
	WORD		SerialAtaFeaturesEnabled;				//79
	WORD		MajorVersion;							//80
	WORD		MinorVersion;							//81
	WORD		CommandSetSupported1;					//82
	WORD		CommandSetSupported2;					//83
	WORD		CommandSetSupported3;					//84
	WORD		CommandSetEnabled1;						//85
	WORD		CommandSetEnabled2;						//86
	WORD		CommandSetDefault;						//87
	WORD		UltraDmaMode;							//88
	WORD		TimeReqForSecurityErase;				//89
	WORD		TimeReqForEnhancedSecure;				//90
	WORD		CurrentPowerManagement;					//91
	WORD		MasterPasswordRevision;					//92
	WORD		HardwareResetResult;					//93
	WORD		AcoustricManagement;					//94
	WORD		StreamMinRequestSize;					//95
	WORD		StreamingTimeDma;						//96
	WORD		StreamingAccessLatency;					//97
	DWORD		StreamingPerformance;					//98-99
	ULONGLONG	MaxUserLba;								//100-103
	WORD		StremingTimePio;						//104
	WORD		Reserved3;								//105
	WORD		SectorSize;								//106
	WORD		InterSeekDelay;							//107
	WORD		IeeeOui;								//108
	WORD		UniqueId3;								//109
	WORD		UniqueId2;								//110
	WORD		UniqueId1;								//111
	WORD		Reserved4[4];							//112-115
	WORD		Reserved5;								//116
	DWORD		WordsPerLogicalSector;					//117-118
	WORD		Reserved6[8];							//119-126
	WORD		RemovableMediaStatus;					//127
	WORD		SecurityStatus;							//128
	WORD		VendorSpecific[31];						//129-159
	WORD		CfaPowerMode1;							//160
	WORD		ReservedForCompactFlashAssociation[7];	//161-167
	WORD		DeviceNominalFormFactor;				//168
	WORD		DataSetManagement;						//169
	WORD		AdditionalProductIdentifier[4];			//170-173
	WORD		Reserved7[2];							//174-175
	CHAR		CurrentMediaSerialNo[60];				//176-205
	WORD		SctCommandTransport;					//206
	WORD		ReservedForCeAta1[2];					//207-208
	WORD		AlignmentOfLogicalBlocks;				//209
	DWORD		WriteReadVerifySectorCountMode3;		//210-211
	DWORD		WriteReadVerifySectorCountMode2;		//212-213
	WORD		NvCacheCapabilities;					//214
	DWORD		NvCacheSizeLogicalBlocks;				//215-216
	WORD		NominalMediaRotationRate;				//217
	WORD		Reserved8;								//218
	WORD		NvCacheOptions1;						//219
	WORD		NvCacheOptions2;						//220
	WORD		Reserved9;								//221
	WORD		TransportMajorVersionNumber;			//222
	WORD		TransportMinorVersionNumber;			//223
	WORD		ReservedForCeAta2[10];					//224-233
	WORD		MinimumBlocksPerDownloadMicrocode;		//234
	WORD		MaximumBlocksPerDownloadMicrocode;		//235
	WORD		Reserved10[19];							//236-254
	WORD		IntegrityWord;							//255
}IDENTIFY_DEVICE;

typedef	struct _SMART_THRESHOLD
{
	BYTE	Id;
	BYTE	ThresholdValue;
	BYTE	Reserved[10];
} SMART_THRESHOLD;

/*
typedef	struct _SMART_ATTRIBUTE
{
	BYTE	Id;
	WORD	StatusFlags;
	BYTE	CurrentValue;
	BYTE	WorstValue;
	BYTE	RawValue[6];
	BYTE	Reserved;
} SMART_ATTRIBUTE;
*/

typedef struct _SMART_READ_DATA
{
	BYTE revision[2];
	SMART_ATTRIBUTE attribute[NUMBER_ATA_SMART_ATTRIBUTES];
	BYTE reserved[150];
} SMART_READ_DATA;

typedef struct _SMART_READ_THRESHOLD
{
	BYTE revision[2];
	SMART_THRESHOLD threshold[NUMBER_ATA_SMART_ATTRIBUTES];
	BYTE reserved[150];
} SMART_READ_THRESHOLD;

typedef struct _BYTE512
{
	BYTE b[512];
} BYTE512;

typedef union _UNION_IDENTIFY_DEVICE
{
	IDENTIFY_DEVICE I;
	BYTE512 B;
} UNION_IDENTIFY_DEVICE;

typedef union _UNION_SMART_ATTRIBUTE
{
	SMART_READ_DATA A;
	BYTE512 B;
} UNION_SMART_ATTRIBUTE;

typedef union _UNION_SMART_THRESHOLD
{
	SMART_READ_THRESHOLD T;
	BYTE512 B;
} UNION_SMART_THRESHOLD;

#pragma	pack(pop)

//-----------------------------------------------------------------------------
//
// Type Defines
//
//-----------------------------------------------------------------------------

// JMicron USB RAID API
typedef int(WINAPI* _GetControllerCount) ();
typedef BOOL(WINAPI* _GetSmartInfoFx) (int index, unsigned char port, UNION_SMART_ATTRIBUTE* attribute, UNION_SMART_THRESHOLD* threshold);
typedef BOOL(WINAPI* _GetIdentifyInfoFx) (int index, unsigned char port, UNION_IDENTIFY_DEVICE* identify);
