package de.fhwf.humanoid.tools.debugger.views;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Cursor;
import java.awt.Dimension;
import java.awt.FileDialog;
import java.awt.FlowLayout;
import java.awt.Frame;
import java.awt.Graphics;
import java.awt.Point;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.image.BufferedImage;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.SortedSet;
import java.util.TreeSet;
import javax.imageio.ImageIO;
import javax.swing.BorderFactory;
import javax.swing.ButtonGroup;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JRadioButton;
import javax.swing.JSlider;
import javax.swing.SwingUtilities;
import javax.swing.border.BevelBorder;
import javax.swing.event.PopupMenuEvent;
import javax.swing.event.PopupMenuListener;
import de.fhwf.humanoid.tools.debugger.data.DrawObject;
import de.fhwf.humanoid.tools.debugger.data.LookUpTable;
import de.fhwf.humanoid.tools.debugger.data.PBFrame;
import de.fhwf.humanoid.tools.debugger.data.YUVColor;
import de.fhwf.humanoid.tools.debugger.data.YUVImage;
import de.fhwf.humanoid.tools.debugger.events.ColorClickEvent;
import de.fhwf.humanoid.tools.debugger.events.ColorConfigurationUpdateEvent;
import de.fhwf.humanoid.tools.debugger.events.ColorConfigurationUpdateListener;
import de.fhwf.humanoid.tools.debugger.events.ScalingChangedEvent;
import de.fhwf.humanoid.tools.debugger.services.ColorCalibrationService;
import de.fhwf.humanoid.tools.debugger.services.ColorCalibrationService.ColorID;
import de.fhwf.humanoid.tools.debugger.services.FileService;
import de.fhwf.humanoid.tools.debugger.services.ImageService;
import de.fhwf.humanoid.tools.debugger.services.ScalingChangedService;
import de.fhwf.humanoid.tools.debugger.services.ServiceManager;
import de.fhwf.humanoid.tools.debugger.tools.CheckCombo;
import de.fhwf.humanoid.tools.debugger.tools.ColorInfo;
import de.fhwf.humanoid.tools.debugger.tools.ExtendedFlowLayout;
import de.fhwf.humanoid.tools.debugger.tools.ImageViewer;
import de.fhwf.humanoid.tools.debugger.tools.Stencil;
import de.fhwf.humanoid.tools.debugger.undoredo.UndoRedoHistory;
import de.ostfalia.robocup.message.MsgProtoBufFrame.CameraLocation;

public class CameraView extends JPanel implements ActionListener,
		MouseListener, MouseMotionListener, ColorConfigurationUpdateListener,
		PopupMenuListener {
	private static final long serialVersionUID = -7656954260691728080L;
	private static final int LOG_MODULE_WIDTH = 200;

	private ImageService mImageService;
	private ScalingChangedService mScalingChangedService;
	private ImageViewer mImage;
	private YUVImage mYUVImage;
	private JButton mBtnGetImage;
	private JButton mBtnSavePBF;
	private JButton mBtnSaveImage;
	private JButton mBtnLoadImage;
	private JButton mBtnSendImage;
	private JCheckBox mChkAugmented;
	private JRadioButton mRadLeftCam;
	private JRadioButton mRadRightCam;
	private JCheckBox mChkYUVMode;
	private JSlider mSldDownsampleFactor;
	private JSlider mMarkColorSlider;
	private JSlider mMarkColorSpaceSlider;
	private JCheckBox mChkAutoRefresh;
	private Thread mImagePollThread;
	private ColorInfo mColorInfo;
	private JLabel mPixelPosition;
	private JCheckBox mBoundingBox;
	private CheckCombo mAnnotations;
	private String mAnnotastionsText = null;
	

	// TODO: GlassPanes einbauen!!!

	public CameraView() {
		super();

		mImageService = (ImageService) ServiceManager.getInstance().getService(
				ImageService.SERVICENAME);

		mImagePollThread = null;

		mScalingChangedService = (ScalingChangedService) ServiceManager
				.getInstance().getService(ScalingChangedService.SERVICENAME);

		this.setLayout(new BorderLayout());

		mImage = new ImageViewer();
		mImage.setBorder(BorderFactory.createBevelBorder(BevelBorder.RAISED));
		mImage.setPreferredSize(new Dimension(405, 305));
		mImage.addMouseListener(this);
		mImage.addMouseMotionListener(this);
		this.add(mImage, BorderLayout.CENTER);
		JPanel container = new JPanel();
		container.setLayout(new BorderLayout());
		JPanel upperContainer = new JPanel();
		upperContainer.setLayout(new ExtendedFlowLayout(FlowLayout.CENTER, 10,
				5));
		JPanel lowerContainer = new JPanel();
		lowerContainer.setLayout(new ExtendedFlowLayout(FlowLayout.CENTER, 10,
				0));
		mBtnGetImage = new JButton("Get image");
		mBtnGetImage.addActionListener(this);
		upperContainer.add(mBtnGetImage);
		mBtnSavePBF = new JButton("Save as PBF");
		mBtnSavePBF.addActionListener(this);
		upperContainer.add(mBtnSavePBF);
		mChkYUVMode = new JCheckBox("YUV Mode");
		upperContainer.add(mChkYUVMode);
		mChkYUVMode.setEnabled(false);
		mChkAutoRefresh = new JCheckBox("Auto-Refresh");
		upperContainer.add(mChkAutoRefresh);
		mChkAugmented = new JCheckBox("Augmented");
		upperContainer.add(mChkAugmented);
		mRadLeftCam = new JRadioButton("Left", true);
		mRadRightCam = new JRadioButton("Right", true);

		mBtnSaveImage = new JButton("Save image");
		mBtnSaveImage.addActionListener(this);
		upperContainer.add(mBtnSaveImage);

		mBtnLoadImage = new JButton("Load image");
		mBtnLoadImage.addActionListener(this);
		upperContainer.add(mBtnLoadImage);

		mBtnSendImage = new JButton("Send PBF to bot");
		mBtnSendImage.addActionListener(this);
		upperContainer.add(mBtnSendImage);

		mBoundingBox = new JCheckBox("Show BoundingBoxes");
		upperContainer.add(mBoundingBox);
		mBoundingBox.setEnabled(true);
		mBoundingBox.addActionListener(this);

		mAnnotations = new CheckCombo();
		upperContainer.add(mAnnotations);
		mAnnotations.setEnabled(true);
		mAnnotations.setEditable(true);
		mAnnotations.addActionListener(this);
		mAnnotations.setActionCommand("setAnnotation");
		mAnnotations.addPopupMenuListener(this);
		mAnnotations.setPreferredSize(new Dimension(LOG_MODULE_WIDTH, 22));

		ButtonGroup group = new ButtonGroup();
		group.add(mRadLeftCam);
		group.add(mRadRightCam);
		JPanel camSelector = new JPanel(new FlowLayout(FlowLayout.LEFT, 5, 0));
		camSelector.add(new JLabel("Camera: "));
		camSelector.add(mRadLeftCam);
		camSelector.add(mRadRightCam);
		lowerContainer.add(camSelector);
		mSldDownsampleFactor = new JSlider(1, 4, 2);
		mSldDownsampleFactor.setPreferredSize(new Dimension(75, 20));
		mSldDownsampleFactor.setMajorTickSpacing(1);
		mSldDownsampleFactor.setSnapToTicks(true);
		mSldDownsampleFactor.setPaintTicks(true);
		JPanel downsampleSelector = new JPanel();
		downsampleSelector.add(new JLabel("Scale factor: "));
		downsampleSelector.add(mSldDownsampleFactor);
		lowerContainer.add(downsampleSelector);

		mMarkColorSlider = new JSlider(0, 20, 0);
		mMarkColorSlider.setPreferredSize(new Dimension(75, 20));
		mMarkColorSlider.setMajorTickSpacing(1);
		mMarkColorSlider.setSnapToTicks(true);
		mMarkColorSlider.setPaintTicks(true);
		JPanel ColorMarker = new JPanel();
		ColorMarker.add(new JLabel("CCImage: "));
		ColorMarker.add(mMarkColorSlider);

		mMarkColorSpaceSlider = new JSlider(0, 20, 0);
		mMarkColorSpaceSlider.setPreferredSize(new Dimension(75, 20));
		mMarkColorSpaceSlider.setMajorTickSpacing(1);
		mMarkColorSpaceSlider.setSnapToTicks(true);
		mMarkColorSpaceSlider.setPaintTicks(true);
		ColorMarker.add(new JLabel("CCSpace: "));
		ColorMarker.add(mMarkColorSpaceSlider);
		lowerContainer.add(ColorMarker);
		mImage.setCursor(createCircleCursor(Color.white,
				mMarkColorSlider.getValue()));

		JPanel southContainer = new JPanel(new BorderLayout());

		container.add(upperContainer, BorderLayout.NORTH);
		container.add(lowerContainer, BorderLayout.SOUTH);

		JPanel pixelInfo = new JPanel(new BorderLayout());
		mColorInfo = new ColorInfo();
		pixelInfo.add(mColorInfo, BorderLayout.CENTER);
		mPixelPosition = new JLabel("X: - Y: -");
		pixelInfo.add(mPixelPosition, BorderLayout.SOUTH);
		southContainer.add(pixelInfo, BorderLayout.EAST);
		southContainer.add(container, BorderLayout.CENTER);

		this.add(southContainer, BorderLayout.SOUTH);

		mImageService.addColorConfigurationUpdateListener(this);
	}

	@Override
	public void actionPerformed(ActionEvent e) {
		if (e.getSource().equals(mBoundingBox)) {
			if (mBoundingBox.isSelected()) {
				mImage.activateImageStream();
			} else if (!mBoundingBox.isSelected()) {
				mImage.deactivateImageStream();
			}
		}
		if (e.getSource().equals(mBtnGetImage)) {
			mImagePollThread = new Thread() {
				@Override
				public void run() {
					mBtnGetImage.setEnabled(false);
					do {
						try {
							mYUVImage = mImageService
									.getImage(mRadRightCam.isSelected(),
											mChkAugmented.isSelected(),
											(int) Math.pow(2,
													mSldDownsampleFactor
															.getValue() - 1),
											mChkYUVMode.isSelected());
							mScalingChangedService
									.notifyNewScalingChanged(new ScalingChangedEvent(
											this, (int) Math.pow(2,
													mSldDownsampleFactor
															.getValue() - 1)));
							if (mYUVImage != null) {
								mImage.setImage(mYUVImage.getImage());
							}
						} catch (IOException e1) {
							if (!mChkAutoRefresh.isSelected()) {
								JOptionPane.showMessageDialog(null,
										e1.getMessage(),
										"File transfer exception",
										JOptionPane.ERROR_MESSAGE, null);
							} else {
								System.err.println("File transfer exception: "
										+ e1.getMessage());
							}
						} catch (IllegalStateException e1) {
							if (!mChkAutoRefresh.isSelected()) {
								JOptionPane.showMessageDialog(null,
										e1.getMessage(),
										"File transfer exception",
										JOptionPane.ERROR_MESSAGE, null);
							} else {
								System.err.println("File transfer exception: "
										+ e1.getMessage());
							}
						}
						try {
							Thread.sleep(100);
						} catch (InterruptedException e) {
							e.printStackTrace();
						}
					} while (mChkAutoRefresh.isSelected());
					mBtnGetImage.setEnabled(true);
				}
			};
			mImagePollThread.start();
		} else if (e.getSource().equals(mBtnSavePBF)) {
			try {
				mImageService.getImage(false, false, 1, true);
			} catch (IOException e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
			}
		} else if (e.getSource().equals(mBtnSaveImage)) {

			if (mImage.getImage() != null) {
				FileDialog filed = new FileDialog((Frame) null, "Save image",
						FileDialog.SAVE);
				DateFormat dateFormat = new SimpleDateFormat("yyyyMMdd-HHmm");
				java.util.Date date = new java.util.Date();
				String name = dateFormat.format(date);
				name += "-";
				if (mRadRightCam.isSelected()) {
					name += "right-";
				} else {
					name += "left-";
				}
				if (mChkAugmented.isSelected()) {
					name += "augmented";
				} else {
					name += "raw";
				}
				filed.setFile(name + ".jpg");
				filed.setVisible(true);
				String filename = filed.getFile();
				if (filename != null) {
					filename = filed.getDirectory() + filename;
					try {
						BufferedImage bi = mImage.getImage();
						File outputfile = new File(filename);
						ImageIO.write(bi, "jpg", outputfile);
					} catch (IOException e1) {
						JOptionPane.showMessageDialog(this, e1.getMessage(),
								"Image save exception",
								JOptionPane.ERROR_MESSAGE, null);
					}
				}
				filed.dispose();

			}
		} else if (e.getSource().equals(mBtnLoadImage)) {
			FileDialog filed = new FileDialog((Frame) null, "Load image",
					FileDialog.LOAD);
			filed.setVisible(true);
			String fileName = filed.getDirectory() + filed.getFile();

			BufferedImage bi = null;
			try {
				if (fileName.endsWith(".jpg")) {
				 bi = ImageIO.read(new File(fileName));
					mYUVImage = new YUVImage(bi);
				} else if (fileName.endsWith(".yuv")) {
					BufferedReader br = new BufferedReader(
							new InputStreamReader(
									new FileInputStream(fileName), "ISO-8859-1"));

					char[] header = new char[7];
					br.mark(7);
					br.read(header);
					if (header[0] == 'Y' && header[1] == 'U'
							&& header[2] == 'V') {
						int width = (header[3] << 8) + header[4];
						int height = (header[5] << 8) + header[6];
						char[] imageData = new char[width * height * 2];
						br.read(imageData);
						mYUVImage = new YUVImage(imageData, width, height);
					} else {
						br.reset();
						StringBuffer colorBuffer = new StringBuffer();

						String line;
						while ((line = br.readLine()) != null) {
							colorBuffer.append(line);
						}
						String imageData = colorBuffer.toString();
						mYUVImage = new YUVImage(imageData);
					}
					br.close();
				} else if (fileName.endsWith(".pbf")) {
					PBFrame pbf = new PBFrame();
					pbf.load(fileName);
					CameraLocation loc = CameraLocation.HEAD_LEFT;
					if (mRadRightCam.isSelected()) {
						loc = CameraLocation.HEAD_RIGHT;
					}
					mYUVImage = pbf.getImage(loc);
					if (mYUVImage == null) {
						JOptionPane.showMessageDialog(null,
								"PBF does not contain an image with location "
										+ loc.toString(), "No image found",
								JOptionPane.ERROR);
					}
				}
			} catch (IOException e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
			}
			mImage.setImage(mYUVImage.getImage());
			filed.dispose();
		} else if (e.getSource().equals(mBtnSendImage)) {
			FileDialog filed = new FileDialog((Frame) null, "Load image",
					FileDialog.LOAD);
			filed.setVisible(true);
			String fileName = filed.getDirectory() + filed.getFile();
			if (fileName.endsWith(".pbf")) {
				FileService fs = (FileService) ServiceManager.getInstance()
						.getService(FileService.SERVICENAME);

				boolean useColorTable = (JOptionPane.showConfirmDialog(this,
						"Use ColorCalibration contained in PBF?",
						"ColorCalibration", JOptionPane.YES_NO_OPTION,
						JOptionPane.QUESTION_MESSAGE) == JOptionPane.YES_OPTION);
				try {
					fs.writeFile("/tmp/image.pbf", new File(fileName));
					mImageService.sendImage("/tmp/image.pbf", useColorTable);
				} catch (IllegalStateException e1) {
					JOptionPane.showMessageDialog(null, e1.getMessage(),
							"Error sending image", JOptionPane.ERROR_MESSAGE,
							null);
				} catch (IOException e1) {
					JOptionPane.showMessageDialog(null, e1.getMessage(),
							"Error sending file", JOptionPane.ERROR_MESSAGE,
							null);
				}
			}
		}

		if (e.getSource().equals(mAnnotations)) {			
			mAnnotastionsText = ",";
			String annotaionSources = "";
			for (Object m : mAnnotations.getSelectedItems()) {
				if (m != null) {
					String annotationSource = (String) m;
					if (annotationSource.equals("< All modules >")) {
						mAnnotastionsText = null;
						break;
					} else {
						mAnnotastionsText += annotationSource.toLowerCase() + ",";
						annotaionSources += annotationSource + ", ";
					}
				} else {
					mAnnotastionsText = null;
				}
			}
			if (mAnnotastionsText != null) {
				if (annotaionSources.length() > 2) {
					annotaionSources = annotaionSources.substring(0,
							annotaionSources.length() - 2);
				}
				final String annotaionsFin = annotaionSources;
				SwingUtilities.invokeLater(new Runnable() {
					public void run() {
						mAnnotations.getEditor().setItem(annotaionsFin);
					}
				});
				
			} else {
				final String annotaionsFin = "< All modules >";
				SwingUtilities.invokeLater(new Runnable() {
					public void run() {
						mAnnotations.getEditor().setItem(annotaionsFin);
					}
				});
			}
			mImage.setSelectedAnnotations(mAnnotastionsText);
		}
	}

	@Override
	public void mouseClicked(MouseEvent e) {
		if (SwingUtilities.isRightMouseButton(e)) {
			if (mImage.getImage() != null) {
				int x = e.getX();
				int y = e.getY();
				ColorCalibrationService colorService = (ColorCalibrationService) ServiceManager
						.getInstance().getService(
								ColorCalibrationService.SERVICENAME);
				colorService.notifyColorClick(new ColorClickEvent(this,
						calculateColors(x, y), false, mMarkColorSpaceSlider
								.getValue()));
			}
		} else if (SwingUtilities.isLeftMouseButton(e)) {
			if (mImage.getImage() != null) {
				int x = e.getX();
				int y = e.getY();
				ColorCalibrationService colorService = (ColorCalibrationService) ServiceManager
						.getInstance().getService(
								ColorCalibrationService.SERVICENAME);
				colorService.notifyColorClick(new ColorClickEvent(this,
						calculateColors(x, y), true, mMarkColorSpaceSlider
								.getValue()));
			}
		}
	}

	@Override
	public void mouseEntered(MouseEvent e) {
		mImage.setCursor(createCircleCursor(Color.white,
				mMarkColorSlider.getValue()));

	}

	@Override
	public void mouseExited(MouseEvent e) {

	}

	@Override
	public void mousePressed(MouseEvent e) {
		ColorCalibrationService colorService = (ColorCalibrationService) ServiceManager
				.getInstance().getService(ColorCalibrationService.SERVICENAME);
		UndoRedoHistory<LookUpTable> history = colorService.getLookUpTable()
				.getUndoRedoHistory();
		history.beginCompoundDo();
	}

	@Override
	public void mouseReleased(MouseEvent e) {
		ColorCalibrationService colorService = (ColorCalibrationService) ServiceManager
				.getInstance().getService(ColorCalibrationService.SERVICENAME);
		UndoRedoHistory<LookUpTable> history = colorService.getLookUpTable()
				.getUndoRedoHistory();
		history.endCompoundDo();
		// Inform about changed history
		colorService.notifyColorClick(null);
	}

	/**
	 * To gather the pixels belonging to one Color it is important, to track the
	 * clicks on the image.
	 * 
	 */
	@Override
	public void mouseDragged(MouseEvent e) {
		if (SwingUtilities.isLeftMouseButton(e)) {
			if (mImage.getImage() != null) {
				int x = e.getX();
				int y = e.getY();

				if (x > 0 && y > 0) {
					ColorCalibrationService colorService = (ColorCalibrationService) ServiceManager
							.getInstance().getService(
									ColorCalibrationService.SERVICENAME);
					colorService.notifyColorClick(new ColorClickEvent(this,
							calculateColors(x, y), true, mMarkColorSpaceSlider
									.getValue()));
				}
			}
		} else if (SwingUtilities.isRightMouseButton(e)) {
			if (mImage.getImage() != null) {
				int x = e.getX();
				int y = e.getY();

				if (x > 0 && y > 0) {
					ColorCalibrationService colorService = (ColorCalibrationService) ServiceManager
							.getInstance().getService(
									ColorCalibrationService.SERVICENAME);
					colorService.notifyColorClick(new ColorClickEvent(this,
							calculateColors(x, y), false, mMarkColorSpaceSlider
									.getValue()));
				}
			}
		}
	}

	@Override
	public void mouseMoved(MouseEvent e) {
		if (mImage.getImage() != null) {
			int x = e.getX();
			int y = e.getY();

			int maxWidth = mYUVImage.getWidth();
			if (mImage.getWidth() > maxWidth)
				maxWidth = mImage.getImage().getWidth();
			int maxHeight = mYUVImage.getHeigth();
			if (mImage.getHeight() > maxHeight)
				maxHeight = mImage.getImage().getHeight();

			if (x >= 0 && y >= 0 && x < maxWidth && y < maxHeight) {
				mPixelPosition.setText("X: " + x + " Y: " + y);
				if (mYUVImage.isInYUVMode()) {
					mColorInfo.setColor(mYUVImage.getImageData(x, y));
				} else {
					int rgb = mImage.getImage().getRGB(x, y);
					mColorInfo.setColor(new YUVColor(rgb));
				}
			}
		}
	}

	/**
	 * To gather more COlor then the current Color click upon this method uses a
	 * stencil, 2-Dimensional Array with ones standing for a pixel should be
	 * added and 0 for ignoring it. The Stencil(Matrix) is placed around the
	 * clicked COlor
	 * 
	 * @param x
	 *            coordinate of the click
	 * @param y
	 *            coordinate of the click
	 * @return All Colors grabbed by the stencil around the click.
	 */
	public List<YUVColor> calculateColors(int x, int y) {

		int stencil[][] = Stencil.getStencil2D(mMarkColorSlider.getValue());

		List<YUVColor> colors = new ArrayList<YUVColor>();
		try {
			for (int i = 0; i < stencil.length; i++) {
				for (int i2 = 0; i2 < stencil[i].length; i2++) {
					if (stencil[i][i2] == 1) {
						if (mYUVImage.isInYUVMode()) {
							int newX = x - stencil.length / 2 + i;
							int newY = y - stencil[0].length / 2 + i2;
							colors.add(mYUVImage.getImageData(newX, newY));
						} else {

							int newX = x - stencil.length / 2 + i;
							int newY = y - stencil[0].length / 2 + i2;
							int rgb = mImage.getImage().getRGB(newX, newY);
							YUVColor c = new YUVColor(rgb);
							colors.add(c);
						}
					} else {
						// No operation needed, Pixel should be ignored
					}
				}
			}
		} catch (ArrayIndexOutOfBoundsException e) {

		}

		return colors;

	}

	/**
	 * Marks the configured Colors in the image.
	 * 
	 */
	@Override
	public void colorConfigurationUpdated(ColorConfigurationUpdateEvent e) {
		if (mImage.getImage() != null) {
			BufferedImage bf = mYUVImage.getImage();
			int marked = 0;
			for (int x = 0; x < bf.getWidth(); x++) {
				for (int y = 0; y < bf.getHeight(); y++) {
					if (y % 5 == 0 | x % 5 == 0) {
						for (ColorID color : ColorID.values()) {
							if (e.getLookUpTable().isColor(color,
									mYUVImage.getImageData(x, y))) {
								marked = 0;
								if (color.equals(ColorID.White)) {
									bf.setRGB(x, y, Color.white.getRGB());
									marked++;
								} else if (color.equals(ColorID.Green)) {
									bf.setRGB(x, y, Color.green.getRGB());
									marked++;
								} else if (color.equals(ColorID.Blue)) {
									bf.setRGB(x, y, Color.blue.getRGB());
									marked++;
								} else if (color.equals(ColorID.Yellow)) {
									bf.setRGB(x, y, Color.yellow.getRGB());
									marked++;
								} else if (color.equals(ColorID.Orange)) {
									bf.setRGB(x, y, Color.orange.getRGB());
									marked++;
								} else if (color.equals(ColorID.Magenta)) {
									bf.setRGB(x, y, Color.magenta.getRGB());
									marked++;
								} else if (color.equals(ColorID.Cyan)) {
									bf.setRGB(x, y, Color.cyan.getRGB());
									marked++;
								} else if (color.equals(ColorID.Black)) {
									bf.setRGB(x, y, Color.black.getRGB());
									marked++;
								} else {
									System.out
											.println("[CameraView] colorConfigurationUpdated: Unsupported ColorType!");
								}
								if (marked > 1) {
									bf.setRGB(x, y, Color.white.getRGB());
									System.out
											.println("[CameraView] Double Marking!!!");
								}
							}
						}
					}
				}
			}
			mImage.setImage(bf);

			repaint();

		}
	}

	/**
	 * Creates different sized Cursors
	 * 
	 * 
	 */
	public static Cursor createCircleCursor(Color cursorColor, int cursorSize) {
		if (cursorSize == 0) {
			return Cursor.getDefaultCursor();
		}

		int minSize = 2 * cursorSize;
		Dimension best = Toolkit.getDefaultToolkit().getBestCursorSize(minSize,
				minSize);
		if (minSize < best.width) {
			minSize = best.width;
		}

		BufferedImage bi = new BufferedImage(minSize, minSize,
				BufferedImage.TYPE_INT_ARGB);
		Graphics g = bi.getGraphics();

		g.setColor(cursorColor);

		g.drawRect(0, 0, 2 * cursorSize - 1, 2 * cursorSize - 1);
		g.setColor(Color.black);
		g.drawRect(1, 1, 2 * cursorSize - 2, 2 * cursorSize - 2);

		return Toolkit.getDefaultToolkit().createCustomCursor(bi,
				new Point(cursorSize, cursorSize), cursorColor + " Circle");

	}

	@Override
	public void popupMenuCanceled(PopupMenuEvent arg0) {
		// TODO Auto-generated method stub

	}

	@Override
	public void popupMenuWillBecomeInvisible(PopupMenuEvent arg0) {
		// TODO Auto-generated method stub

	}

	@Override
	public void popupMenuWillBecomeVisible(PopupMenuEvent e) {
		Set<String> existingModules = new HashSet<String>();
		for (int i = 0; i < mAnnotations.getItemCount(); ++i) {
			existingModules.add(mAnnotations.getItemAt(i).toString());
		}

		SortedSet<String> modules = new TreeSet<String>();
		modules.add("< All modules >");
		for (DrawObject draw : mImage.getDrawObjects()) {
			if (draw.getSource() == null){
				System.out.println("Break");
			}
			modules.add(draw.getSource());
		}

		// Remove vanished items
		for (String log : existingModules) {
			if (!modules.contains(log)) {
				mAnnotations.removeItem(log);
			}
		}

		// Add new items
		for (String log : modules) {
			if (!existingModules.contains(log)) {
				mAnnotations.addItem(log);
			}
		}
	}
}
