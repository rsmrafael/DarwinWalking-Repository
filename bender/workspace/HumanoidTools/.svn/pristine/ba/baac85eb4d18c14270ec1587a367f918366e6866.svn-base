package de.fhwf.humanoid.tools.debugger.events;

import java.net.InetAddress;
import java.util.EventObject;

public class StatusEvent extends EventObject {
	private static final long serialVersionUID = 1L;

	private String mStatus;
	private InetAddress mSender;
  
  public StatusEvent(Object source, String status, InetAddress sender) { 
    super(source); 
    mStatus = status; 
    mSender = sender;
  } 
 
  public String getStatus() { 
    return mStatus;
  }
  
  public InetAddress getSender() {
  	return mSender;
  }
}