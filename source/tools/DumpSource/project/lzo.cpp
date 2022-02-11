#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "lzo.h"
#include "tea.h"

CLZO __Project_Name__;
uint32_t CLZObject::ms_dwFourCC = MAKEFOURCC('M', 'C', 'O', 'Z');

CLZObject::CLZObject()
{
	Initialize();
}

void CLZObject::Initialize()
{
	m_pHeader = nullptr;
	m_pbBuffer = nullptr;
	m_dwBufferSize = 0;
	m_pbIn = nullptr;
	m_bCompressed = false;
}

void CLZObject::Clear()
{
	if (m_pbBuffer)
		delete [] m_pbBuffer;

	Initialize();
}

CLZObject::~CLZObject()
{
	Clear();
}

uint32_t CLZObject::GetSize()
{
	assert(m_pHeader);
	if (m_bCompressed)
	{
		if (m_pHeader->dwEncryptSize)
			return sizeof(THeader) + sizeof(uint32_t) + m_pHeader->dwEncryptSize;
		else
			return sizeof(THeader) + sizeof(uint32_t) + m_pHeader->dwCompressedSize;
	}
	else
		return m_pHeader->dwRealSize;
}

void CLZObject::BeginCompress(const void * pvIn, uint32_t uiInLen)
{
	m_pbIn = (const uint8_t*) pvIn;
	m_dwBufferSize = sizeof(THeader) + sizeof(uint32_t) + (uiInLen + uiInLen / 64 + 16 + 3) + 8;

	m_pbBuffer = new uint8_t[m_dwBufferSize];
	memset(m_pbBuffer, 0, m_dwBufferSize);

	m_pHeader = (THeader * ) m_pbBuffer;
	m_pHeader->dwFourCC = ms_dwFourCC;
	m_pHeader->dwEncryptSize = m_pHeader->dwCompressedSize = m_pHeader->dwRealSize = 0;
	m_pHeader->dwRealSize = uiInLen;
}

bool CLZObject::Compress()
{
	uint32_t iOutLen;
	uint8_t * pbBuffer;

	pbBuffer = m_pbBuffer + sizeof(THeader);
	*(uint32_t*) pbBuffer = ms_dwFourCC;
	pbBuffer += sizeof(uint32_t);

	int32_t r = lzo1x_1_compress((uint8_t*) m_pbIn, m_pHeader->dwRealSize, pbBuffer, &iOutLen, CLZO::instance().GetWorkMemory());

	if (LZO_E_OK != r)
	{
		fprintf(stderr, "LZO: lzo1x_compress failed\n");
		return false;
	}

	m_pHeader->dwCompressedSize = iOutLen;
	m_bCompressed = true;

	return true;
}

bool CLZObject::BeginDecompress(const void * pvIn)
{
	THeader * pHeader = (THeader * ) pvIn;

	if (pHeader->dwFourCC != ms_dwFourCC)
	{
		fprintf(stderr, "LZObject: not a valid data");
		return false;
	}

	m_pHeader = pHeader;
	m_pbIn = (const uint8_t*) pvIn + (sizeof(THeader) + sizeof(uint32_t));

	m_pbBuffer = new uint8_t[pHeader->dwRealSize];
	memset(m_pbBuffer, 0, pHeader->dwRealSize);
	return true;
}

bool CLZObject::Decompress(uint32_t * pdwKey)
{
	uint32_t uiSize;
	int32_t r;

	if (m_pHeader->dwEncryptSize)
	{
		uint8_t * pbDecryptedBuffer = Decrypt(pdwKey);
		if (*(uint32_t*) pbDecryptedBuffer != ms_dwFourCC)
		{
			fprintf(stderr, "LZObject: key incorrect");
			return false;
		}

		if (LZO_E_OK != (r = lzo1x_decompress(pbDecryptedBuffer + sizeof(uint32_t), m_pHeader->dwCompressedSize, m_pbBuffer, &uiSize, nullptr)))
		{
			fprintf(stderr, "LZObject: Decompress failed(decrypt) ret %d\n", r);
			return false;
		}

		delete [] pbDecryptedBuffer;
	}
	else
	{
		uiSize = m_pHeader->dwRealSize;
		if (LZO_E_OK != (r = lzo1x_decompress_safe(m_pbIn, m_pHeader->dwCompressedSize, m_pbBuffer, &uiSize, nullptr)))
		{
			fprintf(stderr, "LZObject: Decompress failed : ret %d, CompressedSize %d\n", r, m_pHeader->dwCompressedSize);
			return false;
		}
	}

	if (uiSize != m_pHeader->dwRealSize)
	{
		fprintf(stderr, "LZObject: Size differs");
		return false;
	}

	return true;
}

bool CLZObject::Encrypt(uint32_t * pdwKey)
{
	if (!m_bCompressed)
	{
		assert(!"not compressed yet");
		return false;
	}

	uint8_t * pbBuffer = m_pbBuffer + sizeof(THeader);
	m_pHeader->dwEncryptSize = tea_encrypt((uint32_t*) pbBuffer, (const uint32_t*) pbBuffer, pdwKey, m_pHeader->dwCompressedSize + 19);
	return true;
}

uint8_t * CLZObject::Decrypt(uint32_t * pdwKey)
{
	assert(m_pbBuffer);
	uint8_t * pbDecryptBuffer = new uint8_t[m_pHeader->dwEncryptSize];

	tea_encrypt((uint32_t*) pbDecryptBuffer, (const uint32_t*) (m_pbIn - sizeof(uint32_t)), pdwKey, m_pHeader->dwEncryptSize);
	return pbDecryptBuffer;
}

CLZO::CLZO() : m_pWorkMem(nullptr)
{
	if (lzo_init() != LZO_E_OK)
	{
		fprintf(stderr, "LZO: cannot initialize\n");
		return;
	}

	m_pWorkMem = (uint8_t*) malloc(LZO1X_MEM_COMPRESS);

	if (nullptr == m_pWorkMem)
	{
		fprintf(stderr, "LZO: cannot alloc memory\n");
		return;
	}
}

CLZO::~CLZO()
{
	if (m_pWorkMem)
	{
		free (m_pWorkMem);
		m_pWorkMem = nullptr;
	}
}

bool CLZO::CompressMemory(CLZObject& rObj, const void * pIn, uint32_t uiInLen)
{
	rObj.BeginCompress(pIn, uiInLen);
	return rObj.Compress();
}

bool CLZO::CompressEncryptedMemory(CLZObject& rObj, const void * pIn, uint32_t uiInLen, uint32_t * pdwKey)
{
	rObj.BeginCompress(pIn, uiInLen);
	if (rObj.Compress())
	{
		if (rObj.Encrypt(pdwKey))
			return true;

		return false;
	}

	return false;
}

bool CLZO::Decompress(CLZObject& rObj, const uint8_t * pbBuf, uint32_t * pdwKey)
{
	if (!rObj.BeginDecompress(pbBuf))
		return false;

	if (!rObj.Decompress(pdwKey))
		return false;

	return true;
}

uint8_t * CLZO::GetWorkMemory()
{
	return m_pWorkMem;
}
