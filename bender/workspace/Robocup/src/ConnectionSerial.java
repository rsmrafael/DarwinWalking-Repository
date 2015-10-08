

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintStream;
import java.util.Enumeration;
import java.util.TooManyListenersException;
import javax.comm.CommDriver;
import javax.comm.CommPortIdentifier;
import javax.comm.PortInUseException;
import javax.comm.SerialPort;
import javax.comm.SerialPortEvent;
import javax.comm.SerialPortEventListener;
import javax.comm.UnsupportedCommOperationException;

/**
 * @author Tobias Bolze.
 * 
 */
public class ConnectionSerial implements Runnable, SerialPortEventListener
{
	/** The Name of the Driver */
	String driverName = "com.sun.comm.Win32Driver"; 
	
	/** The communication Driver */
	CommDriver commDriver;
	
	/** The Name of the Port*/
	String portName;
	
	/**The Buffered Reader */
	BufferedReader is;
	
	/** The Stream wich is print on the port*/
	PrintStream    os;
	
	/** The Stream wich go out*/
	OutputStream outputStream;
	
	/** The Serial Port*/
	SerialPort port;
	
	/** The Stream wich go in*/
	InputStream inputstream;
	
	/** The Thread for the read of the Port*/
	Thread readThread;
	
	/**The Class with the method wich will be aktivated when there is a message on the Port*/
	private Control control;
	
	/**
	 * Normal construcktor
	 */
	public ConnectionSerial()
	{
		
	}
	
	/**
	 * The Construcktor with the choose to read from the Port.
	 * @param control
	 */
	public ConnectionSerial(Control control)
	{
		this.control = control;
	}
	
	/**
	 * Get the actual name of the port.
	 * @return The Name of the Port
	 */
	public String getPortName() {
		return portName;
	}
	
	/**
	 * Set the Portname to a String.
	 * @param portName of the Port
	 */
	public void setPortName(String portName) {
		this.portName = portName;
	} 
	
	/**
	 * Open the Serial Channel and connact it.
	 * @param portName of the Port
	 * @throws ClassNotFoundException
	 * @throws IllegalAccessException
	 * @throws InstantiationException
	 * @throws UnsupportedCommOperationException
	 * @throws TooManyListenersException
	 * @throws IOException
	 * @throws PortInUseException 
	 */
	@SuppressWarnings({ "unchecked" })
	public void connect(String portName) throws ClassNotFoundException, IllegalAccessException, InstantiationException, UnsupportedCommOperationException, TooManyListenersException, IOException, PortInUseException
	{
		if (port != null) {
			port.close();
		}
		
		setPortName(portName);
		commDriver = (CommDriver)Class.forName(driverName).newInstance();
		commDriver.initialize();
		//CommPortIdentifier.addPortName("COM9",CommPortIdentifier.PORT_SERIAL,commDriver);
		CommPortIdentifier portId = null;  
		String wantedPortName = portName;
		Enumeration portIdentifiers = CommPortIdentifier.getPortIdentifiers();
		while (portIdentifiers.hasMoreElements())
		{
		    CommPortIdentifier pid = (CommPortIdentifier) portIdentifiers.nextElement();
		    
		    if(pid.getPortType() == CommPortIdentifier.PORT_SERIAL &&
		       pid.getName().equals(wantedPortName)) 
		    {
		        portId = pid;
		        break;
		        
		    }
		}
		if(portId == null)
		{
		    System.err.println("Could not find serial port " + wantedPortName);
		    System.exit(1);
		}
		port = null;
		port = (SerialPort) portId.open("ContactSerial",30000);
		port.setFlowControlMode(SerialPort.FLOWCONTROL_NONE);
		port.setSerialPortParams( control.getBaudRate(), SerialPort.DATABITS_8, SerialPort.STOPBITS_1,SerialPort.PARITY_NONE);
		port.setDTR(false);
		port.setRTS(false);
		port.notifyOnDataAvailable(true);
		port.notifyOnBreakInterrupt(true);
		try {
		    port.enableReceiveTimeout(30);
		} catch (UnsupportedCommOperationException e) {
		}
		if(port.isCD())
		{
			System.out.println("CD");
		}
		if(port.isCTS())
		{
			System.out.println("CTS");
		}
		if(port.isDSR())
		{
			System.out.println("DSR");
		}
		if(port.isDTR())
		{
			System.out.println("DTR");
		}
		if(port.isRTS())
		{
			System.out.println("RTS");
		}
		if(port.isReceiveFramingEnabled())
		{
			System.out.println("ReceaiveFraming");
		}
		
	    port.addEventListener(this);
   
		try {
			inputstream =port.getInputStream();
		  is = new BufferedReader(new InputStreamReader(inputstream));
		} catch (IOException e) {
		  System.err.println("Can't open input stream: write-only");
		  is = null;
		}
		readThread = new Thread(this);
	    readThread.start();
	    
	}
	
	/**
	 * Test the Port.
	 * @param portName
	 * @return ok is true when there is a port, false if there is no port
	 * @throws InstantiationException
	 * @throws IllegalAccessException
	 * @throws ClassNotFoundException
	 */
	@SuppressWarnings("unchecked")
	public boolean testPort(String portName) throws InstantiationException, IllegalAccessException, ClassNotFoundException
	{
		boolean ok = true;
		setPortName(portName);
		commDriver = (CommDriver)Class.forName(driverName).newInstance();
		commDriver.initialize();
		CommPortIdentifier portId = null;  
		String wantedPortName = portName;
		Enumeration portIdentifiers = CommPortIdentifier.getPortIdentifiers();
		while (portIdentifiers.hasMoreElements())
		{
		    CommPortIdentifier pid = (CommPortIdentifier) portIdentifiers.nextElement();
		    
		    if(pid.getPortType() == CommPortIdentifier.PORT_SERIAL &&
		       pid.getName().equals(wantedPortName)) 
		    {
		        portId = pid;
		        break;
		        
		    }
		}
		if(portId == null)
		{
		    ok = false;
		}
		return ok;
	}
	
	/**
	 * Write a String to a Serial Port.
	 * @param send the String to the Serial Port
	 * @throws IOException
	 */
	public void write(String send) throws IOException
	{
		outputStream = port.getOutputStream();
		os = new PrintStream(outputStream, true);
		char[] c = send.toCharArray();
		byte[] b = new byte[24];
		for(int i = 0;i< c.length;i++)
		{
			b[i]=(byte)c[i];
		}
		port.sendBreak(10);
		os.write(b);
		//os.print(send);
	}
	
	public void write(char[] send) throws IOException
	{
		outputStream = port.getOutputStream();
		os = new PrintStream(outputStream, true);
		byte[] b = new byte[24];
		for(int i = 0;i< send.length;i++)
		{
			b[i]=(byte)send[i];
		}
		port.sendBreak(10);
		os.write(b);
	}
	public void write(char send) throws IOException
	{
		outputStream = port.getOutputStream();
		os = new PrintStream(outputStream, true);
		os.write(send);
	}
	/**
	 * Close the open Port.
	 * @throws IOException
	 */
	public void close() throws IOException
	{
		if (is != null) is.close();
		if (os != null) os.close();
		if (port != null) port.close();
		readThread.interrupt();
	}
	
	/**
	 * The Event who listen on the port
	 */
	public void serialEvent(SerialPortEvent event) {
	    switch (event.getEventType()) {
	    case SerialPortEvent.BI:
	    case SerialPortEvent.OE:
	    case SerialPortEvent.FE:
	    case SerialPortEvent.PE:
	    case SerialPortEvent.CD:
	    case SerialPortEvent.CTS:
	    case SerialPortEvent.DSR:
	    case SerialPortEvent.RI:
	    case SerialPortEvent.OUTPUT_BUFFER_EMPTY:
	      break;
	    case SerialPortEvent.DATA_AVAILABLE:
	      byte[] readBuffer = new byte[20];

	      try {
		        while (inputstream.available() > 0) {
		          @SuppressWarnings("unused")
				int numBytes = inputstream.read(readBuffer);
		        }
		        control.messageACK(new String(readBuffer));
		      } catch (IOException e) {
		      }
		      break;
		    }
		  }
	/**
	 * the Thread for the serialEvent
	 */
	@Override
	public void run() {
		try {
		      Thread.sleep(500000);
		    } catch (InterruptedException e) {
		    }
		    
		
	}
	
	
}
