package de.fhwf.humanoid.tools.debugger.data;

import java.awt.Point;
import java.awt.Rectangle;

public class VisionObject {

	private ObjectType mObjectType;
	private Point mPosition;
	private Rectangle mBoundingBox;
	private double mDistance;
	
	private double mAngle;
	
	
	public enum ObjectType {
		BALL,
		YELLOW_GOAL_POLE,
		BLUE_GOAL_POLE,
		YELLOW_GOAL,
		BLUE_GOAL,
		YELLOW_LANDMARK_POLE,
		BLUE_LANDMARK_POLE,
		OBSTACLE		
	}
	
	public VisionObject(ObjectType objectType, int xPos, int yPos, double distance, int xLeft, int yUpper, int xRight, int yLower, double angle) {
		mObjectType = objectType;
		mPosition = new Point(xPos, yPos);
		mBoundingBox = new Rectangle(xLeft, yUpper, xRight - xLeft, yLower - yUpper);
		mDistance = distance;
		mAngle = angle;   // Convert Angle
	}
	
	
	/**
	 * Returns the angle of the object ranging from - PI to + PI. wehre left from the robot is minus and right is plus.
	 * 
	 * @return
	 */
	public double getAngle(){
		return mAngle;
	}

	public ObjectType getType() {
		return mObjectType;
	}

	public Point getPosition() {
		return mPosition;
	}

	public Rectangle getBoundingBox() {
		return mBoundingBox;
	}
	
	/**
	 * Return the Distance of the object relativ to the robot. In meter
	 * 
	 * @return
	 */
	public double getDistance() {
		return mDistance;		// in meters
	}

	public static VisionObject parseString(String str) {
		String[] parts = str.split(",");
		if (parts.length == 9) {
  		ObjectType type = ObjectType.values()[Integer.parseInt(parts[0])];
  		int xPos = Integer.parseInt(parts[1]);
  		int yPos = Integer.parseInt(parts[2]);

  		double dist = Double.parseDouble(parts[3]);

  		int xL = Integer.parseInt(parts[4]);
  		int yU = Integer.parseInt(parts[5]);
  		int xR = Integer.parseInt(parts[6]);
  		int yL = Integer.parseInt(parts[7]);

  		double angle = 0;
  		try {
  		 angle = Double.parseDouble(parts[8]);
  		} catch (NumberFormatException ex) {
  		}
  		return new VisionObject(type, xPos, yPos, dist, xL, yU, xR, yL, angle);  
		}
		return null;
	}
}
