package de.fhwf.humanoid.tools.debugger.events;

import java.util.EventListener;

public interface ColorSelectChangeListener extends EventListener {
  void ColorSelectChange(ColorSelectChangeEvent event); 
}