package de.fhwf.humanoid.tools.debugger.events;

import java.util.EventListener;

public interface DrawObjectsListener extends EventListener {
  void newDrawObjects(DrawObjectsEvent e); 
}