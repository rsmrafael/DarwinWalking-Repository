package de.fhwf.humanoid.tools.debugger.services;

import java.awt.image.BufferedImage;
import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.text.DateFormat;
import java.text.SimpleDateFormat;

import javax.imageio.ImageIO;
import javax.swing.event.EventListenerList;

import de.fhwf.humanoid.tools.debugger.connection.IConnector;
import de.fhwf.humanoid.tools.debugger.connection.PacketEvent;
import de.fhwf.humanoid.tools.debugger.connection.PacketListener;
import de.fhwf.humanoid.tools.debugger.data.YUVImage;
import de.fhwf.humanoid.tools.debugger.events.ColorConfigurationUpdateEvent;
import de.fhwf.humanoid.tools.debugger.events.ColorConfigurationUpdateListener;

public class ImageService implements IService, PacketListener {
	public static final String SERVICENAME = "ImageService";
	private static final int TIMEOUT = 5000; // ms

	public enum IMGState {
    IDLE, WAITINGFORPICTURE, GOTFILENAME, ERROR
	}

	private IConnector mConnector;
	private FileService mFileProcessor;
	private IMGState mState;
	private String mFilename;
	
	private EventListenerList mListeners = new EventListenerList();

	public ImageService(IConnector connector, FileService fileProcessor) {
		mConnector = connector;
		mFileProcessor = fileProcessor;
		mState = IMGState.IDLE;
		mFilename = "";
	}
	
	public YUVImage getImage(boolean rightCamera, boolean augmented, int downsamplingFactor, boolean yuvMode) throws IOException {
		if (mState != IMGState.IDLE) {
			throw new IllegalStateException("The last image request is not yet completed!");
		}
		mState = IMGState.WAITINGFORPICTURE;
		mFilename = "";
		String cmd = "GET ";
		if (augmented) {
			cmd += "AUG ";
		} else {
			cmd += "RAW ";
		}
		if (rightCamera) {
			cmd += "RGHT ";
		} else {
			cmd += "LEFT ";
		}
		if (yuvMode) {
			cmd += "YUV ";
		} else {
			cmd += "RGB ";
		}
		cmd += Integer.toString(downsamplingFactor);
		if (yuvMode) {
			DateFormat dateFormat = new SimpleDateFormat("yyyyMMdd-HHmmss");;
			java.util.Date date = new java.util.Date();
			String name = dateFormat.format(date);
			if (augmented) {
				name += "-augmented";
			} 
			cmd += " " + name;
		}
		cmd += "\0";
		mConnector.sendPacket(getPacketID(), cmd);

		long startTime = System.currentTimeMillis();
		do {
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
			}
		} while ((mState != IMGState.GOTFILENAME && mState != IMGState.ERROR) && (System.currentTimeMillis() - startTime) < TIMEOUT);
		if (mState != IMGState.GOTFILENAME && mState != IMGState.ERROR) {
			mState = IMGState.IDLE;
			throw new IOException("The image request timed out!");
		}
		if(!yuvMode){
		try {
			byte[] imageData = mFileProcessor.getFile(mFilename);
			if (!yuvMode){
				BufferedImage image = ImageIO.read(new ByteArrayInputStream(imageData));
				return new YUVImage(image);
			} else {
				String yuvData = new String(imageData);
				return new YUVImage(yuvData);
			}
			
		} finally {
			mState = IMGState.IDLE;
		}
		} else {
			mState = IMGState.IDLE;
			return null;
		}
		
	}
	
	public void sendImage(String filename, boolean useColorTable) throws IllegalStateException {
		if (mState != IMGState.IDLE) {
			throw new IllegalStateException("The last image request is not yet completed!");
		}
		
		mFilename = "";
		String cmd = "SET ";
		if (useColorTable) {
		 cmd += "COL ";
		} else {
		 cmd += "    ";
		}
		cmd += filename;
		cmd += "\0";
		mConnector.sendPacket(getPacketID(), cmd);

	}


	@Override
	public void newPacket(PacketEvent e) {
		String reply = e.getPacket();
		if (reply.substring(0, 5).equals("READY")) {
			if (mState == IMGState.WAITINGFORPICTURE) {
				mFilename = reply.substring(6);
				mState = IMGState.GOTFILENAME;
			}
		}
	}

	@Override
	public String getPacketID() {
		return "IMG";
	}

	@Override
	public String getServiceName() {
		return SERVICENAME;
	}

	@Override
	public boolean supportsMultipleSources() {
		return false;
	}

	public void addColorConfigurationUpdateListener(ColorConfigurationUpdateListener listener) {
		mListeners.add(ColorConfigurationUpdateListener.class, listener);
	}

	public void removeColorConfigurationUpdateListener(ColorConfigurationUpdateListener listener) {
		mListeners.remove(ColorConfigurationUpdateListener.class, listener);
	}

	public synchronized void notifyColorConfigurationUpdate(ColorConfigurationUpdateEvent event) {
		for (ColorConfigurationUpdateListener l : mListeners.getListeners(ColorConfigurationUpdateListener.class)) {
			l.colorConfigurationUpdated(event);
		}
	}
}