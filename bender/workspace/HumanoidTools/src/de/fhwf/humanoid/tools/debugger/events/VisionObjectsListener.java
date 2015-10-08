package de.fhwf.humanoid.tools.debugger.events;

import java.util.EventListener;

public interface VisionObjectsListener extends EventListener {
  void newVisionObjects(VisionObjectsEvent e); 
}