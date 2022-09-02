
void SendComData(std::list<BYTE> list_byte, BYTE* pData, int nLen) {

	for (int i = 0; i < nLen; i++) {
		pData[i] = list_byte.front();
		list_byte.pop_front();
	}

	for (int i = 0; i < nLen; i++) {
		printf("%02X ", pData[i]);
	}
}

// SendComData(list_data, pData, list_data.size());