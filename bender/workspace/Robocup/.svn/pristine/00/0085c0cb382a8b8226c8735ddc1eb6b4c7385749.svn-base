
import java.awt.event.ActionEvent;
import java.io.IOException;
import java.util.TooManyListenersException;

import javax.comm.PortInUseException;
import javax.comm.UnsupportedCommOperationException;


public class Control implements Runnable {
	private ConnectionSerial connectionSerial;
	private HeadCommunication headCommunication;
	private static String versionsAbfrage = "FFFFAA55AA5537BA1201000000010101";
	private static String directControlMode = "FFFFAA55AA5537BA1004000000010101";
	private static String exitDirectControlMode = "FFE0FB01001A";
	private static String headerByte = "FF";
	private boolean connection  = false;
	private boolean serial = false;
	private boolean wlan = false;
	private boolean debugger = false;
	private boolean motions = false;
	private boolean servoId = false;
	private boolean servoCommands = false;
	private String waitingMessage = "";
	private String oldMessage = "";
	private RobotControlLayout robotControlLayout;
	private String[] motionNames = new String[20];
	private DT_INIFile ini;
	private int nummer = -1;
	private HeadLayout headLayout;
	private int baudRate = 19200;
	boolean run = false;
	DT_INIFile ini2;
	private int protokoll;
	Thread capture;

	/**
	 * Main Constructor
	 */
	public Control() {
	}

	/**
	 * Constructor for the Control Class to connect to the robot.
	 * 
	 * @param robotControlLayout
	 */
	public Control(RobotControlLayout robotControlLayout) {
		this.robotControlLayout = robotControlLayout;
		connectionSerial = new ConnectionSerial(this);
		headCommunication = new HeadCommunication(connectionSerial);
		headLayout = new HeadLayout(robotControlLayout,this);
		ini = new DT_INIFile("DT_INIFile\\Daten.ini");
		ini2 = new DT_INIFile("DT_INIFile\\Protokoll.ini");
		setProtokoll(0);
		capture = new Thread(this);
		capture.start();
		
	}

	/**
	 * Is used to send a Reapeted Command
	 * 
	 * @param command
	 *            is the command that is read out of the INI-File
	 * @param overwriteRunning
	 *            is if the Commands should overwrite the running Configuration.
	 * @param repetitions
	 * @throws IOException
	 */
	private void RepeatedMovementCommand(char command,
			boolean overwriteRunning, short repetitions) throws IOException {
		char[] data = new char[5];
		data[0] = 0;
		if (overwriteRunning) {
			data[0] |= 1;
		}
		data[1] = command;
		data[2] = 1; // 1 parameter
		data[3] = (char) ((repetitions >> 8) & 0xFF);
		data[4] = (char) (repetitions & 0xFF);
		sendCommand((char) 0x10, (char) 0x10, 4, 5, data);
	}
	private void StrengthDirectionMovementCommand(char page, boolean overwriteRunning, short strength, short direction) throws IOException {
		char[] data = new char[7];
	    data[0] = 0;
		if (overwriteRunning) {
	    	data[0] |= 1;
		}
	    data[1] = page;
	    data[2] = 2; // 2 parameters
	    data[3] = ( char)((strength >> 8) & 0xFF);
		data[4] = (char)(strength & 0xFF);
	    data[5] = (char)((direction >> 8) & 0xFF);
		data[6] = (char)(direction & 0xFF);
		sendCommand((char) 0x10, (char) 0x10, 4, 7, data);
	}

	/**
	 * Make the Frame to transfer the Command.
	 * 
	 * @param type
	 *            of the message.
	 * @param instruction
	 *            of the message.
	 * @param controller
	 *            address.
	 * @param commandLen
	 * @param commandContent
	 * @throws IOException
	 */
	public void sendCommand(char type, char instruction, int controller,
			int commandLen, char[] commandContent) throws IOException {
		// String ergebnis = "";
		char[] header = new char[5];
		int checksum = 0;
		header[0] = (char) 0xFF;
		header[1] = (char) ((type & 0xF0) | (controller & 0x0F));
		header[2] = instruction;
		header[3] = (char) ((commandLen >> 8) & 0xFF);
		header[4] = (char) (commandLen & 0xFF);
		for (char i = 1; i < 5; i++) { // 1 = without startbyte
			checksum = checksum + header[i];
		}
		for (char i = 0; i < commandLen; i++) {
			checksum = checksum + commandContent[i];
		}
		char[] ergebnis = new char[header.length + commandContent.length + 1];
		for (int i = 0; i < header.length; i++) {
			ergebnis[i] = header[i];
		}
		for (int i = 0; i < commandContent.length; i++) {
			ergebnis[i + header.length] = commandContent[i];
		}
		char checksum1 = (char) checksum;
		ergebnis[ergebnis.length - 1] = checksum1;
		connectionSerial.write(ergebnis);
	}


	/**
	 * Copy the Commands from the INI-File to the MotionList.
	 */
	public void setMotionListData() {
		robotControlLayout.motions1.setListData(headCommunication.getAllCommands());
	}
	
		/**
	 * Set if the controler is Connected.
	 * @param connection true or fals.
	 */
	public void setConnection(boolean connection) {
		this.connection = connection;
		System.out.println(connection);
		robotControlLayout.setIsConnected();
	}
	/**
	 * Proof it is Connected.
	 * @return the status of connection.
	 */
	public boolean isConnection() {
		return connection;
	}
	
	/**
	 * Set the Serial Connection true or false.
	 * @param serial if the connection is serial true
	 */
	public void setSerial(boolean serial) {
		this.serial = serial;
	}
	
	/**
	 * Proof if the connection is Serial
	 * @return true if the connection is Serial
	 */
	public boolean isSerial() {
		return serial;
	}
	
	/**
	 * Set the WLAN Connection true or false.
	 * @param wlan if the connection is wlan true.
	 */
	public void setWlan(boolean wlan) {
		this.wlan = wlan;
	}

	/**
	 * Proof if the connection is WLAN.
	 * @return true if the connection is WLAN.
	 */
	public boolean isWlan() {
		return wlan;
	}
	
	/**
	 * Proof wich Ports can be opened.
	 * @return an Array of the Avaidable COM Port Numbers.
	 */
	public int[] getAvailablePorts() {
		int[] ports = new int[20];
		if (isSerial()) {
			for (int i = 0; i < ports.length; i++) {
				ports[i] = -1;
			}
			for (int i = 0; i < 30; i++) {
				try {
					if (connectionSerial.testPort("COM" + i)) {
						char p = 0;
						for (int i2 = 0; i2 < ports.length && p == 0; i2++) {
							if (ports[i2] == -1) {
								ports[i2] = i;
								p = 100;
							}
						}
					}
				} catch (ClassNotFoundException e) {

				} catch (IllegalAccessException e) {

				} catch (InstantiationException e) {

				}

			}
		}

		return ports;
	}
	
	/**
	 * Send a version query to the bot.
	 * @throws IOException
	 */
	private void versionsAbfrage() throws IOException {
		this.write(versionsAbfrage);
	}
	
	/**
	 * Send if its true a Command to give the controller Direct Command over the Servos.
	 * @param on if it should be direkt Controlled.
	 * @throws IOException
	 */
	public void setDirectControlMode(boolean on) throws IOException {
		if (isConnection()) {
			if (on) {
				this.write(directControlMode);
			}
		}
	}

	/**
	 * Conact the Controller to the Bot
	 * @param value
	 * @throws ClassNotFoundException
	 * @throws IllegalAccessException
	 * @throws InstantiationException
	 * @throws UnsupportedCommOperationException
	 * @throws TooManyListenersException
	 * @throws IOException
	 * @throws InterruptedException
	 * @throws PortInUseException
	 */
	public void connect(int value) throws ClassNotFoundException,
			IllegalAccessException, InstantiationException,
			UnsupportedCommOperationException, TooManyListenersException,
			IOException, InterruptedException, PortInUseException {
		if (isSerial()) {
			connectionSerial.connect("COM" + value);
			exitDirectControlMode();
			Thread.sleep(100);
			versionsAbfrage();
			setConnection(true);

		}

	}

	/**
	 * Send a Message to the Bot to exit Direct Control Mode. (Careful because the Bot go to standart Mode)
	 * @throws IOException
	 */
	public void exitDirectControlMode() throws IOException {
		write(exitDirectControlMode);
	}
	/**
	 * Set Servo to a given position.
	 * @param servo wich schould be move.
	 * @param position of the servo.
	 * @throws IOException
	 */
	public void setServo(int servo, int position) throws IOException {
		String message = headerByte;
		String data = "";
		String id = "";
		if (servo <= 15) {
			id = "4" + Integer.toHexString(servo);
		} else {
			id = "5" + Integer.toHexString(servo - 16);
		}
		message = message + id;
		data = Integer.toHexString(position);
		if (data.length() <= 1) {
			data = "0" + data;
		}
		message = message + data;
		String checksum = checksum(id, data);
		message = message + checksum;
		write(message);

	}

	/**
	 * Get the Motions out of a INI-File.
	 * @return an Object Array of the Motions.
	 */
	public Object[] getMotions() {
		String[] zwischen = new String[ini.getPropertyNames("MotionNames").length];
		Object[] ergebnis = new Object[zwischen.length];
		for (int i = 0; i < ini.getPropertyNames("MotionNames").length; i++) {
			zwischen[i] = ini.getStringProperty("MotionNames", "" + i);
			ergebnis[i] = zwischen[i];
		}
		return ergebnis;
	}

	/**
	 * Ping all the Servos and return the one wich send ACK
	 * @return an object Array with the ACK Servos.
	 * @throws IOException
	 * @throws InterruptedException
	 */
	public Object[] pingAll() throws IOException, InterruptedException {
		Object[] ergebnis = new Object[30];
		for (int i = 0; i < ergebnis.length; i++) {
			String message = headerByte;
			String id = "";
			if (i < 16) {
				id = "A" + Integer.toHexString(i);
			} else {
				id = "B" + Integer.toHexString(i - 16);
			}
			message = message + id;
			String data2 = "00";
			message = message + data2;
			message = message + checksum(id, data2);
			if (!id.equals("")) {
				write(message);
				Thread.sleep(100);
				if (!waitingMessage.equals("") || !oldMessage.equals("")) {
					ergebnis[i] = "Servo :" + i;
					waitingMessage = "";
					oldMessage = "";

				}
			}
		}
		return ergebnis;
	}

	/**
	 * send the text to the Bot
	 * @param text a String that is send as Hex to Bot
	 * @throws IOException
	 */
	public void write(String text) throws IOException {
		if (isSerial()) {
			char[] tex = text.toCharArray();
			int erg1 = 0;
			int erg2 = 0;
			int erg = 0;
			String ergebnis = "";
			for (int i = 0; i < (tex.length / 2); i++) {
				erg1 = Integer.parseInt("" + tex[i * 2], 16);
				erg2 = Integer.parseInt("" + tex[(i * 2) + 1], 16);
				erg = 0;
				for (int i2 = erg1; i2 > 0; i2--) {
					erg = erg + 16;
				}
				erg = erg + erg2;
				ergebnis = ergebnis + (char) erg;
			}
			connectionSerial.write(ergebnis);
		}

	}

	/**
	 * Get The Message came from the bot. If it is a ACK on the versionsAbfrage the connection is set to true.
	 * @param message
	 */
	public void messageACK(String message) {
		nummer++;
		char[] zwischen = message.toCharArray();
		String[] ergebnis = new String[30];
		for (int i = 0; i < ergebnis.length; i++) {
			ergebnis[i] = "";
		}
		for (int i = 0; i < zwischen.length; i++) {
			int zwischen1 = (int) zwischen[i];
			int erg2 = zwischen1 / 16;
			int erg1 = zwischen1 % 16;
			ergebnis[i] = ergebnis[i] + Integer.toHexString(erg2);
			ergebnis[i] = ergebnis[i] + Integer.toHexString(erg1);
		}
		for (int i = 0; i < ergebnis.length - 8; i++) {
			if (ergebnis[i].equals("ff")) {
				if (ergebnis[i + 1].equals("ff")
						&& ergebnis[i + 2].equals("aa")
						&& ergebnis[i + 3].equals("55")
						&& ergebnis[i + 4].equals("aa")
						&& ergebnis[i + 5].equals("55")
						&& ergebnis[i + 6].equals("37")
						&& ergebnis[i + 7].equals("ba")) {
					if (ergebnis[i + 8].equals("12")
							&& ergebnis[i + 9].equals("01")
							&& ergebnis[i + 10].equals("00")
							&& ergebnis[i + 11].equals("00")
							&& ergebnis[i + 12].equals("00")
							&& ergebnis[i + 13].equals("02")
							&& ergebnis[i + 14].equals("02")
							&& ergebnis[i + 15].equals("17")
							&& ergebnis[i + 16].equals("15")) {
						setConnection(true);
						robotControlLayout.setIsConnected();
					} else if (!connection) {
						
						 try { versionsAbfrage(); } catch (IOException e) { 
						  //TODO Auto-generated catch block
							 e.printStackTrace();
						 }
						
					}
				} else if (!connection) {
				
					 try { versionsAbfrage(); } catch (IOException e) { 
					 // TODO Auto-generated catch block 
						 e.printStackTrace(); }
					 
				}
			} else if (debugger || isServoId() || isServoCommands()) {
				String zwischen3 = ergebnis[i] + ergebnis[i + 1];
				if (!zwischen3.equals("0000") && !zwischen3.equals("00")
						&& !zwischen3.equals("")) {
					if (waitingMessage != oldMessage) {
						oldMessage = waitingMessage;
					}
					waitingMessage = zwischen3;
				} else {
				}

			}
		}

	}

	/**
	 * close the Program and exit direct control mode.
	 * @throws IOException
	 * @throws InstantiationException
	 * @throws IllegalAccessException
	 * @throws ClassNotFoundException
	 * @throws UnsupportedCommOperationException
	 */
	public void close() throws IOException, InstantiationException,
			IllegalAccessException, ClassNotFoundException,
			UnsupportedCommOperationException {
		exitDirectControlMode();
		connectionSerial.close();
	}

	/**
	 * Add a Motion to the the List of Motions.
	 * @param name der Motion.
	 * @param servos are the count of the used
	 * @param scenes
	 */
	public void newMotion(String name, int servos, int scenes) {
		ini.addSection(name, null);
		newMotionName(name);
		for (int i = 0; i < scenes; i++) {
			String data = "";
			for (int i2 = 0; i2 < servos; i2++) {
				if (i2 != servos - 1) {
					data = data + "0,";
				} else {
					data = data + "0";
				}
			}
			ini.setStringProperty(name, "" + i, data, null);
		}
		ini.save();
	}

	public int[] getScene(String sektion, int scene) {
		int[] ergebnis = new int[20];
		String[] zwischen = ini.getStringProperty(sektion, "" + scene).split(
				",");
		for (int i = 0; i < zwischen.length; i++) {
			ergebnis[i] = Integer.parseInt(zwischen[i]);
		}
		return ergebnis;
	}

	/**
	 * Erstellt einen neuen Motionnamen.
	 * @param name der Motion.
	 */
	private void newMotionName(String name) {
		boolean stop = false;
		for (int i = 0; i < ini.getPropertyNames("MotionNames").length && !stop; i++) {
			if (ini.getStringProperty("MotionNames", "" + i).equals("NICHTS")) {
				ini.setStringProperty("MotionNames", "" + i, name, null);
				stop = true;

			}
		}
		if (!stop) {
			ini.setStringProperty("MotionNames", ""
					+ (ini.getPropertyNames("MotionNames").length), name, null);
		}
		ini.save();
		getMotionNames();
	}

	/**
	 * Set the Motions in the Motionlist.
	 */
	private void getMotionNames() {
		for (int i = 0; i < ini.getPropertyNames("MotionNames").length; i++) {
			motionNames[i] = ini.getStringProperty("MotionNames", "" + i);
		}
	}

	/**
	 * Create the Checksum for the protokoll beetween the body and the head.
	 * @param byte1 first data.
	 * @param byte2 second data.
	 * @return the result
	 */
	public String checksum(String byte1, String byte2) {
		String ergebnis = AND(XOR(byte1, byte2), "7F");
		return ergebnis;
	}

	/**
	 * Create the Checksum for the protokoll beetween the body and the head.
	 * @param byte1 first data.
	 * @param byte2 second data. 
	 * @param byte3 third data.
	 * @return the result
	 */
	public String checksum(String byte1, String byte2, String byte3) {
		String ergebnis = AND(XOR(XOR(byte1, byte2), byte3), "7F");
		return ergebnis;
	}

	/**
	 * Create the Checksum for the protokoll beetween the body and the head.
	 * @param byte1 first data.
	 * @param byte2 second data. 
	 * @param byte3 third data.
	 * @param byte4 fourth data.
	 * @return
	 */
	public String checksum(String byte1, String byte2, String byte3,
			String byte4) {
		String ergebnis = AND(XOR(XOR(XOR(byte1, byte2), byte3), byte4), "7F");
		return ergebnis;
	}

	/**
	 * 
	 * @param byte1
	 * @param byte2
	 * @return
	 */
	public String AND(String byte1, String byte2) {
		String bin3 = Integer.toBinaryString(Integer.parseInt(byte1, 16));
		String bin4 = Integer.toBinaryString(Integer.parseInt(byte2, 16));

		for (int i = 8 - bin3.length(); i > 0; i--) {
			bin3 = "0" + bin3;
		}
		for (int i = 8 - bin4.length(); i > 0; i--) {
			bin4 = "0" + bin4;
		}
		char[] bin3_1 = bin3.toCharArray();
		char[] bin4_1 = bin4.toCharArray();
		String ergebnis = "";
		for (int i = 0; i < 8; i++) {
			if (!(bin3_1[i] == '1' && bin4_1[i] == '1')) {
				ergebnis = ergebnis + "0";
			} else {
				ergebnis = ergebnis + "1";
			}
		}
		int zwischen2 = Integer.parseInt(ergebnis, 2);
		ergebnis = Integer.toHexString(zwischen2);
		if (ergebnis.length() == 1) {
			ergebnis = "0" + ergebnis;
		}
		return ergebnis;
	}

	public String XOR(String byte1, String byte2) {
		String ergebnis = "";
		String bin1 = Integer.toBinaryString(Integer.parseInt(byte1, 16));
		String bin2 = Integer.toBinaryString(Integer.parseInt(byte2, 16));
		for (int i = 8 - bin1.length(); i > 0; i--) {
			bin1 = "0" + bin1;
		}
		for (int i = 8 - bin2.length(); i > 0; i--) {
			bin2 = "0" + bin2;
		}
		char[] bin1_1 = bin1.toCharArray();
		char[] bin2_1 = bin2.toCharArray();
		for (int i = 0; i < 8; i++) {
			if ((bin1_1[i] == '0' && bin2_1[i] == '0')
					|| (bin1_1[i] == '1' && bin2_1[i] == '1')) {
				ergebnis = ergebnis + "0";
			} else if ((bin1_1[i] == '1' && bin2_1[i] == '0')
					|| (bin1_1[i] == '0' && bin2_1[i] == '1')) {
				ergebnis = ergebnis + "1";
			}
		}
		int zwischen2 = Integer.parseInt(ergebnis, 2);
		ergebnis = Integer.toHexString(zwischen2);
		if (ergebnis.length() == 1) {
			ergebnis = "0" + ergebnis;
		}
		return ergebnis;
	}
	/**
	 * Set the servo and the picture for the debugger to okay.
	 * @param servo the servo id wich is worked.
	 * @param ok is true when DebugServo is okay.
	 */
	
	/**
	 * Set the used Protokoll.
	 * @param protokoll 
	 */
	public void setProtokoll(int protokoll) {
		this.protokoll = protokoll;
	}

	public int getProtokoll() {
		return protokoll;
	}

	public void doMotion_ActionPerformed(ActionEvent evt) throws IOException {
		int index = robotControlLayout.motions1.getSelectedIndex();
		headCommunication.controlCommand(index);

	}

	public void setMotions(boolean motions) throws IOException {

		this.motions = motions;
		if (motions) {
			setServoId(false);
			robotControlLayout.jScrollPane3.setVisible(true);
			robotControlLayout.doMotion.setVisible(true);
			setMotionListData();
		} else {
			robotControlLayout.jScrollPane3.setVisible(false);
			robotControlLayout.doMotion.setVisible(false);
		}

	}

	public boolean isMotions() {
		return motions;
	}

	public void setServoId(boolean servoId) throws IOException {
		if(servoId)
		{
			headLayout.addObjeckts();
		}

		this.servoId = servoId;
	}

	public boolean isServoId() {
		return servoId;
	}

	public String getBoundarys(int id) throws IOException, InterruptedException {
		String ergebnis = "";
		String message = headerByte;
		String id2 = "";
		if (id < 16) {
			id2 = "E" + Integer.toHexString(id);
		} else {
			id2 = "F" + Integer.toHexString(id - 16);
		}
		message = message + id2;
		String data2 = "12";
		message = message + data2;
		String data3 = "00";
		message = message + data3 + data3;
		message = message + checksum(id2, data2, data3, data3);
		if (!id2.equals("") && !data3.equals("")) {
			write(message);
			Thread.sleep(100);
			if (!waitingMessage.equals("") || !oldMessage.equals("")) {
				if (!oldMessage.equals("")) {
					ergebnis = oldMessage;
				}
			}
		}
		return ergebnis;
	}
	public String getPD(int id) throws IOException, InterruptedException {
		String ergebnis = "";
		String message = headerByte;
		String id2 = "";
		if (id < 16) {
			id2 = "E" + Integer.toHexString(id);
		} else {
			id2 = "F" + Integer.toHexString(id - 16);
		}
		message = message + id2;
		String data2 = "0A";
		message = message + data2;
		String data3 = "00";
		message = message + data3 + data3;
		message = message + checksum(id2, data2, data3, data3);
		if (!id2.equals("") && !data3.equals("")) {
			write(message);
			Thread.sleep(100);
			if (!waitingMessage.equals("") || !oldMessage.equals("")) {
				if (!oldMessage.equals("")) {
					ergebnis = oldMessage;
				}
			}
		}
		return ergebnis;
	}
	public String getI(int id) throws IOException, InterruptedException {
		String ergebnis = "";
		String message = headerByte;
		String id2 = "";
		if (id < 16) {
			id2 = "E" + Integer.toHexString(id);
		} else {
			id2 = "F" + Integer.toHexString(id - 16);
		}
		message = message + id2;
		String data2 = "16";
		message = message + data2;
		String data3 = "00";
		message = message + data3 + data3;
		message = message + checksum(id2, data2, data3, data3);
		if (!id2.equals("") && !data3.equals("")) {
			write(message);
			Thread.sleep(100);
			if (!waitingMessage.equals("") || !oldMessage.equals("")) {
				if (!oldMessage.equals("")) {
					ergebnis = oldMessage;
				}
			}
		}
		return ergebnis;
	}
public boolean PDSet(int id, int P, int D)throws IOException, InterruptedException {
boolean ok = false;
String message = headerByte;
String id2 = "";
if (id < 16) {
	id2 = "E" + Integer.toHexString(id);
} else {
	id2 = "F" + Integer.toHexString(id - 16);
}
message = message + id2;
String data2 = "09";
message = message + data2;
String data3 = Integer.toHexString(P);
if (data3.length() == 1) {
	data3 = "0" + data3;
}
String data4 = Integer.toHexString(D);
if (data4.length() == 1) {
	data4 = "0" + data4;
}
message = message + data3 + data4;
message = message + checksum(id2, data2, data3, data4);
if (!id2.equals("") && !data3.equals("")) {
	write(message);
	Thread.sleep(100);
	if (!waitingMessage.equals("") || !oldMessage.equals("")) {
		ok = true;
	}
}
return ok;
}
public boolean ISet(int id, int I)throws IOException, InterruptedException {
	boolean ok = false;
	String message = headerByte;
	String id2 = "";
	if (id < 16) {
		id2 = "E" + Integer.toHexString(id);
	} else {
		id2 = "F" + Integer.toHexString(id - 16);
	}
	message = message + id2;
	String data2 = "15";
	message = message + data2;
	String data3 = Integer.toHexString(I);
	if (data3.length() == 1) {
		data3 = "0" + data3;
	}
	message = message + data3 + data3;
	message = message + checksum(id2, data2, data3, data3);
	if (!id2.equals("") && !data3.equals("")) {
		write(message);
		Thread.sleep(100);
		if (!waitingMessage.equals("") || !oldMessage.equals("")) {
			ok = true;
		}
	}
	return ok;
	}
	public boolean boundarySet(int id, int lowBound, int maxBound)
			throws IOException, InterruptedException {
		boolean ok = false;
		String message = headerByte;
		String id2 = "";
		if (id < 16) {
			id2 = "E" + Integer.toHexString(id);
		} else {
			id2 = "F" + Integer.toHexString(id - 16);
		}
		message = message + id2;
		String data2 = "11";
		message = message + data2;
		String data3 = Integer.toHexString(lowBound);
		if (data3.length() == 1) {
			data3 = "0" + data3;
		}
		String data4 = Integer.toHexString(maxBound);
		if (data4.length() == 1) {
			data4 = "0" + data4;
		}
		message = message + data3 + data4;
		message = message + checksum(id2, data2, data3, data4);
		if (!id2.equals("") && !data3.equals("")) {
			write(message);
			Thread.sleep(100);
			if (!waitingMessage.equals("") || !oldMessage.equals("")) {
				ok = true;
			}
		}
		return ok;
	}

	public boolean newId(String old, String newId) throws IOException,
			InterruptedException {
		boolean ok = false;
		int oldInt = Integer.parseInt(old);
		int newIdInt = Integer.parseInt(newId);
		String message = headerByte;
		String id = "";
		if (oldInt < 16) {
			id = "E" + Integer.toHexString(oldInt);
		} else {
			id = "F" + Integer.toHexString(oldInt - 16);
		}
		message = message + id;
		String data2 = "0C";
		message = message + data2;
		String data3 = Integer.toHexString(newIdInt);
		if (data3.length() == 1) {
			data3 = "0" + data3;
		}

		message = message + data3 + data3;
		message = message + checksum(id, data2, data3, data3);
		if (!id.equals("") && !data3.equals("")) {
			write(message);
			Thread.sleep(100);
			if (waitingMessage.equals(data3 + data3)
					|| oldMessage.equals(data3 + data3)) {
				ok = true;
			}
		}
		return ok;
	}

	public void writeMotionList() {
		robotControlLayout.motionList.setListData(getMotions());
	}

	public void setServoCommands(boolean servoCommands) {
		this.servoCommands = servoCommands;
		if (servoCommands) {
			robotControlLayout.servoCount.setVisible(true);
			robotControlLayout.newMotion.setVisible(true);
			robotControlLayout.newMotion2.setVisible(true);
			robotControlLayout.scenes.setVisible(true);
			robotControlLayout.servoCount.setVisible(true);
			robotControlLayout.jScrollPane5.setVisible(true);
			robotControlLayout.jScrollPane6.setVisible(true);
			robotControlLayout.doSzene.setVisible(true);
			robotControlLayout.save.setVisible(true);
			robotControlLayout.capture.setVisible(true);
			writeMotionList();

			for (int i = 0; i < 30; i++) {
				robotControlLayout.servoPositions[i]
						.setToolTipText("Servo" + i);
				if (i < 15) {
					robotControlLayout.servoPositions[i].setBounds(500,
							10 + (i * 28), 50, 20);
					robotControlLayout.servoNames[i].setBounds(430,10 + (i * 28),60,20);
				} else {
					robotControlLayout.servoPositions[i].setBounds(650,
							10 + ((i - 15) * 28), 50, 20);
					robotControlLayout.servoNames[i].setBounds(580,10 + ((i - 15) * 28),60,20);
				}
				robotControlLayout.servoPositions[i].setVisible(true);
				robotControlLayout.servoNames[i].setVisible(true);
			}	
		}else
		{
			robotControlLayout.servoCount.setVisible(false);
			robotControlLayout.newMotion.setVisible(false);
			robotControlLayout.newMotion2.setVisible(false);
			robotControlLayout.scenes.setVisible(false);
			robotControlLayout.servoCount.setVisible(false);
			robotControlLayout.jScrollPane5.setVisible(false);
			robotControlLayout.jScrollPane6.setVisible(false);
			robotControlLayout.doSzene.setVisible(false);
			robotControlLayout.capture.setVisible(false);
			robotControlLayout.save.setVisible(false);
			for (int i = 0; i < 30; i++) {
				robotControlLayout.servoPositions[i].setVisible(false);
				robotControlLayout.servoNames[i].setVisible(false);
			}
		}
		
	}

	public int[] getServoPositions(String motion, int scene) {
		String[] zwischen;
		if (scene != -1) {
			zwischen = ini.getStringProperty(motion, "" + scene).split(",");
		} else {
			zwischen = ini.getStringProperty(motion, "0").split(",");
		}
		int[] ergebnis = new int[zwischen.length];
		for (int i = 0; i < zwischen.length; i++) {
			ergebnis[i] = Integer.parseInt(zwischen[i]);
		}
		return ergebnis;

	}

	public void setServoPosition() {
		int[] zwischen = new int[getServoPositions(
				(String) robotControlLayout.motionList.getSelectedValue(),
				robotControlLayout.sceneList.getSelectedIndex()).length];
		String ergebnis = "";
		for (int i = 0; i < getServoPositions(
				(String) robotControlLayout.motionList.getSelectedValue(),
				robotControlLayout.sceneList.getSelectedIndex()).length; i++) {
			zwischen[i] = Integer.parseInt(robotControlLayout.servoPositions[i]
					.getText());
			if (i != getServoPositions((String) robotControlLayout.motionList
					.getSelectedValue(), robotControlLayout.sceneList
					.getSelectedIndex()).length - 1) {
				ergebnis = ergebnis + zwischen[i] + ",";
			} else {
				ergebnis = ergebnis + zwischen[i];
			}
		}
		ini.setStringProperty((String) robotControlLayout.motionList
				.getSelectedValue(), ""
				+ robotControlLayout.sceneList.getSelectedIndex(), ergebnis,
				null);
		ini.save();

	}

	public void writeServoPositions() {
		int[] zwischen = getServoPositions(
				(String) robotControlLayout.motionList.getSelectedValue(),
				robotControlLayout.sceneList.getSelectedIndex());
		for (int i = 0; i < zwischen.length; i++) {
			robotControlLayout.servoPositions[i].setText("" + zwischen[i]);
		}
	}

	public Object[] getSceneList(String motion) {
		int anzahl = ini.getPropertyNames(motion).length;
		Object[] ergebnis = new Object[anzahl];
		for (int i = 0; i < anzahl; i++) {
			ergebnis[i] = "Szene " + i;
		}
		return ergebnis;
	}

	public void writeSceneList() {
		robotControlLayout.sceneList.setListData(getSceneList(""
				+ robotControlLayout.motionList.getSelectedValue()));
	}

	public int getServoAnzahl(String motion, int szene) {
		String[] zwischen = ini.getStringProperty(motion, "" + szene)
				.split(",");
		return zwischen.length;
	}

	public boolean isServoCommands() {
		return servoCommands;
	}

	/**
	 * 
	 * @param servo
	 * @throws IOException
	 * @throws InterruptedException
	 */
	public void passiveServo(int servo) throws IOException,
			InterruptedException {

	}

	/**
	 * Catch the ServoPositions
	 */
	public void run() {
		while (true) {
			if (run) {
				int serv = -1;
				for (int i = 0; i < getServoAnzahl(
						(String) robotControlLayout.motionList
								.getSelectedValue(),
						robotControlLayout.sceneList.getSelectedIndex()); i++) {
					if (robotControlLayout.servoPositions[i].isFocusOwner()) {
						serv = i;
					}

				}
				if (serv != -1) {
					run = true;
					String message = headerByte;
					String data = "";
					String id = "";
					if (serv <= 15) {
						id = "C" + Integer.toHexString(serv);
					} else {
						id = "D" + Integer.toHexString(serv - 16);
					}
					message = message + id;
					data = "10";
					if (data.length() <= 1) {
						data = "0" + data;
					}
					message = message + data;
					String checksum = checksum(id, data);
					message = message + checksum;
					try {
						write(message);
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					try {
						Thread.sleep(300);
					} catch (InterruptedException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}

				}
				if (!waitingMessage.equals("") || !oldMessage.equals("")) {
					int waitingMes = Integer.parseInt(oldMessage, 16);
					System.out.println(waitingMes + "   " + oldMessage);
					if (waitingMes < 256 && serv != -1) {
						robotControlLayout.servoPositions[serv].setText(""
								+ waitingMes);
					}

				}
			}
		}

	}

	public void setBaudRate(int baudRate) {
		this.baudRate = baudRate;
	}

	public int getBaudRate() {
		return baudRate;
	}
	/**
	 * Read the Standart Baudrates out of a ini File(Max 5).
	 * @return The Baudrates in a int Array.
	 */
	public String[] getBaudrates()
	{
		String[] ergebnis = new String[5];
		for(int i= 0;i< 5;i++)
		{
			ergebnis[i]= ini.getStringProperty("Baudrate", ""+i);
		}
		return ergebnis;
	}
}
