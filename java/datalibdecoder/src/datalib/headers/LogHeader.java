package datalib.headers;

import datalib.util.Print;

public class LogHeader implements PacketHeaderInterface {
	public static final byte HEADERBYTE = 'L';
	byte header;
	byte len;
	
	@Override
	public byte getHeaderCode() {
		return HEADERBYTE;
	}

	@Override
	public int getHeaderLength() {
		return 2;
	}

	@Override
	public int getPacketLength() {
		return(len);  // in these packets, the len is the TOTAL size of the packet !!!
	}

	@Override
	public int getBodyLength() {
		return (getPacketLength() - getHeaderLength());
	}
	
	@Override
	public boolean decode(byte array[]) {
		if (array[0] != HEADERBYTE)
			return (false);
		header = array[0];
		len = array[1];
		return(true);
	}
	
	@Override
	public String toString() {
		StringBuilder sb = new StringBuilder();
		sb.append(Print.printByteHex(header));
		sb.append(" ");
		sb.append(Print.printByteHex(len));
		return(sb.toString());
	}
	
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		LogHeader hdr = new LogHeader();
		byte test[] = { HEADERBYTE, 25 };
		if (hdr.decode(test)) {
			System.out.println("decoding success!");
			System.out.println("hex = "+hdr.toString());
			System.out.println("packet length = "+hdr.getPacketLength());
		} else {
			System.out.println("decoding failed!");
		}
	}
}
