package de.fhwf.humanoid.tools.debugger.events;

import java.util.EventObject;

import de.fhwf.humanoid.tools.debugger.data.LookUpTable;
import de.fhwf.humanoid.tools.debugger.services.ColorCalibrationService.ColorID;

public class ColorConfigurationUpdateEvent extends EventObject {
	private static final long serialVersionUID = 1L;

	private LookUpTable mTable;
	private ColorID mCalibratedColor;
  
	/**
	 * Creates a ColorConfigurationUpdateEvent used to get the information from the Image to the CameraView.
	 * 
	 * @param source	
	 * @param colors	List of Colors that shall be marked
	 */
  public ColorConfigurationUpdateEvent(Object source, LookUpTable table, ColorID calibratedColor) { 
    super(source); 
    mTable = table;
    mCalibratedColor = calibratedColor;
  } 
 
  /**
   * Returns the LookUpTable
   * 
   * @return
   */
  public LookUpTable getLookUpTable() { 
    return mTable;
  } 
  
  public ColorID getSelctedColor(){
	  return mCalibratedColor;
  }


}