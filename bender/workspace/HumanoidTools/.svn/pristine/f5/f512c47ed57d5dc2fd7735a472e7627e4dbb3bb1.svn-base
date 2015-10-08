package de.fhwf.humanoid.tools.debugger.views;

import java.awt.AlphaComposite;
import java.awt.BasicStroke;
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Composite;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Rectangle;
import java.awt.RenderingHints;
import java.awt.geom.Ellipse2D;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import javax.swing.JButton;
import javax.swing.JPanel;

import de.fhwf.humanoid.tools.debugger.data.VisionObject;
import de.fhwf.humanoid.tools.debugger.data.VisionObject.ObjectType;
import de.fhwf.humanoid.tools.debugger.events.VisionObjectsEvent;
import de.fhwf.humanoid.tools.debugger.events.VisionObjectsListener;

public class LocalizationView extends JPanel implements VisionObjectsListener {
	private static final long serialVersionUID = 1L;
	//private VisionDataService mVisionDataService;
	private List<VisionObject> mVisionObjects;
	private JPanel mControllPanel;
	private JPanel mFieldPanel;
	private JButton dummy1;
	private JButton dummy2;
	private JButton dummy3;
	private int width;
	private int height;

	private Graphics2D g2d;

	public LocalizationView() {
		super();
		setLayout(new BorderLayout(2, 2));
		//mVisionDataService = (VisionDataService) ServiceManager.getInstance()
		//		.getService(VisionDataService.SERVICENAME);
		mVisionObjects = new ArrayList<VisionObject>();

		mControllPanel = new JPanel();

		dummy1 = new JButton("Dummy1");
		dummy2 = new JButton("Dummy2");
		dummy3 = new JButton("Dummy3");

		mControllPanel.add(dummy1);
		mControllPanel.add(dummy2);
		mControllPanel.add(dummy3);

		mFieldPanel = new JPanel() {

			private static final long serialVersionUID = 1L;

			@Override
			public void paintComponent(Graphics g) {
				try {
					paintField(g);
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		};
		mFieldPanel.setPreferredSize(new Dimension(600, 400));
		add(mControllPanel, BorderLayout.PAGE_START);
		add(mFieldPanel);
	}

	private void createPlayfield() {
		g2d.setColor(new Color(0, 128, 0));
		g2d.fill(new Rectangle.Double(0, 0, ((double) width), ((double) height)));
		System.out.println(width);
		System.out.println(height);
		System.out.println("******");

		g2d.setColor(Color.WHITE);
		if (width >= 600) {
			g2d.setStroke(new BasicStroke(5));
		} else if (width >= 500) {
			g2d.setStroke(new BasicStroke(4));
		} else if (width >= 400) {
			g2d.setStroke(new BasicStroke(3));
		} else if (width >= 300) {
			g2d.setStroke(new BasicStroke(2));
		}

		g2d.draw(new Rectangle.Double(0, 0, ((double) width), ((double) height)));
		g2d.drawLine(width / 2, 0, width / 2, height);
		// Mittelkreis
		g2d.draw(new Ellipse2D.Double((double) width / 2.0 - (1.0 / 10.0)
				* (double) width, (double) height / 2 - (1.0 / 10.0)
				* (double) width, (1.0 / 5.0) * (double) width, (1.0 / 5.0)
				* (double) width));
		// 16m mitte links
		g2d.drawLine(
				(int) (1.0 / 10.0 * (double) width),
				(int) (1.0 / 12.0 * (double) width),
				(int) (1.0 / 10.0 * (double) width),
				(int) (1.0 / 12.0 * (double) width + (1.0 / 2.0 * (double) width)));
		// 16m mitte rechts
		g2d.drawLine(
				width - (int) (1.0 / 10.0 * (double) width),
				(int) (1.0 / 12.0 * (double) width),
				width - (int) (1.0 / 10.0 * (double) width),
				(int) (1.0 / 12.0 * (double) width + (1.0 / 2.0 * (double) width)));
		// 16m oben links
		g2d.drawLine(0, (int) (1.0 / 12.0 * (double) width),
				(int) (1.0 / 10.0 * (double) width),
				(int) (1.0 / 12.0 * (double) width));
		// 16m oben rechts
		g2d.drawLine(width - (int) (1.0 / 10.0 * (double) width),
				(int) (1.0 / 12.0 * (double) width), width,
				(int) (1.0 / 12.0 * (double) width));
		// 16m unten rechts
		g2d.drawLine(
				width - (int) (1.0 / 10.0 * (double) width),
				(int) (1.0 / 12.0 * (double) width + (1.0 / 2.0 * (double) width)),
				width,
				(int) (1.0 / 12.0 * (double) width + (1.0 / 2.0 * (double) width)));
		// 16m unten links
		g2d.drawLine(
				0,
				(int) (1.0 / 12.0 * (double) width + (1.0 / 2.0 * (double) width)),
				(int) (1.0 / 10.0 * (double) width),
				(int) (1.0 / 12.0 * (double) width + (1.0 / 2.0 * (double) width)));
		// penalty links
		g2d.fill(new Rectangle.Double(3.0 / 10.0 * (double) width
				- (1.0 / 120.0) * (double) width, (double) height / 2.0
				- (1.0 / 120.0) * (double) width,
				(1.0 / 60.0) * (double) width, (1.0 / 60.0) * (double) width));
		// penalty rechts
		g2d.fill(new Rectangle.Double(
				width
						- (3.0 / 10.0 * (double) width - (1.0 / 120.0)
								* (double) width), (double) height / 2.0
						- (1.0 / 120.0) * (double) width, (1.0 / 60.0)
						* (double) width, (1.0 / 60.0) * (double) width));

		// goal blue
		g2d.setColor(Color.BLUE);
		g2d.fill(new Rectangle.Double(0, (5.0 / 24.0 * (double) width),
				(1.0 / 80.0) * (double) width, ((1.0 / 4.0 * (double) width))));
		// goal yellow
		g2d.setColor(Color.YELLOW);
		g2d.fill(new Rectangle.Double(width - (1.0 / 80.0) * (double) width,
				(5.0 / 24.0 * (double) width), (1.0 / 80.0) * (double) width,
				((1.0 / 4.0 * (double) width))));
		g2d.setStroke(new BasicStroke(1));

	}

	protected void paintField(Graphics g) throws IOException {
		g2d = (Graphics2D) g;
		g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING,
				RenderingHints.VALUE_ANTIALIAS_ON);
		super.paintComponent(g);
		width = mFieldPanel.getWidth();
		height = mFieldPanel.getHeight();

		int LabelWidth = 600;
		int LabelHeight = 400;

		if (width < height) {
			height = (int) ((double) width * ((double) LabelHeight / (double) LabelWidth));
			if (height > getHeight()) {
				width = getWidth();
				height = getHeight();
				width = (int) ((double) height * ((double) LabelWidth / (double) LabelHeight));
			}
		} else {
			width = (int) ((double) height * ((double) LabelWidth / (double) LabelHeight));
			if (width > getWidth()) {
				width = getWidth();
				height = getHeight();
				height = (int) ((double) width * ((double) LabelHeight / (double) LabelWidth));
			}
		}

		createPlayfield();
		createObject(ObjectType.OBSTACLE, 100, 100, 0.2f);
		createObject(ObjectType.OBSTACLE, 200, 200, 0.4f);
		createObject(ObjectType.BALL, 300, 300, 0.6f);
		createObject(ObjectType.BALL, 400, 200, 1f);

	}

	public void createObject(ObjectType o, int x, int y, float alpha) {
		Composite original = g2d.getComposite();
		AlphaComposite ac = AlphaComposite.getInstance(AlphaComposite.SRC_OVER,
				alpha);
		double delta = (double) width / 600.0;
		g2d.setComposite(ac);
		if (o == ObjectType.BALL) {
			g2d.setColor(new Color(255 ,140, 0));
			x = (int) (x * delta);
			y = (int) (y * delta);
			g2d.fill(new Ellipse2D.Double((double) x - (1.0 / 60.0)
					* (double) width, (double) y - (1.0 / 60.0)
					* (double) width, (1.0 / 30.0) * (double) width,
					(1.0 / 30.0) * (double) width));
		} else if (o == ObjectType.OBSTACLE) {
			g2d.setColor(Color.BLACK);
			x = (int) (x * delta);
			y = (int) (y * delta);
			g2d.fill(new Ellipse2D.Double((double) x - (1.0 / 60.0)
					* (double) width, (double) y - (1.0 / 60.0)
					* (double) width, (1.0 / 30.0) * (double) width,
					(1.0 / 30.0) * (double) width));

		}

		g2d.setComposite(original);
	}

	@Override
	public void newVisionObjects(VisionObjectsEvent e) {
		synchronized (mVisionObjects) {
			mVisionObjects.clear();
			mVisionObjects.addAll(e.getVisionObjects());
		}
		repaint();

	}
}