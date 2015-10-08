package de.fhwf.humanoid.tools.debugger.services;

import java.io.IOException;

import de.fhwf.humanoid.tools.debugger.connection.IConnector;
import de.fhwf.humanoid.tools.debugger.connection.PacketEvent;
import de.fhwf.humanoid.tools.debugger.connection.PacketListener;
import de.fhwf.humanoid.tools.debugger.data.PIDParameters;

public class BodyCalibrationService implements IService, PacketListener {
	public static final String SERVICENAME = "BodyCalibrationService";
	private static final int TIMEOUT = 3000; // ms

	public enum PidID {
	  Inclination,
	  Walk
	};
	
	private enum CALState {
    IDLE, WAITINGFORDATA, GOTDATA, ERROR
	}

	private IConnector mConnector;
	private CALState mState;
	private PIDParameters mPIDParameters;

	public BodyCalibrationService(IConnector connector) {
		mConnector = connector;
		mState = CALState.IDLE;
	}	

	public PIDParameters getPIDParameters(PidID pid) throws IOException {
		if (mState != CALState.IDLE) {
			throw new IllegalStateException("The last body parameters request is not yet completed!");
		}
		mState = CALState.WAITINGFORDATA;
		String cmd = "GET ";
		String colorStr = pid.toString();
		cmd += colorStr.toUpperCase();
		for (int i = colorStr.length(); i <= 11; i++) {
			cmd += " ";
		}
		cmd += "\0";
		mConnector.sendPacket(getPacketID(), cmd);

		long startTime = System.currentTimeMillis();
		do {
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
			}
		} while ((mState != CALState.GOTDATA && mState != CALState.ERROR) && (System.currentTimeMillis() - startTime) < TIMEOUT);
		if (mState != CALState.GOTDATA && mState != CALState.ERROR) {
			mState = CALState.IDLE;
			throw new IOException("The body parameters request timed out!");
		}

		mState = CALState.IDLE;
		return mPIDParameters;
	}
	
	public void setPIDParameters(PidID pid, PIDParameters parameters) throws IOException {
		if (mState != CALState.IDLE) {
			throw new IllegalStateException("The last body parameters request is not yet completed!");
		}
		mState = CALState.WAITINGFORDATA;
		String cmd = "SET ";
		String colorStr = pid.toString();
		cmd += colorStr.toUpperCase();
		for (int i = colorStr.length(); i <= 11; i++) {
			cmd += " ";
		}
		cmd += parameters.toString();
		cmd += "\0";
		mConnector.sendPacket(getPacketID(), cmd);

		long startTime = System.currentTimeMillis();
		do {
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
			}
		} while ((mState != CALState.GOTDATA && mState != CALState.ERROR) && (System.currentTimeMillis() - startTime) < TIMEOUT);
		if (mState != CALState.GOTDATA && mState != CALState.ERROR) {
			mState = CALState.IDLE;
			throw new IOException("The body parameters set request timed out!");
		}

		mState = CALState.IDLE;
	}

	@Override
	public void newPacket(PacketEvent e) {
		String reply = e.getChoppedPacket();
		if (reply.substring(0, 5).equals("VALUE")) {
			if (mState == CALState.WAITINGFORDATA) {
				reply = reply.replace("  ", " ");
				reply = reply.replace("  ", " ");
				reply = reply.replace("  ", " ");
				String[] parts = reply.split(" ");
				mPIDParameters = PIDParameters.parseString(parts[2]);
				mState = CALState.GOTDATA;
			}
		} else if (reply.substring(0, 5).equals("READY")) {
			if (mState == CALState.WAITINGFORDATA) {
				mState = CALState.GOTDATA;
			}
		}
	}

	@Override
	public String getPacketID() {
		return "BCL";
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
