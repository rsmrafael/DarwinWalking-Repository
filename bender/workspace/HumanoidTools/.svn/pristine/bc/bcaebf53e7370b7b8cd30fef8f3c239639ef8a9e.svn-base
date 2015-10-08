package de.fhwf.humanoid.tools.debugger.events;

import java.util.EventObject;
import java.util.List;

import de.fhwf.humanoid.tools.debugger.data.YUVColor;
import de.fhwf.humanoid.tools.debugger.services.ColorCalibrationService.ColorID;

public class MarkedColorsChangedEvent extends EventObject {
	private static final long serialVersionUID = 1L;
	public enum MChangeType{Cleared,
					Removed,
					Added};
	private MChangeType mType;
	private ColorID mColor;
	private List<YUVColor> mColors;
	private int mStencilSize;
  
	
  public MarkedColorsChangedEvent(Object source, MChangeType type, ColorID color, List<YUVColor> colors, int ColorSapceStencilSize){
	super(source); 
    mType = type;
    mColor = color;
    mColors = colors;
    mStencilSize = ColorSapceStencilSize;
  } 
  
  public MChangeType getType(){
	  return mType;
  }
  
  public ColorID getColor(){
	  return mColor;
  }
  
  public List<YUVColor> getColors(){
	  return mColors;
  }
  
  public int getStencilSize(){
	  return mStencilSize;
  }
}