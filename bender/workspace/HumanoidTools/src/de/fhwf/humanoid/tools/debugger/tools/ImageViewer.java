package de.fhwf.humanoid.tools.debugger.tools;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.RenderingHints;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.event.MouseWheelEvent;
import java.awt.event.MouseWheelListener;
import java.awt.image.BufferedImage;
import java.util.ArrayList;
import java.util.List;

import javax.swing.ImageIcon;
import javax.swing.JLabel;
import javax.swing.JScrollPane;

import de.fhwf.humanoid.tools.debugger.data.DrawObject;
import de.fhwf.humanoid.tools.debugger.data.VisionObject;
import de.fhwf.humanoid.tools.debugger.data.VisionObject.ObjectType;
import de.fhwf.humanoid.tools.debugger.events.DrawObjectsEvent;
import de.fhwf.humanoid.tools.debugger.events.DrawObjectsListener;
import de.fhwf.humanoid.tools.debugger.events.ScalingChangedEvent;
import de.fhwf.humanoid.tools.debugger.events.ScalingListener;
import de.fhwf.humanoid.tools.debugger.events.VisionObjectsEvent;
import de.fhwf.humanoid.tools.debugger.events.VisionObjectsListener;
import de.fhwf.humanoid.tools.debugger.services.ScalingChangedService;
import de.fhwf.humanoid.tools.debugger.services.ServiceManager;
import de.fhwf.humanoid.tools.debugger.services.VisionDataService;

public class ImageViewer extends JScrollPane implements MouseListener,
		MouseMotionListener, VisionObjectsListener, ComponentListener,
		ScalingListener, DrawObjectsListener {
	private static final long serialVersionUID = 6127397178526170361L;

	private int CAMERA_WIDTH = 800;
	private int CAMERA_HEIGHT = 600;

	private VisionDataService mVisionDataService;
	private ScalingChangedService mScalingChangedService;
	private JLabel mViewer;
	private BufferedImage mOriginalImage;
	private Image mCurrentImage = null;
	private double mScale;
	private List<VisionObject> mVisionObjects;
	private boolean drawing = false;
	private int scalingFactor = 1;
	private List<DrawObject> mDrawObjects;
	private String mSelectedAnnotations = null;

	public ImageViewer() {
		this(null);
	}

	public ImageViewer(BufferedImage image) {
		super();
		mVisionDataService = (VisionDataService) ServiceManager.getInstance()
				.getService(VisionDataService.SERVICENAME);
		mVisionObjects = new ArrayList<VisionObject>();
		mVisionDataService.addVisionObjectsListener(this);
		mVisionDataService.addDrawObjectsListener(this);

		mDrawObjects = new ArrayList<DrawObject>();
		
		mScalingChangedService = (ScalingChangedService) ServiceManager
				.getInstance().getService(ScalingChangedService.SERVICENAME);
		addComponentListener(this);
		mScale = 1.0;
		mOriginalImage = image;
		mCurrentImage = mOriginalImage;
		mViewer = new JLabel() {
			private static final long serialVersionUID = -6386423129618604443L;

			@Override
			public void paint(Graphics g) {
				super.paint(g);
				paintVisionObjects(g);
			}
		};

		mViewer.setVerticalAlignment(1); // Bild oben
		// mViewer.setHorizontalAlignment(0); // Mitte

		mViewer.addMouseListener(this);
		mViewer.addMouseMotionListener(this);
		setViewportView(mViewer);
		if (image != null) {
			setImage(image);
		}

		mViewer.addMouseWheelListener(new MouseWheelListener() {
			public void mouseWheelMoved(MouseWheelEvent e) {
				int u = e.getUnitsToScroll();
				if (u > 0)
					setActualScale(-0.1);
				if (u < 0)
					setActualScale(0.1);
			}
		});
		/*
		 * // Drag to move functionality mViewer.addMouseListener(new
		 * MouseAdapter() { private Point initPoint;
		 * 
		 * public void mousePressed(MouseEvent e) { initPoint = e.getPoint(); }
		 * 
		 * public void mouseReleased(MouseEvent e) { int x = initPoint.x -
		 * e.getPoint().x; int y = initPoint.y - e.getPoint().y; JScrollBar h =
		 * getHorizontalScrollBar(); if (h != null) { h.setValue(h.getValue() +
		 * (x / h.getUnitIncrement())); } JScrollBar v = getVerticalScrollBar();
		 * if (v != null) { v.setValue(v.getValue() + (y /
		 * v.getUnitIncrement())); } } });
		 */
	}

	public void activateImageStream() {
		drawing = true;
		mVisionDataService.startVisionObjectStream();
		repaint();
	}

	public void deactivateImageStream() {
		drawing = false;
		mVisionDataService.stopVisionObjectStream();
		mVisionObjects.clear();
		repaint();
	}

	public void setActualScale(double adjust) {
		mScale += adjust;

		int w = (int) (mOriginalImage.getWidth() * mScale);
		int h = (int) (mOriginalImage.getHeight() * mScale);
		CAMERA_WIDTH = mOriginalImage.getWidth();
		CAMERA_HEIGHT = mOriginalImage.getHeight();
		double ratio = 1;
		if (w <= this.getWidth() && getWidth() <= getHeight()) {
			ratio = (double) this.getWidth() / (double) w;

		}
		if (h <= this.getHeight() && getWidth() > getHeight()) {
			ratio = (double) this.getHeight() / (double) h;
			if (getWidth() - getHeight() < 100) {
				ratio = (double) this.getWidth() / (double) w;
			}
		}
		if (ratio < 1) {
			ratio = 1;
		}
		CAMERA_WIDTH = (int) (((double) CAMERA_WIDTH) * ratio);
		CAMERA_HEIGHT = (int) (((double) CAMERA_HEIGHT) * ratio);
		BufferedImage tmp = new BufferedImage(w, h, BufferedImage.TYPE_INT_RGB);
		Graphics2D g2 = tmp.createGraphics();
		g2.setRenderingHint(RenderingHints.KEY_INTERPOLATION,
				RenderingHints.VALUE_INTERPOLATION_NEAREST_NEIGHBOR);
		g2.drawImage(mOriginalImage, 0, 0, w, h, null);
		g2.dispose();
		mCurrentImage = tmp;

		// TODO Richtige Rechnung?

		mViewer.setIcon(new ImageIcon(mCurrentImage));
		mViewer.repaint();
	}

	public void setImage(BufferedImage image) {
		if (mOriginalImage == null) {
			if (image != null) {
				mOriginalImage = image;
				mCurrentImage = mOriginalImage;

				// System.out.println("OrginalImage Width: "
				// + mOriginalImage.getWidth());
				// System.out.println("OrginalImage Heigth: "
				// + mOriginalImage.getHeight());

				setActualScale(0);
				mScalingChangedService.addScalingListener(this);
			}
		} else {
			if (image != null) {
				mOriginalImage = image;
				mCurrentImage = mOriginalImage;

				// System.out.println("OrginalImage Width: "
				// + mOriginalImage.getWidth());
				// System.out.println("OrginalImage Heigth: "
				// + mOriginalImage.getHeight());

				setActualScale(0);
			}
		}

	}

	public BufferedImage getImage() {
		return mOriginalImage;
	}
	
	public void setSelectedAnnotations(String selectedAnnotations){
		mSelectedAnnotations = selectedAnnotations;
	}

	public List<DrawObject> getDrawObjects(){
		return mDrawObjects;
	}
	
	@Override
	public void mouseClicked(MouseEvent e) {
		int newX = (int) (e.getX() / mScale);
		int newY = (int) (e.getY() / mScale);
		MouseEvent e2 = new MouseEvent(e.getComponent(), e.getID(),
				e.getWhen(), e.getModifiers(), newX, newY, e.getClickCount(),
				e.isPopupTrigger(), e.getButton());
		this.dispatchEvent(e2);
	}

	@Override
	public void mouseEntered(MouseEvent e) {
		int newX = (int) (e.getX() / mScale);
		int newY = (int) (e.getY() / mScale);
		MouseEvent e2 = new MouseEvent(e.getComponent(), e.getID(),
				e.getWhen(), e.getModifiers(), newX, newY, e.getClickCount(),
				e.isPopupTrigger(), e.getButton());
		this.dispatchEvent(e2);
	}

	@Override
	public void mouseExited(MouseEvent e) {
		int newX = (int) (e.getX() / mScale);
		int newY = (int) (e.getY() / mScale);
		MouseEvent e2 = new MouseEvent(e.getComponent(), e.getID(),
				e.getWhen(), e.getModifiers(), newX, newY, e.getClickCount(),
				e.isPopupTrigger(), e.getButton());
		this.dispatchEvent(e2);
	}

	@Override
	public void mousePressed(MouseEvent e) {
		int newX = (int) (e.getX() / mScale);
		int newY = (int) (e.getY() / mScale);
		MouseEvent e2 = new MouseEvent(e.getComponent(), e.getID(),
				e.getWhen(), e.getModifiers(), newX, newY, e.getClickCount(),
				e.isPopupTrigger(), e.getButton());
		this.dispatchEvent(e2);
	}

	@Override
	public void mouseReleased(MouseEvent e) {
		int newX = (int) (e.getX() / mScale);
		int newY = (int) (e.getY() / mScale);
		MouseEvent e2 = new MouseEvent(e.getComponent(), e.getID(),
				e.getWhen(), e.getModifiers(), newX, newY, e.getClickCount(),
				e.isPopupTrigger(), e.getButton());
		this.dispatchEvent(e2);
	}

	@Override
	public void mouseDragged(MouseEvent e) {
		int newX = (int) (e.getX() / mScale);
		int newY = (int) (e.getY() / mScale);
		MouseEvent e2 = new MouseEvent(e.getComponent(), e.getID(),
				e.getWhen(), e.getModifiers(), newX, newY, e.getClickCount(),
				e.isPopupTrigger(), e.getButton());
		this.dispatchEvent(e2);
	}

	@Override
	public void mouseMoved(MouseEvent e) {
		int newX = (int) (e.getX() / mScale);
		int newY = (int) (e.getY() / mScale);
		MouseEvent e2 = new MouseEvent(e.getComponent(), e.getID(),
				e.getWhen(), e.getModifiers(), newX, newY, e.getClickCount(),
				e.isPopupTrigger(), e.getButton());
		this.dispatchEvent(e2);
	}

	protected void paintVisionObjects(Graphics g) {
		int width = mViewer.getWidth();
		int height = mViewer.getHeight();

		if (width < height) {
			height = (int) ((double) width * ((double) CAMERA_HEIGHT / (double) CAMERA_WIDTH));
			if (height > mViewer.getHeight()) {
				width = mViewer.getWidth();
				height = mViewer.getHeight();
				width = (int) ((double) height * ((double) CAMERA_WIDTH / (double) CAMERA_HEIGHT));
			}
		} else {
			width = (int) ((double) height * ((double) CAMERA_WIDTH / (double) CAMERA_HEIGHT));
			if (width > mViewer.getWidth()) {
				width = mViewer.getWidth();
				height = mViewer.getHeight();
				height = (int) ((double) width * ((double) CAMERA_HEIGHT / (double) CAMERA_WIDTH));
			}
		}

		Graphics2D g2d = (Graphics2D) g;
		g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING,
				RenderingHints.VALUE_ANTIALIAS_ON);
		/*
		 * g2d.clearRect(0, 0, mViewer.getWidth(), mViewer.getHeight());
		 * g2d.setColor(new Color(0, 128, 0)); g2d.fillRect(0, 0, width,
		 * height);
		 */
		if (mVisionObjects.size() > 0) {
			synchronized (mVisionObjects) {
				for (VisionObject object : mVisionObjects) {
					if (object != null) {
						if (object.getBoundingBox().width > 0
								|| object.getBoundingBox().height > 0) {
							Color c = Color.BLACK;
							if (object.getType().equals(
									ObjectType.YELLOW_GOAL_POLE)) {
								c = Color.YELLOW;
							} else if (object.getType().equals(
									ObjectType.BLUE_GOAL_POLE)) {
								c = Color.BLUE;
							} else if (object.getType().equals(ObjectType.BALL)) {
								c = Color.RED;
							} else if (object.getType().equals(
									ObjectType.BLUE_LANDMARK_POLE)) {
								c = Color.BLUE;
							} else if (object.getType().equals(
									ObjectType.YELLOW_LANDMARK_POLE)) {
								c = Color.YELLOW;
							}

							Rectangle r = rescaleBoundingBox(
									object.getBoundingBox(), width, height);
							g2d.setPaint(new Color(c.getRed(), c.getGreen(), c
									.getBlue(), 100));
							g2d.fill(r);

							g2d.setPaint(new Color(c.getRed(), c.getGreen(), c
									.getBlue(), 200));
							g2d.draw(r);
							Point pos = rescalePosition(object.getPosition(),
									width, height);
							// Point pos = new Point(
							// (int)(r.getMinX()+((r.getMaxX()-r.getMinX())/2)),
							// (int)(r.getMinY()+((r.getMaxY()-r.getMinY())/2)));
							g2d.setPaint(Color.BLACK);
							drawCross(g2d, pos);
							g2d.drawString("d=" + object.getDistance(),
									pos.x + 8, pos.y + 4);
							/*
							 * g2d.drawString("bb=("
							 * +object.getBoundingBox().getMinX()+", "+
							 * object.getBoundingBox().getMinY()+") - ("+
							 * object.getBoundingBox().getMaxX()+", "+
							 * object.getBoundingBox().getMaxY()+")" , pos.x +
							 * 8, pos.y + 14);
							 * 
							 * g2d.drawString("w=" +
							 * object.getBoundingBox().width, pos.x + 8, pos.y +
							 * 24); g2d.drawString("h=" +
							 * object.getBoundingBox().height, pos.x + 8, pos.y
							 * + 34);
							 */
							g2d.drawString("center = " + object.getPosition().x
									+ "," + object.getPosition().y, pos.x + 8,
									pos.y + 14);
						}

						// Point pos = rescalePosition(object.getPosition());
						/*
						 * Point pos = new Point(, y);
						 * g2d.setPaint(Color.BLACK); drawCross(g2d, pos);
						 * g2d.drawString("d=" + object.getDistance(), pos.x +
						 * 8, pos.y + 4);
						 */
					}
				}
			}
		}
		if (mDrawObjects.size() > 0) {
			double xScale = (double) width / (double) CAMERA_WIDTH / scalingFactor;
			double yScale = (double) height / (double) CAMERA_HEIGHT / scalingFactor;
			synchronized (mDrawObjects) {
				for (DrawObject object : mDrawObjects) {
					if (object != null) {
						if (mSelectedAnnotations == null || mSelectedAnnotations.contains("," + object.getSource().toLowerCase() + ",")){
							object.draw(g2d, xScale, yScale);
						}
					}
				}
			}
		}
		
	}

	private Rectangle rescaleBoundingBox(Rectangle boundingBox, int width,
			int height) {
		double xScale = (double) width / (double) CAMERA_WIDTH;
		double yScale = (double) height / (double) CAMERA_HEIGHT;

		return new Rectangle((int) (boundingBox.x * xScale),
				(int) (boundingBox.y * xScale),
				(int) (boundingBox.width * xScale),
				(int) (boundingBox.height * yScale));
	}

	private Point rescalePosition(Point position, int width, int height) {
		double xScale = (double) width / (double) CAMERA_WIDTH;
		double yScale = (double) height / (double) CAMERA_HEIGHT;

		return new Point((int) (position.x * xScale),
				(int) (position.y * yScale));
	}

	private void drawCross(Graphics2D g2d, Point pos) {
		g2d.drawLine(pos.x - 4, pos.y - 4, pos.x + 4, pos.y + 4);
		g2d.drawLine(pos.x + 4, pos.y - 4, pos.x - 4, pos.y + 4);
	}

	@Override
	public void newVisionObjects(VisionObjectsEvent e) {
		if (drawing) {
			synchronized (mVisionObjects) {
				mVisionObjects.clear();
				mVisionObjects.addAll(e.getVisionObjects());
			}
		} else {
			mVisionObjects.clear();
		}
		mViewer.repaint();
	}

	@Override
	public void componentHidden(ComponentEvent arg0) {
		// TODO Auto-generated method stub

	}

	@Override
	public void componentMoved(ComponentEvent arg0) {
		// TODO Auto-generated method stub

	}

	@Override
	public void componentResized(ComponentEvent arg0) {
		if (mCurrentImage != null) {
			setActualScale(0);
		}

	}

	@Override
	public void componentShown(ComponentEvent arg0) {
		// TODO Auto-generated method stub

	}

	@Override
	public void newScaling(ScalingChangedEvent e) {
		// System.out.println("Event: "+e.getScalingFactor());
		if (e.getScalingFactor() != scalingFactor) {
			scalingFactor = e.getScalingFactor();
		}

	}

	@Override
	public void newDrawObjects(DrawObjectsEvent e) {
		synchronized (mDrawObjects) {
			mDrawObjects.clear();
			mDrawObjects.addAll(e.getDrawObjects());
		}
		mViewer.repaint();
	}
}