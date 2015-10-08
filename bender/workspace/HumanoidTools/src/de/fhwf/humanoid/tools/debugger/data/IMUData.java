package de.fhwf.humanoid.tools.debugger.data;

public class IMUData {
	public int Pitch;
	public int Roll;
	
	public IMUData(int pitch, int roll) {
		this.Pitch = pitch;
		this.Roll = roll;
	}
	
	public static IMUData parseString(String data) {
		String[] parts = data.split(",");
		
		int p = Integer.parseInt(parts[0]);
		int r = Integer.parseInt(parts[1]);
		
		return new IMUData(p, r);
	}
}
