package de.fhwf.humanoid.tools.debugger.data;

import java.awt.image.BufferedImage;

public class YUVImage {
	
	private BufferedImage mImage;
	private int mWidth;
	private int mHeigth;
	private YUVColor[][] mImageData;
	private boolean mYUVMode;
	
	public YUVImage(BufferedImage bf){
		mImage = bf;
		mYUVMode = false;
	}
	
	public YUVImage(String yuvString){
		parseYUVImage(yuvString.split(" "));
		mYUVMode = true;
	}
	
	public YUVImage(char[] imageData, int width, int height) {
		mWidth = width;
		mHeigth = height;
		mImageData = new YUVColor[mWidth][mHeigth];
		
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				int position = (2 * x) + (2 * y * width);
				
				int Y = imageData[position];
				int U, V;
				if (x % 2 == 0) {
					V = imageData[position + 3];
					U = imageData[position + 1];
				} else {
					V = imageData[position + 1];
					U = imageData[position - 1];
				}
				mImageData[x][y] = new YUVColor(Y, U, V);
			}
		}
		
		mYUVMode = true;
	}

 public YUVImage(byte[] imageData, int width, int height) {
  mWidth = width;
  mHeigth = height;
  mImageData = new YUVColor[mWidth][mHeigth];
  
  for (int y = 0; y < height; y++) {
   for (int x = 0; x < width; x++) {
    int position = (2 * x) + (2 * y * width);
    
    int Y = (int)imageData[position] & 0xFF;
    int U, V;
    if (x % 2 == 0) {
     V = (int)imageData[position + 3] & 0xFF;
     U = (int)imageData[position + 1] & 0xFF;
    } else {
     V = (int)imageData[position + 1] & 0xFF;
     U = (int)imageData[position - 1] & 0xFF;
    }
    mImageData[x][y] = new YUVColor(Y, U, V);
   }
  }
  
  mYUVMode = true;
 }
 
	private void parseYUVImage(String[] yuvArray){
		
		int size = yuvArray.length/2;
		
		if (size == 480000){
			mWidth = 800;
			mHeigth = 600;
		} else if (size == 240000){
			mWidth = 400;
			mHeigth = 300;
		} else if (size == 120000){
			mWidth = 200;
			mHeigth = 150;
		} else {
			mWidth = 800;
			mHeigth = 600;
		}
		int yC,u, v;
		mImageData = new YUVColor[mWidth][mHeigth];
		int dataPointer = 0;
		for (int y = 0; y < mHeigth; y++){
			for (int x = 0; x < mWidth; x++){
				yC = Integer.parseInt(yuvArray[dataPointer++]);
				u = Integer.parseInt(yuvArray[dataPointer++]);
				v = Integer.parseInt(yuvArray[dataPointer++]);
				mImageData[x][y] = new YUVColor(yC, u, v);
				
			}
		}
		
		mImage = getBufferdImage();
		
	}
	
	public boolean isInYUVMode(){
		return mYUVMode;
	}
	
	
	private BufferedImage getBufferdImage(){
		BufferedImage bf = new BufferedImage(mWidth, mHeigth, BufferedImage.TYPE_USHORT_555_RGB);
		
		for (int y = 0; y < mHeigth; y++){
			for (int x = 0; x < mWidth; x++){
				bf.setRGB(x, y, mImageData[x][y].getRGB());
			}
		}
		return bf;
	}
	
	public BufferedImage getYImage(){
		BufferedImage bf = new BufferedImage(mWidth, mHeigth, BufferedImage.TYPE_USHORT_GRAY);
		
		for (int y = 0; y < mHeigth; y++){
			for (int x = 0; x < mWidth; x++){
				bf.setRGB(x, y, mImageData[x][y].getY());
			}
		}
		return bf;
	}
	
	public BufferedImage getUImage(){
		BufferedImage bf = new BufferedImage(mWidth, mHeigth, BufferedImage.TYPE_USHORT_GRAY);
		
		for (int y = 0; y < mHeigth; y++){
			for (int x = 0; x < mWidth; x++){
				bf.setRGB(x, y, mImageData[x][y].getU());
			}
		}
		return bf;
	}
	
	public BufferedImage getVImage(){
		BufferedImage bf = new BufferedImage(mWidth, mHeigth, BufferedImage.TYPE_USHORT_GRAY);
		
		for (int y = 0; y < mHeigth; y++){
			for (int x = 0; x < mWidth; x++){
				bf.setRGB(x, y, mImageData[x][y].getV());
			}
		}
		return bf;
	}
	
	public BufferedImage getImage() {
		if (mYUVMode) {
			return getBufferdImage();
		}else {
			return mImage;
		}
	}

	public int getWidth() {
		return mWidth;
	}

	public int getHeigth() {
		return mHeigth;
	}

	public YUVColor getImageData(int x, int y) {
		if (mYUVMode){
			return mImageData[x][y];
		} else {
			return new YUVColor(mImage.getRGB(x, y));
		}
	}
	
	

}
