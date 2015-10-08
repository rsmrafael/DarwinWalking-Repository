package de.fhwf.humanoid.tools.debugger.services;

import java.io.IOException;

import de.fhwf.humanoid.tools.debugger.connection.IConnector;
import de.fhwf.humanoid.tools.debugger.connection.PacketEvent;
import de.fhwf.humanoid.tools.debugger.connection.PacketListener;
import de.fhwf.humanoid.tools.debugger.data.IMUData;

public class BodyDataService implements IService, PacketListener {
	public static final String SERVICENAME = "BodyDataService";
	private static final int TIMEOUT = 300; // ms

	private enum DataState {
    IDLE, WAITINGFORDATA, GOTDATA, ERROR
	}

	private IConnector mConnector;
	private DataState mState;
	private IMUData mIMUData;
	private double mBatteryVoltage;

	public BodyDataService(IConnector connector) {
		mConnector = connector;
		mState = DataState.IDLE;
	}	

	public IMUData getIMUData() throws IOException {
		if (mState != DataState.IDLE) {
			throw new IllegalStateException("The last body data request is not yet completed!");
		}
		mState = DataState.WAITINGFORDATA;
		String cmd = "GET ";
		cmd += "IMU";
		cmd += "\0";
		mConnector.sendPacket(getPacketID(), cmd);

		long startTime = System.currentTimeMillis();
		do {
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
			}
		} while ((mState != DataState.GOTDATA && mState != DataState.ERROR) && (System.currentTimeMillis() - startTime) < TIMEOUT);
		if (mState != DataState.GOTDATA && mState != DataState.ERROR) {
			mState = DataState.IDLE;
			throw new IOException("The body data request timed out!");
		}

		mState = DataState.IDLE;
		return mIMUData;
	}

	public double getBatteryVoltage() throws IOException {
		if (mState != DataState.IDLE) {
			throw new IllegalStateException("The last body data request is not yet completed!");
		}
		mState = DataState.WAITINGFORDATA;
		String cmd = "GET ";
		cmd += "BAT";
		cmd += "\0";
		mConnector.sendPacket(getPacketID(), cmd);

		long startTime = System.currentTimeMillis();
		do {
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
			}
		} while ((mState != DataState.GOTDATA && mState != DataState.ERROR) && (System.currentTimeMillis() - startTime) < TIMEOUT);
		if (mState != DataState.GOTDATA && mState != DataState.ERROR) {
			mState = DataState.IDLE;
			throw new IOException("The body data request timed out!");
		}

		mState = DataState.IDLE;
		return mBatteryVoltage;
	}

	public void setRemoteActive(boolean state) throws IOException {
		if (mState != DataState.IDLE) {
			throw new IllegalStateException("The last body data request is not yet completed!");
		}
		mState = DataState.WAITINGFORDATA;
		String cmd = "SET ";
		cmd += "ACT ";
		cmd += state ? "ENABLE" : "DISABLE";
		cmd += "\0";
		mConnector.sendPacket(getPacketID(), cmd);
		mState = DataState.IDLE;
	}

	public void setVisionActive(boolean state) throws IOException {
		if (mState != DataState.IDLE) {
			throw new IllegalStateException("The last body data request is not yet completed!");
		}
		mState = DataState.WAITINGFORDATA;
		String cmd = "SET ";
		cmd += "VAC ";
		cmd += state ? "ENABLE" : "DISABLE";
		cmd += "\0";
		mConnector.sendPacket(getPacketID(), cmd);
		mState = DataState.IDLE;
	}

	public void sendMovementCommand(String command) throws IOException {
		if (mState != DataState.IDLE) {
			throw new IllegalStateException("The last body data request is not yet completed!");
		}
		mState = DataState.WAITINGFORDATA;
		String cmd = "SET ";
		cmd += "MOV ";
		cmd += command;
		cmd += "\0";
		mConnector.sendPacket(getPacketID(), cmd);
		mState = DataState.IDLE;
	}

	public void sendPanTiltCommand(int pan, int tilt) throws IOException {
		if (mState != DataState.IDLE) {
			throw new IllegalStateException("The last body data request is not yet completed!");
		}
		mState = DataState.WAITINGFORDATA;
		String cmd = "SET ";
		cmd += "P/T ";
		cmd += pan + "," + tilt;
		cmd += "\0";
		mConnector.sendPacket(getPacketID(), cmd);
		mState = DataState.IDLE;
	}

 public void sendWalkerCommand(int x, int y, int rotation, boolean turningAim) {
  if (mState != DataState.IDLE) {
   throw new IllegalStateException("The last body data request is not yet completed!");
  }
  mState = DataState.WAITINGFORDATA;
  String cmd = "SET ";
  cmd += "WLK ";
  cmd += Integer.toString(x) + "," + Integer.toString(y) + "," + Integer.toString(rotation) + "," + (turningAim ? "1" : "0");
  cmd += "\0";
  mConnector.sendPacket(getPacketID(), cmd);
  mState = DataState.IDLE;
 }

	@Override
	public void newPacket(PacketEvent e) {
		String reply = e.getChoppedPacket();
		if (reply.substring(0, 5).equals("VALUE")) {
			if (mState == DataState.WAITINGFORDATA) {
				reply = reply.replace("  ", " ");
				reply = reply.replace("  ", " ");
				reply = reply.replace("  ", " ");
				String[] parts = reply.split(" ");
				if (parts[1].equals("BAT")) {
					mBatteryVoltage = Double.parseDouble(parts[2]);
				} else if (parts[1].equals("IMU")) {
					mIMUData = IMUData.parseString(parts[2]);
				}
				mState = DataState.GOTDATA;
			}
		}
	}

	@Override
	public String getPacketID() {
		return "BDA";
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
