#define LIGHTSTONE_VID_PID_PAIRS_COUNT 2
unsigned int lightstone_vid_pid_pairs[LIGHTSTONE_VID_PID_PAIRS_COUNT][2] = { {0x0483, 0x0035}, {0x14FA, 0x0001} };

LIGHTSTONE_DECLSPEC unsigned int hex2dec(char *data, unsigned int len)
{
	unsigned int i;
	unsigned int value = 0;

	for (i = 0; i < len; i++) {
		value = value*0x10;
		if (data[i] >= '0' && data[i] <= '9')
			value += data[i]-'0';
		else if (data[i] >= 'A' && data[i] <= 'F')
			value += data[i]-'A' + 10;
		else
			return 0;
	}
	return value;
}

LIGHTSTONE_DECLSPEC int lightstone_get_count(lightstone* dev)
{
	int i;
	int count = 0;
	for(i = 0; i < LIGHTSTONE_VID_PID_PAIRS_COUNT; ++i)
	{
		count += lightstone_get_count_vid_pid(dev, lightstone_vid_pid_pairs[i][0], lightstone_vid_pid_pairs[i][1]);
	}
	return count;
}

LIGHTSTONE_DECLSPEC int lightstone_open(lightstone* dev, unsigned int index)
{
	int i;
	int count = 0;
	int internal_index = index;
	for(i = 0; i < LIGHTSTONE_VID_PID_PAIRS_COUNT; ++i)
	{
		// Dealing with the odd edge case of having a bunch of different lightstones hooked to the same system
		// I'd like to believe no one would ever do that, but, well...
		count = lightstone_get_count_vid_pid(dev, lightstone_vid_pid_pairs[i][0], lightstone_vid_pid_pairs[i][1]);
		if(count == 0) continue;
		if((count - 1) <= internal_index)
		{
			return lightstone_open_vid_pid(dev, internal_index, lightstone_vid_pid_pairs[i][0], lightstone_vid_pid_pairs[i][1]);
		}
		internal_index -= count;
	}
	return -1;
}

LIGHTSTONE_DECLSPEC int lightstone_valid(lightstone* d)
{	
	return d->_is_open;
}

LIGHTSTONE_DECLSPEC lightstone_info lightstone_get_info(lightstone* dev)
{
	lightstone_info ret;
	//hid_return t;
	ret.hrv = -1;
	ret.scl = -1;
	if (lightstone_valid(dev))
	{
		char rawAscii[300];
		unsigned char InputReport[256];
		char message_started = 0;
		int transferred = 0;
		int char_count = 0;
		int ii;

		while(1)
		{
			transferred = lightstone_read(dev, InputReport, 8);
			if(transferred == 0x8 || transferred == 0x9)
			{
				//printf("%d\n", InputReport[0]);
				for(ii = 1; ii < InputReport[0]+1; ++ii)
				{
					if(!message_started && InputReport[ii] != '<') continue;
					message_started = 1;
					if (InputReport[ii] != 10 && InputReport[ii] != 13)
					{
						rawAscii[char_count] = InputReport[ii];
						++char_count;
					}
					else if ( InputReport[ii] == 13 ) 
					{
						rawAscii[char_count] = 0;
						if ( strlen(rawAscii) > 18) 
						{
							ret.scl = ((float)(((hex2dec(rawAscii+5,2)) << 8) | (hex2dec(rawAscii+7,2)))) * .01;
							ret.hrv = ((float)(((hex2dec(rawAscii+10,2)) << 8) | (hex2dec(rawAscii+12,2)))) * .001;
							return ret;
						}
						message_started = 0;
						char_count = 0;
					}
				}
			}
		}
	}
	return ret;
}
