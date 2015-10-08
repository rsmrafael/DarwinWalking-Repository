package de.fhwf.humanoid.tools.debugger.data;

public class PIDParameters {
	public double P;
	public double I;
	public double D;
	public double setPoint;
	
	public PIDParameters(double P, double I, double D, double setPoint) {
		this.P = P;
		this.I = I;
		this.D = D;
		this.setPoint = setPoint;
	}
	
	@Override
	public String toString() {
		return P + "," + I + "," + D + "," + setPoint;
	}

	public static PIDParameters parseString(String parameters) {
		String[] parts = parameters.split(",");
		
		double p = Double.parseDouble(parts[0]);
		double i = Double.parseDouble(parts[1]);
		double d = Double.parseDouble(parts[2]);
		double sP = Double.parseDouble(parts[3]);
		
		return new PIDParameters(p, i, d, sP);
	}
}
