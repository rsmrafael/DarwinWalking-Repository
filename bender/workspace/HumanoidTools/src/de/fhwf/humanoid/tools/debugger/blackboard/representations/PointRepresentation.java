package de.fhwf.humanoid.tools.debugger.blackboard.representations;

import de.ostfalia.robocup.message.MsgPoint.Point;

public class PointRepresentation extends IRepresentation {

	public int x;
	public int y;

	public PointRepresentation(String name, int x, int y) {
		this.mName = name;
		this.x = x;
		this.y = y;
	}

	public PointRepresentation(Point point) {
		this.x = point.getX();
		this.y = point.getY();
	}

	@Override
	public void serialize() {
		// TODO Auto-generated method stub

	}

	@Override
	public void showEditor(String visualizer) {
		// TODO Auto-generated method stub

	}
}
