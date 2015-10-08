package de.fhwf.humanoid.tools.debugger.data;

import java.awt.Color;

public class YUVColor {
	public int Y;
	public int U;
	public int V;

	public int R;
	public int G;
	public int B;

	public YUVColor(int Y, int U, int V) {
		this.Y = Y;
		this.U = U;
		this.V = V;
		int[] rgb = {0,0,0};
		int[] yuv = {Y,U,V};
		convertYUVtoRGB(yuv, rgb);
		
		this.R = rgb[0];
		this.G = rgb[1];
		this.B = rgb[2];
		
	}

	public YUVColor(Color c) {
		YUVColor tmp = this.convertRGBtoYUV(c);
		this.R = c.getRed();
		this.G = c.getGreen();
		this.B = c.getBlue();
		
		this.Y = tmp.Y;
		this.U = tmp.U;
		this.V = tmp.V;

	}

	public YUVColor(int rgb) {
		this(new Color(rgb));
	}

	@Override
	public String toString() {
		return Y + "," + U + "," + V;
	}

	public static YUVColor parseString(String color) {
		String[] parts = color.split(",");
		int y = Integer.parseInt(parts[0]);
		int u = Integer.parseInt(parts[1]);
		int v = Integer.parseInt(parts[2]);

		return new YUVColor(y, u, v);
	}
	
	public int getRGB(){
		return new Color(R,G,B).getRGB();
	}
	
	public int getY(){
		return new YUVColor(Y,0,0).getRGB();
	}
	
	public int getU(){
		return new YUVColor(U,0,0).getRGB();
	}
	
	public int getV(){
		return new YUVColor(V,0,0).getRGB();
	}

	private YUVColor convertRGBtoYUV(Color color) {
		double r = color.getRed();
		double g = color.getGreen();
		double b = color.getBlue();
		double y;
		double u;
		double v;

		y = 0.299 * r + 0.587 * g + 0.114 * b;
		u = (((b - y) * 0.565) * 1.7) + 128.0;
		v = (((r - y) * 0.713) * 1.32) + 128.0;

		if (y > 255)
			y = 255;
		if (y < 0)
			y = 0;
		if (u > 255)
			u = 255;
		if (u < 0)
			u = 0;
		if (v > 255)
			v = 255;
		if (v < 0)
			v = 0;

		return new YUVColor((int) y, (int) u, (int) v);
	}
	
	private void convertYUVtoRGB(int[] yuv, int[] rgb) {
	    int y = yuv[0];
	    int u = yuv[1];
	    int v = yuv[2];

			rgb[2] = u - 128 + y;
			if (rgb[2] > 255) rgb[2] = 255;
			if (rgb[2] < 0  ) rgb[2] = 0;
			rgb[0] = v - 128 + y;
			if (rgb[0] > 255) rgb[0] = 255;
			if (rgb[0] < 0  ) rgb[0] = 0;
			rgb[1] = (169 * y - 52 * rgb[0] - 19 * rgb[2]) / 100;
			if (rgb[1] > 255) rgb[1] = 255;
			if (rgb[1] < 0  ) rgb[1] = 0;
	  }
}
