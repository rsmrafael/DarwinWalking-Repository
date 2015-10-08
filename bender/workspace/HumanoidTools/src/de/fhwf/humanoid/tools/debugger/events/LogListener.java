package de.fhwf.humanoid.tools.debugger.events;

import java.util.EventListener;

public interface LogListener extends EventListener {
  void newLog(LogEvent e); 
}