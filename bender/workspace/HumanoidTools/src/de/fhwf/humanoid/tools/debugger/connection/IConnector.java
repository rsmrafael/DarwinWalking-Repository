package de.fhwf.humanoid.tools.debugger.connection;

import java.net.InetAddress;


public interface IConnector {
	public abstract void addPacketListener(PacketListener listener);
	public abstract void removePacketListener(PacketListener listener);
	public abstract void sendPacket(String packetID, String data);
	public abstract void sendPacket(String packetID, byte[] data);
	public abstract String getCurrentServer();
	public abstract InetAddress getCurrentServerAddress();
	public abstract void setCurrentServer(String server);
}