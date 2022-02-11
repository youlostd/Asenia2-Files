#pragma once

#include "../lzo/lzoconf.h"
#include "../lzo/lzo1x.h"
#include "Singleton.h"
#include <windows.h>

class CLZObject
{
public:
	#pragma pack(4)
	typedef struct
	{
		uint32_t dwFourCC;
		uint32_t dwEncryptSize;
		uint32_t dwCompressedSize;
		uint32_t dwRealSize;
	} THeader;
	#pragma pack()

	CLZObject();
	~CLZObject();

	void Clear();

	void BeginCompress(const void * pvIn, uint32_t uiInLen);
	bool Compress();

	bool BeginDecompress(const void * pvIn);
	bool Decompress(uint32_t * pdwKey = nullptr);

	bool Encrypt(uint32_t * pdwKey);
	uint8_t * Decrypt(uint32_t * pdwKey);

	const THeader& GetHeader() {return * m_pHeader;}
	uint8_t * GetBuffer() {return m_pbBuffer;}
	uint32_t GetSize();

private:
	void Initialize();

	uint8_t * m_pbBuffer;
	uint32_t m_dwBufferSize;

	THeader * m_pHeader;
	const uint8_t * m_pbIn;
	bool m_bCompressed;

	static uint32_t ms_dwFourCC;
};

class CLZO : public singleton<CLZO>
{
public:
	CLZO();
	virtual ~CLZO();

	bool CompressMemory(CLZObject& rObj, const void * pIn, uint32_t uiInLen);
	bool CompressEncryptedMemory(CLZObject& rObj, const void * pIn, uint32_t uiInLen, uint32_t * pdwKey);
	bool Decompress(CLZObject& rObj, const uint8_t * pbBuf, uint32_t * pdwKey = nullptr);
	uint8_t * GetWorkMemory();

private:
	uint8_t * m_pWorkMem;
};
