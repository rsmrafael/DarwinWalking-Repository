package de.fhwf.humanoid.tools.debugger.tools;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Point;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.image.BufferedImage;
import java.util.ArrayList;
import java.util.List;

import javax.swing.JPanel;
import javax.swing.JSlider;
import javax.swing.SwingUtilities;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

import de.fhwf.humanoid.tools.debugger.data.LookUpTable;
import de.fhwf.humanoid.tools.debugger.data.YUVColor;
import de.fhwf.humanoid.tools.debugger.events.ColorClickEvent;
import de.fhwf.humanoid.tools.debugger.events.ColorSelectChangeEvent;
import de.fhwf.humanoid.tools.debugger.events.ColorSelectChangeListener;
import de.fhwf.humanoid.tools.debugger.events.MarkedColorsChangedEvent;
import de.fhwf.humanoid.tools.debugger.events.MarkedColorsChangedListener;
import de.fhwf.humanoid.tools.debugger.services.ColorCalibrationService;
import de.fhwf.humanoid.tools.debugger.services.ColorCalibrationService.ColorID;
import de.fhwf.humanoid.tools.debugger.services.ServiceManager;

public class YUVSelector extends JPanel implements ChangeListener, MouseListener, MouseMotionListener, ColorSelectChangeListener, MarkedColorsChangedListener{
	private static final long serialVersionUID = -6303746771123593892L;

	private JSlider mYSlider;
	private JPanel mColorRect;
	private int mYValue;
//	private HashMap<ColorID, LinkedList<YUVColor>> mMarkedColors = new HashMap<ColorID, LinkedList<YUVColor>>();
	private ColorID mSelectedColor;
	
	public YUVSelector(int yValue) {
		mYValue = yValue;
		mSelectedColor = ColorID.Orange;
		final ColorCalibrationService colorService = (ColorCalibrationService) ServiceManager
		.getInstance().getService(
				ColorCalibrationService.SERVICENAME);
		colorService.addColorSelectChangeListener(this);
		colorService.addMarkedColorsChangeListener(this);
	

		mColorRect = new JPanel() {
			private static final long serialVersionUID = 1564633303376381316L;

			@Override
			public void paint(Graphics g) {
				
				int[] yuv = new int[3];
				int[] lastyuv = new int[3];
				int[] rgb = new int[3];
				
				yuv[0] = mYValue;
				
				float stepX = 255f / (float)this.getWidth();
				float stepY = 255f / (float)this.getHeight();

				int w = this.getWidth();
				int h = this.getHeight();
				
				BufferedImage imgBuffer = new BufferedImage(w, h, BufferedImage.TYPE_INT_RGB);
				
				for (int x = 0; x < w; x++) {
					for (int y = 0; y < h; y++) {
						yuv[2] = (int)(x * stepX);
						yuv[1] = (int)(y * stepY);
						
						if (yuv[2] != lastyuv[2] || yuv[1] != lastyuv[1]) {
  						convertYUVtoRGB(yuv, rgb);
  						lastyuv[2] = yuv[2];
  						lastyuv[1] = yuv[1];
						}
						imgBuffer.setRGB(x, y, (rgb[0] << 16) + (rgb[1] << 8) + rgb[2]);
					}
				}
				g.drawImage(imgBuffer, 0, 0, null);

  			stepX = (float)this.getWidth() / 255f;
  			stepY = (float)this.getHeight() / 255f;

			LookUpTable mTable = colorService.getLookUpTable();
  			for(int u = 0; u < 256; u++){
  				for (int v = 0; v < 256; v++){
  					if (mTable.isColor(mSelectedColor, new YUVColor(mYValue, u, v))){
	
  					Point start = new Point((int)(u * stepX), (int)(v * stepY));
		  			Point end = new Point((int)((u + 1) * stepX), (int)((v + 1) * stepY));
		  			end.translate(-start.x - 1, -start.y - 1);
		  			
		  			if(mYValue < 128) {
		  				g.setColor(Color.WHITE);
		  			} else {
		  				g.setColor(Color.BLACK);
		  			}					
		  			g.fillRect(start.x - 2, start.y - 3, end.x + 2, end.y + 3);		
  					}
		  			}
  				}					
			}
		};
		mColorRect.addMouseListener(this);
		mColorRect.addMouseMotionListener(this);
		this.setLayout(new BorderLayout());
		mYSlider = new JSlider(0, 255);
		mYSlider.setOrientation(JSlider.VERTICAL);
		mYSlider.addChangeListener(this);
		this.add(mYSlider, BorderLayout.EAST);
		this.add(mColorRect, BorderLayout.CENTER);
	}
	
	public void setYValue(int value) {
		mYValue = value;
		this.repaint();
	}
	
	public int getYValue() {
		return mYValue;
	}
	
	
  private void convertYUVtoRGB(int[] yuv, int[] rgb) {
    int y = yuv[0];
    int u = yuv[1];
    int v = yuv[2];

		rgb[2] = v - 128 + y;
		if (rgb[2] > 255) rgb[2] = 255;
		if (rgb[2] < 0  ) rgb[2] = 0;
		rgb[0] = u - 128 + y;
		if (rgb[0] > 255) rgb[0] = 255;
		if (rgb[0] < 0  ) rgb[0] = 0;
		rgb[1] = (169 * y - 52 * rgb[0] - 19 * rgb[2]) / 100;
		if (rgb[1] > 255) rgb[1] = 255;
		if (rgb[1] < 0  ) rgb[1] = 0;
  }

  @Override
	public void mousePressed(MouseEvent e) {
	}

	@Override
	public void mouseReleased(MouseEvent e) {
	}

	@Override
	public void mouseMoved(MouseEvent e) {
  	
	}

	@Override
	public void mouseDragged(MouseEvent e) {
		updateColorCalc(e);
		
	}

	@Override
	public void mouseClicked(MouseEvent e) {
		updateColorCalc(e);
	}

	@Override
	public void mouseEntered(MouseEvent e) {
	}

	@Override
	public void mouseExited(MouseEvent e) {
	}

	@Override
	public void stateChanged(ChangeEvent e) {
		if (e.getSource().equals(mYSlider)) {
			mYValue = mYSlider.getValue();
			this.repaint();
		}
	}

	@Override
	public void ColorSelectChange(ColorSelectChangeEvent event) {
		mSelectedColor = event.getColor();
		this.repaint();
	}

	@Override
	public void MarkedColorsChanged(MarkedColorsChangedEvent event) {
		this.repaint();
	}
	
	public void updateColorCalc(MouseEvent e){
		float x = e.getX();
		float y = e.getY();
		float stepX = (float)mColorRect.getWidth() / 255f;
		float stepY = (float)mColorRect.getHeight() / 255f;

		ColorCalibrationService colorService = (ColorCalibrationService)ServiceManager.getInstance().getService(ColorCalibrationService.SERVICENAME);
		List<YUVColor> color = new ArrayList<YUVColor>(1);
		color.add(new YUVColor(mYValue, (int)(x / stepX), (int)(y / stepY)));

		if (SwingUtilities.isRightMouseButton(e)) {	
				colorService.notifyColorClick(new ColorClickEvent(this, color, false, 1));
		} else if (SwingUtilities.isLeftMouseButton(e)) {
				colorService.notifyColorClick(new ColorClickEvent(this, color, true, 1));
		}
		
	}
}
