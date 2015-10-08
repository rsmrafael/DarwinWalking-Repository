package de.fhwf.humanoid.tools.debugger.data.ColorConfig;

import de.fhwf.humanoid.tools.debugger.data.YUVColor;
import de.fhwf.humanoid.tools.debugger.services.ColorCalibrationService.ColorID;

public class YUVTableScale extends IYUVTable {
	
	public int[][][] mTable;
	private int mScale;
	
	public YUVTableScale(int scale){
		mScale = scale;
		mTable =  new int[256/scale][256/scale][256/scale]; 
	}
		
	/* (non-Javadoc)
	 * @see de.fhwf.humanoid.tools.debugger.data.ColorConfig.IYUVTable#updateTable(byte[])
	 */
	@Override
	public void updateTable(byte[] colorCalibrationData) {
		String stringColorCalibrationData = new String(colorCalibrationData);
		updateTable(stringColorCalibrationData);
	}
	
	

	/* (non-Javadoc)
	 * @see de.fhwf.humanoid.tools.debugger.data.ColorConfig.IYUVTable#updateTable(java.lang.String)
	 */
	@Override
	public void updateTable(String stringColorCalibrationData){	 
	 if (!stringColorCalibrationData.startsWith("NEW")) {
		 System.err.println("[YUVTableScale] updateTable: Wrong header: " + stringColorCalibrationData.substring(0, 3));
		}

	 byte[] data = stringColorCalibrationData.getBytes();	 
		mScale =  data[4];		
		mTable = new int[256/mScale][256/mScale][256/mScale]; 
		
		int pos = 5;
		for (int y = 0; y < mTable.length; y++){
			for (int u = 0; u < mTable.length; u++){
				for (int v = 0; v < mTable.length; v++){
					mTable[y][u][v] = data[pos];
					pos++;
				}
			}
		}
		System.out.println("[LookUpTable] New ColorCalibration from Robot loaded.");
	}

	/* (non-Javadoc)
	 * @see de.fhwf.humanoid.tools.debugger.data.ColorConfig.IYUVTable#getByteArray()
	 */
	@Override
	public byte[] getByteArray(){
	 
	 // +4 = "NEW "
	 // +1 = mScale
	 byte[] data = new byte[5 + (int)Math.pow((double)mTable.length, 3)];
	 data[0] = 'N'; data[1] = 'E'; data[2] = 'W'; data[3] = ' ';
	 
	 data[4] = (byte)mScale;
	 		
	 int pos = 5;
		for (int y = 0; y < mTable.length; y++){
			for (int u = 0; u < mTable.length; u++){
				for (int v = 0; v < mTable.length; v++){
					data[pos] = (byte)mTable[y][u][v];
					pos++;
				}
			}
		}

		return data;
	}
	
	
	/* (non-Javadoc)
	 * @see de.fhwf.humanoid.tools.debugger.data.ColorConfig.IYUVTable#addColor(de.fhwf.humanoid.tools.debugger.services.ColorCalibrationService.ColorID, de.fhwf.humanoid.tools.debugger.data.YUVColor)
	 */
	@Override
	public void addColor(ColorID colorID, YUVColor yuv) {
		int additionColor = getByteRepresentation(colorID);
		mTable[yuv.Y/mScale][yuv.U/mScale][yuv.V/mScale] = mTable[yuv.Y/mScale][yuv.U/mScale][yuv.V/mScale]|additionColor;
	}

	
	/* (non-Javadoc)
	 * @see de.fhwf.humanoid.tools.debugger.data.ColorConfig.IYUVTable#removeColor(de.fhwf.humanoid.tools.debugger.services.ColorCalibrationService.ColorID, de.fhwf.humanoid.tools.debugger.data.YUVColor)
	 */
	@Override
	public void removeColor(ColorID colorID, YUVColor yuv) {
		int negationColor = getByteRepresentation(colorID)^255;
		mTable[yuv.Y/mScale][yuv.U/mScale][yuv.V/mScale] = mTable[yuv.Y/mScale][yuv.U/mScale][yuv.V/mScale]&negationColor;
	}

	
	/* (non-Javadoc)
	 * @see de.fhwf.humanoid.tools.debugger.data.ColorConfig.IYUVTable#clearTable(de.fhwf.humanoid.tools.debugger.services.ColorCalibrationService.ColorID)
	 */
	@Override
	public void clearTable(ColorID color) {
		int negationColor = getByteRepresentation(color)^255;

		for (int y = 0; y < mTable.length; y++){
			for (int u = 0; u < mTable.length; u++){
				for (int v = 0; v < mTable.length; v++){
					mTable[y][u][v] = mTable[y][u][v]&negationColor;
				}
			}
		}
	}

	
	/* (non-Javadoc)
	 * @see de.fhwf.humanoid.tools.debugger.data.ColorConfig.IYUVTable#isColor(de.fhwf.humanoid.tools.debugger.services.ColorCalibrationService.ColorID, de.fhwf.humanoid.tools.debugger.data.YUVColor)
	 */
	@Override
	public boolean isColor(ColorID colorID, YUVColor yuv) {
		int intData = mTable[yuv.Y/mScale][yuv.U/mScale][yuv.V/mScale];
		
		if(colorID.equals(ColorID.White)){
			if((intData & 0x01) == 0x01){
					return true;
				} else {
					return false;
				}
		} else if(colorID.equals(ColorID.Green)){
			if((intData &  0x02) ==  0x02){
				return true;
			} else {
				return false;
			}
		} else if(colorID.equals(ColorID.Blue)){
			if((intData & 0x04) == 0x04){
				return true;
			} else {
				return false;
			}
		} else if(colorID.equals(ColorID.Yellow)){
			if((intData & 0x08 ) == 0x08){
				return true;
			} else {
				return false;
			}
		} else if(colorID.equals(ColorID.Orange)){
			if((intData & 0x10) == 0x10){
				return true;
			} else {
				return false;
			}
		} else if(colorID.equals(ColorID.Magenta)){
			if((intData & 0x20) == 0x20){
				return true;
			} else {
				return false;
			}
		} else if(colorID.equals(ColorID.Cyan)){
			if((intData & 0x40) == 0x40){
				return true;
			} else {
				return false;
			}
		} else if(colorID.equals(ColorID.Black)){
			if((intData & 0x80) == 0x80){
				return true;
			} else {
				return false;
			}
		} else {
			System.out.println("[LookUpTable] isColor: Unsupported ColorType!");
			return false;
		}
	}
	

	@Override
	public Object clone() throws CloneNotSupportedException {
		YUVTableScale clone = (YUVTableScale) super.clone();
		clone.mTable = mTable.clone();
		return clone;
	}

}
