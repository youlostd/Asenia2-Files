#pragma once

#ifdef _IMPROVED_PACKET_ENCRYPTION_
#include <cryptopp/cryptlib.h>
using CryptoPP::byte;
class KeyAgreement;

class Cipher
{
public:
	Cipher();
	~Cipher();

	void CleanUp();

	size_t Prepare(void* buffer, size_t* length);
	bool Activate(bool polarity, size_t agreed_length,
	const void* buffer, size_t length);

	void Encrypt(void* buffer, size_t length)
	{
		assert(activated_);
		if (!activated_)
			return;
		encoder_->ProcessData((byte*)buffer, (const byte*)buffer, length);
	}

	void Decrypt(void* buffer, size_t length)
	{
		assert(activated_);
		if (!activated_)
			return;
		decoder_->ProcessData((byte*)buffer, (const byte*)buffer, length);
	}

	bool activated() const {return activated_;}
	void set_activated(bool value) {activated_ = value;}

	bool IsKeyPrepared() {return key_agreement_ != NULL;}

	private:
	bool SetUp(bool polarity);

	bool activated_;

	CryptoPP::SymmetricCipher* encoder_;
	CryptoPP::SymmetricCipher* decoder_;

	KeyAgreement* key_agreement_;
};
#endif
