package de.fhwf.humanoid.tools.debugger.services;

import javax.swing.event.EventListenerList;

import de.fhwf.humanoid.tools.debugger.connection.IConnector;
import de.fhwf.humanoid.tools.debugger.connection.PacketEvent;
import de.fhwf.humanoid.tools.debugger.connection.PacketListener;
import de.fhwf.humanoid.tools.debugger.events.ScalingChangedEvent;
import de.fhwf.humanoid.tools.debugger.events.ScalingListener;

public class ScalingChangedService implements IService, PacketListener {
	public static final String SERVICENAME = "ScalingChangedService";
	private EventListenerList mListeners = new EventListenerList();

	private enum DataState {
		IDLE, WAITINGFORDATA, GOTDATA, ERROR
	}

	private IConnector mConnector;
	private DataState mState;

	public ScalingChangedService(IConnector connector) {
		setmConnector(connector);
		setmState(DataState.IDLE);
	}

	public void addScalingListener(ScalingListener listener) {
		mListeners.add(ScalingListener.class, listener);
	}

	public void removeScalingListener(ScalingListener listener) {
		mListeners.remove(ScalingListener.class, listener);
	}

	public synchronized void notifyNewScalingChanged(ScalingChangedEvent event) {
		for (ScalingListener l : mListeners.getListeners(ScalingListener.class)) {
			l.newScaling(event);
		}
	}

	@Override
	public void newPacket(PacketEvent e) {

	}

	@Override
	public String getPacketID() {
		return "VDA";
	}

	@Override
	public boolean supportsMultipleSources() {
		return false;
	}

	@Override
	public String getServiceName() {
		return SERVICENAME;
	}

	public DataState getmState() {
		return mState;
	}

	public void setmState(DataState mState) {
		this.mState = mState;
	}

	public IConnector getmConnector() {
		return mConnector;
	}

	public void setmConnector(IConnector mConnector) {
		this.mConnector = mConnector;
	}

}
