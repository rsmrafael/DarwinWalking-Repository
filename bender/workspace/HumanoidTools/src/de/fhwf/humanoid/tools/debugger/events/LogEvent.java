package de.fhwf.humanoid.tools.debugger.events;

import java.util.EventObject;

import de.fhwf.humanoid.tools.debugger.data.LogEntry;

public class LogEvent extends EventObject {
	private static final long serialVersionUID = 1L;

	private LogEntry mLog; 
  
  public LogEvent(Object source, LogEntry log) { 
    super(source); 
    mLog = log; 
  } 
 
  public LogEntry getLog() { 
    return mLog;
  } 
}