import java.io.IOException;


public class HeadCommunication 
{
	private DT_INIFile ini;
	private String[][] commands = new String[30][3];
	private ConnectionSerial conact;
	private int commandLenght;
	public HeadCommunication(ConnectionSerial conact)
	{
		ini = new DT_INIFile("DT_INIFile\\HeadCommands.ini");
		this.conact= conact;
		commandLenght = ini.getPropertyNames("Commands").length;
		loadCommands();
	}
	public boolean controlCommand(int index) throws IOException
	{
		return controlCommand((commands[index][0]));
	}
	/**
	 * Used to Simulate the head. Take Data out of the INI_File Daten.ini.
	 * 
	 * @param command
	 *            name
	 * @return true if the command is Acknoleged.
	 * @throws IOException
	 */
	public boolean controlCommand(String command) throws IOException {
		boolean ok = false;
		int command2 = -1;
		for (int i = 0; i < commandLenght; i++) {
			if (command.equals(commands[i][0])) {
				command2 = i;
			}
		}
		if (Integer.parseInt(commands[command2][2]) == 1) {
			RepeatedMovementCommand((char) Integer.parseInt(
					commands[command2][1], 16), true, (short) 1);
			ok = true;
		}
		if(Integer.parseInt(commands[command2][2]) == 2){
			StrengthDirectionMovementCommand((char) Integer.parseInt(
					commands[command2][1], 16), true, (short)127,(short)127);
		
		}
		return ok;
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
		conact.write(ergebnis);
	}
	public Object[] getAllCommands()
	{
		Object[] ergebnis = new Object[commandLenght];
		for (int i = 0; i < commandLenght; i++) {
			ergebnis[i] = commands[i][0];
		}
		return ergebnis;
	}
	private void loadCommands()
	{
		for (int i = 0; i < commandLenght; i++) {
			String[] b = ini.getStringProperty("Commands", "" + i).split(";");
			commands[i][0] = b[0];
			commands[i][1] = b[1];
			commands[i][2] = b[2];
		}
	}
}
