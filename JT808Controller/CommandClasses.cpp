#include "stdafx.h"
#include "CommandClasses.h"

using namespace ContentType;

CommandClasses::CommandClasses()
{
}


CommandClasses::~CommandClasses()
{
}

void cmd0x9101::Compile()
{
	this->content.Reset();
	this->content.Put((BYTE)this->sIP.length());
	this->content.Put(this->sIP);
	this->content.Put(this->wTcpPort);
	this->content.Put(this->wUdpPort);
	this->content.Put(this->byLogicChannel);
	this->content.Put(this->byStreamType);
	this->content.Put(this->byCodecType);
}

bool cmd0x9101::Parse()
{
	BYTE len = 0;
	return
		this->content.Get(len) &&
		this->content.Get(this->sIP, len) &&
		this->content.Get(this->wTcpPort) &&
		this->content.Get(this->wUdpPort) &&
		this->content.Get(this->byLogicChannel) &&
		this->content.Get(this->byStreamType) &&
		this->content.Get(this->byCodecType);
}

void cmd0x9102::Compile()
{
	this->content.Reset();
	this->content.Put(this->byLogicChannel);
	this->content.Put(this->byCommand);
	this->content.Put(this->byCloseType);
	this->content.Put(this->bySwitchType);
}

bool cmd0x9102::Parse()
{
	return
		this->content.Get(this->byLogicChannel) &&
		this->content.Get(this->byCommand) &&
		this->content.Get(this->byCloseType) &&
		this->content.Get(this->bySwitchType);
}

void cmd0x0001::Compile()
{
	this->content.Reset();
	this->content.Put(this->wResponseFloatID);
	this->content.Put(this->wResponseMessageID);
	this->content.Put(this->byResult);
}

bool cmd0x0001::Parse()
{
	return
		this->content.Get(this->wResponseFloatID) &&
		this->content.Get(this->wResponseMessageID) &&
		this->content.Get(this->byResult);
}

void cmd0x0100::Compile()
{
	this->content.Reset();
	this->content.Put(this->wProvince);
	this->content.Put(this->wCity);
	this->content.Put(this->sProductor, 5);
	this->content.Put(this->sModel, 20);
	this->content.Put(this->sTerminalID, 7);
	this->content.Put(this->byPlateColor);
	this->content.Put(this->sPlateID);
}

bool cmd0x0100::Parse()
{
	return
		this->content.Get(this->wProvince) &&
		this->content.Get(this->wCity) &&
		this->content.Get(this->sProductor, 5) &&
		this->content.Get(this->sModel, 20) &&
		this->content.Get(this->sTerminalID, 7) &&
		this->content.Get(this->byPlateColor) &&
		this->content.Get(this->sPlateID, -1);
}

void cmd0x8100::Compile()
{
	this->content.Put(this->wResponseID);
	this->content.Put(this->byResult);
	if (this->byResult == 0)
		this->content.Put(this->sAuthCode);
}

bool cmd0x8100::Parse()
{
	return
		this->content.Get(this->wResponseID) &&
		this->content.Get(this->byResult) &&
		((this->byResult == 0) ? this->content.Get(this->sAuthCode, -1) : true);
}

void cmd0x0102::Compile()
{
	this->content.Put(this->sAuthCode);
}

bool cmd0x0102::Parse()
{
	return this->content.Get(this->sAuthCode, -1);
}

void cmd0x0002::Compile()
{
	this->content.Reset();
}

bool cmd0x0002::Parse()
{
	return true;
}

void cmd0x8001::Compile()
{
	this->content.Put(this->wResponseID);
	this->content.Put(this->wMessageID);
	this->content.Put(this->byResult);
}

bool cmd0x8001::Parse()
{
	return
		this->content.Get(this->wResponseID) &&
		this->content.Get(this->wMessageID) &&
		this->content.Get(this->byResult);
}

void cmd0x0200::Compile()
{
	this->content.Put(this->dAlarm);
	this->content.Put(this->dStatus);
	this->content.Put(this->dLatitude);
	this->content.Put(this->dLongitude);
	this->content.Put(this->wAltitude);
	this->content.Put(this->wSpeed);
	this->content.Put(this->wDirector);
	this->content.AutoIncressContent::Put(this->sTime, 6);
}

bool cmd0x0200::Parse()
{
	return this->content.Get(this->dAlarm) &&
		this->content.Get(this->dStatus) &&
		this->content.Get(this->dLatitude) &&
		this->content.Get(this->dLongitude) &&
		this->content.Get(this->wAltitude) &&
		this->content.Get(this->wSpeed) &&
		this->content.Get(this->wDirector);
}
