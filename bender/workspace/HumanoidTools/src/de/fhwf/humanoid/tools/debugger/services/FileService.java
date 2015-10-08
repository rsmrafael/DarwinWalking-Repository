package de.fhwf.humanoid.tools.debugger.services;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.UnsupportedEncodingException;
import de.fhwf.humanoid.tools.debugger.connection.IConnector;
import de.fhwf.humanoid.tools.debugger.connection.PacketEvent;
import de.fhwf.humanoid.tools.debugger.connection.PacketListener;

public class FileService implements IService, PacketListener {	
	public static final String SERVICENAME = "FileService";
	private static final int RRQ = 1; // Read request
	private static final int WRQ = 2; // Write request
	private static final int ACK = 3; // Acknowledgment
	private static final int DATA = 4; // Data
	private static final int ERROR = 5; // Error

	public enum TFTPState {
    IDLE, RECEIVING, SENDING, COMPLETE, ERROR 
	}
	
	private static final int TIMEOUT = 4000; // ms
	private static final int MAX_REPEAT_CNT = 3;
	
	private IConnector mConnector;
	private TFTPState mState;
	private String mLastErrorMessage;
	private ByteArrayOutputStream mData;
	private byte[] mDataToSend;
	private long mLastDataPacket;
	private byte[] mLastPacket;
	private int mPacketRepeatCnt;
	
	public FileService(IConnector connector) {
		mConnector = connector;
		mState = TFTPState.IDLE;
	}
	
	public byte[] getFile(String filename) throws IOException {
		if (mState != TFTPState.IDLE && mState != TFTPState.ERROR) {
			throw new IllegalStateException("The last transfer is not yet completed!");
		}
		mState = TFTPState.RECEIVING;
		mData = new ByteArrayOutputStream();  
		mPacketRepeatCnt = 0;
		sendRRQ(filename, "binary");
		mLastDataPacket = System.currentTimeMillis();
		do {
  		do {
  			try {
  				Thread.sleep(100);
  			} catch (InterruptedException e) {
  			}
  		} while ((mState != TFTPState.COMPLETE && mState != TFTPState.ERROR) && (System.currentTimeMillis() - mLastDataPacket) < TIMEOUT);
  		if (mState == TFTPState.ERROR) {
  			mState = TFTPState.IDLE;
  			throw new IOException("The server returned an error: " + mLastErrorMessage);
  		}
  		if (mState != TFTPState.COMPLETE && mState != TFTPState.ERROR) {
  			if (mPacketRepeatCnt > MAX_REPEAT_CNT) {
  				mState = TFTPState.IDLE;
  				throw new IOException("The transfer timed out!");
  			} else {
  				System.out.println("[FileService] Last packet timed out, RepeatCount=" + mPacketRepeatCnt);
  				mPacketRepeatCnt++;
  				mConnector.sendPacket(getPacketID(), mLastPacket);
  				mLastDataPacket = System.currentTimeMillis();
  			}
  		}
  		break;
		} while (true);
		mState = TFTPState.IDLE;
		return mData.toByteArray();
	}

 public void writeFile(String filename, File sourceFile) throws IOException {
  InputStream is = new FileInputStream(sourceFile);
  
  long length = sourceFile.length();
  byte[] bytes = new byte[(int)length];

  // Read in the bytes
  int offset = 0;
  int numRead = 0;
  while (offset < bytes.length
         && (numRead=is.read(bytes, offset, bytes.length-offset)) >= 0) {
      offset += numRead;
  }
  is.close();
  
  writeFile(filename, bytes);
 }
 
	public void writeFile(String filename, byte[] data) throws IOException {
		if (mState != TFTPState.IDLE && mState != TFTPState.ERROR) {
			throw new IllegalStateException("The last transfer is not yet completed!");
		}
		mState = TFTPState.SENDING;
		mDataToSend = data;  
		mPacketRepeatCnt = 0;
		sendWRQ(filename, "binary");
		mLastDataPacket = System.currentTimeMillis();
		do {
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
			}
		} while ((mState != TFTPState.COMPLETE && mState != TFTPState.ERROR) && (System.currentTimeMillis() - mLastDataPacket) < TIMEOUT);
		if (mState == TFTPState.ERROR) {
			mState = TFTPState.IDLE;
			throw new IOException("The server returned an error: " + mLastErrorMessage);
		}
		if (mState != TFTPState.COMPLETE && mState != TFTPState.ERROR) {
			mState = TFTPState.IDLE;
			throw new IOException("The transfer timed out!");
		}
		mState = TFTPState.IDLE;
	}
	
	private void sendRRQ(String filename, String mode) {
		StringBuffer buf = new StringBuffer();
		buf.append((char)((RRQ >> 8) & (char)0xFF));
		buf.append((char)((RRQ) & (char)0xFF));
		buf.append(filename);
		buf.append((char)0);
		buf.append(mode);
		buf.append((char)0);
		
		mLastPacket = buf.toString().getBytes();
		mConnector.sendPacket(getPacketID(), mLastPacket);
	}

	private void sendWRQ(String filename, String mode) {
		StringBuffer buf = new StringBuffer();
		buf.append((char)((WRQ >> 8) & (char)0xFF));
		buf.append((char)((WRQ) & (char)0xFF));
		buf.append(filename);
		buf.append((char)0);
		buf.append(mode);
		buf.append((char)0);
		
		mLastPacket = buf.toString().getBytes();
		mConnector.sendPacket(getPacketID(), mLastPacket);
	}

	private void sendDATA(int blockNr, byte[] data) {
  ByteArrayOutputStream baos = new ByteArrayOutputStream(); 
  DataOutputStream out = new DataOutputStream( baos ); 
    
  try {
   out.writeShort(DATA);
   out.writeShort(blockNr);
   out.write(data);
   out.close();
  } catch (IOException e) {
  }

		mLastPacket = baos.toByteArray();
		mConnector.sendPacket(getPacketID(), mLastPacket);
	}

	private void sendACK(int blockNr) {
		ByteArrayOutputStream baos = new ByteArrayOutputStream(); 
		DataOutputStream out = new DataOutputStream( baos ); 
				
		try {
			out.writeShort(ACK);
			out.writeShort(blockNr);
			out.close();
		} catch (IOException e) {
		}
		
		mLastPacket = baos.toByteArray();
		mConnector.sendPacket(getPacketID(), mLastPacket);
	}

	@Override
	public void newPacket(PacketEvent e) {
		int opcode = ((e.getPacket().charAt(0) << 8) + (int)e.getPacket().charAt(1));
		
		switch (opcode) {
			case RRQ:
				break;
			case WRQ:
				break;
			case ACK:
				if (mState == TFTPState.SENDING) {
					mLastDataPacket = System.currentTimeMillis();
					mPacketRepeatCnt = 0;
					
					byte[] bytes = null;
					try {
						bytes = e.getPacket().getBytes("ISO-8859-1");
					} catch (UnsupportedEncodingException e3) {
						e3.printStackTrace();
					}
					
					ByteArrayInputStream bais = new ByteArrayInputStream(bytes);
					DataInputStream in = new DataInputStream(bais); 
					int blockNr = 0;
					try {
						blockNr = in.readShort();
						blockNr = in.readShort();
						in.close();
					} catch (IOException e2) {
					}
					int nextBlockNr = blockNr + 1;
					int size = 512;
					if (mDataToSend.length - (blockNr * 512) < 512) {
						size = mDataToSend.length - (blockNr * 512);
					}
					byte[] block = new byte[size];
					System.arraycopy(mDataToSend, blockNr * 512, block, 0, size);
					sendDATA(nextBlockNr, block);
					if (size < 512) {
						mState = TFTPState.COMPLETE;
					} else {
						mState = TFTPState.SENDING;
					}
				} else if (mState == TFTPState.COMPLETE) {
				} else {
					mState = TFTPState.ERROR;
				}
				break;
			case DATA:
				if (mState == TFTPState.RECEIVING) {
					mLastDataPacket = System.currentTimeMillis();
					mPacketRepeatCnt = 0;
					
					byte[] bytes = null;
					try {
						bytes = e.getPacket().getBytes("ISO-8859-1");
					} catch (UnsupportedEncodingException e3) {
						e3.printStackTrace();
					}
					
					ByteArrayInputStream bais = new ByteArrayInputStream(bytes);
					DataInputStream in = new DataInputStream(bais); 
					int blockNr = 0;
					try {
						blockNr = in.readShort();
						blockNr = in.readShort();
						in.close();
					} catch (IOException e2) {
					}
					try {
						mData.write(e.getPacket().substring(4).getBytes("ISO-8859-1"));
						sendACK(blockNr);
						if (e.getPacket().length() - 4 < 512) {
							mState = TFTPState.COMPLETE;
						}
					} catch (IOException e1) {
						e1.printStackTrace();
					}
				} else {
					mState = TFTPState.ERROR;
				}
				break;
			case ERROR:
				mState = TFTPState.ERROR;
				mLastErrorMessage = e.getPacket().substring(4, e.getPacket().length() - 1);
				break;
		}
	}

	@Override
	public String getPacketID() {
		return "FTP";
	}

	@Override
	public String getServiceName() {
  	return SERVICENAME;
	}
	
	@Override
	public boolean supportsMultipleSources() {
		return false;
	}
}