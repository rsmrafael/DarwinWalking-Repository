package de.fhwf.humanoid.tools.debugger.connection;

import java.net.InetAddress;
import java.util.EventObject;

public class PacketEvent extends EventObject {
	private static final long serialVersionUID = 1L;

	private String mPacket; 
	private String mPacketType;
	private InetAddress mSender;
  
  public PacketEvent(Object source, String packetType, String packet, InetAddress sender) { 
    super(source); 
    mPacketType = packetType;
    mPacket = packet;
    mSender = sender;
  } 
 
  public String getPacket() { 
    return mPacket;
  } 
  
  public String getPacketType() {
  	return mPacketType;
  }

  public InetAddress getSender() {
  	return mSender;
  }
  
	public String getChoppedPacket() {
		if (mPacket.length() > 0) {
  		if (mPacket.charAt(mPacket.length() - 1) == '\0') {
  			return mPacket.substring(0, mPacket.length() - 1);
  		}
		}
		return mPacket;
	}
}