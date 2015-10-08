package de.fhwf.humanoid.tools.debugger.services;

import java.io.IOException;
import com.google.protobuf.InvalidProtocolBufferException;
import de.fhwf.humanoid.tools.debugger.connection.IConnector;
import de.fhwf.humanoid.tools.debugger.connection.PacketEvent;
import de.fhwf.humanoid.tools.debugger.connection.PacketListener;
import de.ostfalia.robocup.message.MsgBlackboard.Blackboard;

public class BlackboardService implements IService, PacketListener {
	public static final String SERVICENAME = "BlackboardService";
	private static final int TIMEOUT = 3000; // ms

	private enum DataState {
		IDLE, WAITINGFORDATA, GOTDATA, GOTFILENAME, ERROR
	}

	private IConnector mConnector;
	private FileService mFileProcessor;
	private DataState mState;
	private Blackboard mBlackboard;
	private String mFilename;

	public BlackboardService(IConnector connector, FileService fileProcessor) {
		mConnector = connector;
		mFileProcessor = fileProcessor;
		mFilename = "";
		mState = DataState.IDLE;
	}

	public Blackboard getTOC() throws IOException {
		if (mState != DataState.IDLE) {
			throw new IllegalStateException(
					"The last blackboard request is not yet completed!");
		}
		mState = DataState.WAITINGFORDATA;
		String cmd = "LST";
		cmd += "\0";
		mConnector.sendPacket(getPacketID(), cmd);

		long startTime = System.currentTimeMillis();
		do {
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
			}
		} while ((mState != DataState.GOTDATA && mState != DataState.ERROR)
				&& (System.currentTimeMillis() - startTime) < TIMEOUT);
		if (mState != DataState.GOTDATA && mState != DataState.ERROR) {
			mState = DataState.IDLE;
			throw new IOException("The blackboard request timed out!");
		}

		mState = DataState.IDLE;
		return mBlackboard;
	}

	public Blackboard getAll() throws IOException {
		if (mState != DataState.IDLE) {
			throw new IllegalStateException(
					"The last blackboard request is not yet completed!");
		}
		mState = DataState.WAITINGFORDATA;
		String cmd = "GET ";
		cmd += "ALL";
		cmd += "\0";
		mConnector.sendPacket(getPacketID(), cmd);

		long startTime = System.currentTimeMillis();
		do {
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
			}
		} while ((mState != DataState.GOTFILENAME && mState != DataState.ERROR)
				&& (System.currentTimeMillis() - startTime) < TIMEOUT);
		if (mState != DataState.GOTFILENAME && mState != DataState.ERROR) {
			mState = DataState.IDLE;
			throw new IOException("The blackboard request timed out!");
		}

		try {
			byte[] blackboardData = mFileProcessor.getFile(mFilename);
			try {
				mBlackboard = Blackboard.parseFrom(blackboardData);
				return mBlackboard;
			} catch (InvalidProtocolBufferException e1) {
				throw new IOException("Received invalid Blackboard data: "
						+ e1.getMessage());
			}
		} finally {
			mState = DataState.IDLE;
		}
	}

	@Override
	public void newPacket(PacketEvent e) {
		String reply = e.getPacket();
		if (reply.substring(0, 4).equals("FILE")) {
			mFilename = reply.substring(5);
			mState = DataState.GOTFILENAME;
		} else {
			try {
				mBlackboard = Blackboard.parseFrom(reply.getBytes());
				mState = DataState.GOTDATA;
			} catch (InvalidProtocolBufferException e1) {
				e1.printStackTrace();
				mState = DataState.ERROR;
			}
		}
	}

	@Override
	public String getPacketID() {
		return "BLK";
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
