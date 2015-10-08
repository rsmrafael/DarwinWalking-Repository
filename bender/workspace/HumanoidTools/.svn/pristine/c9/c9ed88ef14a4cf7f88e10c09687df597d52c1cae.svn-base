package de.fhwf.humanoid.tools.debugger.events;

import java.util.EventObject;

import de.fhwf.humanoid.tools.debugger.services.ColorCalibrationService.ColorID;

public class ColorSelectChangeEvent extends EventObject {
	private static final long serialVersionUID = 1L;
	private ColorID mColor;
  
	
  public ColorSelectChangeEvent(Object source, ColorID color){
	super(source); 
    mColor = color;
  } 
  
  public ColorID getColor(){
	  return mColor;
  }
}