package de.fhwf.humanoid.tools.debugger.connection;

import java.io.UnsupportedEncodingException;
import java.net.DatagramPacket;
import java.net.InetAddress;
import java.util.HashMap;
import java.util.Map;

import javax.swing.event.EventListenerList;

import de.fhwf.humanoid.tools.UDPConnection;

public class ConnectorNetwork implements IConnector {
	public static final int UDP_PORT = 3745;
	
	private UDPConnection mConnection;
	private Thread mExecutor;
	private boolean mRunning;
	private Map<String, EventListenerList> mListeners;
	private InetAddress mCurrentSourceAddress;
	
	public ConnectorNetwork(String botAddress) {
		mListeners = new HashMap<String, EventListenerList>();
		mConnection = new UDPConnection(botAddress, UDP_PORT + 1, UDP_PORT, false);
		mCurrentSourceAddress = mConnection.getServerAdress();
		mExecutor = new Thread() {
			public void run() {
				while (mRunning) {
					DatagramPacket recvPacket = mConnection.receivePacket();
					if (recvPacket != null) {
						String recv = "";
						try {
							recv = new String(recvPacket.getData(), 0, recvPacket.getLength(), "ISO-8859-1");
						} catch (UnsupportedEncodingException e) {
						}
						if (recv.charAt(3) == ':') {
							String packetType = recv.substring(0, 3).toUpperCase();
							if (mListeners.containsKey(packetType)) {
								notifyNewPacket(new PacketEvent(this, packetType, recv.substring(4), recvPacket.getAddress()));
							}
						}
					}
				}
			}
		};
		mRunning = true;
		mExecutor.start();
	}
	
	@Override
	public void addPacketListener(PacketListener listener) {
		String packetType = listener.getPacketID();
		if (packetType.length() != 3) {
			throw new IllegalArgumentException("Length of packetID has to be 3");
		}
		
		if (!mListeners.containsKey(packetType.toUpperCase())) {
			mListeners.put(packetType.toUpperCase(), new EventListenerList());
		}
		mListeners.get(packetType.toUpperCase()).add(PacketListener.class, listener);
	}

	@Override
	public void removePacketListener(PacketListener listener) {
		String packetType = listener.getPacketID();
		
		if (mListeners.containsKey(packetType.toUpperCase())) {
			mListeners.get(packetType.toUpperCase()).remove(PacketListener.class, listener);
		}
	}

	private synchronized void notifyNewPacket(PacketEvent event) {
		String packetType = event.getPacketType().toUpperCase();
		if (mListeners.containsKey(packetType)) {
			for (PacketListener l : mListeners.get(packetType).getListeners(PacketListener.class)) {
				if (event.getSender().equals(mCurrentSourceAddress)) {
					l.newPacket(event);
				} else {
  				if (l.supportsMultipleSources()) {
  					l.newPacket(event);
  				}
				}
			}
		}
	}

	@Override
	public void sendPacket(String packetID, String data) {
		if (packetID.length() != 3) {
			throw new IllegalArgumentException("Length of packetID has to be 3");
		}
		mConnection.send(packetID + ":" + data);
	}

	@Override
	public void sendPacket(String packetID, byte[] data) {
		if (packetID.length() != 3) {
			throw new IllegalArgumentException("Length of packetID has to be 3");
		}
		byte[] d = new byte[4 + data.length];
	  System.arraycopy(packetID.getBytes(), 0, d, 0, 3);
	  d[3] = ':';
	  System.arraycopy(data, 0, d, 4, data.length);
		//byte[] d = packetID.getBytes() + new String(":").getBytes();
		mConnection.send(d);
	}

	@Override
	public String getCurrentServer() {
	  try {
	    return mConnection.getServerAdress().getHostAddress();
	   } catch (NullPointerException e) {
	    return "< Error resolving address >";
	   }
	}

	@Override
	public InetAddress getCurrentServerAddress() {
		return mConnection.getServerAdress();
	}
		
	@Override
	public void setCurrentServer(String server) {
		mConnection.setServerAdress(server);
		mCurrentSourceAddress = mConnection.getServerAdress();
	}
}
