package de.fhwf.humanoid.tools;

import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.net.BindException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.SocketTimeoutException;
import java.net.UnknownHostException;

public class UDPConnection {
 private static final int MAX_MESSAGE_LEN = 4 * 1024;
 
 private DatagramSocket mSocket;
 private DatagramPacket mSendPacket = new DatagramPacket(new byte[MAX_MESSAGE_LEN], MAX_MESSAGE_LEN);
 private InetAddress mServerAddress;
 private int mServerPort;
 private boolean mPacketsFromServerOnly;
 
 public UDPConnection(String serverAddress, int serverPort) {
	 this(serverAddress, serverPort, 0);
 }

 public UDPConnection(String serverAddress, int serverPort, int localPort) {
	 this(serverAddress, serverPort, 0, false);
 }

 public UDPConnection(String serverAddress, int serverPort, int localPort, boolean packetsFromServerOnly) {
  try {
   mServerAddress = InetAddress.getByName(serverAddress);
  } catch (UnknownHostException e) {
   System.err.println("[UDPConnection] UDPConnection: Error resolving address: " + e.getMessage());   
  }
  mServerPort = serverPort;
  mPacketsFromServerOnly = packetsFromServerOnly;
  if (localPort > 0) {
  	try {
			mSocket = new DatagramSocket(localPort);
		} catch (SocketException e) {
      System.err.println("[UDPConnection] UDPConnection: Error creating socket: " + e.getMessage());
		}
  } else {
    int clientPort = 12300 - 1;
    boolean success = false;
    while (clientPort < 12350 && !success) {
     clientPort++;
     try {
      success = true;
      mSocket = new DatagramSocket(clientPort);
     } catch (BindException e) {
      success = false;
     } catch (SocketException e) {
      success = false;
      System.err.println("[UDPConnection] UDPConnection: Error creating socket: " + e.getMessage());
     }
    }
    System.out.println("[UDPConnection] UDPConnection: Connection will be established from port " + clientPort + ".");
  }
  mSendPacket.setAddress(mServerAddress);
  mSendPacket.setPort(mServerPort);
 }
 
 public void send(String data) {
	//System.out.println("[UDPConnection] send: Sending " + data);
  mSendPacket.setData(data.getBytes());
  try {
   mSocket.send(mSendPacket);
  } catch (IOException e) {
   System.err.println("[UDPConnection] send: Error sending data: " + e.getMessage());   
  } catch (NullPointerException e) {
   System.err.println("[UDPConnection] send: Error sending data: " + e.getMessage());   
  }
 }

 public void send(byte[] data) {
		//System.out.println("[UDPConnection] send: Sending " + data);
	  mSendPacket.setData(data);
	  try {
	   mSocket.send(mSendPacket);
	  } catch (IOException e) {
	   System.err.println("[UDPConnection] send: Error sending data: " + e.getMessage());   
	  }
	 }

 public byte[] receiveBytes() {
	  DatagramPacket recvPacket = new DatagramPacket(new byte[MAX_MESSAGE_LEN], MAX_MESSAGE_LEN);
	  boolean timeout = false;
	  try {
	   mSocket.setSoTimeout(1000);
	   try {
	    mSocket.receive(recvPacket);
	   } catch (SocketTimeoutException e) {
	    timeout = true;
	   }
	  } catch (SocketException e) {
	   System.err.println("[UDPConnection] receive: Error receiving socket: " + e.getMessage());
	  } catch (IOException e) {
	   System.err.println("[UDPConnection] receive: Error receiving packet: " + e.getMessage());
	  } 
	  if (!timeout) {
	   //System.out.println("[UDPConnection] Received Information: " + new String(recvPacket.getData(),0,recvPacket.getLength()));
	   return recvPacket.getData();
	  } else {
	   return null;
	  }
	 }

 public String receive() {
  DatagramPacket recvPacket = new DatagramPacket(new byte[MAX_MESSAGE_LEN], MAX_MESSAGE_LEN);
  boolean timeout = false;
  try {
   mSocket.setSoTimeout(1000);
   try {
    mSocket.receive(recvPacket);
   } catch (SocketTimeoutException e) {
    timeout = true;
   }
  } catch (SocketException e) {
   System.err.println("[UDPConnection] receive: Error receiving socket: " + e.getMessage());
  } catch (IOException e) {
   System.err.println("[UDPConnection] receive: Error receiving packet: " + e.getMessage());
  } 
  if (!timeout) {
  	if (mPacketsFromServerOnly) {
  		if (!recvPacket.getAddress().equals(mServerAddress)) {
  			return "";
  		}
  	}
   //System.out.println("[UDPConnection] Received Information: " + new String(recvPacket.getData(),0,recvPacket.getLength()));
   try {
		return new String(recvPacket.getData(), 0, recvPacket.getLength(), "ISO-8859-1");
   } catch (UnsupportedEncodingException e) {
  	e.printStackTrace();
  	return "";
   }
  } else {
   return "";
  }
 }

 public DatagramPacket receivePacket() {
	  DatagramPacket recvPacket = new DatagramPacket(new byte[MAX_MESSAGE_LEN], MAX_MESSAGE_LEN);
	  boolean timeout = false;
	  try {
	   mSocket.setSoTimeout(1000);
	   try {
	    mSocket.receive(recvPacket);
	   } catch (SocketTimeoutException e) {
	    timeout = true;
	   }
	  } catch (SocketException e) {
	   System.err.println("[UDPConnection] receive: Error receiving socket: " + e.getMessage());
	  } catch (IOException e) {
	   System.err.println("[UDPConnection] receive: Error receiving packet: " + e.getMessage());
	  } 
	  if (!timeout) {
	  	if (mPacketsFromServerOnly) {
	  		if (!recvPacket.getAddress().equals(mServerAddress)) {
	  			return null;
	  		}
	  	}
	  	//System.out.println("[UDPConnection] Received Information: " + new String(recvPacket.getData(),0,recvPacket.getLength()));
	  	return recvPacket;
	  } else {
	   return null;
	  }
 }
 
 public void close() {
  mSocket.close();
 }

 public InetAddress getServerAdress() {
	 return mServerAddress;
 }
 
 public void setServerAdress(String serverAddress) {
  try {
   mServerAddress = InetAddress.getByName(serverAddress);
	} catch (UnknownHostException e) {
	 System.err.println("[UDPConnection] UDPConnection: Error resolving address: " + e.getMessage());   
	}
	mSendPacket.setAddress(mServerAddress);	 
 }
}
