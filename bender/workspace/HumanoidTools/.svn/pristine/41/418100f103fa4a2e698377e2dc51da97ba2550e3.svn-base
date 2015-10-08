package de.fhwf.humanoid.tools.debugger.data.ColorConfig;

import de.fhwf.humanoid.tools.debugger.data.YUVColor;
import de.fhwf.humanoid.tools.debugger.services.ColorCalibrationService.ColorID;

public abstract class IYUVTable implements Cloneable{

	/**
	 * Updates the LookUpTable with the Information in the Byte Array. 
	 * The Array must contain a String being formatted like "Y0 U0 V0 Y1 U1 V1 ... V255"
	 * 
	 * @param stringColorCalibrationData
	 */
	public abstract void updateTable(byte[] colorCalibrationData);

	/**
	 * Updates the LookUpTable with the Information in the String. 
	 * The String must be formatted like "Y0 U0 V0 Y1 U1 V1 ... V255"
	 * 
	 * @param stringColorCalibrationData
	 */
	public abstract void updateTable(String stringColorCalibrationData);

	/**
	 * Gets a byte Array of the LookUp Table to write Files or send them.
	 * 
	 * @return
	 */
	public abstract byte[] getByteArray();

	public abstract void addColor(ColorID colorID, YUVColor yuv);

	public abstract void removeColor(ColorID colorID, YUVColor yuv);

	public abstract void clearTable(ColorID color);

	public abstract boolean isColor(ColorID colorID, YUVColor yuv);
	
	protected int getByteRepresentation(ColorID colorID){
		if(colorID.equals(ColorID.White)){
			return 0x01;
		} else if(colorID.equals(ColorID.Green)){
			return 0x02;
		} else if(colorID.equals(ColorID.Blue)){
			return 0x04;
		} else if(colorID.equals(ColorID.Yellow)){
			return 0x08;
		} else if(colorID.equals(ColorID.Orange)){
			return 0x10;
		} else if(colorID.equals(ColorID.Magenta)){
			return 0x20;
		} else if(colorID.equals(ColorID.Cyan)){
			return 0x40;
		} else if(colorID.equals(ColorID.Black)){
			return 0x80;
		} else {
			System.out.println("[LookUpTable] isColor: Unsupported ColorType!");
			return 0x00;
		}
	}
	
	@Override
	public Object clone() throws CloneNotSupportedException {
		IYUVTable clone = (IYUVTable) super.clone();
		return clone;
	}

}