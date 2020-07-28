#include "framework.h"
#include "CMessage.h"

	CMessage::CMessage(int iSize)
	{
		this->front = -1;
		this->rear = -1;
		this->m_iBufferSize = iSize;
		this->m_buffer = new char[iSize];
	}

	CMessage::~CMessage()
	{

		delete[] this->m_buffer;
	}


	void CMessage::Release(void)
	{
		delete[] this->m_buffer;
	}
	
	void CMessage::Clear(void)
	{
		this->front = -1;
		this->rear = -1;
	}
	
	int CMessage::GetBufferSize(void)
	{
		return this->m_iBufferSize;
	}
	
	int CMessage::GetDataSize(void)
	{
		return this->m_iDataSize;
	}

	char* CMessage::GetBufferPtr(void)
	{
		return this->m_buffer;
	}

	int CMessage::MoveWritePos(int iSize)
	{
		this->rear += iSize;

		return iSize;
	}

	int CMessage::MoveReadPos(int iSize)
	{
		this->front += iSize;

		return iSize;
	}

	int CMessage::GetData(char* chpDest, int iSize)
	{
		memcpy(chpDest, &this->m_buffer[this->front + 1], iSize);

		return iSize;
	}

	int CMessage::PutData(char* chpSrc, int iSize)
	{
		memcpy(&this->m_buffer[this->rear + 1], chpSrc, iSize);

		return iSize;
	}

	CMessage& CMessage::operator << (char chValue)
	{	
		memcpy(&this->m_buffer[this->rear + 1], &chValue, sizeof(char));

		this->rear += sizeof(char);

		return *this;
	}

	CMessage& CMessage::operator << (unsigned char uchValue)
	{
		memcpy(&this->m_buffer[this->rear + 1], &uchValue, sizeof(char));

		this->rear += sizeof(char);

		return *this;
	}

	CMessage& CMessage::operator << (short sValue)
	{
		memcpy(&this->m_buffer[this->rear + 1], &sValue, sizeof(short));
	
		this->rear += sizeof(short);

		return *this;
	}

	CMessage& CMessage::operator << (unsigned short usValue)
	{
		memcpy(&this->m_buffer[this->rear + 1], &usValue, sizeof(short));

		this->rear += sizeof(short);

		return *this;
	}

	CMessage& CMessage::operator << (int iValue)
	{		
		memcpy(&this->m_buffer[this->rear+1], &iValue, sizeof(int));

		this->rear += sizeof(int);

		return *this;
	}

	CMessage& CMessage::operator << (unsigned int uiValue)
	{
		memcpy(&this->m_buffer[this->rear + 1], &uiValue, sizeof(int));

		this->rear += sizeof(int);

		return *this;
	}

	CMessage& CMessage::operator << (long lValue)
	{
		memcpy(&this->m_buffer[this->rear+1], &lValue, sizeof(long));

		this->rear += sizeof(long);

		return *this;
	}

	CMessage& CMessage::operator << (unsigned long ulValue)
	{
		memcpy(&this->m_buffer[this->rear + 1], &ulValue, sizeof(long));

		this->rear += sizeof(long);

		return *this;
	}

	CMessage& CMessage::operator >> (char &chValue)
	{		
		memcpy(&chValue, &this->m_buffer[this->front + 1], sizeof(char));
	
		this->front += sizeof(char);


		return *this;
	}

	CMessage& CMessage::operator >> (unsigned char &uchValue)
	{
		memcpy(&uchValue, &this->m_buffer[this->front + 1], sizeof(char));

		this->front += sizeof(char);

		return *this;
	}

	CMessage& CMessage::operator >> (short &sValue)
	{	
		memcpy(&sValue, &this->m_buffer[this->front + 1], sizeof(short));

		this->front += sizeof(short);


		return *this;
	}

	CMessage& CMessage::operator >> (unsigned short &usValue)
	{
		memcpy(&usValue, &this->m_buffer[this->front + 1], sizeof(short));

		this->front += sizeof(short);

		return *this;
	}

	CMessage& CMessage::operator >> (int &iValue)
	{
		
		memcpy(&iValue, &this->m_buffer[this->front + 1], sizeof(int));

		this->front += sizeof(int);


		return *this;
	}

	CMessage& CMessage::operator >> (unsigned int &uiValue)
	{
		memcpy(&uiValue, &this->m_buffer[this->front + 1], sizeof(int));

		this->front += sizeof(int);

		return *this;
	}

	CMessage& CMessage::operator >> (long &lValue)
	{
		memcpy(&lValue, &this->m_buffer[this->front + 1], sizeof(long));

		this->front += sizeof(long);

		return *this;
	}

	CMessage& CMessage::operator >> (unsigned long &ulValue)
	{
		memcpy(&ulValue, &this->m_buffer[this->front + 1], sizeof(long));

		this->front += sizeof(long);

		return *this;
	}
