package de.fhwf.humanoid.tools.debugger.data.ColorConfig;

import de.fhwf.humanoid.tools.debugger.data.YUVColor;
import de.fhwf.humanoid.tools.debugger.services.ColorCalibrationService.ColorID;

public class YUVTable extends IYUVTable {
	public int[] mYTable = new int[256];
	public int[] mUTable = new int[256];
	public int[] mVTable = new int[256];

	public YUVTable() {
		for (int i = 0; i < mYTable.length; i++) {
			mYTable[i] = 0;
			mUTable[i] = 0;
			mVTable[i] = 0;
		}
	}

	@Override
	public Object clone() throws CloneNotSupportedException {
		YUVTable clone = (YUVTable) super.clone();
		clone.mYTable = mYTable.clone();
		clone.mUTable = mUTable.clone();
		clone.mVTable = mVTable.clone();
		return clone;
	}

	@Override
	public void updateTable(byte[] colorCalibrationData) {
		updateTable(colorCalibrationData.toString());
	}

	@Override
	public void updateTable(String stringColorCalibrationData) {
		String[] splittedData = stringColorCalibrationData.split(" ");
		int stringPosition = 0;

		for (int i = 0; i < mYTable.length; i++) {

			mYTable[i] = Integer.parseInt(splittedData[stringPosition]);
			mUTable[i] = Integer.parseInt(splittedData[stringPosition + 1]);
			mVTable[i] = Integer.parseInt(splittedData[stringPosition + 2]);

			stringPosition += 3;
		}
		System.out
				.println("[LookUpTable] New ColorCalibration from Robot loaded.");
	}

	@Override
	public byte[] getByteArray() {
		String byteRepresentation = "";

		for (int i = 0; i < mYTable.length; i++) {
			byteRepresentation += mYTable[i] + " ";
			byteRepresentation += mUTable[i] + " ";
			byteRepresentation += mVTable[i] + " ";
		}

		return byteRepresentation.getBytes();
	}

	@Override
	public void addColor(ColorID colorID, YUVColor yuv) {
		// TODO Auto-generated method stub

	}

	@Override
	public void removeColor(ColorID colorID, YUVColor yuv) {
		// TODO Auto-generated method stub

	}

	@Override
	public void clearTable(ColorID color) {
		int negationColor = getByteRepresentation(color) ^ 255;

		for (int i = 0; i < mYTable.length; i++) {
			mYTable[i] = mYTable[i] & negationColor;
			mUTable[i] = mUTable[i] & negationColor;
			mVTable[i] = mVTable[i] & negationColor;
		}
	}

	@Override
	public boolean isColor(ColorID colorID, YUVColor yuv) {
		int intData = mYTable[yuv.Y] & mVTable[yuv.V] & mUTable[yuv.U];

		if (colorID.equals(ColorID.White)) {
			if ((intData & 0x01) == 0x01) {
				return true;
			} else {
				return false;
			}
		} else if (colorID.equals(ColorID.Green)) {
			if ((intData & 0x02) == 0x02) {
				return true;
			} else {
				return false;
			}
		} else if (colorID.equals(ColorID.Blue)) {
			if ((intData & 0x04) == 0x04) {
				return true;
			} else {
				return false;
			}
		} else if (colorID.equals(ColorID.Yellow)) {
			if ((intData & 0x08) == 0x08) {
				return true;
			} else {
				return false;
			}
		} else if (colorID.equals(ColorID.Orange)) {
			if ((intData & 0x10) == 0x10) {
				return true;
			} else {
				return false;
			}
		} else if (colorID.equals(ColorID.Magenta)) {
			if ((intData & 0x20) == 0x20) {
				return true;
			} else {
				return false;
			}
		} else if (colorID.equals(ColorID.Cyan)) {
			if ((intData & 0x40) == 0x40) {
				return true;
			} else {
				return false;
			}
		} else if (colorID.equals(ColorID.Black)) {
			if ((intData & 0x80) == 0x80) {
				return true;
			} else {
				return false;
			}
		} else {
			System.out.println("[LookUpTable] isColor: Unsupported ColorType!");
			return false;
		}
	}

}