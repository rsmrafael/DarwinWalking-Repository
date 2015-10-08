package de.fhwf.humanoid.tools.debugger.views;

import java.awt.BasicStroke;
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.GridLayout;
import java.awt.Image;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.RenderingHints;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.geom.Ellipse2D;
import java.awt.geom.Line2D;
import java.awt.geom.Rectangle2D;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import javax.imageio.ImageIO;
import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTabbedPane;
import javax.swing.border.BevelBorder;
import de.fhwf.humanoid.tools.debugger.data.DrawObject;
import de.fhwf.humanoid.tools.debugger.data.VisionObject;
import de.fhwf.humanoid.tools.debugger.data.VisionObject.ObjectType;
import de.fhwf.humanoid.tools.debugger.events.DrawObjectsEvent;
import de.fhwf.humanoid.tools.debugger.events.DrawObjectsListener;
import de.fhwf.humanoid.tools.debugger.events.VisionObjectsEvent;
import de.fhwf.humanoid.tools.debugger.events.VisionObjectsListener;
import de.fhwf.humanoid.tools.debugger.services.ServiceManager;
import de.fhwf.humanoid.tools.debugger.services.VisionDataService;

/**
 * This class enables Viewing of Objects in real time. Radar by Dominik Molitor
 * Last change 16.02.2012
 * 
 * @TODO Berechnung der Torplatzierungen �berarbeiten/neu machen.
 */
public class VisionObjectsView extends JPanel implements ActionListener,
		VisionObjectsListener, DrawObjectsListener {
	private static final long serialVersionUID = 8606340494744461143L;
	private static final int MAX_DISPLAYABLE_DISTANCE = 6; // meters

	private VisionDataService mVisionDataService;
	private JTabbedPane tab;
	private JLabel mImage;
	private JLabel mRadarView;
	private JLabel mLblShow;
	private JCheckBox mChkBall;
	private JCheckBox mChkYellowGoalPole;
	private JCheckBox mChkBlueGoalPole;
	private JCheckBox mChkYellowLandmark;
	private JCheckBox mChkBlueLandmark;
	private JButton mBtnStartStopStream;
	private List<VisionObject> mVisionObjects;
	private List<DrawObject> mDrawObjects;
	private double circle;
	private int width;
	private int height;
	private Graphics2D g2d;
	private static int mCameraWidth = 352;
	private static int mCameraHeight = 288;

	public VisionObjectsView() {
		super();

		mVisionDataService = (VisionDataService) ServiceManager.getInstance()
				.getService(VisionDataService.SERVICENAME);
		mVisionObjects = new ArrayList<VisionObject>();
		mDrawObjects = new ArrayList<DrawObject>();

		this.setLayout(new BorderLayout());

		mImage = new JLabel("Vision Objects") {
			private static final long serialVersionUID = -6386423129618604443L;

			@Override
			public void paint(Graphics g) {
				paintVisionObjects(g);
			}
		};
		mImage.setBorder(BorderFactory.createBevelBorder(BevelBorder.RAISED));
		mImage.setPreferredSize(new Dimension(405, 305));

		mRadarView = new JLabel("Radar View") {

			private static final long serialVersionUID = 1L;

			@Override
			public void paintComponent(Graphics g) {
				try {
					paintRadar(g);
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		};
		mRadarView.setBorder(BorderFactory
				.createBevelBorder(BevelBorder.RAISED));
		mRadarView.setPreferredSize(new Dimension(405, 305));

		tab = new JTabbedPane();

		tab.add("VisionObjects", mImage);
		tab.add("RadarView", mRadarView);

		this.add(tab, BorderLayout.CENTER);

		JPanel container = new JPanel();
		container.setLayout(new GridLayout(2, 4));

		mLblShow = new JLabel("Show objects:");
		container.add(mLblShow);

		mChkBall = new JCheckBox("Ball");
		mChkBall.setSelected(true);
		container.add(mChkBall);

		mChkYellowGoalPole = new JCheckBox("Yellow goal");
		mChkYellowGoalPole.setSelected(true);
		container.add(mChkYellowGoalPole);

		mChkBlueGoalPole = new JCheckBox("Blue goal");
		mChkBlueGoalPole.setSelected(true);
		container.add(mChkBlueGoalPole);

		container.add(new JLabel());

		mChkYellowLandmark = new JCheckBox("Yellow landmark");
		mChkYellowLandmark.setSelected(true);
		container.add(mChkYellowLandmark);

		mChkBlueLandmark = new JCheckBox("Blue landmark");
		mChkBlueLandmark.setSelected(true);
		container.add(mChkBlueLandmark);

		mBtnStartStopStream = new JButton("Start object stream");
		mBtnStartStopStream.setActionCommand("start_stream");
		mBtnStartStopStream.addActionListener(this);
		container.add(mBtnStartStopStream);

		this.add(container, BorderLayout.SOUTH);

		mVisionDataService.addVisionObjectsListener(this);
		mVisionDataService.addDrawObjectsListener(this);
	}

	private void createRadar() {
		// Gr�ner Radarkreis
		g2d.setColor(new Color(0, 128, 0));
		g2d.fillOval(2, 2, width - 6, height - 6);

		// Ringe
		g2d.setColor(Color.BLACK);
		g2d.draw(new Ellipse2D.Double(2, 2, height - 6, height - 6));
		g2d.draw(new Ellipse2D.Double((2 + ((1.0 / 12.0) * (double) height)),
				(2 + ((1.0 / 12.0) * (double) height)),
				((double) height * (5.0 / 6.0)) - 6,
				((double) height * (5.0 / 6.0)) - 6));

		g2d.draw(new Ellipse2D.Double((2 + ((2.0 / 12.0) * (double) height)),
				(2 + ((2.0 / 12.0) * (double) height)),
				((double) height * (4.0 / 6.0)) - 6,
				((double) height * (4.0 / 6.0)) - 6));

		g2d.draw(new Ellipse2D.Double((2 + ((3.0 / 12.0) * (double) height)),
				(2 + ((3.0 / 12.0) * (double) height)),
				((double) height * (3.0 / 6.0)) - 6,
				((double) height * (3.0 / 6.0)) - 6));

		g2d.draw(new Ellipse2D.Double((2 + ((4.0 / 12.0) * (double) height)),
				(2 + ((4.0 / 12.0) * (double) height)),
				((double) height * (2.0 / 6.0)) - 6,
				((double) height * (2.0 / 6.0)) - 6));

		g2d.draw(new Ellipse2D.Double((2 + ((5.0 / 12.0) * (double) height)),
				(2 + ((5.0 / 12.0) * (double) height)),
				((double) height * (1.0 / 6.0)) - 6,
				((double) height * (1.0 / 6.0)) - 6));

		// Kamera
		g2d.fill(new Rectangle.Double(((266.0 / 555.0) * (double) height),
				((266.0 / 555.0) * (double) height),
				((1.0 / 25.0) * (double) height),
				((1.0 / 25.0) * (double) height)));

		// Kreuz
		g2d.draw(new Line2D.Double((height / 2) - 2, 2, (height / 2) - 2,
				(height) - 4));
		g2d.draw(new Line2D.Double(2, (height / 2) - 2, (height) - 4,
				(height / 2) - 2));
	}

	private double[] getCircleCords(double distance, double angle) {
		double[] cords = new double[2];
		if (distance > MAX_DISPLAYABLE_DISTANCE) {
			distance = MAX_DISPLAYABLE_DISTANCE;
		}
		cords[0] = (2 + ((double) height / 2.0) + (((distance / 12.0) * (double) height) * (Math
				.cos((angle) - (Math.PI / 2.0))))) - (circle * (5.0 / 6.0));
		cords[1] = (2 + ((double) height / 2.0) + (((distance / 12.0) * (double) height) * (Math
				.sin((angle) - (Math.PI / 2.0))))) - (circle * (5.0 / 6.0));

		return cords;
	}

	protected void paintRadar(Graphics g) throws IOException {
		super.paintComponent(g);
		width = mRadarView.getWidth();
		height = mRadarView.getHeight();
		circle = (1.0 / 25.0) * (double) height;

		int LabelWidth = 600;
		int LabelHeight = 600;

		if (width < height) {
			height = (int) ((double) width * ((double) LabelHeight / (double) LabelWidth));
			if (height > mRadarView.getHeight()) {
				width = mRadarView.getWidth();
				height = mRadarView.getHeight();
				width = (int) ((double) height * ((double) LabelWidth / (double) LabelHeight));
			}
		} else {
			width = (int) ((double) height * ((double) LabelWidth / (double) LabelHeight));
			if (width > mRadarView.getWidth()) {
				width = mRadarView.getWidth();
				height = mRadarView.getHeight();
				height = (int) ((double) width * ((double) LabelHeight / (double) LabelWidth));
			}
		}
		g2d = (Graphics2D) g;
		g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING,
				RenderingHints.VALUE_ANTIALIAS_ON);
		// Clear
		g2d.clearRect(0, 0, mRadarView.getWidth(), mRadarView.getHeight());

		createRadar();

		// Debugg Objekte
		BufferedImage img = ImageIO.read(new File("resources/ball.png"));

		if (mVisionObjects.size() > 0) {
			synchronized (mVisionObjects) {
				for (VisionObject object : mVisionObjects) {
					if (object != null) {
						if (object.getType().equals(ObjectType.BALL)) {
							double[] cords = getCircleCords(
									object.getDistance(), object.getAngle());
							g2d.drawImage(img.getScaledInstance(
									(int) ((2.0 / 37.0) * height),
									(int) ((2.0 / 37.0) * height),
									Image.SCALE_SMOOTH), (int) cords[0],
									(int) cords[1], null);

						}
						if (object.getType()
								.equals(ObjectType.YELLOW_GOAL_POLE)) {
							g2d.setColor(Color.YELLOW);
							double[] cords = getCircleCords(
									object.getDistance(), object.getAngle());
							g2d.fill(new Ellipse2D.Double(cords[0], cords[1],
									circle, circle));
						}
						if (object.getType().equals(ObjectType.BLUE_GOAL_POLE)) {
							g2d.setColor(Color.BLUE);
							double[] cords = getCircleCords(
									object.getDistance(), object.getAngle());
							g2d.fill(new Ellipse2D.Double(cords[0], cords[1],
									circle, circle));
						}
						if (object.getType().equals(ObjectType.OBSTACLE)) {
							g2d.setColor(Color.BLACK);
							double[] cords = getCircleCords(
									object.getDistance(), object.getAngle());
							g2d.fill(new Ellipse2D.Double(cords[0], cords[1],
									circle, circle));
						}
						if (object.getType().equals(ObjectType.YELLOW_GOAL)) {
							g2d.setColor(Color.YELLOW);
							g2d.fill(new Ellipse2D.Double(
									(2 + ((double) height / 2.0) + ((((double) object
											.getDistance() / 12.0) * (double) height) * (Math
											.cos((object.getAngle())
													- ((Math.PI / 6.0) * (1.0 / (double) object
															.getDistance()))
													- (Math.PI / 2.0)))))
											- (((1.0 / 25.0) * (double) height) / 2),
									(2 + ((double) height / 2.0) + ((((double) object
											.getDistance() / 12.0) * (double) height) * (Math
											.sin((object.getAngle())
													- ((Math.PI / 6.0) * (1.0 / (double) object
															.getDistance()))
													- (Math.PI / 2.0)))))
											- (((1 / 25.0) * (double) height) / 2),
									((1.0 / 25.0) * (double) height),
									((1.0 / 25.0) * (double) height)));

							g2d.fill(new Ellipse2D.Double(
									(2 + ((double) height / 2.0) + ((((double) object
											.getDistance() / 12.0) * (double) height) * (Math
											.cos((object.getAngle())
													+ ((Math.PI / 6.0) * (1.0 / (double) object
															.getDistance()))
													- (Math.PI / 2.0)))))
											- (((1.0 / 25.0) * (double) height) / 2),
									(2 + ((double) height / 2.0) + ((((double) object
											.getDistance() / 12.0) * (double) height) * (Math
											.sin((object.getAngle())
													+ ((Math.PI / 6.0) * (1.0 / (double) object
															.getDistance()))
													- (Math.PI / 2.0)))))
											- (((1 / 25.0) * (double) height) / 2),
									((1.0 / 25.0) * (double) height),
									((1.0 / 25.0) * (double) height)));
							g2d.setStroke(new BasicStroke(2));
							g2d.draw(new Line2D.Double(
									(2 + ((double) height / 2.0) + ((((double) object
											.getDistance() / 12.0) * (double) height) * (Math
											.cos((object.getAngle())
													- ((Math.PI / 6.0) * (1.0 / (double) object
															.getDistance()))
													- (Math.PI / 2.0)))))
											- (((1.0 / 25.0) * (double) height) / 2)
											+ ((1.0 / 50.0) * (double) height),
									(2 + ((double) height / 2.0) + ((((double) object
											.getDistance() / 12.0) * (double) height) * (Math
											.sin((object.getAngle())
													- ((Math.PI / 6.0) * (1.0 / (double) object
															.getDistance()))
													- (Math.PI / 2.0)))))
											- (((1 / 25.0) * (double) height) / 2)
											+ ((1.0 / 50.0) * (double) height),
									(2 + ((double) height / 2.0) + ((((double) object
											.getDistance() / 12.0) * (double) height) * (Math
											.cos((object.getAngle())
													+ ((Math.PI / 6.0) * (1.0 / (double) object
															.getDistance()))
													- (Math.PI / 2.0)))))
											- (((1.0 / 25.0) * (double) height) / 2)
											+ ((1.0 / 50.0) * (double) height),
									(2 + ((double) height / 2.0) + ((((double) object
											.getDistance() / 12.0) * (double) height) * (Math
											.sin((object.getAngle())
													+ ((Math.PI / 6.0) * (1.0 / (double) object
															.getDistance()))
													- (Math.PI / 2.0)))))
											- (((1 / 25.0) * (double) height) / 2)
											+ ((1.0 / 50.0) * (double) height)));
							g2d.setStroke(new BasicStroke(1));
						}
						if (object.getType().equals(ObjectType.BLUE_GOAL)) {
							g2d.setColor(Color.BLUE);
							g2d.fill(new Ellipse2D.Double(
									(2 + ((double) height / 2.0) + ((((double) object
											.getDistance() / 12.0) * (double) height) * (Math
											.cos((object.getAngle())
													- ((Math.PI / 6.0) * (1.0 / (double) object
															.getDistance()))
													- (Math.PI / 2.0)))))
											- (((1.0 / 25.0) * (double) height) / 2),
									(2 + ((double) height / 2.0) + ((((double) object
											.getDistance() / 12.0) * (double) height) * (Math
											.sin((object.getAngle())
													- ((Math.PI / 6.0) * (1.0 / (double) object
															.getDistance()))
													- (Math.PI / 2.0)))))
											- (((1 / 25.0) * (double) height) / 2),
									((1.0 / 25.0) * (double) height),
									((1.0 / 25.0) * (double) height)));

							g2d.fill(new Ellipse2D.Double(
									(2 + ((double) height / 2.0) + ((((double) object
											.getDistance() / 12.0) * (double) height) * (Math
											.cos((object.getAngle())
													+ ((Math.PI / 6.0) * (1.0 / (double) object
															.getDistance()))
													- (Math.PI / 2.0)))))
											- (((1.0 / 25.0) * (double) height) / 2),
									(2 + ((double) height / 2.0) + ((((double) object
											.getDistance() / 12.0) * (double) height) * (Math
											.sin((object.getAngle())
													+ ((Math.PI / 6.0) * (1.0 / (double) object
															.getDistance()))
													- (Math.PI / 2.0)))))
											- (((1 / 25.0) * (double) height) / 2),
									((1.0 / 25.0) * (double) height),
									((1.0 / 25.0) * (double) height)));
							g2d.setStroke(new BasicStroke(2));
							g2d.draw(new Line2D.Double(
									(2 + ((double) height / 2.0) + ((((double) object
											.getDistance() / 12.0) * (double) height) * (Math
											.cos((object.getAngle())
													- ((Math.PI / 6.0) * (1.0 / (double) object
															.getDistance()))
													- (Math.PI / 2.0)))))
											- (((1.0 / 25.0) * (double) height) / 2)
											+ ((1.0 / 50.0) * (double) height),
									(2 + ((double) height / 2.0) + ((((double) object
											.getDistance() / 12.0) * (double) height) * (Math
											.sin((object.getAngle())
													- ((Math.PI / 6.0) * (1.0 / (double) object
															.getDistance()))
													- (Math.PI / 2.0)))))
											- (((1 / 25.0) * (double) height) / 2)
											+ ((1.0 / 50.0) * (double) height),
									(2 + ((double) height / 2.0) + ((((double) object
											.getDistance() / 12.0) * (double) height) * (Math
											.cos((object.getAngle())
													+ ((Math.PI / 6.0) * (1.0 / (double) object
															.getDistance()))
													- (Math.PI / 2.0)))))
											- (((1.0 / 25.0) * (double) height) / 2)
											+ ((1.0 / 50.0) * (double) height),
									(2 + ((double) height / 2.0) + ((((double) object
											.getDistance() / 12.0) * (double) height) * (Math
											.sin((object.getAngle())
													+ ((Math.PI / 6.0) * (1.0 / (double) object
															.getDistance()))
													- (Math.PI / 2.0)))))
											- (((1 / 25.0) * (double) height) / 2)
											+ ((1.0 / 50.0) * (double) height)));
							g2d.setStroke(new BasicStroke(1));
						}

					}

				}
			}

		}
	}

	protected void paintVisionObjects(Graphics g) {
		int width = mImage.getWidth();
		int height = mImage.getHeight();

		if (width < height) {
			height = (int) ((double) width * ((double) mCameraHeight / (double) mCameraWidth));
			if (height > mImage.getHeight()) {
				width = mImage.getWidth();
				height = mImage.getHeight();
				width = (int) ((double) height * ((double) mCameraWidth / (double) mCameraHeight));
			}
		} else {
			width = (int) ((double) height * ((double) mCameraWidth / (double) mCameraHeight));
			if (width > mImage.getWidth()) {
				width = mImage.getWidth();
				height = mImage.getHeight();
				height = (int) ((double) width * ((double) mCameraHeight / (double) mCameraWidth));
			}
		}

		Graphics2D g2d = (Graphics2D) g;
		g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING,
				RenderingHints.VALUE_ANTIALIAS_ON);
		// g2d.setBackground(new Color(0, 128, 0));

		g2d.clearRect(0, 0, mImage.getWidth(), mImage.getHeight());
		g2d.setColor(new Color(0, 128, 0));
		g2d.fillRect(0, 0, width, height);
		boolean painted = false;
		if (mVisionObjects.size() > 0) {
			painted = true;
			synchronized (mVisionObjects) {
				for (VisionObject object : mVisionObjects) {
					if (object != null) {
						if (object.getBoundingBox().width > 0
								|| object.getBoundingBox().height > 0) {
							Color c = Color.BLACK;
							if (object.getType().equals(
									ObjectType.YELLOW_GOAL_POLE)) {
								c = Color.YELLOW;
								if (!mChkYellowGoalPole.isSelected())
									continue;
							} else if (object.getType().equals(
									ObjectType.BLUE_GOAL_POLE)) {
								c = Color.BLUE;
								if (!mChkBlueGoalPole.isSelected())
									continue;
							} else if (object.getType().equals(ObjectType.BALL)) {
								c = Color.RED;
								if (!mChkBall.isSelected())
									continue;
							} else if (object.getType().equals(
									ObjectType.BLUE_LANDMARK_POLE)) {
								c = Color.BLUE;
								if (!mChkBlueLandmark.isSelected())
									continue;
							} else if (object.getType().equals(
									ObjectType.YELLOW_LANDMARK_POLE)) {
								c = Color.YELLOW;
								if (!mChkYellowLandmark.isSelected())
									continue;
							}

							if (object.getPosition().x > mCameraWidth) {
								mCameraWidth = 800;
								mCameraHeight = 600;
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
							g2d.drawString(
									"ang="
											+ (object.getAngle() * 180 / Math.PI)
											+ "�", pos.x + 8, pos.y + 14);
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
									pos.y + 24);
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
			painted = true;
			double xScale = (double) width / (double) mCameraWidth;
			double yScale = (double) height / (double) mCameraHeight;
			synchronized (mDrawObjects) {
				for (DrawObject object : mDrawObjects) {
					if (object != null) {
						object.draw(g2d, xScale, yScale);
					}
				}
			}
		}
		if (!painted) {
			g2d.setPaint(Color.BLACK);
			g2d.setFont(g2d.getFont().deriveFont(24.0f));
			Rectangle2D rect = g2d.getFontMetrics().getStringBounds(
					"( No vision objects )", g2d);
			g2d.drawString("( No vision objects )",
					(int) (width - rect.getWidth()) / 2,
					(int) (height - rect.getHeight()) / 2);
		}
	}

	private void drawCross(Graphics2D g2d, Point pos) {
		g2d.drawLine(pos.x - 4, pos.y - 4, pos.x + 4, pos.y + 4);
		g2d.drawLine(pos.x + 4, pos.y - 4, pos.x - 4, pos.y + 4);
	}

	private Rectangle rescaleBoundingBox(Rectangle boundingBox, int width,
			int height) {
		double xScale = (double) width / (double) mCameraWidth;
		double yScale = (double) height / (double) mCameraHeight;

		return new Rectangle((int) (boundingBox.x * xScale),
				(int) (boundingBox.y * xScale),
				(int) (boundingBox.width * xScale),
				(int) (boundingBox.height * yScale));
	}

	private Point rescalePosition(Point position, int width, int height) {
		double xScale = (double) width / (double) mCameraWidth;
		double yScale = (double) height / (double) mCameraHeight;

		return new Point((int) (position.x * xScale),
				(int) (position.y * yScale));
	}

	@Override
	public void actionPerformed(ActionEvent e) {
		if (e.getSource().equals(mBtnStartStopStream)) {
			if (e.getActionCommand().equals("start_stream")) {
				mVisionDataService.startVisionObjectStream();
				mBtnStartStopStream.setActionCommand("stop_stream");
				mBtnStartStopStream.setText("Stop object stream");
			} else if (e.getActionCommand().equals("stop_stream")) {
				mVisionDataService.stopVisionObjectStream();
				mBtnStartStopStream.setActionCommand("start_stream");
				mBtnStartStopStream.setText("Start object stream");
			}
		}
	}

	@Override
	public void newVisionObjects(VisionObjectsEvent e) {
		synchronized (mVisionObjects) {
			mVisionObjects.clear();
			mVisionObjects.addAll(e.getVisionObjects());
		}
		mImage.repaint();
		mRadarView.repaint();
	}

	@Override
	public void newDrawObjects(DrawObjectsEvent e) {
		synchronized (mDrawObjects) {
			mDrawObjects.clear();
			mDrawObjects.addAll(e.getDrawObjects());
		}
		mImage.repaint();
	}
}
