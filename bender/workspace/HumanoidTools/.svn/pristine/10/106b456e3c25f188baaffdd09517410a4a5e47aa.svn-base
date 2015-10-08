package de.fhwf.humanoid.tools.debugger.blackboard.representations;

import java.util.ArrayList;

public abstract class IRepresentation {

	protected String mName;
	protected String mSource;
	protected ArrayList<String> mVisualizer = new ArrayList<String>();

	public abstract void serialize();

	public abstract void showEditor(String visualizer);

	public ArrayList<String> getVisualizer() {
		return mVisualizer;
	}
	
	protected void registerVisualizer(String visualizer){
		mVisualizer.add(visualizer);
	}

	public String getName() {
		return mName;
	}

	public String getSource() {
		return mSource;
	}

	public String toString() {
		return "[" + mSource + "] - " + mName;
	}

}
