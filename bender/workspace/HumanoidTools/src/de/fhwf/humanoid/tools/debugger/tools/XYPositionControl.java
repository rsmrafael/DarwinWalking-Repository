package de.fhwf.humanoid.tools.debugger.tools;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import javax.swing.JPanel;
import javax.swing.event.EventListenerList;
import de.fhwf.humanoid.tools.debugger.events.XYChangedEvent;
import de.fhwf.humanoid.tools.debugger.events.XYChangedListener;

public class XYPositionControl extends JPanel {
 private static final long serialVersionUID = 7479809508185100194L;

 private static final int HANDLE_SIZE_FACTOR = 10;

 private int mCurPosX;
 private int mCurPosY;
 private int mXMin;
 private int mXMax;
 private int mYMin;
 private int mYMax;
 private EventListenerList mListeners = new EventListenerList();
 private boolean mEnabled;
 
 public XYPositionControl(int xMin, int yMin, int xMax, int yMax) {
  mXMin = xMin; mXMax = xMax;
  mYMin = yMin; mYMax = yMax;
  mCurPosX = mXMin + ((mXMax - mXMin) / 2);
  mCurPosY = mYMin + ((mYMax - mYMin) / 2);
  mEnabled = true;
  
  this.addMouseListener(new MouseAdapter() {
   @Override
   public void mouseClicked(MouseEvent e) {
    if (!mEnabled) { return; }
    if (e.getClickCount() == 2) {
     XYPositionControl parent = (XYPositionControl)e.getSource();
     // Re-center value
     mCurPosX = mXMin + ((mXMax - mXMin) / 2);
     mCurPosY = mYMin + ((mYMax - mYMin) / 2);     
     parent.repaint();
     notifyXYChanged(new XYChangedEvent(parent, mCurPosX, mCurPosY));
    } else {
     XYPositionControl parent = (XYPositionControl)e.getSource();
     mCurPosX = (int)(((double)(mXMax - mXMin) / (double)parent.getWidth()) * (double)e.getX()) + mXMin;
     mCurPosY = (int)(((double)(mYMax - mYMin) / (double)parent.getHeight()) * (double)e.getY()) + mYMin;
     if (mCurPosX < mXMin) { mCurPosX = mXMin; }
     if (mCurPosX > mXMax) { mCurPosX = mXMax; }
     if (mCurPosY < mYMin) { mCurPosY = mYMin; }
     if (mCurPosY > mYMax) { mCurPosY = mYMax; }
     parent.repaint();
     notifyXYChanged(new XYChangedEvent(parent, mCurPosX, mCurPosY));
    }
   }
  });
  
  this.addMouseMotionListener(new MouseAdapter() {
   @Override
   public void mouseDragged(MouseEvent e) {
    super.mouseDragged(e);
    if (!mEnabled) { return; }
    XYPositionControl parent = (XYPositionControl)e.getSource();
    mCurPosX = (int)(((double)(mXMax - mXMin) / (double)parent.getWidth()) * (double)e.getX()) + mXMin;
    mCurPosY = (int)(((double)(mYMax - mYMin) / (double)parent.getHeight()) * (double)e.getY()) + mYMin;
    if (mCurPosX < mXMin) { mCurPosX = mXMin; }
    if (mCurPosX > mXMax) { mCurPosX = mXMax; }
    if (mCurPosY < mYMin) { mCurPosY = mYMin; }
    if (mCurPosY > mYMax) { mCurPosY = mYMax; }
    parent.repaint();
    notifyXYChanged(new XYChangedEvent(parent, mCurPosX, mCurPosY));
   }
  });
 }

 public void setXY(int x, int y) {
  mCurPosX = x;
  mCurPosY = y;
  if (mCurPosX < mXMin) { mCurPosX = mXMin; }
  if (mCurPosX > mXMax) { mCurPosX = mXMax; }
  if (mCurPosY < mYMin) { mCurPosY = mYMin; }
  if (mCurPosY > mYMax) { mCurPosY = mYMax; }
  this.repaint();
  notifyXYChanged(new XYChangedEvent(this, mCurPosX, mCurPosY));
 }

 public int getXValue() {
  return mCurPosX;
 }
 
 public int getYValue() {
  return mCurPosY;
 }
 
 @Override
 public void paint(Graphics g) {
  g.setColor(new Color(255, 255, 192));
  g.fillRect(0, 0, this.getWidth()-1, this.getHeight()-1);
  if (mEnabled) {
   g.setColor(Color.BLACK);
  } else {
   g.setColor(Color.DARK_GRAY);
  }
  g.drawRect(0, 0, this.getWidth()-1, this.getHeight()-1);
  g.drawLine(0, this.getHeight() / 2, this.getWidth(), this.getHeight() / 2);
  g.drawLine(this.getWidth() / 2, 0, this.getWidth() / 2, this.getHeight());
  
  int xPos = (int)(((double)this.getWidth() / (double)(mXMax - mXMin)) * (double)(mCurPosX - mXMin));
  int yPos = (int)(((double)this.getHeight() / (double)(mYMax - mYMin)) * (double)(mCurPosY - mYMin));
   
  g.fillOval(xPos - ((this.getWidth() / HANDLE_SIZE_FACTOR) / 2), yPos - ((this.getHeight() / HANDLE_SIZE_FACTOR) / 2), this.getWidth() / HANDLE_SIZE_FACTOR, this.getHeight() / HANDLE_SIZE_FACTOR);
 }
 
 @Override
 public void setEnabled(boolean enabled) {
  super.setEnabled(enabled);
  mEnabled = enabled;
  this.repaint();
 }
 
 public void addXYChangedListener(XYChangedListener listener) {
  mListeners.add(XYChangedListener.class, listener);
 }

 public void removeXYChangedListener(XYChangedListener listener) {
  mListeners.remove(XYChangedListener.class, listener);
 }

 private synchronized void notifyXYChanged(XYChangedEvent event) {
  for (XYChangedListener l : mListeners.getListeners(XYChangedListener.class)) {
   l.XYChanged(event);
  }
 }

 public void raiseChanged() {
  notifyXYChanged(new XYChangedEvent(this, mCurPosX, mCurPosY));
 }
}
