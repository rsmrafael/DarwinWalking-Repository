package de.fhwf.humanoid.tools.debugger.events;

import java.util.EventListener;

public interface ScalingListener extends EventListener {
	public void newScaling(ScalingChangedEvent e);
}
