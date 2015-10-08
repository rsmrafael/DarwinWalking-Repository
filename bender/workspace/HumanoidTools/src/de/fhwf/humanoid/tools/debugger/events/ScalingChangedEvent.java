package de.fhwf.humanoid.tools.debugger.events;

import java.util.EventObject;

public class ScalingChangedEvent extends EventObject {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	private int scalingFactor;

	public ScalingChangedEvent(Object source, int scalingFactor) {
		super(source);
		this.scalingFactor = scalingFactor;
	}

	public int getScalingFactor() {
		return scalingFactor;
	}

}
