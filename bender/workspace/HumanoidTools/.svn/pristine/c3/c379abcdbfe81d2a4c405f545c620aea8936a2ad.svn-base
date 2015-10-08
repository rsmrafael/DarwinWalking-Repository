package de.fhwf.humanoid.tools.debugger.events;

import java.util.EventObject;
import java.util.List;
import de.fhwf.humanoid.tools.debugger.data.DrawObject;

public class DrawObjectsEvent extends EventObject {
	private static final long serialVersionUID = 1L;

	private List<DrawObject> mObjects; 
  
  public DrawObjectsEvent(Object source, List<DrawObject> objects) { 
    super(source); 
    mObjects = objects; 
  } 
 
  public List<DrawObject> getDrawObjects() { 
    return mObjects;
  } 
}