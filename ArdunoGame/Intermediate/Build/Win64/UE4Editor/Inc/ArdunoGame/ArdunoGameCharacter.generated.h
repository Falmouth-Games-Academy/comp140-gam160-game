// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	C++ class header boilerplate exported from UnrealHeaderTool.
	This is automatically generated by the tools.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef ARDUNOGAME_ArdunoGameCharacter_generated_h
#error "ArdunoGameCharacter.generated.h already included, missing '#pragma once' in ArdunoGameCharacter.h"
#endif
#define ARDUNOGAME_ArdunoGameCharacter_generated_h

#define ArdunoGame_Source_ArdunoGame_ArdunoGameCharacter_h_20_RPC_WRAPPERS
#define ArdunoGame_Source_ArdunoGame_ArdunoGameCharacter_h_20_RPC_WRAPPERS_NO_PURE_DECLS
#define ArdunoGame_Source_ArdunoGame_ArdunoGameCharacter_h_20_INCLASS_NO_PURE_DECLS \
	private: \
	static void StaticRegisterNativesAArdunoGameCharacter(); \
	friend ARDUNOGAME_API class UClass* Z_Construct_UClass_AArdunoGameCharacter(); \
	public: \
	DECLARE_CLASS(AArdunoGameCharacter, APaperCharacter, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/ArdunoGame"), NO_API) \
	DECLARE_SERIALIZER(AArdunoGameCharacter) \
	/** Indicates whether the class is compiled into the engine */ \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC}; \
	static const TCHAR* StaticConfigName() {return TEXT("Game");} \



#define ArdunoGame_Source_ArdunoGame_ArdunoGameCharacter_h_20_INCLASS \
	private: \
	static void StaticRegisterNativesAArdunoGameCharacter(); \
	friend ARDUNOGAME_API class UClass* Z_Construct_UClass_AArdunoGameCharacter(); \
	public: \
	DECLARE_CLASS(AArdunoGameCharacter, APaperCharacter, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/ArdunoGame"), NO_API) \
	DECLARE_SERIALIZER(AArdunoGameCharacter) \
	/** Indicates whether the class is compiled into the engine */ \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC}; \
	static const TCHAR* StaticConfigName() {return TEXT("Game");} \



#define ArdunoGame_Source_ArdunoGame_ArdunoGameCharacter_h_20_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API AArdunoGameCharacter(const FObjectInitializer& ObjectInitializer); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(AArdunoGameCharacter) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AArdunoGameCharacter); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AArdunoGameCharacter); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API AArdunoGameCharacter(AArdunoGameCharacter&&); \
	NO_API AArdunoGameCharacter(const AArdunoGameCharacter&); \
public:


#define ArdunoGame_Source_ArdunoGame_ArdunoGameCharacter_h_20_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API AArdunoGameCharacter(AArdunoGameCharacter&&); \
	NO_API AArdunoGameCharacter(const AArdunoGameCharacter&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AArdunoGameCharacter); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AArdunoGameCharacter); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(AArdunoGameCharacter)


#define ArdunoGame_Source_ArdunoGame_ArdunoGameCharacter_h_20_PRIVATE_PROPERTY_OFFSET \
	FORCEINLINE static uint32 __PPO__SideViewCameraComponent() { return STRUCT_OFFSET(AArdunoGameCharacter, SideViewCameraComponent); } \
	FORCEINLINE static uint32 __PPO__CameraBoom() { return STRUCT_OFFSET(AArdunoGameCharacter, CameraBoom); } \
	FORCEINLINE static uint32 __PPO__RunningAnimation() { return STRUCT_OFFSET(AArdunoGameCharacter, RunningAnimation); } \
	FORCEINLINE static uint32 __PPO__IdleAnimation() { return STRUCT_OFFSET(AArdunoGameCharacter, IdleAnimation); }


#define ArdunoGame_Source_ArdunoGame_ArdunoGameCharacter_h_17_PROLOG
#define ArdunoGame_Source_ArdunoGame_ArdunoGameCharacter_h_20_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	ArdunoGame_Source_ArdunoGame_ArdunoGameCharacter_h_20_PRIVATE_PROPERTY_OFFSET \
	ArdunoGame_Source_ArdunoGame_ArdunoGameCharacter_h_20_RPC_WRAPPERS \
	ArdunoGame_Source_ArdunoGame_ArdunoGameCharacter_h_20_INCLASS \
	ArdunoGame_Source_ArdunoGame_ArdunoGameCharacter_h_20_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define ArdunoGame_Source_ArdunoGame_ArdunoGameCharacter_h_20_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	ArdunoGame_Source_ArdunoGame_ArdunoGameCharacter_h_20_PRIVATE_PROPERTY_OFFSET \
	ArdunoGame_Source_ArdunoGame_ArdunoGameCharacter_h_20_RPC_WRAPPERS_NO_PURE_DECLS \
	ArdunoGame_Source_ArdunoGame_ArdunoGameCharacter_h_20_INCLASS_NO_PURE_DECLS \
	ArdunoGame_Source_ArdunoGame_ArdunoGameCharacter_h_20_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID ArdunoGame_Source_ArdunoGame_ArdunoGameCharacter_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
