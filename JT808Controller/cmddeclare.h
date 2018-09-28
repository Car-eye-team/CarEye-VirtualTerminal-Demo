
#ifndef CMD_START
#	error Undefined `CMD_START', you must define before include
#endif
#ifndef CMD_END
#	error Undefined `CMD_END', you must define before include
#endif
#ifndef MDWORD
#	error Undefined `MDWORD', you must define before include
#endif
#ifndef MWORD
#	error Undefined `MWORD', you must define before include
#endif
#ifndef MBYTE
#	error Undefined `MBYTE', you must define before include
#endif
#ifndef ARRAY
#	error Undefined `ARRAY', you must define before include
#endif
#ifndef MBYTES
#	error Undefined `MBYTES', you must define before include
#endif
#ifndef MBYTES
#	error Undefined `MBYTES', you must define before include
#endif
#ifndef MSTRING
#	error Undefined `MSTRING', you must define before include
#endif
#ifndef MBCD
#	error Undefined `MBCD', you must define before include
#endif
#ifndef MSTRING
#	error Undefined `MSTRING', you must define before include
#endif
#ifndef COND_START
#	error Undefined `COND_START', you must define before include
#endif
#ifndef COND_END
#	error Undefined `COND_END', you must define before include
#endif
#ifndef ARRAY_START
#	error Undefined `ARRAY_START', you must define before include
#endif
#ifndef ARRAY_END
#	error Undefined `ARRAY_END', you must define before include
#endif
#ifndef SWITCH_START
#	error Undefined `SWITCH_START', you must define before include
#endif
#ifndef SWITCH_END
#	error Undefined `SWITCH_END', you must define before include
#endif
#ifndef CASE_RANGE
#	error Undefined `CASE_RANGE', you must define before include
#endif
#ifndef CASE
#	error Undefined `CASE_RANGE', you must define before include
#endif
#ifndef CASE_START
#	error Undefined `CASE_RANGE', you must define before include
#endif
#ifndef CASE_END
#	error Undefined `CASE_RANGE', you must define before include
#endif
#ifndef MBITS
#	error Undefined `CASE_RANGE', you must define before include
#endif
#ifndef SUBOBJ_START
#	error Undefined `CASE_RANGE', you must define before include
#endif
#ifndef SUBOBJ_END
#	error Undefined `SUBOBJ_END', you must define before include
#endif
#ifndef OPTION_START
#	error Undefined `OPTION_START', you must define before include
#endif
#ifndef OPTION_END
#	error Undefined `OPTION_END', you must define before include
#endif

CMD_START(0x0001)
	MWORD(respseq)
	MWORD(respmsgid)
	MBYTE(result)
CMD_END()

CMD_START(0x8001)
	MWORD(respseq)
	MWORD(respmsgid)
	MBYTE(result)
CMD_END()

CMD_START(0x0002)
CMD_END()

CMD_START(0x8003)
	MWORD(originseq)
	MBYTE(count)
	ARRAY(WORD, list, count)
CMD_END()

CMD_START(0x0100)
	MWORD(province)
	MWORD(city)
	MBYTES(manufacturer,5)
	MBYTES(model,20)
	MBYTES(terminal,7)
	MBYTE(plateColor)
	MSTRING(plateId, -1)
CMD_END()

CMD_START(0x8004)
	MBCD(time, 6)
CMD_END()

CMD_START(0x8100)
	MWORD(seqR)
	MBYTE(result)
	COND_START(result==0)
		MSTRING(passwd, -1)
	COND_END()
CMD_END()

CMD_START(0x0003)
CMD_END()

CMD_START(0x0102)
	MSTRING(passwd, -1)
CMD_END()

CMD_START(0x8103)
	MBYTE(count)
	ARRAY_START(items, count)
		MDWORD(id)
		MBYTE(size)
		SWITCH_START(id)
			CASE_RANGE(0x0001, 0x0007) CASE_RANGE(0x0018, 0x0019) CASE_RANGE(0x001B, 0x001C) CASE_RANGE(0x0020, 0x0030) CASE_RANGE(0x0045, 0x0047) CASE_RANGE(0x0050, 0x005A)
			CASE_RANGE(0x0070, 0x0074) CASE(0x007A) CASE_RANGE(0x0080, 0x0086) CASE(0x0087) CASE(0x0093) CASE(0x0095) CASE(0x100) CASE(0x0102)
				CASE_START
				MDWORD(value)
				CASE_END
			CASE_RANGE(0x0010, 0x0017) CASE(0x001A) CASE(0x001D) CASE_RANGE(0x0040, 0x0044) CASE_RANGE(0x0048, 0x0049)
				CASE_START
				MBYTES(value, size)
				CASE_END
			CASE(0x0031) CASE_RANGE(0x005B, 0x005E) CASE(0x0101) CASE(0x0103)
				CASE_START
				MWORD(value)
				CASE_END
			CASE(0x0064)
				CASE_START
				MBITS(BYTE, "channelTiming", 5)
				MBITS(BYTE, "channelStorage", 5)
				MWORD(timeInterval)
				CASE_END
			CASE(0x0065)
				CASE_START
				MBITS(BYTE, "channelDistance", 5)
				MBITS(BYTE, "channelStorage", 5)
				MWORD(distanceInterval)
				CASE_END
			CASE_RANGE(0x0090, 0x0092) CASE(0x0094)
				CASE_START
				MBYTE(value)
				CASE_END
			CASE_RANGE(0x0110, 0x01FF)
				CASE_START
				MBYTES(value, 8)
				CASE_END
			CASE(0x0075)
				CASE_START
				MBYTE(mode)
				MBYTE(resolution)
				MWORD(keyinterval)
				MBYTE(fps)
				MDWORD(rate)
				MBYTE(recmode)
				MBYTE(recresolution)
				MDWORD(reckeyinterval)
				MBYTE(recfps)
				MDWORD(recRate)
				MBITS(WORD, "osdsettings", 7)
				MBYTE(audioOn)
				CASE_END
			CASE(0x0076)
				CASE_START
				MBYTE(totalChannel)
				MBYTE(totalAudioChannel)
				MBYTE(totalVedioChannel)
				ARRAY_START(channelmap, totalChannel+totalAudioChannel+totalVedioChannel)
					MBYTE(phychannel)
					MBYTE(logchannel)
					MBYTE(chanelType)
					MBYTE(isconnetplat)
				ARRAY_END()
				CASE_END
			CASE(0x0077)
				CASE_START
				MBYTE(sumtotal)
				ARRAY_START(list, sumtotal)
					MBYTE(logchannel)
					MBYTE(mode)
					MBYTE(resolution)
					MWORD(keyinterval)
					MBYTE(fps)
					MDWORD(rate)
					MBYTE(recmode)
					MBYTE(recresolution)
					MDWORD(reckeyInterval)
					MBYTE(recfps)
					MDWORD(recrate)
					MBITS(WORD, "osdsetting", 7)
				ARRAY_END()
				CASE_END
			CASE(0x0079)
				CASE_START
				MBYTE(ratio)
				MBYTE(period)
				MBYTE(starttime)
				CASE_END
			CASE(0x007B)
				CASE_START
				MBYTE(number)
				MBYTE(period)
				CASE_END
			CASE(0x007C)
				CASE_START
				MBITS(BYTE, "mode", 3)
				MBITS(BYTE, "type", 3)
				MBITS(BYTE, "timing", 7)
				SUBOBJ_START(list)
					MBITS(BYTE, "flag", 4)
					MBCD(time1start, 2)
					MBCD(time1end, 2)
					MBCD(time2start, 2)
					MBCD(time2end, 2)
					MBCD(time3start, 2)
					MBCD(time3end, 2)
					MBCD(time4start, 2)
					MBCD(time4end, 2)
				SUBOBJ_END()
				CASE_END
		SWITCH_END()
	ARRAY_END()
CMD_END()

CMD_START(0x8104)
CMD_END()

CMD_START(0x8105)
	MBYTE(id)
	MSTRING(size, -1)
CMD_END()

CMD_START(0x8106)
	MBYTE(count)
	ARRAY(DWORD, items, count)
CMD_END()

CMD_START(0x0104)
	MWORD(seqR)
	MBYTE(count)
	ARRAY_START(items, count)
		MDWORD(id)
		MBYTE(size)
		SWITCH_START(id)
		CASE_RANGE(0x0001, 0x0007) CASE_RANGE(0x0018, 0x0019) CASE_RANGE(0x001B, 0x001C) CASE_RANGE(0x0020, 0x0030) CASE_RANGE(0x0045, 0x0047) CASE_RANGE(0x0050, 0x005A)
		CASE_RANGE(0x0070, 0x0074) CASE(0x007A) CASE_RANGE(0x0080, 0x0086) CASE(0x0087) CASE(0x0093) CASE(0x0095) CASE(0x100) CASE(0x0102)
			CASE_START
			MDWORD(value)
			CASE_END
		CASE_RANGE(0x0010, 0x0017) CASE(0x001A) CASE(0x001D) CASE_RANGE(0x0040, 0x0044) CASE_RANGE(0x0048, 0x0049)
			CASE_START
			MBYTES(value, size)
			CASE_END
		CASE(0x0031) CASE_RANGE(0x005B, 0x005E) CASE(0x0101) CASE(0x0103)
			CASE_START
			MWORD(value)
			CASE_END
		CASE(0x0064)
			CASE_START
			MBITS(BYTE, "channelTiming", 5)
			MBITS(BYTE, "channelStorage", 5)
			MWORD(timeInterval)
			CASE_END
		CASE(0x0065)
			CASE_START
			MBITS(BYTE, "channelDistance", 5)
			MBITS(BYTE, "channelStorage", 5)
			MWORD(distanceInterval)
			CASE_END
		CASE_RANGE(0x0090, 0x0092) CASE(0x0094)
			CASE_START
			MBYTE(value)
			CASE_END
		CASE_RANGE(0x0110, 0x01FF)
			CASE_START
			MBYTES(value, 8)
			CASE_END
		CASE(0x0075)
			CASE_START
			MBYTE(mode)
			MBYTE(resolution)
			MWORD(keyinterval)
			MBYTE(fps)
			MDWORD(rate)
			MBYTE(recmode)
			MBYTE(recresolution)
			MDWORD(reckeyinterval)
			MBYTE(recfps)
			MDWORD(recRate)
			MBITS(WORD, "osdsettings", 7)
			MBYTE(audioOn)
			CASE_END
		CASE(0x0076)
			CASE_START
			MBYTE(totalChannel)
			MBYTE(totalAudioChannel)
			MBYTE(totalVedioChannel)
			ARRAY_START(channelmap, totalChannel + totalAudioChannel + totalVedioChannel)
				MBYTE(phychannel)
				MBYTE(logchannel)
				MBYTE(chanelType)
				MBYTE(isconnetplat)
			ARRAY_END()
			CASE_END
		CASE(0x0077)
			CASE_START
			MBYTE(sumtotal)
			ARRAY_START(list, sumtotal)
				MBYTE(logchannel)
				MBYTE(mode)
				MBYTE(resolution)
				MWORD(keyinterval)
				MBYTE(fps)
				MDWORD(rate)
				MBYTE(recmode)
				MBYTE(recresolution)
				MDWORD(reckeyInterval)
				MBYTE(recfps)
				MDWORD(recrate)
				MBITS(WORD, "osdsetting", 7)
			ARRAY_END()
			CASE_END
		CASE(0x0079)
			CASE_START
			MBYTE(ratio)
			MBYTE(period)
			MBYTE(starttime)
			CASE_END
		CASE(0x007B)
			CASE_START
			MBYTE(number)
			MBYTE(period)
			CASE_END
		CASE(0x007C)
			CASE_START
			MBITS(BYTE, "mode", 3)
			MBITS(BYTE, "type", 3)
			MBITS(BYTE, "timing", 7)
			SUBOBJ_START(list)
				MBITS(BYTE, "flag", 4)
				MBCD(time1start, 2)
				MBCD(time1end, 2)
				MBCD(time2start, 2)
				MBCD(time2end, 2)
				MBCD(time3start, 2)
				MBCD(time3end, 2)
				MBCD(time4start, 2)
				MBCD(time4end, 2)
			SUBOBJ_END()
			CASE_END
		SWITCH_END()
	ARRAY_END()
CMD_END()

CMD_START(0x8107)
CMD_END()

CMD_START(0x0107)
	MWORD(terminalType)
	MBYTES(manufacturer, 5)
	MBYTES(model, 20)
	MBYTES(terminalId, 7)
	MBCD(iccid, 10)
	MBYTE(hardwareEditionLen)
	MSTRING(hardwareEdition, hardwareEditionLen)
	MBYTE(softwareEdtionLen)
	MSTRING(softwareEdition, softwareEdtionLen)
	MBITS(BYTE, 4, "enableGPS", "enableBeiDou", "enableGLONASS", "enableGalileo")
	MBITS(BYTE, 8, "enableGPRS", "enableCDMA", "enableTDSCDMA", "enableWCDMA", "enableCDMA2000", "enableTDLTE", nullptr, "enableOtherComm")
CMD_END()

CMD_START(0x8108)
	MBYTE(upgradeType)
	MBYTES(manufacturer, 5)
	MBYTE(editionLen)
	MSTRING(edition, editionLen)
	MDWORD(payloadLen)
	MBYTES(payload, payloadLen)
CMD_END()

CMD_START(0x0108)
	MBYTE(upgradeType)
	MBYTE(result)
CMD_END()

CMD_START(0x0200)
	MBITS(DWORD, "alarmFlag", 32)
	MBITS(DWORD, "statusFlag", 22)
	MDWORD(latitude)
	MDWORD(longitude)
	MWORD(high)
	MWORD(speed)
	MWORD(direction)
	MBCD(time, 6)
	OPTION_START()
		ARRAY_START(-1)
			MBYTE(extraID)
			MBYTE(extraLen)
			SWITCH_START(extraID)
			CASE(0x01)
				CASE_START
				MDWORD(mileage)
				CASE_END
			CASE(0x02)
				CASE_START
				MWORD(oil)
				CASE_END
			CASE(0x03)
				CASE_START
				MWORD(speed)
				CASE_END
			CASE(0x04)
				CASE_START
				MWORD(alarmID)
				CASE_END
			CASE(0x11)
				CASE_START
				MBYTE(directionType)
				OPTION_START()
					MDWORD(areaID)
				OPTION_END()
				CASE_END
			CASE(0x12)
				CASE_START
				MBYTE(directionType)
				MDWORD(areaID)
				MBYTE(direction)
				CASE_END
			CASE(0x13)
				CASE_START
				MDWORD(areaID)
				MWORD(time)
				MBYTE(result)
				CASE_END
			CASE(0x25)
				CASE_START
				MBITS(DWORD, "signalStat", 15)
				CASE_END
			CASE(0x2A)
				CASE_START
				MBITS(WORD, "ioFlag", 2)
				CASE_END
			CASE(0x2B)
				CASE_START
				MWORD(AD0)
				MWORD(AD1)
				CASE_END
			CASE(0x30)
				CASE_START
				MBYTE(signalLevel)
				CASE_END
			CASE(0x31)
				CASE_START
				MBYTE(GNSSNum)
				CASE_END
			SWITCH_END()
		ARRAY_END()
	OPTION_END()
CMD_END()

CMD_START(0x8201)
CMD_END()

CMD_START(0x0201)
	MWORD(seqR)
	MBITS(DWORD, "alarmFlag", 32)
	MBITS(DWORD, "statusFlag", 22)
	MDWORD(latitude)
	MDWORD(longitude)
	MWORD(high)
	MWORD(speed)
	MWORD(direction)
	MBCD(time, 6)
	OPTION_START()
		ARRAY_START(-1)
			MBYTE(extraID)
			MBYTE(extraLen)
			SWITCH_START(extraID)
			CASE(0x01)
				CASE_START
				MDWORD(mileage)
				CASE_END
			CASE(0x02)
				CASE_START
				MWORD(oil)
				CASE_END
			CASE(0x03)
				CASE_START
				MWORD(speed)
				CASE_END
			CASE(0x04)
				CASE_START
				MWORD(alarmID)
				CASE_END
			CASE(0x11)
				CASE_START
				MBYTE(directionType)
				OPTION_START()
					MDWORD(areaID)
				OPTION_END()
				CASE_END
			CASE(0x12)
				CASE_START
				MBYTE(directionType)
				MDWORD(areaID)
				MBYTE(direction)
				CASE_END
			CASE(0x13)
				CASE_START
				MDWORD(areaID)
				MWORD(time)
				MBYTE(result)
				CASE_END
			CASE(0x25)
				CASE_START
				MBITS(DWORD, "signalStat", 15)
				CASE_END
			CASE(0x2A)
				CASE_START
				MBITS(WORD, "ioFlag", 2)
				CASE_END
			CASE(0x2B)
				CASE_START
				MWORD(AD0)
				MWORD(AD1)
				CASE_END
			CASE(0x30)
				CASE_START
				MBYTE(signalLevel)
				CASE_END
			CASE(0x31)
				CASE_START
				MBYTE(GNSSNum)
				CASE_END
			SWITCH_END()
		ARRAY_END()
	OPTION_END()
CMD_END()

CMD_START(0x8202)
	MWORD(interval)
	MDWORD(expire)
CMD_END()

CMD_START(0x8203)
	MWORD(alarmSeq)
	MBITS(WORD, 4, "emergency", nullptr, nullptr, "danger")
	MBITS(WORD, 13, nullptr, nullptr, nullptr, nullptr, "areaAlarm", "lineAlarm", "lengthAlarm", nullptr, nullptr, nullptr, nullptr, "ignitionAlarm", "moveAlarm")
CMD_END()

CMD_START(0x8300)
	MBITS(BYTE, 6, "emergency", nullptr, "lcd", "tts", "adv", "action")
	MSTRING(content)
CMD_END()

CMD_START(0x8301)
	MBYTE(type)
	MBYTE(count)
	ARRAY_START(items, count)
		MBYTE(id)
		MBYTE(len)
		MSTRING(content)
	ARRAY_END()
CMD_END()

CMD_START(0x0301)
	MBYTE(eventID)
CMD_END()

CMD_START(0x8302)
	MBITS(BYTE, 5, "tag0", nullptr, nullptr, "tag3", "tag4")
	MBYTE(len)
	MSTRING(content)
	ARRAY_START(items, count)
		MBYTE(id)
		MWORD(len)
		MSTRING(content)
	ARRAY_END()
CMD_END()

CMD_START(0x0302)
	MWORD(seqR)
	MBYTE(answerID)
CMD_END()

CMD_START(0x8303)
	MBYTE(confType)
	MBYTE(count)
	ARRAY_START(items, cunt)
		MBYTE(infoType)
		MWORD(infoLen)
		MSTRING(info)
	ARRAY_END()
CMD_END()

CMD_START(0x0303)
	MBYTE(infoType)
	MBYTE(enablePlay)
CMD_END()

CMD_START(0x8304)
	MBYTE(infoType)
	MWORD(infoLen)
	MSTRING(info)
CMD_END()

CMD_START(0x8305)
CMD_END()

CMD_START(0x8400)
	MBYTE(mark)
	MSTRING(tel)
CMD_END()

CMD_START(0x8401)
	MBYTE(type)
	MBYTE(count)
	ARRAY_START(items, count)
		MBYTE(tag)
		MBYTE(telLen)
		MSTRING(tel)
		MBYTE(ctcLen)
		MSTRING(contact)
	ARRAY_END()
CMD_END()

CMD_START(0x8500)
	MBITS(BYTE, 1, "ctrl")
CMD_END()

CMD_START(0x0500)
	MWORD(seqR)
	MBITS(DWORD, "alarmFlag", 32)
	MBITS(DWORD, "statusFlag", 22)
	MDWORD(latitude)
	MDWORD(longitude)
	MWORD(high)
	MWORD(speed)
	MWORD(direction)
	MBCD(time, 6)
	OPTION_START()
		ARRAY_START(-1)
			MBYTE(extraID)
			MBYTE(extraLen)
			SWITCH_START(extraID)
			CASE(0x01)
				CASE_START
				MDWORD(mileage)
				CASE_END
			CASE(0x02)
				CASE_START
				MWORD(oil)
				CASE_END
			CASE(0x03)
				CASE_START
				MWORD(speed)
				CASE_END
			CASE(0x04)
				CASE_START
				MWORD(alarmID)
				CASE_END
			CASE(0x11)
				CASE_START
				MBYTE(directionType)
				OPTION_START()
					MDWORD(areaID)
				OPTION_END()
				CASE_END
			CASE(0x12)
				CASE_START
				MBYTE(directionType)
				MDWORD(areaID)
				MBYTE(direction)
				CASE_END
			CASE(0x13)
				CASE_START
				MDWORD(areaID)
				MWORD(time)
				MBYTE(result)
				CASE_END
			CASE(0x25)
				CASE_START
				MBITS(DWORD, "signalStat", 15)
				CASE_END
			CASE(0x2A)
				CASE_START
				MBITS(WORD, "ioFlag", 2)
				CASE_END
			CASE(0x2B)
				CASE_START
				MWORD(AD0)
				MWORD(AD1)
				CASE_END
			CASE(0x30)
				CASE_START
				MBYTE(signalLevel)
				CASE_END
			CASE(0x31)
				CASE_START
				MBYTE(GNSSNum)
				CASE_END
			SWITCH_END()
		ARRAY_END()
	OPTION_END()
CMD_END()

CMD_START(0x8600)
	MBYTE(update)
	MBYTE(count)
	ARRAY_START(items, count)
		MDWORD(areaId)
		MBITS(WORD, "attr", 10)
		MDWORD(lat)
		MDWORD(lng)
		MDWORD(radius)
		MBCD(timeS, 6)
		MBCD(timeE, 6)
		MWORD(speedLimit)
		MBYTE(speedTime)
		MSTRING(ddalertinfo)
		MSTRING(driveralertinfo)
	ARRAY_END()
CMD_END()

CMD_START(0x8601)
	MBYTE(count)
	ARRAY(DWORD, items, count)
CMD_END()

CMD_START(0x8602)
	MBYTE(update)
	MBYTE(count)
	ARRAY_START(items, count)
		MDWORD(areaId)
		MBITS(WORD, "attr", 8)
		MDWORD(latlt)
		MDWORD(lnglt)
		MDWORD(latrb)
		MDWORD(lngrb)
		MBCD(timeS, 6)
		MBCD(timeE, 6)
		MWORD(speedLimit)
		MBYTE(speedTime)
	ARRAY_END()
CMD_END()

CMD_START(0x8603)
	MBYTE(count)
	ARRAY(DWORD, items, count)
CMD_END()

CMD_START(0x8604)
	MDWORD(areaId)
	MBITS(WORD, "attr", 8)
	MBCD(timeS, 6)
	MBCD(timeE, 6)
	MWORD(speedLimit)
	MBYTE(speedTime)
	MWORD(count)
	ARRAY_START(items, count)
		MDWORD(lat)
		MDWORD(lng)
	ARRAY_END()
CMD_END()

CMD_START(0x8605)
	MBYTE(count)
	ARRAY(DWORD, items, count)
CMD_END()

CMD_START(0x8606)
	MDWORD(lineID)
	MBITS(WORD, 6, "asTime", nullptr, "enterAlarm", "enterAlarmPlatform", "exitAlarm", "exitAlarmPlatform")
	MBCD(timeS, 6)
	MBCD(timeE, 6)
	MWORD(turnNum)
	ARRAY_START(items, turnNum)
		MDWORD(turnID)
		MDWORD(areaID)
		MDWORD(latitude)
		MDWORD(longitude)
		MBYTE(width)
		MBITS(BYTE, 4, "time", "speedLimit", "south", "east")
	ARRAY_END()
	MWORD(highLimit)
	MWORD(lowLimit)
	MWORD(speedLimit)
	MBYTE(speedTime)
CMD_END()

CMD_START(0x8607)
	MBYTE(count)
	ARRAY(DWORD, areaID, count)
CMD_END()

CMD_START(0x0701)
	MDWORD(size)
	MBYTES(content, size)
CMD_END()

CMD_START(0x8702)
CMD_END()

CMD_START(0x0702)
	MBYTE(status)
	MBCD(time, 6)
	MBYTE(ICReadResult)
	MBYTE(nameLen)
	MSTRING(name)
	MBYTES(cardID, 20)
	MBYTE(agentLen)
	MSTRING(agent)
	MBCD(timeLimit, 4)
CMD_END()

CMD_START(0x0704)
	MWORD(count)
	MBYTE(type)
	ARRAY_START(items, count)
		MWORD(payloadLen)
		MBYTES(payload, payloadLen)
	ARRAY_END()
CMD_END()

CMD_START(0x0705)
	MWORD(count)
	MBCD(recvTime, 5)
	MBYTES(canID, 4)
	MBYTES(canData, 8)
CMD_END()

CMD_START(0x0800)
	MDWORD(mediaID)
	MBYTE(mediaType)
	MBYTE(mediaCodec)
	MBYTE(eventID)
	MBYTE(channel)
CMD_END()

CMD_START(0x0801)
	MDWORD(mediaID)
	MBYTE(mediaType)
	MBYTE(mediaCodec)
	MBYTE(eventID)
	MBYTE(channel)
	MBITS(DWORD, "alarmFlag", 32)
	MBITS(DWORD, "statusFlag", 22)
	MDWORD(latitude)
	MDWORD(longitude)
	MWORD(high)
	MWORD(speed)
	MWORD(direction)
	MBCD(time, 6)
	OPTION_START()
		ARRAY_START(-1)
			MBYTE(extraID)
			MBYTE(extraLen)
			SWITCH_START(extraID)
			CASE(0x01)
				CASE_START
				MDWORD(mileage)
				CASE_END
			CASE(0x02)
				CASE_START
				MWORD(oil)
				CASE_END
			CASE(0x03)
				CASE_START
				MWORD(speed)
				CASE_END
			CASE(0x04)
				CASE_START
				MWORD(alarmID)
				CASE_END
			CASE(0x11)
				CASE_START
				MBYTE(directionType)
				OPTION_START()
					MDWORD(areaID)
				OPTION_END()
				CASE_END
			CASE(0x12)
				CASE_START
				MBYTE(directionType)
				MDWORD(areaID)
				MBYTE(direction)
				CASE_END
			CASE(0x13)
				CASE_START
				MDWORD(areaID)
				MWORD(time)
				MBYTE(result)
				CASE_END
			CASE(0x25)
				CASE_START
				MBITS(DWORD, "signalStat", 15)
				CASE_END
			CASE(0x2A)
				CASE_START
				MBITS(WORD, "ioFlag", 2)
				CASE_END
			CASE(0x2B)
				CASE_START
				MWORD(AD0)
				MWORD(AD1)
				CASE_END
			CASE(0x30)
				CASE_START
				MBYTE(signalLevel)
				CASE_END
			CASE(0x31)
				CASE_START
				MBYTE(GNSSNum)
				CASE_END
			SWITCH_END()
		ARRAY_END()
	OPTION_END()
CMD_END()

CMD_START(0x0805)
	MWORD(seqR)
	MBYTE(result)
	MWORD(count)
	ARRAY(DWORD, list, count)
CMD_END()

CMD_START(0x8800)
	MDWORD(dataId)
	MBYTE(packetSum)
	ARRAY(WORD, ids, packetSum)
CMD_END()

CMD_START(0x8801)
	MBYTE(channel)
	MWORD(cmd)
	MWORD(inv)
	MBYTE(save)
	MBYTE(screen)
	MBYTE(quality)
	MBYTE(bright)
	MBYTE(contrast)
	MBYTE(saturation)
	MBYTE(chroma)
CMD_END()

CMD_START(0x8802)
	MBYTE(type)
	MBYTE(id)
	MBYTE(fmt)
	MBCD(starttime, 6)
	MBCD(endtime ,6)
CMD_END()

CMD_START(0x8803)
	MBYTE(type)
	MBYTE(id)
	MBYTE(fmt)
	MBCD(starttime, 6)
	MBCD(endtime, 6)
	MBYTE(flg)
CMD_END()

CMD_START(0x8900)
	MBYTE(type)
	MSTRING(data)
CMD_END()

CMD_START(0x8D00)
CMD_END()

CMD_START(0x8D01)
	OPTONAL_START()
		MBYTE(filetype)
		MBCD(version, 3)
		MSTRING(filename)
		MBYTE(windowsize)
	OPTIONAL_END()
	MBYTE(verify)
	MBYTES(filedata)
CMD_END()

CMD_START(0x8D02)
	MBYTE(keytype)
	MWORD(uuid)
CMD_END()

CMD_START(0x8D03)
	MBYTE(keytype)
	MBYTES(keydata, 33)
	MBCD(version, 7)
	MDWORD(sno)
	MBYTE(state)
CMD_END()

CMD_START(0x9003)
CMD_END()

CMD_START(0x9101)
	MBYTE(ipLength)
	MSTRING(ip)
	MWORD(tcpPort)
	MWORD(udpPort)
	MBYTE(logicChannel)
	MBYTE(type)
	MBYTE(streamType)
CMD_END()

CMD_START(0x9102)
	MBYTE(logicChannel)
	MBYTE(command)
	MBYTE(closeType)
	MBYTE(switchType)
CMD_END()

CMD_START(0x9105)
	MBYTE(logicChannel)
	MBYTE(lostRatio)
CMD_END()

CMD_START(0x9201)
	MBYTE(ipLength)
	MSTRING(ip)
	MWORD(tcpPort)
	MWORD(udpPort)
	MBYTE(logicChannel)
	MBYTE(type)
	MBYTE(streamType)
	MBYTE(memoryType)
	MBYTE(playBackType)
	MBYTE(playBackRatio)
	MBCD(startTime, 6)
	MBCD(endTime, 6)
CMD_END()

CMD_START(0x9202)
	MBYTE(logicChannel)
	MBYTE(command)
	MBYTE(playBackRatio)
	MBCD(fastPoistion, 6)
CMD_END()

CMD_START(0x9205)
	MBYTE(logicChannel)
	MBCD(startTime ,6)
	MBCD(endTime, 6)
	MBITS(QWORD, "a", 64)
	MBYTE(mediaType)
	MBYTE(streamType)
	MBYTE(memoryType)
CMD_END()

CMD_START(0x9206)
	MBYTE(ipLength)
	MSTRING(ip, ipLength)
	MWORD(Port)
	MBYTE(userLength)
	MSTRING(username, userLength)
	MBYTE(passLength)
	MSTRING(passWord, passLength)
	MBYTE(pootLength)
	MSTRING(poot, pootLength)
	MBYTE(logicChannel)
	MBCD(startTime, 6)
	MBCD(endTime, 6)
	MQWORD(alarmFlag)
	MBYTE(mediaType)
	MBYTE(streamType)
	MBYTE(recordPosition)
	MBYTE(taskOp)
CMD_END()

CMD_START(0x9207)
	MWORD(seqNumber)
	MBYTE(control)
CMD_END()

CMD_START(0x9301)
	MBYTE(logicChannel)
	MBYTE(direction)
	MBYTE(speed)
CMD_END()

CMD_START(0x9302)
	MBYTE(logicChannel)
	MBYTE(direction)
CMD_END()

CMD_START(0x9303)
	MBYTE(logicChannel)
	MBYTE(direction)
CMD_END()

CMD_START(0x9304)
	MBYTE(logicChannel)
	MBYTE(direction)
CMD_END()

CMD_START(0x9305)
	MBYTE(logicChannel)
	MBYTE(direction)
CMD_END()

CMD_START(0x9306)
	MBYTE(logicChannel)
	MBYTE(direction)
CMD_END()

