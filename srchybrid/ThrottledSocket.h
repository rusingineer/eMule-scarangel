// ZZ:UploadBandWithThrottler (UDP) -->

#pragma once

struct SocketSentBytes {
    bool    success;
	uint32	sentBytesStandardPackets;
	uint32	sentBytesControlPackets;
};

class ThrottledControlSocket
{
public:
    virtual SocketSentBytes SendControlData(uint32 maxNumberOfBytesToSend, uint32 minFragSize) = 0;
	//Xman 
	virtual bool IsSocketUploading() = false;
};

class ThrottledFileSocket : public ThrottledControlSocket
{
public:
    virtual SocketSentBytes SendFileAndControlData(uint32 maxNumberOfBytesToSend, uint32 minFragSize) = 0;
    virtual DWORD GetLastCalledSend() = 0;
	virtual bool	IsBusy() const = 0;
	virtual bool    HasQueues() const = 0;

	//Xman Full chunk:
	virtual bool StandardPacketQueueIsEmpty() const = false ;

    
//Xman Xtreme Upload
	bool IsSocketUploading() {return slotstate!=0 && isready==true;} //Xman 

	//virtual uint32	GetNeededBytes() = 0;

	ThrottledFileSocket(void) {slotstate=0; isready=false;} //Xman 

	bool IsTrickle() const	{return slotstate==3;}
	bool IsFull() const		{return slotstate==1;}


	void SetTrickle()	{slotstate=3; CSlope=1;}
	void SetFull()		{slotstate=1;}
	void SetNoUploading() {slotstate=0;} //Xman 

	bool	isready;
	sint32	CSlope;


private:
	uint8	slotstate;	//1=full, 3=trickle
//Xman end
};

// <-- ZZ:UploadBandWithThrottler (UDP)
