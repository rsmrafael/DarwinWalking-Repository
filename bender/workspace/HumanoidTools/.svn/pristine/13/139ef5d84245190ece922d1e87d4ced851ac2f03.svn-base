package de.fhwf.humanoid.tools.debugger.services;

import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

import javax.swing.event.EventListenerList;

import de.fhwf.humanoid.tools.debugger.connection.IConnector;
import de.fhwf.humanoid.tools.debugger.connection.PacketEvent;
import de.fhwf.humanoid.tools.debugger.connection.PacketListener;
import de.fhwf.humanoid.tools.debugger.events.StatusEvent;
import de.fhwf.humanoid.tools.debugger.events.StatusListener;

public class ControlService implements IService, PacketListener {
	public static final String SERVICENAME = "ControlService";
	private static final int TIMEOUT = 300; // ms

	private enum DataState {
    IDLE, WAITINGFORDATA, GOTDATA, ERROR
	}

	private IConnector mConnector;
	private DataState mState;
	private EventListenerList mListeners = new EventListenerList();
	private String mSettings;

	public ControlService(IConnector connector) {
		mConnector = connector;
		mState = DataState.IDLE;
	}	

	public Map<String, String> getSettings() throws IOException {
		if (mState != DataState.IDLE) {
			throw new IllegalStateException("The last control request is not yet completed!");
		}
		mState = DataState.WAITINGFORDATA;
		String cmd = "GET ";
		cmd += "SETTI";
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
			throw new IOException("The control request timed out!");
		}

		mState = DataState.IDLE;
		String settings[] = mSettings.split("\n");
		Map<String, String> ret = new HashMap<String, String>();
		for (String s : settings) {
		 String[] keyval = s.split("=");
		 if (keyval.length > 1){
			 ret.put(keyval[0], keyval[1]);
		 } else {
			 ret.put(keyval[0], "");
		 }
		}
		return ret;
	}

 public void setSetting(String key, String value) throws IOException {
  if (mState != DataState.IDLE) {
   throw new IllegalStateException("The last control request is not yet completed!");
  }
  mState = DataState.WAITINGFORDATA;
  String cmd = "SET ";
  cmd += key.substring(0, 5).toUpperCase() + " ";
  cmd += value;  
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
   throw new IOException("The control request timed out!");
  }

  mState = DataState.IDLE;
 }

 public String getConfigIni() throws IOException {
  if (mState != DataState.IDLE) {
   throw new IllegalStateException("The last control request is not yet completed!");
  }
  mState = DataState.WAITINGFORDATA;
  String cmd = "GET ";
  cmd += "CONFI";
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
   throw new IOException("The control request timed out!");
  }

  mState = DataState.IDLE;
  return mSettings;
 }

 public void setConfigIni(String section, String key, String value) throws IOException {
  if (mState != DataState.IDLE) {
   throw new IllegalStateException("The last control request is not yet completed!");
  }
  mState = DataState.WAITINGFORDATA;
  String cmd = "SET ";
  cmd += "CONFI ";
  cmd += section + ";" + key + ";" + value;
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
   throw new IOException("The control request timed out!");
  }

  mState = DataState.IDLE;
 }

 public void saveConfigIni() throws IOException {
  if (mState != DataState.IDLE) {
   throw new IllegalStateException("The last control request is not yet completed!");
  }
  mState = DataState.WAITINGFORDATA;
  String cmd = "SET ";
  cmd += "CONFI ";
  cmd += "SAVE";
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
   throw new IOException("The control request timed out!");
  }

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
				if (parts[1].equals("SETTI") || parts[1].equals("CONFI")) {
				 mSettings = parts[2];
				}
				mState = DataState.GOTDATA;
			}
		} else if (reply.substring(0, 5).equals("READY")) {
	   if (mState == DataState.WAITINGFORDATA) {
	    mState = DataState.GOTDATA;
	   }
		} else {
			notifyNewStatus(new StatusEvent(this, reply, e.getSender()));
		}
	}

	public void addStatusListener(StatusListener listener) {
		mListeners.add(StatusListener.class, listener);
	}

	public void removeStatusListener(StatusListener listener) {
		mListeners.remove(StatusListener.class, listener);
	}

	private synchronized void notifyNewStatus(StatusEvent event) {
		for (StatusListener l : mListeners.getListeners(StatusListener.class)) {
			l.newStatus(event);
		}
	}

	@Override
	public String getPacketID() {
		return "CON";
	}

	@Override
	public String getServiceName() {
		return SERVICENAME;
	}
	
	@Override
	public boolean supportsMultipleSources() {
		return true;
	}
}
