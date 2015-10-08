package de.fhwf.humanoid.tools.debugger.data;

import java.awt.Color;
import java.awt.Graphics;

public class DrawObject {
 
 public enum DrawColor {
  BLACK(Color.BLACK),
  BLUE(Color.BLUE),
  GREEN(Color.GREEN),
  CYAN(Color.CYAN),
  RED(Color.RED),
  MAGENTA(Color.MAGENTA),
  BROWN(Color.WHITE),
  LIGHTGRAY(Color.LIGHT_GRAY),
  DARKGRAY(Color.DARK_GRAY),
  BRIGHTBLUE(Color.WHITE),
  BRIGHTGREEN(Color.WHITE),
  BRIGHTCYAN(Color.WHITE),
  BRIGHTRED(Color.WHITE),
  BRIGHTMAGENTA(Color.WHITE),
  BRIGHTYELLOW(Color.YELLOW),
  WHITE(Color.WHITE);
  
  private final Color mColor;

  DrawColor(Color color) {
   mColor = color;
  }

  public Color getColor() {
   return mColor;
  }
 };

 public enum DrawShape {
  POINT,
  LINE,
  CIRCLE
 };

 private String mSource;
 private DrawShape mDrawType;
 private DrawColor mPenColor;
 private int mX1;
 private int mY1;
 private int mP1;
 private int mP2;
 
 public DrawObject(String source, DrawShape shape, DrawColor penColor, int x1, int y1, int p1, int p2) {
  mSource = source;
  mDrawType = shape;
  mPenColor = penColor;
  mX1 = x1;
  mY1 = y1;
  mP1 = p1;
  mP2 = p2;
 }

 public void draw(Graphics g, double xScale, double yScale) {
  if (mDrawType.equals(DrawShape.LINE)) {
   g.setColor(mPenColor.getColor());
   g.drawLine((int)(mX1 * xScale), (int)(mY1 * yScale), (int)(mP1 * xScale), (int)(mP2 * yScale));
  } else if (mDrawType.equals(DrawShape.CIRCLE)) {
   g.setColor(mPenColor.getColor());
   g.drawOval((int)(mX1 * xScale), (int)(mY1 * yScale), (int)(mP1 * xScale), (int)(mP1 * xScale));
  } else if (mDrawType.equals(DrawShape.POINT)) {
   g.setColor(mPenColor.getColor());
   g.drawRect((int)(mX1 * xScale), (int)(mY1 * yScale), 1, 1);
  }
 }
 
 public String getSource() {
  return mSource;
 }
 
 public static DrawObject parseString(String source, String data) {
  String[] parts = data.split(",");
  if (parts.length >= 6) {
    DrawShape type = DrawShape.values()[Integer.parseInt(parts[0])];
    int x1 = Integer.parseInt(parts[1]);
    int y1 = Integer.parseInt(parts[2]);
    int p1 = Integer.parseInt(parts[3]);
    int p2 = Integer.parseInt(parts[4]);
    DrawColor penColor = DrawColor.values()[Integer.parseInt(parts[5])];
    return new DrawObject(source, type, penColor, x1, y1, p1, p2);  
  }
  return null;  
 }
}
