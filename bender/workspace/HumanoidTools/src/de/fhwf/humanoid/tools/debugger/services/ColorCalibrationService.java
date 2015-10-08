package de.fhwf.humanoid.tools.debugger.services;

import java.io.IOException;

import javax.swing.event.EventListenerList;

import de.fhwf.humanoid.tools.debugger.connection.IConnector;
import de.fhwf.humanoid.tools.debugger.connection.PacketEvent;
import de.fhwf.humanoid.tools.debugger.connection.PacketListener;
import de.fhwf.humanoid.tools.debugger.data.LookUpTable;
import de.fhwf.humanoid.tools.debugger.events.ColorClickEvent;
import de.fhwf.humanoid.tools.debugger.events.ColorClickListener;
import de.fhwf.humanoid.tools.debugger.events.ColorSelectChangeEvent;
import de.fhwf.humanoid.tools.debugger.events.ColorSelectChangeListener;
import de.fhwf.humanoid.tools.debugger.events.MarkedColorsChangedEvent;
import de.fhwf.humanoid.tools.debugger.events.MarkedColorsChangedListener;

public class ColorCalibrationService implements IService, PacketListener {
	public static final String SERVICENAME = "ColorCalibrationService";
	private static final int TIMEOUT = 3000; // ms

	public enum ColorID {
	  Orange,
	  Yellow,
	  Blue,
	  Green,
	  White,
	  Cyan,
	  Magenta,
	  Black
	};
	
	private enum CALState {
    IDLE, WAITINGFORDATA,GOTFILENAME, GOTDATA, ERROR
	}

	private IConnector mConnector;
	private CALState mState;
	private LookUpTable mTable;
	private FileService mFileProcessor;
	private String mFilename;
	
	private EventListenerList mListeners = new EventListenerList();

	public ColorCalibrationService(IConnector connector, FileService fileProcessor) {
		mConnector = connector;
		mState = CALState.IDLE;
		mTable = new LookUpTable(this);
		mFileProcessor = fileProcessor;
		mFilename = "";
	}
	
	public void getColorCalibration() throws IOException {
		if (mState != CALState.IDLE) {
			throw new IllegalStateException("The last ColorCalibration request is not yet completed!");
		}
		mState = CALState.WAITINGFORDATA;
		
		String cmd = "GET ";
		cmd += "\0";
		mConnector.sendPacket(getPacketID(), cmd);

		long startTime = System.currentTimeMillis();
		do {
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
			}
		} while ((mState != CALState.GOTFILENAME && mState != CALState.ERROR) && (System.currentTimeMillis() - startTime) < TIMEOUT);
		if (mState != CALState.GOTFILENAME && mState != CALState.ERROR) {
			mState = CALState.IDLE;
			throw new IOException("The ColorCalibration request timed out!");
		}
		
		try {
			byte[] colorCalibrationData = mFileProcessor.getFile(mFilename);
			mTable.updateLookUpTable(colorCalibrationData);
		} finally {
			mState = CALState.IDLE;
		}
	}

	public void sendColorCalibration() {
		if (mState != CALState.IDLE) {
			throw new IllegalStateException("The last color request is not yet completed!");
		}	
		try {
			mFileProcessor.writeFile("/mnt/sdcard/YUVTable.txt", mTable.getByteArray());
			String cmd = "SET ";
			cmd += "\0";
			mConnector.sendPacket(getPacketID(), cmd);
			mState = CALState.IDLE;
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}	
	}

	@Override
	public void newPacket(PacketEvent e) {
		String reply = e.getPacket();
		if (reply.substring(0, 5).equals("READY")) {
			if (mState == CALState.WAITINGFORDATA) {
				mFilename = reply.substring(6);
				mState = CALState.GOTFILENAME;
			}
		}
	}
	
	/**
	 * Get the  LookUpTable
	 * 
	 * @return
	 */
	public LookUpTable getLookUpTable(){
		return mTable;
	}
	
	
	@Override
	public String getPacketID() {
		return "CCL";
	}

	@Override
	public String getServiceName() {
		return SERVICENAME;
	}
	
	@Override
	public boolean supportsMultipleSources() {
		return false;
	}

	public void addColorClickListener(ColorClickListener listener) {
		mListeners.add(ColorClickListener.class, listener);
	}

	public void removeColorClickListener(ColorClickListener listener) {
		mListeners.remove(ColorClickListener.class, listener);
	}

	public synchronized void notifyColorClick(ColorClickEvent event) {
		for (ColorClickListener l : mListeners.getListeners(ColorClickListener.class)) {
			l.colorClicked(event);
		}
	}
	
	public void addColorSelectChangeListener(ColorSelectChangeListener listener) {
		mListeners.add(ColorSelectChangeListener.class, listener);
	}

	public void removeColorSelectChangeListener(ColorSelectChangeListener listener) {
		mListeners.remove(ColorSelectChangeListener.class, listener);
	}

	public synchronized void notifyColorSelectChange(ColorSelectChangeEvent event) {
		for (ColorSelectChangeListener l : mListeners.getListeners(ColorSelectChangeListener.class)) {
			l.ColorSelectChange(event);
		}
	}
	
	public void addMarkedColorsChangeListener(MarkedColorsChangedListener listener) {
		mListeners.add(MarkedColorsChangedListener.class, listener);
	}

	public void removeMarkedColorsChangeListener(MarkedColorsChangedListener listener) {
		mListeners.remove(MarkedColorsChangedListener.class, listener);
	}

	public synchronized void notifyMarkedColorsChange(MarkedColorsChangedEvent event) {
		for (MarkedColorsChangedListener l : mListeners.getListeners(MarkedColorsChangedListener.class)) {
			l.MarkedColorsChanged(event);
		}
	}
}
