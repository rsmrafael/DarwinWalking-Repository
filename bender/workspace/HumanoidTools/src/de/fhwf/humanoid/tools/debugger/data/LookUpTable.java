package de.fhwf.humanoid.tools.debugger.data;

import java.io.BufferedWriter;
import java.io.IOException;

import de.fhwf.humanoid.tools.debugger.data.ColorConfig.IYUVTable;
import de.fhwf.humanoid.tools.debugger.data.ColorConfig.YUVTableScale;
import de.fhwf.humanoid.tools.debugger.events.MarkedColorsChangedEvent;
import de.fhwf.humanoid.tools.debugger.events.MarkedColorsChangedEvent.MChangeType;
import de.fhwf.humanoid.tools.debugger.events.MarkedColorsChangedListener;
import de.fhwf.humanoid.tools.debugger.services.ColorCalibrationService;
import de.fhwf.humanoid.tools.debugger.services.ColorCalibrationService.ColorID;
import de.fhwf.humanoid.tools.debugger.services.ServiceManager;
import de.fhwf.humanoid.tools.debugger.tools.Stencil;
import de.fhwf.humanoid.tools.debugger.undoredo.UndoRedoHistory;
import de.fhwf.humanoid.tools.debugger.undoredo.mementos.YUVTableMemento;

public class LookUpTable implements MarkedColorsChangedListener {

	private enum Command {
		ADD, REMOVE
	};

	private IYUVTable mYUV;

	public IYUVTable getYUV() {
		return mYUV;
	}

	private ColorCalibrationService mColorService;

	private UndoRedoHistory<LookUpTable> mHistory;

	public LookUpTable(ColorCalibrationService service) {
		mColorService = service;
		mColorService.addMarkedColorsChangeListener(this);

		mYUV = new YUVTableScale(16);

		mHistory = new UndoRedoHistory<LookUpTable>(this);
	}

	/**
	 * Updates the LookUpTable with the Information in the Byte Array. The Array
	 * must contain a String being formatted like "Y0 U0 V0 Y1 U1 V1 ... V255"
	 * 
	 * @param stringColorCalibrationData
	 */
	public void updateLookUpTable(byte[] colorCalibrationData) {
		mYUV.updateTable(colorCalibrationData);
	}

	/**
	 * Updates the LookUpTable with the Information in the String. The String
	 * must be formatted like "Y0 U0 V0 Y1 U1 V1 ... V255"
	 * 
	 * @param stringColorCalibrationData
	 */
	public void updateLookUpTable(String stringColorCalibrationData) {
		mYUV.updateTable(stringColorCalibrationData);
	}

	/**
	 * Gets a byte Array of the LookUp Table to write Files or send them.
	 * 
	 * @return
	 */
	public byte[] getByteArray() {
		return mYUV.getByteArray();
	}

	/**
	 * This method is used to add a specific color to the lookup table, besides
	 * adding the pixel it self a 3D CUbe around the pixel will be added.
	 * 
	 * @param color
	 *            ColorID
	 * @param yuvColor
	 *            The Color Value in the middle
	 */
	private void addColor(ColorID colorID, YUVColor yuv, int stencilSize) {
		procesRange(Command.ADD, yuv, colorID, stencilSize);
	}

	/**
	 * This method is used to remove a specific color to the lookup table,
	 * besides rem ves pixel it self a 3D CUbe around the pixel will be added.
	 * 
	 * @param colorID
	 * @param yuvColor
	 */
	private void removeColor(ColorID colorID, YUVColor yuv, int stencilSize) {
		procesRange(Command.REMOVE, yuv, colorID, stencilSize);
	}

	/**
	 * To clear the complete Configuration of a specific Color
	 * 
	 */
	public void clearTable(ColorID color) {
		mYUV.clearTable(color);
	}

	/**
	 * Adds all the color in range or removes them.
	 * 
	 */
	private void procesRange(Command command, YUVColor yuv, ColorID color,
			int stencilSize) {
		int[][][] mStencil = Stencil.getStencil3D(stencilSize);

		int startY = yuv.Y - mStencil.length / 2;
		int startU = yuv.U - mStencil.length / 2;
		int startV = yuv.V - mStencil.length / 2;

		for (int y = 0; y < mStencil.length; y++) {
			for (int u = 0; u < mStencil[0].length; u++) {
				for (int v = 0; v < mStencil[0][0].length; v++) {
					try {
						if (command.equals(Command.ADD)) {
							mYUV.addColor(color, new YUVColor(y + startY, u
									+ startU, v + startV));
						} else if (command.equals(Command.REMOVE)) {
							mYUV.removeColor(color, new YUVColor(y + startY, u
									+ startU, v + startV));
						} else {
							System.out
									.println("[LookUpTable] FATAL ERROR: Illeagal Command!");
						}
					} catch (ArrayIndexOutOfBoundsException e) {
						// Marking at the Edge of the Arrays
					}

				}
			}
		}
	}

	/**
	 * Checks if the yuvColor is a member or the specified Color-Type.
	 * 
	 * @param colorID
	 * @param yuv
	 * @return
	 */
	public boolean isColor(ColorID colorID, YUVColor yuv) {
		return mYUV.isColor(colorID, yuv);
	}

	/**
	 * Writes the LookUpTable onto the BufferedWriter
	 * 
	 * @param bw
	 * @throws IOException
	 */
	public void saveToFile(BufferedWriter bw) throws IOException {
		String newData = new String(getByteArray());
		bw.write(newData);
	}

	@Override
	public void MarkedColorsChanged(MarkedColorsChangedEvent event) {
		if (mHistory.isInUndoRedo())
			return;

		if (event.getType() != null) {
			if (event.getType().equals(MChangeType.Added)) {
				mHistory.Do(new YUVTableMemento(mYUV));
				for (YUVColor c : event.getColors()) {
					addColor(event.getColor(), c, event.getStencilSize());
				}
			} else if (event.getType().equals(MChangeType.Cleared)) {
				mHistory.Do(new YUVTableMemento(mYUV));
				clearTable(event.getColor());
			} else if (event.getType().equals(MChangeType.Removed)) {
				mHistory.Do(new YUVTableMemento(mYUV));
				for (YUVColor c : event.getColors()) {
					removeColor(event.getColor(), c, event.getStencilSize());
				}
			}
		}
	}

	public IYUVTable getIYUVTable() {
		return mYUV;
	}

	public void setIYUVTable(IYUVTable table) {
		mYUV = table;
		// Send empty event to inform others that we've changed
		ColorCalibrationService colorService = (ColorCalibrationService) ServiceManager
				.getInstance().getService(ColorCalibrationService.SERVICENAME);
		MarkedColorsChangedEvent event = new MarkedColorsChangedEvent(this,
				null, null, null, 0);
		colorService.notifyMarkedColorsChange(event);
	}

	public UndoRedoHistory<LookUpTable> getUndoRedoHistory() {
		return mHistory;
	}

}
