package de.fhwf.humanoid.tools.debugger.events;

import java.util.EventListener;

public interface MarkedColorsChangedListener extends EventListener {
	void MarkedColorsChanged(MarkedColorsChangedEvent event); 
}