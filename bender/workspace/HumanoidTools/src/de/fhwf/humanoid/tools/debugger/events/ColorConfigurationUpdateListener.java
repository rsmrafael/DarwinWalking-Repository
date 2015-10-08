package de.fhwf.humanoid.tools.debugger.events;

import java.util.EventListener;

public interface ColorConfigurationUpdateListener extends EventListener {
  void colorConfigurationUpdated(ColorConfigurationUpdateEvent e); 
}