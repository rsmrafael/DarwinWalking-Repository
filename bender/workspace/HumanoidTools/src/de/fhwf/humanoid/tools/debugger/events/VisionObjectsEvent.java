package de.fhwf.humanoid.tools.debugger.events;

import java.util.EventObject;
import java.util.List;

import de.fhwf.humanoid.tools.debugger.data.VisionObject;

public class VisionObjectsEvent extends EventObject {
	private static final long serialVersionUID = 1L;

	private List<VisionObject> mObjects; 
  
  public VisionObjectsEvent(Object source, List<VisionObject> objects) { 
    super(source); 
    mObjects = objects; 
  } 
 
  public List<VisionObject> getVisionObjects() { 
    return mObjects;
  } 
}