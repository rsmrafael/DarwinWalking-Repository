package de.fhwf.humanoid.tools.debugger.services;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import javax.swing.event.EventListenerList;
import de.fhwf.humanoid.tools.debugger.connection.IConnector;
import de.fhwf.humanoid.tools.debugger.connection.PacketEvent;
import de.fhwf.humanoid.tools.debugger.connection.PacketListener;
import de.fhwf.humanoid.tools.debugger.data.DrawObject;
import de.fhwf.humanoid.tools.debugger.data.VisionObject;
import de.fhwf.humanoid.tools.debugger.events.DrawObjectsEvent;
import de.fhwf.humanoid.tools.debugger.events.DrawObjectsListener;
import de.fhwf.humanoid.tools.debugger.events.VisionObjectsEvent;
import de.fhwf.humanoid.tools.debugger.events.VisionObjectsListener;

public class VisionDataService implements IService, PacketListener {
	public static final String SERVICENAME = "VisionDataService";
	private static final int TIMEOUT = 300; // ms

	private List<VisionObject> mObjects;
	private List<DrawObject> mDrawObjects;
	private EventListenerList mListeners = new EventListenerList();

	private enum DataState {
		IDLE, WAITINGFORDATA, GOTDATA, ERROR
	}

	private IConnector mConnector;
	private DataState mState;

	public VisionDataService(IConnector connector) {
		mConnector = connector;
		mState = DataState.IDLE;

		mObjects = new ArrayList<VisionObject>();
		mDrawObjects = new ArrayList<DrawObject>();
	}

	public List<VisionObject> getVisionObjects() throws IOException {
		if (mState != DataState.IDLE) {
			throw new IllegalStateException(
					"The last vision data request is not yet completed!");
		}
		mObjects.clear();
		mState = DataState.WAITINGFORDATA;
		String cmd = "GET ";
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
			throw new IOException("The vision data request timed out!");
		}

		mState = DataState.IDLE;
		return mObjects;
	}

	public void startVisionObjectStream() {
		String cmd = "ENA ";
		cmd += "\0";
		mConnector.sendPacket(getPacketID(), cmd);
	}

	public void stopVisionObjectStream() {
		String cmd = "DIS ";
		cmd += "\0";
		mConnector.sendPacket(getPacketID(), cmd);
	}

	@Override
	public void newPacket(PacketEvent e) {
		String reply = e.getChoppedPacket();
		if (reply.substring(0, 1).equals("@")) {
			// if (mState == DataState.WAITINGFORDATA) {
			mObjects.clear();
			reply = reply.replaceAll("\n", "");
			reply = reply.replaceAll("@", "");
			String[] splittedObjects = reply.split("\\|");
			for (String object : splittedObjects) {
				mObjects.add(VisionObject.parseString(object));
			}

			notifyNewVisionObjects(new VisionObjectsEvent(this,
					new ArrayList<VisionObject>(mObjects)));

			mState = DataState.GOTDATA;
			// } else {
			// mState = DataState.ERROR;
			// }
		} else {
			mDrawObjects.clear();

			String[] splittedObjects = reply.split("\n");
			String source = "";
			for (String o : splittedObjects) {
				if (o.substring(0, 1).equals("[")) {
					source = o.substring(1, o.length() - 1);
				} else {
					mDrawObjects.add(DrawObject.parseString(source, o));
				}
			}

			notifyNewDrawObjects(new DrawObjectsEvent(this,
					new ArrayList<DrawObject>(mDrawObjects)));
		}
	}

	@Override
	public String getPacketID() {
		return "VDA";
	}

	public void addVisionObjectsListener(VisionObjectsListener listener) {
		mListeners.add(VisionObjectsListener.class, listener);
	}

	public void removeVisionObjectsListener(VisionObjectsListener listener) {
		mListeners.remove(VisionObjectsListener.class, listener);
	}

	public synchronized void notifyNewVisionObjects(VisionObjectsEvent event) {
		for (VisionObjectsListener l : mListeners
				.getListeners(VisionObjectsListener.class)) {
			l.newVisionObjects(event);
		}
	}

	public void addDrawObjectsListener(DrawObjectsListener listener) {
		mListeners.add(DrawObjectsListener.class, listener);
	}

	public void removeDrawObjectsListener(DrawObjectsListener listener) {
		mListeners.remove(DrawObjectsListener.class, listener);
	}

	public synchronized void notifyNewDrawObjects(DrawObjectsEvent event) {
		for (DrawObjectsListener l : mListeners
				.getListeners(DrawObjectsListener.class)) {
			l.newDrawObjects(event);
		}
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
